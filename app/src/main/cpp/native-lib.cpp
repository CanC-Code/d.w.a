#include <jni.h>
#include <string>
#include <fstream>
#include <android/log.h>
#include <android/bitmap.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstring>
#include "MapperMMC1.h"
#include "recompiled/cpu_shared.h"

#define LOG_TAG "DWA_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// ============================================================================
// GLOBAL HARDWARE & UI STATE
// ============================================================================
uint8_t cpu_ram[0x0800] = {0};
uint8_t ppu_vram[0x0800] = {0};  
uint8_t palette_ram[32] = {0};
uint8_t oam_ram[256] = {0};
uint32_t screen_buffer[256 * 240] = {0};

uint8_t ppu_status = 0;
uint8_t ppu_ctrl = 0;
uint8_t ppu_mask = 0;
uint8_t ppu_addr_latch = 0;

MapperMMC1 mapper;
bool is_running = false, rom_loaded = false, show_debug_menu = false;
int frame_count = 0;

jobject main_activity_instance = nullptr;
JavaVM* java_vm = nullptr;

namespace Dispatcher { void execute(); }

// ============================================================================
// ANDROID UI HELPERS
// ============================================================================
void make_toast(const char* message) {
    if (!java_vm || !main_activity_instance) return;
    JNIEnv* env;
    jint res = java_vm->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (res == JNI_EDETACHED) java_vm->AttachCurrentThread(&env, nullptr);

    jclass clazz = env->GetObjectClass(main_activity_instance);
    jmethodID methodId = env->GetMethodID(clazz, "showToast", "(Ljava/lang/String;)V");
    if (methodId) {
        env->CallVoidMethod(main_activity_instance, methodId, env->NewStringUTF(message));
    }
}

// ============================================================================
// CPU BUS & INTERRUPTS
// ============================================================================
extern "C" {
    uint16_t reg_PC = 0;
    uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;

    uint8_t bus_read(uint16_t addr) {
        if (addr < 0x2000) return cpu_ram[addr & 0x07FF];
        if (addr < 0x4000) {
            uint16_t reg = addr & 0x0007;
            if (reg == 2) { 
                uint8_t s = ppu_status; 
                ppu_status &= ~0x80; 
                ppu_addr_latch = 0; 
                return s; 
            }
            return 0;
        }
        if (addr >= 0x6000) return mapper.read_prg(addr);
        return 0;
    }

    void bus_write(uint16_t addr, uint8_t val) {
        if (addr < 0x2000) cpu_ram[addr & 0x07FF] = val;
        else if (addr < 0x4000) {
            uint16_t reg = addr & 0x0007;
            if (reg == 0) ppu_ctrl = val;
            if (reg == 1) ppu_mask = val;
        }
        else if (addr >= 0x6000) mapper.write(addr, val);
    }

    void power_on_reset() {
        // Reset vector is at $FFFC-$FFFD
        uint8_t lo = bus_read(0xFFFC);
        uint8_t hi = bus_read(0xFFFD);
        reg_PC = (hi << 8) | lo;
        
        reg_A = reg_X = reg_Y = 0;
        reg_S = 0xFD;
        reg_P = 0x24;
        LOGI("Reset Vector: $%04X", reg_PC);
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
// ENGINE LOGIC
// ============================================================================


void engine_loop() {
    // Wait for ROM data to be fully loaded into mapper
    while(!rom_loaded) std::this_thread::sleep_for(std::chrono::milliseconds(10));

    power_on_reset();
    make_toast("Dragon Warrior Initialized");

    is_running = true;
    while(is_running) {
        auto frame_start = std::chrono::steady_clock::now();

        // 1. Visible scanlines
        ppu_status &= ~0x80; 
        for(int i = 0; i < 500; i++) {
            Dispatcher::execute();
        }

        // 2. Vertical Blank
        ppu_status |= 0x80; 
        if (ppu_ctrl & 0x80) nmi_handler();

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
    if (!file.is_open()) {
        env->ReleaseStringUTFChars(romPath, path);
        return env->NewStringUTF("ROM Load Failed");
    }

    file.seekg(16, std::ios::beg); // Skip iNES header
    for(int i=0; i<4; i++) file.read((char*)mapper.prg_rom[i], 16384);
    file.close();

    env->ReleaseStringUTFChars(romPath, path);
    rom_loaded = true;
    return env->NewStringUTF("Success");
}

// This should be called by Java after nativeExtractRom returns Success
extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv *env, jobject thiz) {
    if (is_running) return;
    std::thread engine_thread(engine_loop);
    engine_thread.detach(); 
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_toggleDebugMenu(JNIEnv *env, jobject thiz) {
    show_debug_menu = !show_debug_menu;
    make_toast(show_debug_menu ? "Debug Menu: ON" : "Debug Menu: OFF");
}
