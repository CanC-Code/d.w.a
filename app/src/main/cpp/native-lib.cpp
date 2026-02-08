#include <jni.h>
#include <string>
#include <fstream>
#include <android/log.h>
#include <android/bitmap.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstring>
#include <atomic>

#include "MapperMMC1.h"
#include "PPU.h"
#include "Controller.h" // Your new file
#include "recompiled/cpu_shared.h"

#define LOG_TAG "DWA_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace Dispatcher {
    void execute();
}

// ============================================================================
// GLOBAL EMULATION STATE
// ============================================================================
extern "C" {
    uint16_t reg_PC = 0;
    uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;
    int64_t total_cycles = 0;
    int32_t cycles_to_run = 0;
    
    // Uses the ATOMIC_BOOL defined in your updated cpu_shared.h
    ATOMIC_BOOL is_running{false}; 
    
    bool nmi_pending = false;
    bool irq_pending = false;
    // We remove the old joypad_state byte here to prevent conflicts
}

uint8_t cpu_ram[0x0800] = {0};
MapperMMC1 mapper;
PPU ppu;
Controller joypad1; // Instance of your new thread-safe controller

const uint32_t nes_palette[64] = {
    0xFF757575, 0xFF8F1B27, 0xFFAB0000, 0xFF9F0047, 0xFF77008F, 0xFF1300AB, 0xFF0000A7, 0xFF000B7F,
    0xFF002F43, 0xFF004700, 0xFF004700, 0xFF003B0B, 0xFF472F00, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFBCBCBC, 0xFF0073EF, 0xFF233BEF, 0xFF8300F3, 0xFFBF00BF, 0xFFE7005B, 0xFFDB2B00, 0xFFCB4F0F,
    0xFF8B7300, 0xFF009700, 0xFF00AB00, 0xFF00933B, 0xFF00838B, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFFFFFFF, 0xFF3FBFFF, 0xFF5F97FF, 0xFFA78BFD, 0xFFF77BFF, 0xFFFF77B7, 0xFFFF7763, 0xFFFF9B3B,
    0xFFF3BF3F, 0xFF83D313, 0xFF4FDF4B, 0xFF58F898, 0xFF00EBDB, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFFFFFFF, 0xFFABE7FF, 0xFFC7D7FF, 0xFFD7CBFF, 0xFFFFC7FF, 0xFFFFC7DB, 0xFFFFBFB3, 0xFFFFD7AB,
    0xFFFFE7A3, 0xFFE3FFA3, 0xFFABF3BF, 0xFFB3FFCF, 0xFF9FFFF3, 0xFF000000, 0xFF000000, 0xFF000000
};

std::mutex buffer_mutex;
std::mutex engine_mutex;
std::atomic<bool> is_paused{false};

// ============================================================================
// CORE BUS IMPLEMENTATION
// ============================================================================

extern "C" {
    void cpu_push(uint8_t v) { cpu_ram[0x0100 | (reg_S--)] = v; }
    uint8_t cpu_pop() { return cpu_ram[0x0100 | (++reg_S)]; }

    uint8_t bus_read(uint16_t a) {
        if (a < 0x2000) return cpu_ram[a & 0x07FF];
        if (a < 0x4000) return ppu.cpu_read(0x2000 | (a & 0x0007), mapper);

        // Input Read using Controller class logic
        if (a == 0x4016) return joypad1.read_serial();

        if (a >= 0x4020) return mapper.read_prg(a);
        return 0;
    }

    void bus_write(uint16_t a, uint8_t v) {
        if (a < 0x2000) {
            cpu_ram[a & 0x07FF] = v;
        } else if (a < 0x4000) {
            ppu.cpu_write(0x2000 | (a & 0x0007), v, mapper);
        } else if (a == 0x4014) { 
            // OAM DMA: High byte of address is 'v', source is v*0x100
            ppu.do_dma(&cpu_ram[v << 8]); 
            cycles_to_run -= 513; 
        } else if (a == 0x4016) { 
            joypad1.write_latch(v);
        } else if (a >= 0x6000) {
            mapper.write(a, v);
        }
    }

    void nmi_handler() {
        cpu_push(reg_PC >> 8); 
        cpu_push(reg_PC & 0xFF); 
        cpu_push(reg_P | 0x20); 
        reg_P |= FLAG_I;
        reg_PC = cpu_read_pointer(0xFFFA);
        cycles_to_run -= 7;
    }

    void execute_instruction() {
        if (nmi_pending) { 
            nmi_pending = false;
            nmi_handler(); 
        } 
        Dispatcher::execute();
    }
}

// ============================================================================
// ENGINE LOOP
// ============================================================================

void engine_loop() {
    std::lock_guard<std::mutex> lock(engine_mutex);

    // Initial check for PRG data
    if (mapper.prg_rom[0][0] == 0 && mapper.prg_rom[3][0] == 0) {
        LOGE("Engine Error: PRG ROM not loaded.");
        is_running = false;
        return;
    }

    ppu.reset();
    mapper.reset();

    reg_PC = cpu_read_pointer(0xFFFC); 
    LOGI("Engine: Starting at $%04X", reg_PC);

    while (is_running) {
        if (is_paused) { 
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); 
            continue; 
        }

        auto frame_start = std::chrono::steady_clock::now();

        ppu.status |= 0x80; // Enter VBlank
        if (ppu.ctrl & 0x80) nmi_pending = true;

        cycles_to_run += 29780; 
        while (cycles_to_run > 0 && is_running) {
            execute_instruction();
        }

        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            ppu.render_frame(mapper, nes_palette);
        }

        ppu.status &= ~0x80; // Exit VBlank
        std::this_thread::sleep_until(frame_start + std::chrono::microseconds(16666));
    }
}

// ============================================================================
// JNI INTERFACE
// ============================================================================

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char *path = env->GetStringUTFChars(romPath, nullptr);
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        env->ReleaseStringUTFChars(romPath, path);
        return env->NewStringUTF("Error: ROM not found");
    }

    uint8_t header[16]; 
    file.read((char*)header, 16);
    if (memcmp(header, "NES\x1A", 4) != 0) {
        env->ReleaseStringUTFChars(romPath, path);
        return env->NewStringUTF("Error: Invalid iNES Header");
    }

    uint8_t prg_banks = header[4];
    uint8_t chr_banks = header[5];

    for (int i = 0; i < prg_banks && i < 4; i++) {
        file.read((char*)mapper.prg_rom[i], 16384);
    }

    if (chr_banks > 0) {
        // CHR-ROM is loaded into the first 8KB of our banked CHR-RAM
        file.read((char*)mapper.chr_rom[0], 4096);
        file.read((char*)mapper.chr_rom[1], 4096);
    }

    file.close();
    env->ReleaseStringUTFChars(romPath, path);
    LOGI("ROM Loaded: %d PRG, %d CHR banks", prg_banks, chr_banks);
    return env->NewStringUTF("Success");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* e, jobject t, jstring d) {
    if (!is_running) {
        is_running = true;
        std::thread(engine_loop).detach();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv* env, jobject thiz, jobject bitmap) {
    AndroidBitmapInfo info; void* pixels;
    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0) return;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;

    {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        memcpy(pixels, ppu.screen_buffer, 256 * 240 * 4);
    }

    AndroidBitmap_unlockPixels(env, bitmap);
}

// Fixed Input Injection
extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* e, jobject t, jint b, jboolean p) {
    static uint8_t current_mask = 0;
    if (p) current_mask |= (uint8_t)b; 
    else current_mask &= ~((uint8_t)b);
    
    // Pass the combined mask to the atomic Controller object
    joypad1.update_state(current_mask);
}
