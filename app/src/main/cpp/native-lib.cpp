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
#define DW_RAM_VBLANK_FLAG   0x002D
#define DW_RAM_FRAME_COUNTER 0x003C

// --- Dispatcher Declaration ---
// These symbols are defined in your recompiled Dispatcher.cpp
namespace Dispatcher { 
    void execute(); 
    void request_nmi(); // Corrected name to match recompiled symbol
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

// Full NES Palette (NTSC)
uint32_t nes_palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00,
    0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFEFB0FF, 0xFF64B0FF, 0xFF9290FF, 0xFFC676FF, 0xFFF36AFF, 0xFFFE6ECC, 0xFFFE8170, 0xFFEA9E22,
    0xFFBCBE00, 0xFF88D100, 0xFF5CE430, 0xFF45E082, 0xFF48CDDE, 0xFF4F4F4F, 0xFF000000, 0xFF000000
};

// --- CPU Global State & Instruction Helpers ---
extern "C" {
    uint16_t reg_PC = 0;
    uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;

    void execute_instruction() { Dispatcher::execute(); }

    void update_nz(uint8_t val) {
        reg_P &= ~(0x82);
        if (val == 0) reg_P |= 0x02;
        if (val & 0x80) reg_P |= 0x80;
    }

    void update_flags_cmp(uint8_t reg, uint8_t val) {
        reg_P &= ~(0x83);
        if (reg >= val) reg_P |= 0x01;
        if (reg == val) reg_P |= 0x02;
        if ((uint8_t)(reg - val) & 0x80) reg_P |= 0x80;
    }

    void cpu_adc(uint8_t val) {
        uint16_t carry = (reg_P & 0x01) ? 1 : 0;
        uint16_t sum = reg_A + val + carry;
        if (~(reg_A ^ val) & (reg_A ^ sum) & 0x80) reg_P |= 0x40; else reg_P &= ~0x40;
        if (sum > 0xFF) reg_P |= 0x01; else reg_P &= ~0x01;
        reg_A = (uint8_t)sum;
        update_nz(reg_A);
    }

    void cpu_sbc(uint8_t val) { cpu_adc(~val); }

    void cpu_bit(uint8_t val) {
        reg_P &= ~(0xC2);
        if ((val & reg_A) == 0) reg_P |= 0x02;
        reg_P |= (val & 0xC0);
    }

    uint8_t cpu_asl(uint8_t val) {
        reg_P = (reg_P & ~0x01) | ((val >> 7) & 0x01);
        uint8_t res = val << 1;
        update_nz(res);
        return res;
    }

    uint8_t cpu_lsr(uint8_t val) {
        reg_P = (reg_P & ~0x01) | (val & 0x01);
        uint8_t res = val >> 1;
        update_nz(res);
        return res;
    }

    uint8_t cpu_rol(uint8_t val) {
        uint8_t old_c = (reg_P & 0x01);
        reg_P = (reg_P & ~0x01) | ((val >> 7) & 0x01);
        uint8_t res = (val << 1) | old_c;
        update_nz(res);
        return res;
    }

    uint8_t cpu_ror(uint8_t val) {
        uint8_t old_c = (reg_P & 0x01);
        reg_P = (reg_P & ~0x01) | (val & 0x01);
        uint8_t res = (val >> 1) | (old_c << 7);
        update_nz(res);
        return res;
    }

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
        else if (addr == 0x4014) {
            uint16_t b = val << 8;
            for(int i=0; i<256; i++) ppu.oam_ram[i] = bus_read(b + i);
        }
        else if (addr == 0x4016) {
            if ((val & 0x01) == 0 && (last_strobe & 0x01) == 1) controller_shift = controller_state;
            last_strobe = val;
        }
        else if (addr >= 0x6000) mapper.write(addr, val);
    }

    uint16_t read_pointer(uint16_t addr) {
        return bus_read(addr) | (bus_read(addr + 1) << 8); 
    }

    uint16_t read_pointer_indexed_x(uint16_t addr) {
        uint8_t zp = (uint8_t)(addr + reg_X);
        return bus_read(zp) | (bus_read((uint8_t)(zp + 1)) << 8);
    }

    uint16_t read_pointer_indexed_y(uint16_t zp_addr) {
        uint16_t lo = bus_read((uint8_t)zp_addr);
        uint16_t hi = bus_read((uint8_t)(zp_addr + 1));
        return (uint16_t)((lo | (hi << 8)) + reg_Y);
    }

    void push_stack(uint8_t val) { cpu_ram[0x0100 | (reg_S--)] = val; }
    uint8_t pop_stack() { return cpu_ram[0x0100 | (++reg_S)]; }
}



void trigger_nmi() {
    push_stack(reg_PC >> 8);
    push_stack(reg_PC & 0xFF);
    push_stack(reg_P);

    // Update Dragon Warrior synchronization flags in RAM
    cpu_ram[DW_RAM_VBLANK_FLAG] = 1;
    cpu_ram[DW_RAM_FRAME_COUNTER]++;

    // Call the NMI request in the recompiled Dispatcher
    Dispatcher::request_nmi(); 
}

// --- Main Engine Loop ---
void engine_loop() {
    while (!rom_loaded && is_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    mapper.reset();
    ppu.reset();
    reg_PC = read_pointer(0xFFFC);

    while (is_running) {
        if (is_paused) { 
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); 
            continue; 
        }

        auto frame_start = std::chrono::steady_clock::now();

        // CPU Execution Slice (Standard NTSC cycles per frame)
        for (int i = 0; i < 29780; i++) {
            Dispatcher::execute();
        }

        ppu.status |= 0x80; 
        if (ppu.ctrl & 0x80) trigger_nmi();

        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            ppu.render_frame(mapper, nes_palette);
        }

        ppu.status &= ~0x80;
        std::this_thread::sleep_until(frame_start + std::chrono::microseconds(16666));
    }
}

// --- JNI Implementation ---

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeTriggerNMI(JNIEnv *env, jobject thiz) {
    if (!rom_loaded || is_paused) return;
    ppu.status |= 0x80;
    if (ppu.ctrl & 0x80) trigger_nmi();
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv *env, jobject thiz, jstring romPath, jstring outDir) {
    const char *path = env->GetStringUTFChars(romPath, nullptr);
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        env->ReleaseStringUTFChars(romPath, path);
        return env->NewStringUTF("Error: Could not open ROM");
    }

    file.seekg(16, std::ios::beg); 
    for (int i = 0; i < 4; i++) file.read((char*)mapper.prg_rom[i], 16384);
    for (int i = 0; i < 4; i++) file.read((char*)mapper.chr_rom[i], 4096);
    file.close();

    rom_loaded = true; 
    env->ReleaseStringUTFChars(romPath, path);
    return env->NewStringUTF("Success");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv *env, jobject thiz, jstring filesDir) {
    if (!is_running) {
        is_running = true;
        std::thread(engine_loop).detach();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv *env, jobject thiz, jobject bitmap) {
    void* pixels;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0) {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        memcpy(pixels, ppu.screen_buffer, 256 * 240 * 4);
        AndroidBitmap_unlockPixels(env, bitmap);
    }
}

extern "C" JNIEXPORT void JNICALL 
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv *env, jobject thiz, jint bit, jboolean p) {
    if (p) controller_state |= (uint8_t)bit; else controller_state &= ~((uint8_t)bit);
}

extern "C" JNIEXPORT void JNICALL Java_com_canc_dwa_MainActivity_nativePauseEngine(JNIEnv *env, jobject thiz) { is_paused = true; }
extern "C" JNIEXPORT void JNICALL Java_com_canc_dwa_MainActivity_nativeResumeEngine(JNIEnv *env, jobject thiz) { is_paused = false; }
