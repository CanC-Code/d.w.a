#include <jni.h>
#include <android/bitmap.h>
#include <android/log.h>
#include <cstring>
#include <cstdint>
#include <atomic>
#include <mutex>

#define LOG_TAG "DWA_Native"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// External declarations
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

extern void controller_set_button(uint8_t button, bool pressed);
extern uint8_t controller_read();
extern void controller_write(uint8_t value);

namespace recompiled {
    extern void execute_at(uint16_t pc);
}

// CPU State - shared with recompiled code
uint8_t reg_A = 0;
uint8_t reg_X = 0;
uint8_t reg_Y = 0;
uint8_t reg_P = 0x24;  // IRQ disabled on startup
uint8_t reg_SP = 0xFD;
uint16_t reg_PC = 0;
int cycles_to_run = 0;

// RAM
static uint8_t ram[0x800];
static uint8_t sram[0x2000];  // Battery-backed save RAM

// ROM storage
static uint8_t* prg_rom = nullptr;
static uint8_t* chr_rom = nullptr;
static size_t prg_size = 0;
static size_t chr_size = 0;

// Emulation state
static std::atomic<bool> emulator_running{false};
static std::mutex emu_mutex;
static bool rom_loaded = false;

// Frame buffer (256x240 NES resolution)
static uint32_t frame_buffer[256 * 240];

// CPU Bus Read
uint8_t bus_read(uint16_t addr) {
    if (addr < 0x2000) {
        return ram[addr & 0x7FF];  // RAM mirrored
    } else if (addr < 0x4000) {
        return ppu_read_register(0x2000 | (addr & 0x7));  // PPU registers mirrored
    } else if (addr == 0x4016) {
        return controller_read();
    } else if (addr >= 0x6000 && addr < 0x8000) {
        return sram[addr - 0x6000];  // SRAM
    } else if (addr >= 0x8000) {
        return mapper_read(addr);  // ROM through mapper
    }
    return 0;
}

// CPU Bus Write
void bus_write(uint16_t addr, uint8_t value) {
    if (addr < 0x2000) {
        ram[addr & 0x7FF] = value;
    } else if (addr < 0x4000) {
        ppu_write_register(0x2000 | (addr & 0x7), value);
    } else if (addr == 0x4014) {
        // OAM DMA
        uint16_t dma_addr = value << 8;
        for (int i = 0; i < 256; i++) {
            uint8_t byte = bus_read(dma_addr + i);
            ppu_write_register(0x2004, byte);
        }
        cycles_to_run -= 513;  // DMA takes 513 cycles
    } else if (addr == 0x4016) {
        controller_write(value);
    } else if (addr >= 0x6000 && addr < 0x8000) {
        sram[addr - 0x6000] = value;
    } else if (addr >= 0x8000) {
        mapper_write(addr, value);
    }
}

// CPU Helper functions (used by recompiled code)
extern "C" {
    void cpu_push(uint8_t value) {
        bus_write(0x100 | reg_SP, value);
        reg_SP--;
    }

    uint8_t cpu_pop() {
        reg_SP++;
        return bus_read(0x100 | reg_SP);
    }

    uint16_t cpu_read_pointer(uint16_t addr) {
        uint8_t lo = bus_read(addr);
        uint8_t hi = bus_read(addr + 1);
        return lo | (hi << 8);
    }

    void update_nz(uint8_t value) {
        if (value == 0) reg_P |= 0x02;   // Zero flag
        else reg_P &= ~0x02;
        if (value & 0x80) reg_P |= 0x80; // Negative flag
        else reg_P &= ~0x80;
    }
}

// Reset CPU to power-on state
static void cpu_reset() {
    reg_A = 0;
    reg_X = 0;
    reg_Y = 0;
    reg_P = 0x24;  // IRQ disabled
    reg_SP = 0xFD;
    
    // Read reset vector from $FFFC
    reg_PC = cpu_read_pointer(0xFFFC);
    LOGI("CPU Reset: PC set to $%04X", reg_PC);
    
    cycles_to_run = 0;
    memset(ram, 0, sizeof(ram));
}

// Execute CPU for one frame (29780 cycles @ 1.79 MHz)
static void cpu_run_frame() {
    cycles_to_run = 29780;
    
    while (cycles_to_run > 0) {
        recompiled::execute_at(reg_PC);
        
        // Step PPU (3 PPU cycles per CPU cycle)
        for (int i = 0; i < 3; i++) {
            ppu_step();
        }
    }
}

// JNI: Load ROM
extern "C" JNIEXPORT jboolean JNICALL
Java_com_canc_dwa_MainActivity_loadROM(JNIEnv* env, jobject, jbyteArray rom_data) {
    std::lock_guard<std::mutex> lock(emu_mutex);
    
    LOGI("Loading ROM...");
    
    jsize rom_length = env->GetArrayLength(rom_data);
    if (rom_length < 16) {
        LOGE("ROM too small");
        return JNI_FALSE;
    }
    
    uint8_t* rom_bytes = (uint8_t*)env->GetByteArrayElements(rom_data, nullptr);
    
    // Verify iNES header
    if (rom_bytes[0] != 'N' || rom_bytes[1] != 'E' || rom_bytes[2] != 'S' || rom_bytes[3] != 0x1A) {
        LOGE("Invalid NES header");
        env->ReleaseByteArrayElements(rom_data, (jbyte*)rom_bytes, JNI_ABORT);
        return JNI_FALSE;
    }
    
    // Parse header
    uint8_t prg_banks = rom_bytes[4];  // 16KB banks
    uint8_t chr_banks = rom_bytes[5];  // 8KB banks
    uint8_t mapper_num = (rom_bytes[6] >> 4) | (rom_bytes[7] & 0xF0);
    
    LOGI("PRG Banks: %d, CHR Banks: %d, Mapper: %d", prg_banks, chr_banks, mapper_num);
    
    if (mapper_num != 1) {
        LOGE("Only MMC1 (mapper 1) is supported, got mapper %d", mapper_num);
        env->ReleaseByteArrayElements(rom_data, (jbyte*)rom_bytes, JNI_ABORT);
        return JNI_FALSE;
    }
    
    // Allocate and copy PRG-ROM
    prg_size = prg_banks * 16384;
    if (prg_rom) delete[] prg_rom;
    prg_rom = new uint8_t[prg_size];
    memcpy(prg_rom, rom_bytes + 16, prg_size);
    
    // Allocate and copy CHR-ROM (or RAM if chr_banks == 0)
    chr_size = chr_banks > 0 ? chr_banks * 8192 : 8192;
    if (chr_rom) delete[] chr_rom;
    chr_rom = new uint8_t[chr_size];
    if (chr_banks > 0) {
        memcpy(chr_rom, rom_bytes + 16 + prg_size, chr_size);
    } else {
        memset(chr_rom, 0, chr_size);  // CHR-RAM
    }
    
    env->ReleaseByteArrayElements(rom_data, (jbyte*)rom_bytes, JNI_ABORT);
    
    // Initialize subsystems
    ppu_init();
    mapper_init(prg_rom, prg_size, chr_rom, chr_size);
    
    // Reset to power-on state
    ppu_reset();
    mapper_reset();
    cpu_reset();
    
    rom_loaded = true;
    emulator_running = true;
    
    LOGI("ROM loaded successfully!");
    return JNI_TRUE;
}

// JNI: Run one frame
extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_runFrame(JNIEnv* env, jobject, jobject bitmap) {
    if (!rom_loaded || !emulator_running) return;
    
    std::lock_guard<std::mutex> lock(emu_mutex);
    
    // Clear frame buffer
    memset(frame_buffer, 0, sizeof(frame_buffer));
    
    // Run CPU for one frame
    cpu_run_frame();
    
    // Render PPU output to frame buffer
    ppu_render_frame(frame_buffer);
    
    // Copy frame buffer to Android bitmap
    AndroidBitmapInfo info;
    void* pixels;
    if (AndroidBitmap_getInfo(env, bitmap, &info) == ANDROID_BITMAP_RESULT_SUCCESS) {
        if (AndroidBitmap_lockPixels(env, bitmap, &pixels) == ANDROID_BITMAP_RESULT_SUCCESS) {
            // Scale 256x240 to bitmap size (should be 256x240 or larger)
            if (info.width == 256 && info.height == 240) {
                memcpy(pixels, frame_buffer, sizeof(frame_buffer));
            } else {
                // Simple nearest-neighbor scaling if needed
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

// JNI: Input handling
extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_setButton(JNIEnv*, jobject, jint button, jboolean pressed) {
    controller_set_button((uint8_t)button, pressed);
}

// JNI: Cleanup
extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_cleanup(JNIEnv*, jobject) {
    std::lock_guard<std::mutex> lock(emu_mutex);
    emulator_running = false;
    rom_loaded = false;
    
    if (prg_rom) {
        delete[] prg_rom;
        prg_rom = nullptr;
    }
    if (chr_rom) {
        delete[] chr_rom;
        chr_rom = nullptr;
    }
}