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
// GLOBAL STATE
// ============================================================================
uint8_t cpu_ram[0x0800] = {0};
uint8_t ppu_status = 0, ppu_ctrl = 0, ppu_mask = 0, ppu_addr_latch = 0;
uint32_t screen_buffer[256 * 240] = {0};

MapperMMC1 mapper;
bool is_running = false, rom_loaded = false;
jobject main_activity_instance = nullptr;
JavaVM* java_vm = nullptr;

namespace Dispatcher { void execute(); }

// ============================================================================
// SAFE JNI TOAST (Prevents Detached Thread Crash)
// ============================================================================
void make_toast(const char* message) {
    if (!java_vm || !main_activity_instance) return;
    
    JNIEnv* env;
    bool is_attached = false;
    jint get_env_res = java_vm->GetEnv((void**)&env, JNI_VERSION_1_6);
    
    if (get_env_res == JNI_EDETACHED) {
        if (java_vm->AttachCurrentThread(&env, nullptr) != JNI_OK) return;
        is_attached = true;
    }

    jclass clazz = env->GetObjectClass(main_activity_instance);
    jmethodID methodId = env->GetMethodID(clazz, "showToast", "(Ljava/lang/String;)V");
    if (methodId) {
        jstring jmsg = env->NewStringUTF(message);
        env->CallVoidMethod(main_activity_instance, methodId, jmsg);
        env->DeleteLocalRef(jmsg);
    }
    
    if (is_attached) java_vm->DetachCurrentThread();
}

// ============================================================================
// BUS & ENGINE
// ============================================================================
extern "C" {
    void power_on_reset() {
        uint8_t lo = mapper.read_prg(0xFFFC);
        uint8_t hi = mapper.read_prg(0xFFFD);
        reg_PC = (hi << 8) | lo;
        reg_S = 0xFD; reg_P = 0x24;
        LOGI("CPU Reset: PC set to $%04X", reg_PC);
    }

    void nmi_handler() {
        push_stack((reg_PC >> 8) & 0xFF);
        push_stack(reg_PC & 0xFF);
        push_stack(reg_P);
        uint8_t lo = mapper.read_prg(0xFFFA);
        uint8_t hi = mapper.read_prg(0xFFFB);
        reg_PC = (hi << 8) | lo;
        reg_P |= FLAG_I;
    }
}

void engine_loop() {
    power_on_reset();
    is_running = true;
    
    while(is_running) {
        auto frame_start = std::chrono::steady_clock::now();
        
        ppu_status &= ~0x80; 
        for(int i = 0; i < 400; i++) Dispatcher::execute();

        ppu_status |= 0x80; 
        if (ppu_ctrl & 0x80) nmi_handler();

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
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    
    if (!file.is_open()) {
        env->ReleaseStringUTFChars(romPath, path);
        return env->NewStringUTF("File not found");
    }

    // Validation: Dragon Warrior (MMC1) is usually 64KB or 128KB PRG
    std::streamsize size = file.tellg();
    if (size < 32768) { 
        env->ReleaseStringUTFChars(romPath, path);
        return env->NewStringUTF("ROM too small");
    }

    file.seekg(16, std::ios::beg); // Skip iNES Header
    for(int i=0; i<4; i++) {
        file.read((char*)mapper.prg_rom[i], 16384);
    }
    file.close();
    
    env->ReleaseStringUTFChars(romPath, path);
    rom_loaded = true;
    return env->NewStringUTF("Success");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv *env, jobject thiz) {
    if (is_running || !rom_loaded) return;
    std::thread t(engine_loop);
    t.detach();
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv *env, jobject thiz, jobject bitmap) {
    AndroidBitmapInfo info;
    void* pixels;
    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0) return;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;
    
    // Copy our internal NES buffer to the Android Bitmap
    memcpy(pixels, screen_buffer, 256 * 240 * 4);
    
    AndroidBitmap_unlockPixels(env, bitmap);
}
