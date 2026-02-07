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

// --- Global Hardware State ---
uint16_t reg_PC = 0;
uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;
uint8_t cpu_ram[0x0800] = {0};
bool is_running = false;
bool is_paused = false;
bool rom_loaded = false; // Guard to prevent execution before loading

// Controller State
uint8_t controller_state = 0;
uint8_t controller_shift = 0;

// Hardware Instances
MapperMMC1 mapper;
PPU ppu;
std::mutex buffer_mutex;

// Recompiled Dispatcher Namespace
namespace Dispatcher { void execute(); }

// NES Color Palette (64 Colors)
const uint32_t nes_palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00,
    0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFFFFFFF, 0xFF64B0FF, 0xFF9290FF, 0xFFC676FF, 0xFFF36AFF, 0xFFFE6ECC, 0xFFFE8170, 0xFFEA9E22,
    0xFFBCBE00, 0xFF88D100, 0xFF5CE430, 0xFF45E082, 0xFF48CDDE, 0xFF4F4F4F, 0xFF000000, 0xFF000000,
    0xFFFFFFFF, 0xFFABE7FF, 0xFFC7D1FF, 0xFFD9C1FF, 0xFFEBB7FF, 0xFFFFB7E9, 0xFFFFC0D0, 0xFFFFCBAB,
    0xFFF1E094, 0xFFD7EB94, 0xFFB9F1B2, 0xFFAEF1D1, 0xFFAFF1F1, 0xFFC8C8C8, 0xFF000000, 0xFF000000
};

// --- Shared 6502 CPU Implementation ---
extern "C" {

void execute_instruction() { Dispatcher::execute(); }

void update_nz(uint8_t val) {
    reg_P &= ~(FLAG_Z | FLAG_N);
    if (val == 0) reg_P |= FLAG_Z;
    if (val & 0x80) reg_P |= FLAG_N;
}

void update_flags_cmp(uint8_t reg, uint8_t val) {
    reg_P &= ~(FLAG_N | FLAG_Z | FLAG_C);
    if (reg >= val) reg_P |= FLAG_C;
    if (reg == val) reg_P |= FLAG_Z;
    if ((uint8_t)(reg - val) & 0x80) reg_P |= FLAG_N;
}

void cpu_adc(uint8_t val) {
    uint16_t carry = (reg_P & FLAG_C) ? 1 : 0;
    uint16_t sum = reg_A + val + carry;
    // Overflow flag logic
    if (~(reg_A ^ val) & (reg_A ^ (uint8_t)sum) & 0x80) reg_P |= FLAG_V; else reg_P &= ~FLAG_V;
    if (sum > 0xFF) reg_P |= FLAG_C; else reg_P &= ~FLAG_C;
    reg_A = (uint8_t)sum;
    update_nz(reg_A);
}

void cpu_sbc(uint8_t val) { cpu_adc(~val); }

void cpu_bit(uint8_t val) {
    reg_P &= ~(FLAG_Z | FLAG_V | FLAG_N);
    if ((val & reg_A) == 0) reg_P |= FLAG_Z;
    reg_P |= (val & 0xC0); // Bits 7 and 6 of value are copied to N and V flags
}

uint8_t cpu_asl(uint8_t val) {
    (val & 0x80) ? reg_P |= FLAG_C : reg_P &= ~FLAG_C;
    uint8_t res = val << 1;
    update_nz(res);
    return res;
}

uint8_t cpu_lsr(uint8_t val) {
    (val & 0x01) ? reg_P |= FLAG_C : reg_P &= ~FLAG_C;
    uint8_t res = val >> 1;
    update_nz(res);
    return res;
}

uint8_t cpu_rol(uint8_t val) {
    uint8_t old_c = (reg_P & FLAG_C) ? 1 : 0;
    (val & 0x80) ? reg_P |= FLAG_C : reg_P &= ~FLAG_C;
    uint8_t res = (val << 1) | old_c;
    update_nz(res);
    return res;
}

uint8_t cpu_ror(uint8_t val) {
    uint8_t old_c = (reg_P & FLAG_C) ? 0x80 : 0;
    (val & 0x01) ? reg_P |= FLAG_C : reg_P &= ~FLAG_C;
    uint8_t res = (val >> 1) | old_c;
    update_nz(res);
    return res;
}

// --- Memory Bus Implementation ---

uint8_t bus_read(uint16_t addr) {
    if (addr < 0x2000) return cpu_ram[addr % 0x0800];
    if (addr >= 0x2000 && addr <= 0x3FFF) return ppu.cpu_read(addr, mapper);
    if (addr >= 0x6000) return mapper.read_prg(addr);
    if (addr == 0x4016) {
        uint8_t ret = (controller_shift & 0x80) >> 7;
        controller_shift <<= 1;
        return ret;
    }
    return 0;
}

void bus_write(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) cpu_ram[addr % 0x0800] = val;
    else if (addr >= 0x2000 && addr <= 0x3FFF) ppu.cpu_write(addr, val, mapper);
    else if (addr == 0x4014) { // OAM DMA
        uint16_t base = val << 8;
        for (int i = 0; i < 256; i++) ppu.oam_ram[i] = bus_read(base + i);
    }
    else if (addr == 0x4016) { 
        if ((val & 0x01) == 0) controller_shift = controller_state; 
    }
    else if (addr >= 0x6000) mapper.write(addr, val);
}

// Helpers for recompiled logic
uint16_t read_pointer(uint16_t addr) { return bus_read(addr) | (bus_read(addr + 1) << 8); }
uint16_t read_pointer_indexed_x(uint16_t zp) { return bus_read((uint8_t)(zp + reg_X)) | (bus_read((uint8_t)(zp + reg_X + 1)) << 8); }
uint16_t read_pointer_indexed_y(uint16_t zp) { return (bus_read(zp) | (bus_read((uint8_t)(zp + 1)) << 8)) + reg_Y; }
void push_stack(uint8_t val) { cpu_ram[0x0100 + (reg_S--)] = val; }
uint8_t pop_stack() { return cpu_ram[0x0100 + (++reg_S)]; }
void clear_screen_to_black() { std::lock_guard<std::mutex> lock(buffer_mutex); std::memset(ppu.screen_buffer, 0, 256 * 240 * 4); }

} // extern "C"

// --- Emulator Cycle ---

void power_on_reset() {
    mapper.reset();
    ppu.reset();
    uint8_t lo = bus_read(0xFFFC);
    uint8_t hi = bus_read(0xFFFD);
    reg_PC = (hi << 8) | lo;
    reg_S = 0xFD; reg_P = 0x24; reg_A = 0; reg_X = 0; reg_Y = 0;
    LOGI("CPU Reset. Entry Point: $%04X", reg_PC);
}

void nmi_handler() {
    push_stack(reg_PC >> 8);
    push_stack(reg_PC & 0xFF);
    push_stack(reg_P);
    reg_PC = bus_read(0xFFFA) | (bus_read(0xFFFB) << 8);
}

void engine_loop() {
    while(!rom_loaded) std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    power_on_reset();
    is_running = true;
    
    while (is_running) {
        if (is_paused) { std::this_thread::sleep_for(std::chrono::milliseconds(16)); continue; }

        auto start = std::chrono::steady_clock::now();
        
        // Execute frame cycles
        for (int i = 0; i < 29780; i++) {
            execute_instruction();
            // In a real NES, NMI is triggered by PPU at start of VBlank
            // Check if PPU wants an NMI (VBlank bit and NMI enabled bit)
            if ((ppu.status & 0x80) && (ppu.ctrl & 0x80)) {
                ppu.status &= ~0x80; // Clear flag to prevent loop
                nmi_handler();
            }
        }

        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            ppu.render_frame(mapper, nes_palette);
        }

        std::this_thread::sleep_until(start + std::chrono::microseconds(16666));
    }
}

// --- JNI Bridge ---

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv *env, jobject thiz, jstring romPath, jstring outDir) {
    const char *path = env->GetStringUTFChars(romPath, nullptr);
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) { 
        env->ReleaseStringUTFChars(romPath, path); 
        return env->NewStringUTF("Error: Could not open ROM file"); 
    }

    // Skip iNES Header
    file.seekg(16, std::ios::beg);
    
    // Dragon Warrior 1: 4x16KB PRG Banks
    for (int i = 0; i < 4; i++) file.read((char*)mapper.prg_rom[i], 16384);
    // 2x8KB CHR Banks (represented as 4x4KB in our header)
    for (int i = 0; i < 4; i++) file.read((char*)mapper.chr_rom[i], 4096);

    file.close();
    env->ReleaseStringUTFChars(romPath, path);
    rom_loaded = true;
    return env->NewStringUTF("Success");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv *env, jobject thiz, jstring filesDir) {
    if (!is_running) {
        std::thread(engine_loop).detach();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv *env, jobject thiz, jobject bitmap) {
    AndroidBitmapInfo info;
    void* pixels;
    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0) return;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;

    {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        std::memcpy(pixels, ppu.screen_buffer, 256 * 240 * 4);
    }

    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInjectInput(JNIEnv *env, jobject thiz, jint buttonBit, jboolean isPressed) {
    if (isPressed) controller_state |= (uint8_t)buttonBit;
    else controller_state &= ~((uint8_t)buttonBit);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativePauseEngine(JNIEnv *env, jobject thiz) { is_paused = true; }

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeResumeEngine(JNIEnv *env, jobject thiz) { is_paused = false; }
