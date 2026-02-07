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
// SYSTEM STATE & QOL GLOBALS
// ============================================================================
uint16_t reg_PC = 0;
uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;
uint8_t cpu_ram[0x0800] = {0};
uint8_t ppu_status = 0, ppu_ctrl = 0, ppu_mask = 0;

uint32_t screen_buffer[256 * 240] = {0};
MapperMMC1 mapper;

// QOL Variables
bool is_running = false;
bool rom_loaded = false;
bool show_debug_menu = false;
float speed_multiplier = 1.0f; // 2.0f = Turbo mode
std::string internal_data_path = "";
uint8_t controller_state = 0;
uint8_t latched_controller = 0;

std::mutex state_mutex;

namespace Dispatcher { void execute(); }

// ============================================================================
// QOL: PERSISTENCE (SRAM & SAVESTATES)
// ============================================================================

void save_sram() {
    if (internal_data_path.empty()) return;
    std::string path = internal_data_path + "/dw_save.sav";
    std::ofstream file(path, std::ios::binary);
    if (file.is_open()) {
        file.write((char*)mapper.prg_ram, 8192);
        file.close();
        LOGI("SRAM (Save Data) persisted to disk.");
    }
}

void load_sram() {
    std::string path = internal_data_path + "/dw_save.sav";
    std::ifstream file(path, std::ios::binary);
    if (file.is_open()) {
        file.read((char*)mapper.prg_ram, 8192);
        file.close();
        LOGI("SRAM (Save Data) loaded from disk.");
    }
}

// ============================================================================
// BUS INTERFACE
// ============================================================================
extern "C" {
    uint8_t bus_read(uint16_t addr) {
        if (addr < 0x2000) return cpu_ram[addr & 0x07FF];
        if (addr >= 0x2000 && addr < 0x4000) {
            uint16_t reg = addr & 0x2007;
            if (reg == 0x2002) { uint8_t t = ppu_status; ppu_status &= ~0x80; return t; }
            return 0;
        }
        if (addr == 0x4016) {
            uint8_t ret = (latched_controller & 0x80) ? 1 : 0;
            latched_controller <<= 1;
            return ret | 0x40;
        }
        if (addr >= 0x8000) return mapper.read_prg(addr);
        if (addr >= 0x6000) return mapper.prg_ram[addr - 0x6000]; // Direct SRAM access
        return 0;
    }

    void bus_write(uint16_t addr, uint8_t val) {
        if (addr < 0x2000) cpu_ram[addr & 0x07FF] = val;
        else if (addr >= 0x2000 && addr < 0x4000) {
            uint16_t reg = addr & 0x2007;
            if (reg == 0x2000) ppu_ctrl = val;
            if (reg == 0x2001) ppu_mask = val;
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
        reg_S = 0xFD; reg_P = 0x24;
        LOGI("Engine Reset at PC: $%04X", reg_PC);
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
// MAIN ENGINE LOOP (With QOL Fast-Forward)
// ============================================================================
void engine_loop() {
    while (!rom_loaded) std::this_thread::sleep_for(std::chrono::milliseconds(10));
    load_sram();
    power_on_reset();

    is_running = true;
    auto last_sram_save = std::chrono::steady_clock::now();

    while (is_running) {
        auto frame_start = std::chrono::steady_clock::now();

        {
            std::lock_guard<std::mutex> lock(state_mutex);
            // Simulate NES Scanlines / Frame
            ppu_status &= ~0x80;
            for (int i = 0; i < 450; i++) Dispatcher::execute();
            ppu_status |= 0x80;
            if (ppu_ctrl & 0x80) nmi_handler();
        }

        // QOL: Auto-save SRAM every 10 seconds
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - last_sram_save).count() > 10) {
            save_sram();
            last_sram_save = now;
        }

        // QOL: Fast-Forward calculation
        long delay_micros = (long)(16666 / speed_multiplier);
        std::this_thread::sleep_until(frame_start + std::chrono::microseconds(delay_micros));
    }
    save_sram(); // Final save on exit
}

// ============================================================================
// JNI INTERFACE
// ============================================================================
extern "C" {

JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* c_path = env->GetStringUTFChars(romPath, nullptr);
    const char* c_dir = env->GetStringUTFChars(outDir, nullptr);
    internal_data_path = std::string(c_dir);

    std::ifstream file(c_path, std::ios::binary);
    if (!file.is_open()) return env->NewStringUTF("Error: ROM Not Found");

    file.seekg(16, std::ios::beg); // Skip iNES Header
    for (int i = 0; i < 4; i++) {
        file.read((char*)mapper.prg_rom[i], 16384);
        if (file.gcount() < 16384 && i >= 2) memcpy(mapper.prg_rom[i], mapper.prg_rom[i-2], 16384);
    }
    file.close();

    env->ReleaseStringUTFChars(romPath, c_path);
    env->ReleaseStringUTFChars(outDir, c_dir);
    rom_loaded = true;
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
    memcpy(pixels, screen_buffer, 256 * 240 * 4);
    AndroidBitmap_unlockPixels(env, bitmap);
}

JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* env, jobject thiz, jint bit, jboolean pressed) {
    if (pressed) controller_state |= bit; else controller_state &= ~bit;
}

JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_toggleDebugMenu(JNIEnv* env, jobject thiz) {
    show_debug_menu = !show_debug_menu;
}

// --- NEW QOL JNI CALLS (Add these to your MainActivity.java) ---

JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeSetTurbo(JNIEnv* env, jobject thiz, jboolean enabled) {
    speed_multiplier = enabled ? 3.0f : 1.0f; // 3x speed for grinding levels
}

JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeSaveState(JNIEnv* env, jobject thiz) {
    std::lock_guard<std::mutex> lock(state_mutex);
    std::string path = internal_data_path + "/dw_snapshot.state";
    std::ofstream file(path, std::ios::binary);
    if (file.is_open()) {
        file.write((char*)&reg_PC, 2);
        file.write((char*)&reg_A, 1);
        file.write((char*)&reg_X, 1);
        file.write((char*)&reg_Y, 1);
        file.write((char*)&reg_S, 1);
        file.write((char*)&reg_P, 1);
        file.write((char*)cpu_ram, 2048);
        file.write((char*)mapper.prg_ram, 8192);
        file.close();
    }
}

JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeLoadState(JNIEnv* env, jobject thiz) {
    std::lock_guard<std::mutex> lock(state_mutex);
    std::string path = internal_data_path + "/dw_snapshot.state";
    std::ifstream file(path, std::ios::binary);
    if (file.is_open()) {
        file.read((char*)&reg_PC, 2);
        file.read((char*)&reg_A, 1);
        file.read((char*)&reg_X, 1);
        file.read((char*)&reg_Y, 1);
        file.read((char*)&reg_S, 1);
        file.read((char*)&reg_P, 1);
        file.read((char*)cpu_ram, 2048);
        file.read((char*)mapper.prg_ram, 8192);
        file.close();
    }
}
}
