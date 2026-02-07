#include <jni.h>
#include <string>
#include <fstream>
#include <android/log.h>
#include <android/bitmap.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <cstring>
#include <cstdio>
#include "MapperMMC1.h"
#include "recompiled/cpu_shared.h"

#define LOG_TAG "DWA_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// ============================================================================
// SYSTEM STATE
// ============================================================================
uint16_t reg_PC = 0;
uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;
uint8_t cpu_ram[0x0800] = {0};
uint8_t ppu_status = 0, ppu_ctrl = 0, ppu_mask = 0;

uint32_t screen_buffer[256 * 240] = {0};
MapperMMC1 mapper;

// QOL Variables
std::atomic<bool> is_running{false};
bool rom_loaded = false;
float speed_multiplier = 1.0f;
std::string internal_data_path = "";
uint8_t controller_state = 0;
uint8_t latched_controller = 0;

std::mutex state_mutex;

namespace Dispatcher { void execute(); }

// ============================================================================
// BUS INTERFACE (CRITICAL FIXES)
// ============================================================================
extern "C" {
    uint8_t bus_read(uint16_t addr) {
        if (addr < 0x2000) return cpu_ram[addr & 0x07FF];
        
        // PPU Registers
        if (addr >= 0x2000 && addr < 0x4000) {
            uint16_t reg = addr & 0x2007;
            if (reg == 0x2002) { 
                uint8_t t = ppu_status; 
                ppu_status &= ~0x80; // Clear VBlank flag on read
                return t; 
            }
            return 0;
        }
        
        // Controller Port 1
        if (addr == 0x4016) {
            uint8_t ret = (latched_controller & 0x80) ? 1 : 0;
            latched_controller <<= 1;
            return ret | 0x40;
        }
        
        // SRAM (0x6000 - 0x7FFF)
        if (addr >= 0x6000 && addr < 0x8000) return mapper.prg_ram[addr - 0x6000];
        
        // PRG ROM (0x8000 - 0xFFFF)
        if (addr >= 0x8000) return mapper.read_prg(addr);
        
        return 0;
    }

    void bus_write(uint16_t addr, uint8_t val) {
        if (addr < 0x2000) {
            cpu_ram[addr & 0x07FF] = val;
        } else if (addr >= 0x2000 && addr < 0x4000) {
            uint16_t reg = addr & 0x2007;
            if (reg == 0x2000) ppu_ctrl = val;
            if (reg == 0x2001) ppu_mask = val;
        } else if (addr == 0x4014) {
            // OAM DMA (Simplistic implementation for now)
            uint16_t source = val << 8;
            for(int i=0; i<256; i++) { /* OAM logic usually goes here */ }
        } else if (addr == 0x4016) {
            if ((val & 0x01) == 0) latched_controller = controller_state;
        } else if (addr >= 0x6000 && addr < 0x8000) {
            mapper.prg_ram[addr - 0x6000] = val;
        } else if (addr >= 0x8000) {
            mapper.write(addr, val);
        }
    }

    void power_on_reset() {
        reg_PC = (bus_read(0xFFFD) << 8) | bus_read(0xFFFC);
        reg_S = 0xFD; 
        reg_P = 0x24;
        LOGI("CPU Reset. Entry Point: $%04X", reg_PC);
    }

    void nmi_handler() {
        push_stack((reg_PC >> 8) & 0xFF);
        push_stack(reg_PC & 0xFF);
        push_stack(reg_P);
        reg_PC = (bus_read(0xFFFB) << 8) | bus_read(0xFFFA);
        reg_P |= 0x04; // Set Interrupt Disable
    }
}

// ============================================================================
// MAIN ENGINE LOOP
// ============================================================================
void engine_loop() {
    power_on_reset();
    is_running = true;

    while (is_running) {
        auto frame_start = std::chrono::steady_clock::now();

        {
            std::lock_guard<std::mutex> lock(state_mutex);
            
            // Dragon Warrior usually waits for VBlank. 
            // We simulate a frame by running approx 29780 cycles
            for (int i = 0; i < 29780; i++) {
                Dispatcher::execute();
                // A very crude PPU status update to satisfy the ROM's wait loops
                if (i == 27000) {
                    ppu_status |= 0x80; // Trigger VBlank
                    if (ppu_ctrl & 0x80) nmi_handler();
                }
            }
        }

        // Target 60 FPS (16.6ms) adjusted by speed multiplier
        long delay_micros = (long)(16666 / speed_multiplier);
        std::this_thread::sleep_until(frame_start + std::chrono::microseconds(delay_micros));
    }
}

// ============================================================================
// JNI EXPORTS
// ============================================================================
extern "C" {

JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* c_path = env->GetStringUTFChars(romPath, nullptr);
    const char* c_dir = env->GetStringUTFChars(outDir, nullptr);
    internal_data_path = std::string(c_dir);

    std::ifstream file(c_path, std::ios::binary);
    if (!file.is_open()) return env->NewStringUTF("Error: File not found");

    // iNES Header Validation
    char header[16];
    file.read(header, 16);
    if (header[0] != 'N' || header[1] != 'E' || header[2] != 'S') return env->NewStringUTF("Error: Invalid NES File");

    // Load PRG banks (Assuming 4x16KB banks for Dragon Warrior MMC1)
    for (int i = 0; i < 4; i++) {
        file.read((char*)mapper.prg_rom[i], 16384);
    }
    file.close();

    env->ReleaseStringUTFChars(romPath, c_path);
    env->ReleaseStringUTFChars(outDir, c_dir);
    
    rom_loaded = true;
    return env->NewStringUTF("Success");
}

JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz, jstring filesDir) {
    if (is_running) {
        is_running = false; // Stop existing loop
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    std::thread(engine_loop).detach();
}

JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv* env, jobject thiz, jobject bitmap) {
    AndroidBitmapInfo info;
    void* pixels;
    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0) return;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;

    // Simple placeholder to see if engine is alive: Fill screen with a test pattern if rom not running
    if (!is_running) {
        memset(pixels, 0x11, info.width * info.height * 4);
    } else {
        memcpy(pixels, screen_buffer, 256 * 240 * 4);
    }

    AndroidBitmap_unlockPixels(env, bitmap);
}

JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* env, jobject thiz, jint bit, jboolean pressed) {
    if (pressed) controller_state |= (uint8_t)bit; 
    else controller_state &= ~(uint8_t)bit;
}

JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeSetTurbo(JNIEnv* env, jobject thiz, jboolean enabled) {
    speed_multiplier = enabled ? 3.0f : 1.0f;
}

}
