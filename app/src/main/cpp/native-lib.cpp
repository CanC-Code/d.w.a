#include <jni.h>
#include <string>
#include <fstream>
#include <android/log.h>
#include <android/bitmap.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <cstring>
#include "MapperMMC1.h"
#include "PPU.h"
#include "recompiled/cpu_shared.h"

#define LOG_TAG "DWA_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// ============================================================================
// CPU REGISTERS & MEMORY (Declared as extern in cpu_shared.h)
// ============================================================================
uint16_t reg_PC = 0;
uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;
uint8_t cpu_ram[0x0800] = {0};

MapperMMC1 mapper;
PPU ppu;

const uint32_t nes_palette[64] = {
    0xFF545454, 0xFF741E00, 0xFF901008, 0xFF880030, 0xFF640044, 0xFF30005C, 0xFF000454, 0xFF00183C,
    0xFF002A20, 0xFF003A08, 0xFF004000, 0xFF003C00, 0xFF3C3200, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFF989898, 0xFFBC4C08, 0xFFD84030, 0xFFD8285C, 0xFFB02884, 0xFF783090, 0xFF004498, 0xFF005888,
    0xFF006C60, 0xFF007828, 0xFF007800, 0xFF447C00, 0xFF887000, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFECECEC, 0xFFFCA444, 0xFFFF8C64, 0xFFFF7C94, 0xFFE474D0, 0xFFB47CF0, 0xFF6888FC, 0xFF1C9CFC,
    0xFF24B0D0, 0xFF34C090, 0xFF58D054, 0xFFA0D444, 0xFFD8D820, 0xFF343434, 0xFF000000, 0xFF000000,
    0xFFF0F0F0, 0xFFFCDCA8, 0xFFFECBB0, 0xFFFEBCC4, 0xFFFFBCE4, 0xFFE4C4F8, 0xFFD0C8FF, 0xFFA8E0FC,
    0xFFB0E8E4, 0xFFB8F0D0, 0xFFC8F0AC, 0xFFD8F4A0, 0xFFFCF4B8, 0xFFB8B8B8, 0xFF000000, 0xFF000000
};

// ============================================================================
// SYSTEM STATE
// ============================================================================
std::atomic<bool> is_running{false};
std::atomic<bool> rom_ready{false};
float speed_multiplier = 1.0f;
uint8_t controller_state = 0;
uint8_t latched_controller = 0;
std::mutex state_mutex;

// Recompiled Dispatcher Signature
namespace Dispatcher { void execute_at(uint16_t pc); }

// ============================================================================
// BUS INTERFACE (Extern "C" for recompiled visibility)
// ============================================================================
extern "C" {
    uint8_t bus_read(uint16_t addr) {
        if (addr < 0x2000) return cpu_ram[addr & 0x07FF];
        if (addr >= 0x2000 && addr < 0x4000) return ppu.cpu_read(addr, mapper);
        if (addr == 0x4016) {
            uint8_t ret = (latched_controller & 0x80) ? 1 : 0;
            latched_controller <<= 1;
            return ret | 0x40;
        }
        if (addr >= 0x6000) return mapper.read_prg(addr);
        return 0;
    }

    void bus_write(uint16_t addr, uint8_t val) {
        if (addr < 0x2000) {
            cpu_ram[addr & 0x07FF] = val;
        } else if (addr >= 0x2000 && addr < 0x4000) {
            ppu.cpu_write(addr, val, mapper);
        } else if (addr == 0x4014) { // OAM DMA
            uint16_t source = val << 8;
            for (int i = 0; i < 256; i++) {
                ppu.oam_ram[i] = bus_read(source + i);
            }
        } else if (addr == 0x4016) {
            if ((val & 0x01) == 0) latched_controller = controller_state;
        } else if (addr >= 0x6000) {
            mapper.write(addr, val);
        }
    }
}

// ============================================================================
// MAIN ENGINE LOOP
// ============================================================================
void engine_loop() {
    // Wait for JNI to signal ROM extraction is finished
    while (!rom_ready) std::this_thread::sleep_for(std::chrono::milliseconds(20));

    // Power-on Reset: Fetch start address from Vector $FFFC
    reg_PC = (bus_read(0xFFFD) << 8) | bus_read(0xFFFC);
    reg_S = 0xFD; 
    reg_P = 0x24; // Default flags
    is_running = true;
    
    LOGI("Engine Thread Started. Entry PC: 0x%04X", reg_PC);

    while (is_running) {
        auto frame_start = std::chrono::steady_clock::now();

        {
            std::lock_guard<std::mutex> lock(state_mutex);

            // 1. CPU Execution (~29780 cycles per frame)
            // Using a loop count that approximates standard NES frame timing
            for (int i = 0; i < 22000; i++) {
                Dispatcher::execute_at(reg_PC);
            }

            // 2. PPU VBlank Handshake & NMI
            ppu.status |= 0x80; // Set VBlank flag
            if (ppu.ctrl & 0x80) { 
                // NMI Interrupt Vector: $FFFA
                push_stack((reg_PC >> 8) & 0xFF);
                push_stack(reg_PC & 0xFF);
                push_stack(reg_P);
                reg_PC = (bus_read(0xFFFB) << 8) | bus_read(0xFFFA);
                reg_P |= 0x04; // Set Interrupt Disable
            }

            // 3. Render Graphics
            ppu.render_frame(mapper, nes_palette);
            
            // Clear VBlank flag so the game code can wait for the next frame
            ppu.status &= ~0x80; 
        }

        // Maintain 60 FPS
        auto frame_end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(frame_end - frame_start).count();
        long sleep_us = (16666 / speed_multiplier) - elapsed;
        
        if (sleep_us > 0) {
            std::this_thread::sleep_for(std::chrono::microseconds(sleep_us));
        }
    }
}

// ============================================================================
// JNI INTERFACE
// ============================================================================
extern "C" {

JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* c_path = env->GetStringUTFChars(romPath, nullptr);
    std::ifstream file(c_path, std::ios::binary);
    
    if (!file.is_open()) return env->NewStringUTF("Error: ROM Access Denied");

    file.seekg(16); // Skip iNES Header
    
    // Dragon Warrior 1 (MMC1) Setup
    // Load PRG banks into mapper memory
    for (int i = 0; i < 4; i++) {
        file.read((char*)mapper.prg_rom[i], 16384);
    }
    
    // Note: CHR-RAM initialization happens within MapperMMC1::write logic
    
    file.close();
    env->ReleaseStringUTFChars(romPath, c_path);
    
    rom_ready = true;
    return env->NewStringUTF("Success");
}

JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz, jstring filesDir) {
    if (is_running) return;
    std::thread(engine_loop).detach();
}

JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv* env, jobject thiz, jobject bitmap) {
    void* pixels;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;
    
    {
        std::lock_guard<std::mutex> lock(state_mutex);
        memcpy(pixels, ppu.screen_buffer, 256 * 240 * 4);
    }
    
    AndroidBitmap_unlockPixels(env, bitmap);
}

JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* env, jobject thiz, jint bit, jboolean pressed) {
    std::lock_guard<std::mutex> lock(state_mutex);
    if (pressed) controller_state |= (uint8_t)bit; 
    else controller_state &= ~(uint8_t)bit;
}

}
