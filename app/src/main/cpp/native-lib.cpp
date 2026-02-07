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

// ============================================================================
// GLOBAL HARDWARE & UI STATE
// ============================================================================
uint8_t cpu_ram[0x0800] = {0};
uint8_t ppu_vram[0x0800] = {0};  
uint8_t palette_ram[32] = {0};
uint8_t oam_ram[256] = {0};
uint32_t screen_buffer[256 * 240] = {0};

// PPU Registers (Required for boot initialization)
uint8_t ppu_status = 0;
uint8_t ppu_ctrl = 0;
uint8_t ppu_mask = 0;
uint8_t ppu_addr_latch = 0;

MapperMMC1 mapper;
bool is_running = false, rom_loaded = false, show_debug_menu = false;
int frame_count = 0;

// JNI Reference for Toasts
jobject main_activity_instance = nullptr;
JavaVM* java_vm = nullptr;

// ============================================================================
// ANDROID UI HELPERS (Toasts & Menus)
// ============================================================================
void make_toast(const char* message) {
    if (!java_vm || !main_activity_instance) return;
    JNIEnv* env;
    // Ensure the thread is attached to the JVM to allow JNI calls
    java_vm->AttachCurrentThread(&env, nullptr);
    jclass clazz = env->GetObjectClass(main_activity_instance);
    jmethodID methodId = env->GetMethodID(clazz, "showToast", "(Ljava/lang/String;)V");
    if (methodId) {
        env->CallVoidMethod(main_activity_instance, methodId, env->NewStringUTF(message));
    }
}

// ============================================================================
// CPU CORE & BUS (Extern "C" for Linker)
// ============================================================================
extern "C" {
    uint16_t reg_PC = 0;
    uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;

    uint8_t bus_read(uint16_t addr) {
        // CPU Internal RAM (0x0000 - 0x1FFF mirrored)
        if (addr < 0x2000) return cpu_ram[addr & 0x07FF];
        
        // PPU Registers (0x2000 - 0x3FFF mirrored)
        if (addr < 0x4000) {
            uint16_t reg = addr & 0x0007;
            if (reg == 2) { // PPUSTATUS ($2002)
                uint8_t s = ppu_status; 
                ppu_status &= ~0x80; // Clear VBlank flag on read
                ppu_addr_latch = 0; 
                return s; 
            }
            return 0;
        }

        // Mapper Space ($6000 - $FFFF)
        if (addr >= 0x6000) return mapper.read_prg(addr);
        
        return 0;
    }

    void bus_write(uint16_t addr, uint8_t val) {
        if (addr < 0x2000) {
            cpu_ram[addr & 0x07FF] = val;
        } else if (addr < 0x4000) {
            uint16_t reg = addr & 0x0007;
            if (reg == 0) ppu_ctrl = val;
            if (reg == 1) ppu_mask = val;
        } else if (addr >= 0x6000) {
            mapper.write(addr, val);
        }
    }

    // Helper functions called by recompiled code
    void update_nz(uint8_t val) {
        reg_P &= ~0x82; // Clear N and Z
        if (val == 0) reg_P |= 0x02;
        if (val & 0x80) reg_P |= 0x82;
    }
}

// ============================================================================
// ENGINE LOGIC
// ============================================================================


void engine_loop() {
    // Wait for ROM to be injected via JNI
    while(!rom_loaded) std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Equivalent to hitting the Reset button
    power_on_reset();
    
    is_running = true;
    while(is_running) {
        auto frame_start = std::chrono::steady_clock::now();

        // 1. VISIBLE FRAME: Run roughly 27k - 29k cycles
        // Dragon Warrior loops on $2002 bit 7; it MUST be 0 here.
        ppu_status &= ~0x80; 
        for(int i=0; i<28000; i++) {
            // In a static recompiled setup, we call the Dispatcher
            // instead of a generic execute_instruction()
            extern void dispatcher_execute(); 
            dispatcher_execute();
        }

        // 2. VBLANK PERIOD: Triggered every ~16.6ms
        ppu_status |= 0x80; // Set VBlank bit
        if (ppu_ctrl & 0x80) {
            // Only trigger NMI if the game has enabled it in PPUCTRL
            nmi_handler(); 
        }

        // 3. UI SYNC
        frame_count++;
        
        // Maintain 60 FPS
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
    // Save a global reference to the Activity so we can call showToast later
    main_activity_instance = env->NewGlobalRef(thiz);

    const char *path = env->GetStringUTFChars(romPath, nullptr);
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return env->NewStringUTF("ROM Load Failed");

    // Skip the 16-byte iNES header and load 4 banks (64KB) of PRG
    file.seekg(16, std::ios::beg);
    for(int i=0; i<4; i++) {
        file.read((char*)mapper.prg_rom[i], 16384);
    }
    file.close();

    rom_loaded = true;
    make_toast("Dragon Warrior ROM Loaded");
    return env->NewStringUTF("Success");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_toggleDebugMenu(JNIEnv *env, jobject thiz) {
    show_debug_menu = !show_debug_menu;
    make_toast(show_debug_menu ? "Debug Overlay: Enabled" : "Debug Overlay: Disabled");
}
