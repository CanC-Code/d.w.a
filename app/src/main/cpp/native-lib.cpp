#include <jni.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <android/log.h>
#include <android/bitmap.h>
#include <aaudio/AAudio.h>
#include <thread>
#include <chrono>
#include <cstring>

#define LOG_TAG "DWA_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// --- Hardware State ---
uint8_t cpu_ram[0x0800];
uint8_t prg_rom[4][16384];             
uint8_t chr_rom[8192];                 
uint8_t sram[0x2000];                  
std::string global_save_path;
uint32_t screen_buffer[256 * 240];

[span_4](start_span)// NES Palette definition[span_4](end_span)
uint32_t nes_palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4142FF, 0xFF7C1AFF, 0xFFB513F9, 0xFFD210BE, 0xFFD42C44, 0xFFB84B00,
    0xFF816D00, 0xFF458800, 0xFF129800, 0xFF009923, 0xFF008985, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFFFFFFF, 0xFF64B0FF, 0xFF9290FF, 0xFFC676FF, 0xFFF35AFF, 0xFFFE66CC, 0xFFFE7373, 0xFFF38913,
    0xFFD3AB00, 0xFFA1D000, 0xFF7FDE00, 0xFF72E248, 0xFF72D1AF, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFFFFFFF, 0xFFC0DFFF, 0xFFD3D2FF, 0xFFE8C8FF, 0xFFFBC2FF, 0xFFFEC4EA, 0xFFFECCC5, 0xFFF7D8A5,
    0xFFE4E594, 0xFFCFEF96, 0xFFBDF4AB, 0xFFB3F3CC, 0xFFB5EBF2, 0xFF000000, 0xFF000000, 0xFF000000
};

[span_5](start_span)// ... (Audio engine and CPU logic remain as per your existing code) [cite: 70-105]

extern "C" {

[cite_start]// FIX: Added missing extraction logic[span_5](end_span)
JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* cRomPath = env->GetStringUTFChars(romPath, nullptr);
    const char* cOutDir = env->GetStringUTFChars(outDir, nullptr);
    
    std::ifstream rom(cRomPath, std::ios::binary);
    if (!rom) return env->NewStringUTF("Error: Could not open ROM file");

    // Basic iNES extraction (Skipping 16-byte header)
    rom.seekg(16, std::ios::beg);
    std::string outDirStr(cOutDir);

    for (int i = 0; i < 4; i++) {
        char buffer[16384];
        rom.read(buffer, 16384);
        [span_6](start_span)// Corrected naming to match Java: Bank00.bin, Bank01.bin, etc.[span_6](end_span)
        std::ofstream bank(outDirStr + "/Bank0" + std::to_string(i) + ".bin", std::ios::binary);
        bank.write(buffer, 16384);
        bank.close();
    }

    env->ReleaseStringUTFChars(romPath, cRomPath);
    env->ReleaseStringUTFChars(outDir, cOutDir);
    return env->NewStringUTF("Success: Banks Extracted");
}

JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz, jstring filesDir) {
    const char* cPath = env->GetStringUTFChars(filesDir, nullptr);
    std::string pathStr(cPath);
    global_save_path = pathStr + "/hero.sav";

    [span_7](start_span)// Corrected loading loop to match extraction names[span_7](end_span)
    for(int i = 0; i < 4; i++) {
        std::ifstream in(pathStr + "/Bank0" + std::to_string(i) + ".bin", std::ios::binary);
        if (in.is_open()) {
            in.read((char*)prg_rom[i], 16384);
            in.close();
        }
    }
    
    [span_8](start_span)// Clear screen buffer to prevent garbage pixels[span_8](end_span)
    memset(screen_buffer, 0, sizeof(screen_buffer));

    [span_9](start_span)// ... (AAudio and CPU state initialization) [cite: 108-110]
    env->ReleaseStringUTFChars(filesDir, cPath);
}

JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv* env, jobject thiz, jobject bitmap) {
    void* pixels;
    [cite_start]if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;[span_9](end_span)
    [span_10](start_span)memcpy(pixels, screen_buffer, 256 * 240 * 4);[span_10](end_span)
    [span_11](start_span)AndroidBitmap_unlockPixels(env, bitmap);[span_11](end_span)
}

JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* env, jobject thiz, jint buttonBit, jboolean isPressed) {
    [span_12](start_span)// Logic for updating NES controller state bitmask[span_12](end_span)
}

} // extern "C"
