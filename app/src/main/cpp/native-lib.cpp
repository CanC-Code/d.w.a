#include <jni.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <android/log.h>

#define LOG_TAG "DWA_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// --- NES Hardware State ---
uint8_t cpu_ram[0x0800];               // 2KB Internal RAM ($0000-$07FF)
uint8_t prg_rom[4][16384];             // 4 Banks of 16KB PRG
uint8_t chr_rom[8192];                 // 8KB CHR Graphics
uint8_t sram[0x2000];                  // 8KB Save RAM ($6000-$7FFF)

// --- MMC1 Mapper State ---
uint8_t current_prg_bank = 0;          // Which bank is at $8000
uint8_t mmc1_shift_reg = 0x10;         // MMC1 uses a 5-bit shift register

// --- Memory Map Addresses (from Dragon_Warrior_Defines.asm) ---
#define ADDR_JOYPAD1 0x00F5 

// --- Core Memory Access Functions ---
// This is the "Engine" of a native port. It maps 6502 addresses to C++ arrays.
uint8_t read_byte(uint16_t addr) {
    if (addr < 0x2000) {
        return cpu_ram[addr % 0x0800]; // Mirroring handled
    } else if (addr >= 0x6000 && addr < 0x8000) {
        return sram[addr - 0x6000];
    } else if (addr >= 0x8000 && addr < 0xC000) {
        return prg_rom[current_prg_bank][addr - 0x8000];
    } else if (addr >= 0xC000) {
        return prg_rom[3][addr - 0xC000]; // Bank 03 is fixed at $C000
    }
    return 0;
}

void write_byte(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) {
        cpu_ram[addr % 0x0800] = val;
    } else if (addr >= 0x8000) {
        // MMC1 Mapper Logic: Writing to ROM space controls bank switching
        if (val & 0x80) {
            mmc1_shift_reg = 0x10;
        } else {
            bool complete = (mmc1_shift_reg & 0x01);
            mmc1_shift_reg >>= 1;
            mmc1_shift_reg |= ((val & 0x01) << 4);
            if (complete) {
                if (addr >= 0xE000) { // PRG Bank Select
                    current_prg_bank = mmc1_shift_reg & 0x03;
                }
                mmc1_shift_reg = 0x10;
            }
        }
    }
}

// --- JNI Implementation ---

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* cRomPath = env->GetStringUTFChars(romPath, nullptr);
    const char* cOutDir = env->GetStringUTFChars(outDir, nullptr);

    std::ifstream rom(cRomPath, std::ios::binary);
    if (!rom.is_open()) return env->NewStringUTF("Error: File not found.");

    char header[16];
    rom.read(header, 16);

    if (header[0] != 'N' || header[1] != 'E' || header[2] != 'S') return env->NewStringUTF("Error: Invalid Header.");

    std::string outStr(cOutDir);
    // Extract & Save PRG
    for(int i = 0; i < 4; i++) {
        rom.read(reinterpret_cast<char*>(prg_rom[i]), 16384);
        std::ofstream out(outStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        out.write(reinterpret_cast<char*>(prg_rom[i]), 16384);
    }
    // Extract & Save CHR
    rom.read(reinterpret_cast<char*>(chr_rom), 8192);
    std::ofstream out_chr(outStr + "/chr_assets.bin", std::ios::binary);
    out_chr.write(reinterpret_cast<char*>(chr_rom), 8192);

    rom.close();
    env->ReleaseStringUTFChars(romPath, cRomPath);
    env->ReleaseStringUTFChars(outDir, cOutDir);
    return env->NewStringUTF("Success: Native assets prepared.");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz, jstring filesDir) {
    const char* cFilesDir = env->GetStringUTFChars(filesDir, nullptr);
    std::string path(cFilesDir);

    for(int i = 0; i < 4; i++) {
        std::ifstream in(path + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        in.read(reinterpret_cast<char*>(prg_rom[i]), 16384);
    }
    
    // Clear CPU RAM to match Power-on state
    memset(cpu_ram, 0, 0x0800);
    
    LOGI("Native Engine Ready. Mapper: MMC1, PRG: 64KB, CHR: 8KB");
    env->ReleaseStringUTFChars(filesDir, cFilesDir);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* env, jobject thiz, jint buttonBit, jboolean isPressed) {
    if (isPressed) cpu_ram[ADDR_JOYPAD1] |= (uint8_t)buttonBit;
    else cpu_ram[ADDR_JOYPAD1] &= ~(uint8_t)buttonBit;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_canc_dwa_MainActivity_getRamValue(JNIEnv* env, jobject thiz, jint address) {
    return read_byte((uint16_t)address);
}
