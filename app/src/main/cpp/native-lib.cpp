#include <jni.h>
#include <string>
#include <fstream>
#include <android/log.h>
#include <android/bitmap.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstring>
#include <cstdio>
#include "MapperMMC1.h"
#include "recompiled/cpu_shared.h"

#define LOG_TAG "DWA_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// ============================================================================
// GLOBAL CPU & EMULATOR STATE
// ============================================================================
uint16_t reg_PC = 0;
uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;

uint8_t cpu_ram[0x0800] = {0};
uint8_t ppu_status = 0, ppu_ctrl = 0, ppu_mask = 0, ppu_addr_latch = 0;
uint8_t controller_state = 0; // Standard NES controller bitmask

uint32_t screen_buffer[256 * 240] = {0};

MapperMMC1 mapper;
bool is_running = false, rom_loaded = false, show_debug_menu = false;
int frame_count = 0;

jobject main_activity_instance = nullptr;
JavaVM* java_vm = nullptr;

namespace Dispatcher { void execute(); }

// ============================================================================
// DEBUG RENDERER (Manual Pixel Font)
// ============================================================================
const uint8_t font5x7[] = {
    0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
    0x00, 0x42, 0x7F, 0x40, 0x00, // 1
    0x42, 0x61, 0x51, 0x49, 0x46, // 2
    0x21, 0x41, 0x45, 0x4B, 0x31, // 3
    0x18, 0x14, 0x12, 0x7F, 0x10, // 4
    0x27, 0x45, 0x45, 0x45, 0x39, // 5
    0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
    0x01, 0x71, 0x09, 0x05, 0x03, // 7
    0x36, 0x49, 0x49, 0x49, 0x36, // 8
    0x06, 0x49, 0x49, 0x29, 0x1E, // 9
    0x7E, 0x11, 0x11, 0x11, 0x7E, // A
    0x7F, 0x49, 0x49, 0x49, 0x36, // B
    0x3E, 0x41, 0x41, 0x41, 0x22, // C
    0x7F, 0x41, 0x41, 0x22, 0x1C, // D
    0x7F, 0x49, 0x49, 0x49, 0x41, // E
    0x7F, 0x09, 0x09, 0x09, 0x01  // F
};

void draw_char(int x, int y, char c, uint32_t color) {
    int idx = -1;
    if (c >= '0' && c <= '9') idx = (c - '0') * 5;
    else if (c >= 'A' && c <= 'F') idx = (10 + (c - 'A')) * 5;
    if (idx == -1) return;
    for (int i = 0; i < 5; i++) {
        uint8_t line = font5x7[idx + i];
        for (int j = 0; j < 7; j++) {
            if (line & (1 << j)) {
                if ((x + i) < 256 && (y + j) < 240)
                    screen_buffer[(y + j) * 256 + (x + i)] = color;
            }
        }
    }
}

void draw_debug_ui() {
    char buf[16];
    sprintf(buf, "%04X", reg_PC);
    for(int i=0; i<4; i++) draw_char(10 + (i*6), 10, buf[i], 0xFFFF0000);
}

// ============================================================================
// BUS INTERFACE (Critical for Dragon Warrior/MMC1)
// ============================================================================
extern "C" {
    uint8_t bus_read(uint16_t addr) {
        if (addr < 0x2000) return cpu_ram[addr & 0x07FF];
        if (addr == 0x4016) { // Read Controller
            uint8_t ret = (controller_state & 0x80) ? 1 : 0;
            controller_state <<= 1;
            return ret;
        }
        if (addr >= 0x6000) return mapper.read_prg(addr);
        return 0;
    }

    void bus_write(uint16_t addr, uint8_t val) {
        if (addr < 0x2000) cpu_ram[addr & 0x07FF] = val;
        else if (addr >= 0x6000) mapper.write(addr, val);
    }

    void power_on_reset() {
        uint8_t lo = bus_read(0xFFFC);
        uint8_t hi = bus_read(0xFFFD);
        reg_PC = (hi << 8) | lo;
        reg_S = 0xFD; reg_P = 0x24;
        LOGI("Dragon Warrior Reset: PC=$%04X", reg_PC);
    }

    void nmi_handler() {
        push_stack((reg_PC >> 8) & 0xFF);
        push_stack(reg_PC & 0xFF);
        push_stack(reg_P);
        uint8_t lo = bus_read(0xFFFA);
        uint8_t hi = bus_read(0xFFFB);
        reg_PC = (hi << 8) | lo;
        reg_P |= FLAG_I;
    }
}

// ============================================================================
// ENGINE LOOP
// ============================================================================
void engine_loop() {
    while(!rom_loaded) std::this_thread::sleep_for(std::chrono::milliseconds(50));
    power_on_reset();

    is_running = true;
    while(is_running) {
        auto frame_start = std::chrono::steady_clock::now();

        // Clear only if needed, or the game will flicker
        // memset(screen_buffer, 0, sizeof(screen_buffer));

        ppu_status &= ~0x80; 
        for(int i = 0; i < 400; i++) Dispatcher::execute();

        ppu_status |= 0x80; 
        if (ppu_ctrl & 0x80) nmi_handler();

        if (show_debug_menu) draw_debug_ui();

        frame_count++;
        std::this_thread::sleep_until(frame_start + std::chrono::microseconds(16666));
    }
}

// ============================================================================
// JNI EXPORTS
// ============================================================================
extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    java_vm = vm;
    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv *env, jobject thiz, jstring romPath, jstring outDir) {
    if (main_activity_instance) env->DeleteGlobalRef(main_activity_instance);
    main_activity_instance = env->NewGlobalRef(thiz);

    const char *path = env->GetStringUTFChars(romPath, nullptr);
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return env->NewStringUTF("Fail: File Not Found");

    file.seekg(16, std::ios::beg); // Skip iNES header
    
    // Dragon Warrior is 64KB PRG. We must fill all 4 banks to avoid "empty" memory.
    for(int i=0; i<4; i++) {
        file.read((char*)mapper.prg_rom[i], 16384);
        if (file.gcount() < 16384) {
            LOGI("Mirroring ROM Bank %d", i);
            memcpy(mapper.prg_rom[i], mapper.prg_rom[i%2], 16384);
        }
    }
    file.close();

    env->ReleaseStringUTFChars(romPath, path);
    rom_loaded = true;
    return env->NewStringUTF("Success");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv *env, jobject thiz, jstring filesDir) {
    if (is_running) return;
    std::thread t(engine_loop);
    t.detach();
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv *env, jobject thiz, jobject bitmap) {
    void* pixels;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;
    memcpy(pixels, screen_buffer, 256 * 240 * 4);
    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv *env, jobject thiz, jint buttonBit, jboolean isPressed) {
    if (isPressed) controller_state |= buttonBit;
    else controller_state &= ~buttonBit;
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_toggleDebugMenu(JNIEnv *env, jobject thiz) {
    show_debug_menu = !show_debug_menu;
}
