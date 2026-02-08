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

// Subsystem Links
extern void ppu_init();
extern void ppu_reset();
extern void ppu_render_frame(uint32_t* pixels);
extern void ppu_step();
extern uint8_t ppu_read_register(uint16_t addr);
extern void ppu_write_register(uint16_t addr, uint8_t value);

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
// GLOBAL CPU STATE - Wrapped in extern "C" to match header linkage
// ============================================================================
extern "C" {
    uint8_t reg_A = 0;
    uint8_t reg_X = 0;
    uint8_t reg_Y = 0;
    uint8_t reg_P = 0x24; 
    uint8_t reg_SP = 0xFD; 
    uint16_t reg_PC = 0;
    int cycles_to_run = 0;
}

// Memory Layout
static uint8_t ram[0x0800];
static uint8_t sram[0x2000]; 
static uint8_t* prg_rom_data = nullptr;
static uint8_t* chr_rom_data = nullptr;
static size_t prg_sz = 0;
static size_t chr_sz = 0;

static std::atomic<bool> emulator_running{false};
static std::mutex emu_mutex;
static bool rom_loaded = false;
static uint32_t frame_buffer[256 * 240];

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
        // OAM DMA Transfer
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
// EMULATION CORE LOGIC
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
        LOGE("CPU PC Escaped ROM: $%04X", pc);
        emulator_running = false;
    }
}

static void cpu_run_frame() {
    cycles_to_run += 29780; // Standard NTSC frame cycles
    while (cycles_to_run > 0 && emulator_running) {
        uint16_t prev_pc = reg_PC;
        execute_recompiled(reg_PC);
        
        if (reg_PC == prev_pc) {
            LOGE("CPU Stuck at $%04X", reg_PC);
            reg_PC++; // Attempt escape
        }

        // PPU clocks at 3x CPU rate
        for (int i = 0; i < 3; i++) ppu_step();
    }
}

// ============================================================================
// JNI INTERFACE
// ============================================================================

extern "C" JNIEXPORT jboolean JNICALL
Java_com_canc_dwa_MainActivity_loadROM(JNIEnv* env, jobject, jbyteArray rom_data) {
    std::lock_guard<std::mutex> lock(emu_mutex);
    jsize len = env->GetArrayLength(rom_data);
    if (len < 16) return JNI_FALSE;

    uint8_t* raw_rom = (uint8_t*)env->GetByteArrayElements(rom_data, nullptr);
    if (memcmp(raw_rom, "NES\x1A", 4) != 0) {
        env->ReleaseByteArrayElements(rom_data, (jbyte*)raw_rom, JNI_ABORT);
        return JNI_FALSE;
    }

    uint8_t prg_banks = raw_rom[4];
    uint8_t chr_banks = raw_rom[5];
    
    prg_sz = prg_banks * 16384;
    if (prg_rom_data) delete[] prg_rom_data;
    prg_rom_data = new uint8_t[prg_sz];
    memcpy(prg_rom_data, raw_rom + 16, prg_sz);

    chr_sz = chr_banks > 0 ? chr_banks * 8192 : 8192;
    if (chr_rom_data) delete[] chr_rom_data;
    chr_rom_data = new uint8_t[chr_sz];
    if (chr_banks > 0) memcpy(chr_rom_data, raw_rom + 16 + prg_sz, chr_sz);
    else memset(chr_rom_data, 0, chr_sz);

    env->ReleaseByteArrayElements(rom_data, (jbyte*)raw_rom, JNI_ABORT);

    ppu_init();
    mapper_init(prg_rom_data, prg_sz, chr_rom_data, chr_sz);
    
    // Power-on State
    memset(ram, 0, sizeof(ram));
    ppu_reset();
    mapper_reset();
    
    reg_PC = cpu_read_pointer(0xFFFC); // Reset Vector
    reg_P = 0x24;
    reg_SP = 0xFD;
    
    rom_loaded = true;
    emulator_running = true;
    LOGI("Dragon Warrior Recompiled: Booting at $%04X", reg_PC);
    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_runFrame(JNIEnv* env, jobject, jobject bitmap) {
    if (!rom_loaded || !emulator_running) return;
    std::lock_guard<std::mutex> lock(emu_mutex);

    cpu_run_frame();
    ppu_render_frame(frame_buffer);

    AndroidBitmapInfo info;
    void* pixels;
    if (AndroidBitmap_getInfo(env, bitmap, &info) == ANDROID_BITMAP_RESULT_SUCCESS) {
        if (AndroidBitmap_lockPixels(env, bitmap, &pixels) == ANDROID_BITMAP_RESULT_SUCCESS) {
            memcpy(pixels, frame_buffer, 256 * 240 * 4);
            AndroidBitmap_unlockPixels(env, bitmap);
        }
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_setButton(JNIEnv*, jobject, jint b, jboolean p) {
    controller_set_button((uint8_t)b, p);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_cleanup(JNIEnv*, jobject) {
    std::lock_guard<std::mutex> lock(emu_mutex);
    emulator_running = false;
    rom_loaded = false;
    if (prg_rom_data) { delete[] prg_rom_data; prg_rom_data = nullptr; }
    if (chr_rom_data) { delete[] chr_rom_data; chr_rom_data = nullptr; }
}
