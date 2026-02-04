#include <jni.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <android/log.h>
#include <thread>
#include <chrono>

#define LOG_TAG "DWA_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// --- NES Hardware State ---
uint8_t cpu_ram[0x0800];               // 2KB Internal RAM ($0000-$07FF)
uint8_t prg_rom[4][16384];             // 4 Banks of 16KB PRG
uint8_t chr_rom[8192];                 // 8KB CHR Graphics
uint8_t sram[0x2000];                  // 8KB Save RAM ($6000-$7FFF)

// --- 6502 CPU Registers ---
uint16_t reg_PC;
uint8_t  reg_A, reg_X, reg_Y, reg_S, reg_P;
bool is_running = false;

// Status Flag Bits
#define FLAG_C (1 << 0)
#define FLAG_Z (1 << 1)
#define FLAG_I (1 << 2)
#define FLAG_D (1 << 3)
#define FLAG_U (1 << 5)
#define FLAG_N (1 << 7)

// --- MMC1 Mapper State ---
uint8_t current_prg_bank = 0;
uint8_t mmc1_shift_reg = 0x10;

#define ADDR_JOYPAD1 0x00F5 

// --- Memory Dispatcher ---
uint8_t read_byte(uint16_t addr) {
    if (addr < 0x2000) return cpu_ram[addr % 0x0800];
    if (addr >= 0x6000 && addr < 0x8000) return sram[addr - 0x6000];
    if (addr >= 0x8000 && addr < 0xC000) return prg_rom[current_prg_bank][addr - 0x8000];
    if (addr >= 0xC000) return prg_rom[3][addr - 0xC000]; 
    return 0;
}

void write_byte(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) {
        cpu_ram[addr % 0x0800] = val;
    } else if (addr >= 0x8000) {
        if (val & 0x80) {
            mmc1_shift_reg = 0x10;
        } else {
            bool complete = (mmc1_shift_reg & 0x01);
            mmc1_shift_reg >>= 1;
            mmc1_shift_reg |= ((val & 0x01) << 4);
            if (complete) {
                if (addr >= 0xE000) current_prg_bank = mmc1_shift_reg & 0x03;
                mmc1_shift_reg = 0x10;
            }
        }
    }
}

// --- Instruction Logic ---
void set_zn(uint8_t val) {
    if (val == 0) reg_P |= FLAG_Z; else reg_P &= ~FLAG_Z;
    if (val & 0x80) reg_P |= FLAG_N; else reg_P &= ~FLAG_N;
}

int step_cpu() {
    uint8_t opcode = read_byte(reg_PC++);
    switch (opcode) {
        case 0x78: reg_P |= FLAG_I; return 2; // SEI
        case 0xD8: reg_P &= ~FLAG_D; return 2; // CLD
        case 0xA9: reg_A = read_byte(reg_PC++); set_zn(reg_A); return 2; // LDA Imm
        case 0x8D: { // STA Abs
            uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++);
            write_byte((h << 8) | l, reg_A); return 4; 
        }
        case 0x4C: { // JMP Abs
            uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++);
            reg_PC = (h << 8) | l; return 3;
        }
        default: return 1; // Simplification for briefness
    }
}

// --- Master Clock Loop ---
void master_clock() {
    LOGI("Master Clock Started.");
    auto next_frame = std::chrono::steady_clock::now();
    
    while (is_running) {
        // NES runs ~29780 cycles per frame
        int cycles_this_frame = 0;
        while (cycles_this_frame < 29780) {
            cycles_this_frame += step_cpu();
        }

        // Sync to 60 FPS
        next_frame += std::chrono::microseconds(16666); 
        std::this_thread::sleep_until(next_frame);
    }
}

// --- JNI Bridge ---

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* cRomPath = env->GetStringUTFChars(romPath, nullptr);
    const char* cOutDir = env->GetStringUTFChars(outDir, nullptr);
    std::ifstream rom(cRomPath, std::ios::binary);
    if (!rom.is_open()) return env->NewStringUTF("Error: ROM not found.");

    char header[16]; rom.read(header, 16);
    std::string outStr(cOutDir);
    for(int i = 0; i < 4; i++) {
        rom.read(reinterpret_cast<char*>(prg_rom[i]), 16384);
        std::ofstream out(outStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        out.write(reinterpret_cast<char*>(prg_rom[i]), 16384);
    }
    rom.read(reinterpret_cast<char*>(chr_rom), 8192);
    std::ofstream oc(outStr + "/chr_assets.bin", std::ios::binary);
    oc.write(reinterpret_cast<char*>(chr_rom), 8192);

    rom.close();
    env->ReleaseStringUTFChars(romPath, cRomPath);
    env->ReleaseStringUTFChars(outDir, cOutDir);
    return env->NewStringUTF("Success: Extracted.");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz, jstring filesDir) {
    const char* cFilesDir = env->GetStringUTFChars(filesDir, nullptr);
    std::string path(cFilesDir);
    for(int i = 0; i < 4; i++) {
        std::ifstream in(path + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        in.read(reinterpret_cast<char*>(prg_rom[i]), 16384);
    }
    
    // Set PC to Reset Vector
    uint16_t low = read_byte(0xFFFC), high = read_byte(0xFFFD);
    reg_PC = (high << 8) | low;
    reg_S = 0xFD; reg_P = FLAG_I | FLAG_U;
    
    is_running = true;
    std::thread(master_clock).detach(); // Start the heart of the game
    
    env->ReleaseStringUTFChars(filesDir, cFilesDir);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* env, jobject thiz, jint buttonBit, jboolean isPressed) {
    if (isPressed) cpu_ram[ADDR_JOYPAD1] |= (uint8_t)buttonBit;
    else cpu_ram[ADDR_JOYPAD1] &= ~(uint8_t)buttonBit;
}
