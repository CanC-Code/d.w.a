#include <jni.h>
#include <android/bitmap.h>
#include <android/log.h>
#include <cstring>
#include <cstdint>
#include <atomic>
#include <mutex>

#include "recompiled/cpu_shared.h"

#define LOG_TAG "DWA_Native"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// External subsystem declarations
extern void ppu_init();
extern void ppu_reset();
extern void ppu_render_frame(uint32_t* pixels);
extern void ppu_step();
extern uint8_t ppu_read_register(uint16_t addr);
extern void ppu_write_register(uint16_t addr, uint8_t value);
extern bool ppu_in_vblank();  // NEW: Check if PPU is in VBlank

extern void mapper_init(uint8_t* prg_rom, size_t prg_size, uint8_t* chr_rom, size_t chr_size);
extern void mapper_reset();
extern uint8_t mapper_read(uint16_t addr);
extern void mapper_write(uint16_t addr, uint8_t value);
extern uint8_t mapper_get_current_bank();

extern void controller_set_button(uint8_t button, bool pressed);
extern uint8_t controller_read();
extern void controller_write(uint8_t value);

// Forward declarations for recompiled banks
namespace Bank00 { void execute_at(uint16_t pc); }
namespace Bank01 { void execute_at(uint16_t pc); }
namespace Bank02 { void execute_at(uint16_t pc); }
namespace Bank03 { void execute_at(uint16_t pc); }

// ============================================================================
// GLOBAL CPU STATE
// ============================================================================
extern "C" {
    uint8_t reg_A = 0;
    uint8_t reg_X = 0;
    uint8_t reg_Y = 0;
    uint8_t reg_P = 0x24;  // IRQ disabled on power-on
    uint8_t reg_SP = 0xFD;
    uint16_t reg_PC = 0;
    int cycles_to_run = 0;
}

// Memory
static uint8_t ram[0x0800];
static uint8_t sram[0x2000];
static uint8_t* prg_rom_data = nullptr;
static uint8_t* chr_rom_data = nullptr;
static size_t prg_sz = 0;
static size_t chr_sz = 0;

// Emulation state
static std::atomic<bool> emulator_running{false};
static std::mutex emu_mutex;
static bool rom_loaded = false;
static uint32_t frame_buffer[256 * 240];
static bool nmi_pending = false;  // NEW: Track NMI state

// ============================================================================
// BUS COMMUNICATIONS
// ============================================================================
extern "C" uint8_t bus_read(uint16_t addr) {
    if (addr < 0x2000) return ram[addr & 0x7FF];
    if (addr < 0x4000) return ppu_read_register(0x2000 | (addr & 0x7));
    if (addr == 0x4016) return controller_read();
    if (addr >= 0x6000 && addr < 0x8000) return sram[addr - 0x6000];
    if (addr >= 0x8000) return mapper_read(addr);
    return 0;
}

extern "C" void bus_write(uint16_t addr, uint8_t value) {
    if (addr < 0x2000) {
        ram[addr & 0x7FF] = value;
    } else if (addr < 0x4000) {
        ppu_write_register(0x2000 | (addr & 0x7), value);
    } else if (addr == 0x4014) {
        // OAM DMA
        uint16_t dma_addr = value << 8;
        for (int i = 0; i < 256; i++) {
            ppu_write_register(0x2004, bus_read(dma_addr + i));
        }
        cycles_to_run -= 513;
    } else if (addr == 0x4016) {
        controller_write(value);
    } else if (addr >= 0x6000 && addr < 0x8000) {
        sram[addr - 0x6000] = value;
    } else if (addr >= 0x8000) {
        mapper_write(addr, value);
    }
}

// ============================================================================
// CPU EXECUTION
// ============================================================================
static void execute_recompiled(uint16_t pc) {
    if (pc >= 0xC000) {
        Bank03::execute_at(pc);
    } else if (pc >= 0x8000) {
        uint8_t bank = mapper_get_current_bank();
        switch (bank) {
            case 0: Bank00::execute_at(pc); break;
            case 1: Bank01::execute_at(pc); break;
            case 2: Bank02::execute_at(pc); break;
            default: Bank00::execute_at(pc); break;
        }
    } else {
        LOGE("CPU PC out of ROM range: $%04X", pc);
        reg_PC++;
    }
}

// NEW: Handle NMI (Non-Maskable Interrupt)
static void cpu_nmi() {
    // Push PC
    cpu_push((reg_PC >> 8) & 0xFF);
    cpu_push(reg_PC & 0xFF);
    
    // Push status (with B flag clear)
    cpu_push(reg_P & ~0x10);
    
    // Set I flag
    reg_P |= 0x04;
    
    // Jump to NMI vector
    reg_PC = cpu_read_pointer(0xFFFA);
    
    cycles_to_run -= 7;  // NMI takes 7 cycles
    nmi_pending = false;
}

static void cpu_run_frame() {
    cycles_to_run = 29780;  // NTSC frame = 29780 CPU cycles
    int ppu_cycles = 0;
    
    while (cycles_to_run > 0 && emulator_running) {
        // Handle pending NMI
        if (nmi_pending && !(reg_P & 0x04)) {  // NMI not disabled
            cpu_nmi();
        }
        
        uint16_t prev_pc = reg_PC;
        int prev_cycles = cycles_to_run;
        
        // Execute one instruction
        execute_recompiled(reg_PC);
        
        // Safety check
        if (reg_PC == prev_pc && cycles_to_run == prev_cycles) {
            LOGE("CPU stuck at $%04X", reg_PC);
            reg_PC++;
            cycles_to_run -= 2;
        }
        
        // Run PPU (3 PPU cycles per CPU cycle)
        int cpu_cycles_used = prev_cycles - cycles_to_run;
        for (int i = 0; i < cpu_cycles_used * 3; i++) {
            ppu_step();
            
            // Check for VBlank NMI
            if (ppu_in_vblank() && !nmi_pending) {
                nmi_pending = true;
            }
        }
    }
}

// ============================================================================
// JNI INTERFACE
// ============================================================================
extern "C" JNIEXPORT jboolean JNICALL
Java_com_canc_dwa_MainActivity_loadROM(JNIEnv* env, jobject, jbyteArray rom_data) {
    std::lock_guard<std::mutex> lock(emu_mutex);
    
    jsize len = env->GetArrayLength(rom_data);
    if (len < 16) {
        LOGE("ROM too small: %d bytes", len);
        return JNI_FALSE;
    }
    
    uint8_t* raw_rom = (uint8_t*)env->GetByteArrayElements(rom_data, nullptr);
    
    // Verify iNES header
    if (memcmp(raw_rom, "NES\x1A", 4) != 0) {
        LOGE("Invalid NES header");
        env->ReleaseByteArrayElements(rom_data, (jbyte*)raw_rom, JNI_ABORT);
        return JNI_FALSE;
    }
    
    uint8_t prg_banks = raw_rom[4];
    uint8_t chr_banks = raw_rom[5];
    uint8_t mapper = (raw_rom[6] >> 4) | (raw_rom[7] & 0xF0);
    
    LOGI("ROM: PRG=%d banks, CHR=%d banks, Mapper=%d", prg_banks, chr_banks, mapper);
    
    if (mapper != 1) {
        LOGE("Only MMC1 (mapper 1) supported, got %d", mapper);
        env->ReleaseByteArrayElements(rom_data, (jbyte*)raw_rom, JNI_ABORT);
        return JNI_FALSE;
    }
    
    // Load PRG-ROM
    prg_sz = prg_banks * 16384;
    if (prg_rom_data) delete[] prg_rom_data;
    prg_rom_data = new uint8_t[prg_sz];
    memcpy(prg_rom_data, raw_rom + 16, prg_sz);
    
    // Load CHR-ROM/RAM
    chr_sz = chr_banks > 0 ? chr_banks * 8192 : 8192;
    if (chr_rom_data) delete[] chr_rom_data;
    chr_rom_data = new uint8_t[chr_sz];
    if (chr_banks > 0) {
        memcpy(chr_rom_data, raw_rom + 16 + prg_sz, chr_sz);
    } else {
        memset(chr_rom_data, 0, chr_sz);  // CHR-RAM
    }
    
    env->ReleaseByteArrayElements(rom_data, (jbyte*)raw_rom, JNI_ABORT);
    
    // Initialize subsystems
    ppu_init();
    mapper_init(prg_rom_data, prg_sz, chr_rom_data, chr_sz);
    
    // Power-on state
    memset(ram, 0, sizeof(ram));
    ppu_reset();
    mapper_reset();
    
    // CPU power-on state
    reg_A = 0;
    reg_X = 0;
    reg_Y = 0;
    reg_P = 0x24;
    reg_SP = 0xFD;
    reg_PC = cpu_read_pointer(0xFFFC);  // Reset vector
    cycles_to_run = 0;
    nmi_pending = false;
    
    rom_loaded = true;
    emulator_running = true;
    
    LOGI("ROM loaded! Starting at PC=$%04X", reg_PC);
    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_runFrame(JNIEnv* env, jobject, jobject bitmap) {
    if (!rom_loaded || !emulator_running) return;
    
    std::lock_guard<std::mutex> lock(emu_mutex);
    
    // Run one frame of CPU/PPU
    cpu_run_frame();
    
    // Render to frame buffer
    ppu_render_frame(frame_buffer);
    
    // Copy to Android bitmap
    AndroidBitmapInfo info;
    void* pixels;
    if (AndroidBitmap_getInfo(env, bitmap, &info) == ANDROID_BITMAP_RESULT_SUCCESS) {
        if (AndroidBitmap_lockPixels(env, bitmap, &pixels) == ANDROID_BITMAP_RESULT_SUCCESS) {
            if (info.width == 256 && info.height == 240) {
                memcpy(pixels, frame_buffer, sizeof(frame_buffer));
            } else {
                // Scale if needed
                uint32_t* dest = (uint32_t*)pixels;
                for (uint32_t y = 0; y < info.height; y++) {
                    for (uint32_t x = 0; x < info.width; x++) {
                        uint32_t src_x = (x * 256) / info.width;
                        uint32_t src_y = (y * 240) / info.height;
                        dest[y * info.width + x] = frame_buffer[src_y * 256 + src_x];
                    }
                }
            }
            AndroidBitmap_unlockPixels(env, bitmap);
        }
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_setButton(JNIEnv*, jobject, jint button, jboolean pressed) {
    controller_set_button((uint8_t)button, pressed);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_cleanup(JNIEnv*, jobject) {
    std::lock_guard<std::mutex> lock(emu_mutex);
    emulator_running = false;
    rom_loaded = false;
    
    if (prg_rom_data) {
        delete[] prg_rom_data;
        prg_rom_data = nullptr;
    }
    if (chr_rom_data) {
        delete[] chr_rom_data;
        chr_rom_data = nullptr;
    }
}