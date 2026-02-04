#include <jni.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <android/log.h>

#define LOG_TAG "DWA_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// Global emulation state
uint8_t nes_ram[0x0800];               // 2KB Internal RAM
uint8_t prg_rom[4][16384];             // 4 Banks of 16KB PRG
uint8_t chr_rom[8192];                 // 8KB CHR Graphics
uint8_t current_bank = 0;              // MMC1 state placeholder

// RAM Addresses from Dragon_Warrior_Defines.asm
#define ADDR_JOYPAD1 0x00F5

struct NESHeader {
    char magic[4];
    uint8_t prg_size;
    uint8_t chr_size;
    uint8_t flags6;
    uint8_t flags7;
    uint8_t padding[8];
};

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* cRomPath = env->GetStringUTFChars(romPath, nullptr);
    const char* cOutDir = env->GetStringUTFChars(outDir, nullptr);

    std::ifstream rom(cRomPath, std::ios::binary);
    if (!rom.is_open()) return env->NewStringUTF("Error: File not found.");

    NESHeader header;
    rom.read(reinterpret_cast<char*>(&header), 16);

    if (std::string(header.magic, 3) != "NES") return env->NewStringUTF("Error: Not a NES ROM.");

    std::string outStr(cOutDir);
    // Extract 4 PRG Banks
    for(int i = 0; i < 4; i++) {
        rom.read(reinterpret_cast<char*>(prg_rom[i]), 16384);
        std::ofstream out(outStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        out.write(reinterpret_cast<char*>(prg_rom[i]), 16384);
    }

    // Extract CHR
    rom.read(reinterpret_cast<char*>(chr_rom), 8192);
    std::ofstream out(outStr + "/chr_assets.bin", std::ios::binary);
    out.write(reinterpret_cast<char*>(chr_rom), 8192);

    rom.close();
    env->ReleaseStringUTFChars(romPath, cRomPath);
    env->ReleaseStringUTFChars(outDir, cOutDir);
    return env->NewStringUTF("Success: All banks extracted.");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz, jstring filesDir) {
    const char* cFilesDir = env->GetStringUTFChars(filesDir, nullptr);
    std::string path(cFilesDir);

    // Load extracted banks into memory for fast "Native" access
    for(int i = 0; i < 4; i++) {
        std::ifstream in(path + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        in.read(reinterpret_cast<char*>(prg_rom[i]), 16384);
        in.close();
    }
    
    LOGI("Engine Initialized: PRG memory populated.");
    env->ReleaseStringUTFChars(filesDir, cFilesDir);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* env, jobject thiz, jint buttonBit, jboolean isPressed) {
    // Modify RAM address $00F5 (Joypad1Hld) directly
    if (isPressed) {
        nes_ram[ADDR_JOYPAD1] |= (uint8_t)buttonBit;
    } else {
        nes_ram[ADDR_JOYPAD1] &= ~(uint8_t)buttonBit;
    }
}

extern "C" JNIEXPORT jint JNICALL
Java_com_canc_dwa_MainActivity_getRamValue(JNIEnv* env, jobject thiz, jint address) {
    if (address < 0x0800) return nes_ram[address];
    return 0;
}
