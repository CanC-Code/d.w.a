#include <jni.h>
#include <string>
#include <vector>
#include <fstream>
#include <android/log.h>
#include <android/bitmap.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstring>
#include "MapperMMC1.h"
#include "PPU.h"
#include "recompiled/cpu_shared.h"

#define LOG_TAG "DWA_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Dragon Warrior specific RAM addresses for synchronization
#define DW_RAM_HEARTBEAT     0x002D
#define DW_RAM_FRAME_COUNTER 0x0012

namespace Dispatcher { 
    void execute(); 
    void request_interrupt_nmi(); 
}

// --- Hardware State ---
uint8_t cpu_ram[0x0800] = {0};
MapperMMC1 mapper;
PPU ppu;
std::mutex buffer_mutex;

bool is_running = false;
bool is_paused = false;
bool rom_loaded = false; 
uint8_t controller_state = 0;
uint8_t controller_shift = 0;
uint8_t last_strobe = 0;

// NES Palette (NTSC Standard)
uint32_t nes_palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00,
    0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFEFB0FF, 0xFF64B0FF, 0xFF9290FF, 0xFFC676FF, 0xFFF36AFF, 0xFFFE6ECC, 0xFFFE8170, 0xFFEA9E22,
    0xFFBCBE00, 0xFF88D100, 0xFF5CE430, 0xFF45E082, 0xFF48CDDE, 0xFF4F4F4F, 0xFF000000, 0xFF000000
};

extern "C" {
    uint16_t reg_PC = 0;
    uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;

    // --- Bus Logic ---
    uint8_t bus_read(uint16_t addr) {
        if (addr < 0x2000) return cpu_ram[addr % 0x0800];
        if (addr >= 0x2000 && addr <= 0x3FFF) return ppu.cpu_read(0x2000 + (addr % 8), mapper);
        if (addr == 0x4016) {
            uint8_t r = (controller_shift & 0x80) >> 7;
            controller_shift <<= 1;
            return 0x40 | r;
        }
        return mapper.read_prg(addr);
    }

    void bus_write(uint16_t addr, uint8_t val) {
        if (addr < 0x2000) cpu_ram[addr % 0x0800] = val;
        else if (addr >= 0x2000 && addr <= 0x3FFF) ppu.cpu_write(0x2000 + (addr % 8), val, mapper);
        else if (addr == 0x4014) { // OAM DMA
            uint16_t base = val << 8;
            for(int i=0; i<256; i++) ppu.oam_ram[i] = bus_read(base + i);
        }
        else if (addr == 0x4016) {
            if ((val & 0x01) == 0 && (last_strobe & 0x01) == 1) controller_shift = controller_state;
            last_strobe = val;
        }
        else if (addr >= 0x6000) mapper.write(addr, val);
    }

    // --- Helpers used by recompiled Bank files ---
    void execute_instruction() { Dispatcher::execute(); }
    void update_nz(uint8_t val) { reg_P &= ~(0x82); if (val == 0) reg_P |= 0x02; if (val & 0x80) reg_P |= 0x80; }
    void update_flags_cmp(uint8_t reg, uint8_t val) {
        reg_P &= ~(0x83);
        if (reg >= val) reg_P |= 0x01;
        if (reg == val) reg_P |= 0x02;
        if ((uint8_t)(reg - val) & 0x80) reg_P |= 0x80;
    }
    void cpu_adc(uint8_t v) { 
        uint16_t c = (reg_P & 0x01); uint16_t s = reg_A + v + c;
        if (~(reg_A ^ v) & (reg_A ^ s) & 0x80) reg_P |= 0x40; else reg_P &= ~0x40;
        if (s > 0xFF) reg_P |= 0x01; else reg_P &= ~0x01;
        reg_A = (uint8_t)s; update_nz(reg_A);
    }
    void cpu_sbc(uint8_t v) { cpu_adc(~v); }
    void push_stack(uint8_t v) { cpu_ram[0x0100 | (reg_S--)] = v; }
    uint8_t pop_stack() { return cpu_ram[0x0100 | (++reg_S)]; }
    uint16_t read_pointer(uint16_t a) { return bus_read(a) | (bus_read(a + 1) << 8); }
}

/**
 * JNI: nativeTriggerNMI
 * This is the critical "Pulse" called from Java 60 times a second.
 */
extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeTriggerNMI(JNIEnv *env, jobject thiz) {
    if (!rom_loaded || is_paused) return;

    // 1. Update Dragon Warrior Heartbeat in RAM
    // The recompiled assembly checks this address to break out of "WaitForNMI"
    cpu_ram[DW_RAM_HEARTBEAT]++;
    cpu_ram[DW_RAM_FRAME_COUNTER]++;

    // 2. Set PPU Status VBlank
    ppu.status |= 0x80;

    // 3. Trigger CPU Interrupt logic
    if (ppu.ctrl & 0x80) {
        Dispatcher::request_interrupt_nmi();
    }
}

/**
 * JNI: nativeUpdateSurface
 * Runs the engine and copies the PPU buffer to the Android Bitmap.
 */
extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv *env, jobject thiz, jobject bitmap) {
    if (!rom_loaded || is_paused) return;

    // Run a slice of instructions (approx 1 frame's worth)
    for (int i = 0; i < 29780; i++) {
        Dispatcher::execute();
    }

    void* pixels;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0) {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        ppu.render_frame(mapper, nes_palette);
        memcpy(pixels, ppu.screen_buffer, 256 * 240 * 4);
        AndroidBitmap_unlockPixels(env, bitmap);
    }
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv *env, jobject thiz, jstring romPath, jstring outDir) {
    const char *path = env->GetStringUTFChars(romPath, nullptr);
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return env->NewStringUTF("Error: Open Failed");

    file.seekg(16, std::ios::beg); // Skip iNES header
    for (int i = 0; i < 4; i++) file.read((char*)mapper.prg_rom[i], 16384);
    for (int i = 0; i < 4; i++) file.read((char*)mapper.chr_rom[i], 4096);
    file.close();

    // Set Initial Reset Vector
    reg_PC = bus_read(0xFFFC) | (bus_read(0xFFFD) << 8);
    rom_loaded = true;
    
    env->ReleaseStringUTFChars(romPath, path);
    return env->NewStringUTF("Success");
}

extern "C" JNIEXPORT void JNICALL Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv *env, jobject thiz, jstring filesDir) {
    is_running = true;
    is_paused = false;
}

extern "C" JNIEXPORT void JNICALL Java_com_canc_dwa_MainActivity_injectInput(JNIEnv *env, jobject thiz, jint bit, jboolean p) {
    if (p) controller_state |= (uint8_t)bit; else controller_state &= ~((uint8_t)bit);
}

extern "C" JNIEXPORT void JNICALL Java_com_canc_dwa_MainActivity_nativePauseEngine(JNIEnv *env, jobject thiz) { is_paused = true; }
extern "C" JNIEXPORT void JNICALL Java_com_canc_dwa_MainActivity_nativeResumeEngine(JNIEnv *env, jobject thiz) { is_paused = false; }
