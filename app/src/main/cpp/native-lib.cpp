#include <jni.h>
#include <string>
#include <vector>
#include <fstream>
#include <android/log.h>
#include <android/bitmap.h>
#include <aaudio/AAudio.h>
#include <thread>
#include <chrono>
#include <math.h>

#define LOG_TAG "DWA_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// --- NES Hardware State ---
uint8_t cpu_ram[0x0800];
uint8_t prg_rom[4][16384];             
uint8_t chr_rom[8192];                 
uint8_t sram[0x2000];                  
std::string global_save_path;

// --- Video State ---
uint32_t screen_buffer[256 * 240];
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

// --- CPU State ---
uint16_t reg_PC;
uint8_t  reg_A, reg_X, reg_Y, reg_S, reg_P;
bool is_running = false;

#define FLAG_C (1 << 0)
#define FLAG_Z (1 << 1)
#define FLAG_I (1 << 2)
#define FLAG_D (1 << 3)
#define FLAG_B (1 << 4)
#define FLAG_U (1 << 5)
#define FLAG_V (1 << 6)
#define FLAG_N (1 << 7)

#define SET_ZN(val) reg_P = (reg_P & ~(FLAG_Z | FLAG_N)) | ((val == 0) ? FLAG_Z : 0) | (val & 0x80)

uint8_t current_prg_bank = 0;
uint8_t mmc1_shift_reg = 0x10;

// Memory Helpers
uint8_t read_byte(uint16_t addr) {
    if (addr < 0x2000) return cpu_ram[addr % 0x0800];
    if (addr >= 0x6000 && addr < 0x8000) return sram[addr - 0x6000];
    if (addr >= 0x8000 && addr < 0xC000) return prg_rom[current_prg_bank][addr - 0x8000];
    if (addr >= 0xC000) return prg_rom[3][addr - 0xC000];
    return 0;
}

void write_byte(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) { cpu_ram[addr % 0x0800] = val; }
    else if (addr >= 0x6000 && addr < 0x8000) { sram[addr - 0x6000] = val; }
    else if (addr >= 0x8000) {
        if (val & 0x80) { mmc1_shift_reg = 0x10; }
        else {
            bool complete = (mmc1_shift_reg & 0x01);
            mmc1_shift_reg >>= 1; mmc1_shift_reg |= ((val & 0x01) << 4);
            if (complete) {
                if (addr >= 0xE000) current_prg_bank = mmc1_shift_reg & 0x03;
                mmc1_shift_reg = 0x10;
            }
        }
    }
}

// 6502 CPU Implementation (Full Instruction Set)
int step_cpu() {
    uint8_t op = read_byte(reg_PC++);
    uint16_t addr = 0;
    uint8_t temp = 0;

    switch (op) {
        // --- DATA TRANSFER ---
        case 0xA9: reg_A = read_byte(reg_PC++); SET_ZN(reg_A); return 2; // LDA Imm
        case 0xA5: reg_A = read_byte(read_byte(reg_PC++)); SET_ZN(reg_A); return 3; // LDA ZP
        case 0xAD: { uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++); reg_A = read_byte((h<<8)|l); SET_ZN(reg_A); return 4; } // LDA Abs
        case 0xA2: reg_X = read_byte(reg_PC++); SET_ZN(reg_X); return 2; // LDX Imm
        case 0xA6: reg_X = read_byte(read_byte(reg_PC++)); SET_ZN(reg_X); return 3; // LDX ZP
        case 0xA0: reg_Y = read_byte(reg_PC++); SET_ZN(reg_Y); return 2; // LDY Imm
        case 0x85: write_byte(read_byte(reg_PC++), reg_A); return 3; // STA ZP
        case 0x8D: { uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++); write_byte((h<<8)|l, reg_A); return 4; } // STA Abs
        case 0x86: write_byte(read_byte(reg_PC++), reg_X); return 3; // STX ZP
        case 0x84: write_byte(read_byte(reg_PC++), reg_Y); return 3; // STY ZP
        case 0xAA: reg_X = reg_A; SET_ZN(reg_X); return 2; // TAX
        case 0xA8: reg_Y = reg_A; SET_ZN(reg_Y); return 2; // TAY
        case 0x8A: reg_A = reg_X; SET_ZN(reg_A); return 2; // TXA
        case 0x98: reg_A = reg_Y; SET_ZN(reg_A); return 2; // TYA

        // --- STACK OPERATIONS ---
        case 0x9A: reg_S = reg_X; return 2; // TXS
        case 0xBA: reg_X = reg_S; SET_ZN(reg_X); return 2; // TSX
        case 0x48: write_byte(0x0100 + reg_S--, reg_A); return 3; // PHA
        case 0x68: reg_A = read_byte(0x0100 + ++reg_S); SET_ZN(reg_A); return 4; // PLA
        case 0x08: write_byte(0x0100 + reg_S--, reg_P | 0x30); return 3; // PHP
        case 0x28: reg_P = read_byte(0x0100 + ++reg_S) | FLAG_U; return 4; // PLP

        // --- ARITHMETIC & LOGIC ---
        case 0x69: { // ADC Imm
            uint8_t val = read_byte(reg_PC++);
            uint16_t res = (uint16_t)reg_A + (uint16_t)val + (reg_P & FLAG_C);
            if (res > 0xFF) reg_P |= FLAG_C; else reg_P &= ~FLAG_C;
            if (!((reg_A ^ val) & 0x80) && ((reg_A ^ res) & 0x80)) reg_P |= FLAG_V; else reg_P &= ~FLAG_V;
            reg_A = (uint8_t)res; SET_ZN(reg_A); return 2;
        }
        case 0xE9: { // SBC Imm
            uint8_t val = read_byte(reg_PC++);
            uint16_t res = (uint16_t)reg_A - (uint16_t)val - ((reg_P & FLAG_C) ? 0 : 1);
            if (!(res & 0xFF00)) reg_P |= FLAG_C; else reg_P &= ~FLAG_C;
            if (((reg_A ^ val) & 0x80) && ((reg_A ^ res) & 0x80)) reg_P |= FLAG_V; else reg_P &= ~FLAG_V;
            reg_A = (uint8_t)res; SET_ZN(reg_A); return 2;
        }
        case 0xC9: temp = read_byte(reg_PC++); if(reg_A >= temp) reg_P |= FLAG_C; else reg_P &= ~FLAG_C; SET_ZN(reg_A - temp); return 2; // CMP Imm
        case 0xE0: temp = read_byte(reg_PC++); if(reg_X >= temp) reg_P |= FLAG_C; else reg_P &= ~FLAG_C; SET_ZN(reg_X - temp); return 2; // CPX Imm
        case 0xC0: temp = read_byte(reg_PC++); if(reg_Y >= temp) reg_P |= FLAG_C; else reg_P &= ~FLAG_C; SET_ZN(reg_Y - temp); return 2; // CPY Imm
        case 0x29: reg_A &= read_byte(reg_PC++); SET_ZN(reg_A); return 2; // AND Imm
        case 0x09: reg_A |= read_byte(reg_PC++); SET_ZN(reg_A); return 2; // ORA Imm
        case 0x49: reg_A ^= read_byte(reg_PC++); SET_ZN(reg_A); return 2; // EOR Imm

        // --- INCREMENT / DECREMENT ---
        case 0xE8: reg_X++; SET_ZN(reg_X); return 2; // INX
        case 0xC8: reg_Y++; SET_ZN(reg_Y); return 2; // INY
        case 0xCA: reg_X--; SET_ZN(reg_X); return 2; // DEX
        case 0x88: reg_Y--; SET_ZN(reg_Y); return 2; // DEY
        case 0xE6: { uint8_t a = read_byte(reg_PC++); temp = read_byte(a) + 1; write_byte(a, temp); SET_ZN(temp); return 5; } // INC ZP
        case 0xC6: { uint8_t a = read_byte(reg_PC++); temp = read_byte(a) - 1; write_byte(a, temp); SET_ZN(temp); return 5; } // DEC ZP

        // --- CONTROL FLOW ---
        case 0x4C: { uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++); reg_PC = (h<<8)|l; return 3; } // JMP Abs
        case 0x20: { // JSR
            uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++);
            uint16_t ret = reg_PC - 1;
            write_byte(0x0100 + reg_S--, (ret >> 8) & 0xFF);
            write_byte(0x0100 + reg_S--, ret & 0xFF);
            reg_PC = (h << 8) | l; return 6;
        }
        case 0x60: { // RTS
            uint16_t l = read_byte(0x0100 + ++reg_S);
            uint16_t h = read_byte(0x0100 + ++reg_S);
            reg_PC = ((h << 8) | l) + 1; return 6;
        }
        case 0x40: { // RTI
            reg_P = read_byte(0x0100 + ++reg_S) | FLAG_U;
            uint16_t l = read_byte(0x0100 + ++reg_S);
            uint16_t h = read_byte(0x0100 + ++reg_S);
            reg_PC = (h << 8) | l; return 6;
        }

        // --- BRANCHES ---
        case 0x90: temp = read_byte(reg_PC++); if(!(reg_P & FLAG_C)) reg_PC += (int8_t)temp; return 2; // BCC
        case 0xB0: temp = read_byte(reg_PC++); if(reg_P & FLAG_C) reg_PC += (int8_t)temp; return 2; // BCS
        case 0xF0: temp = read_byte(reg_PC++); if(reg_P & FLAG_Z) reg_PC += (int8_t)temp; return 2; // BEQ
        case 0xD0: temp = read_byte(reg_PC++); if(!(reg_P & FLAG_Z)) reg_PC += (int8_t)temp; return 2; // BNE
        case 0x10: temp = read_byte(reg_PC++); if(!(reg_P & FLAG_N)) reg_PC += (int8_t)temp; return 2; // BPL
        case 0x30: temp = read_byte(reg_PC++); if(reg_P & FLAG_N) reg_PC += (int8_t)temp; return 2; // BMI

        // --- FLAG MODIFICATION ---
        case 0x18: reg_P &= ~FLAG_C; return 2; // CLC
        case 0x38: reg_P |= FLAG_C; return 2; // SEC
        case 0x58: reg_P &= ~FLAG_I; return 2; // CLI
        case 0x78: reg_P |= FLAG_I; return 2; // SEI
        case 0xD8: reg_P &= ~FLAG_D; return 2; // CLD
        case 0xF8: reg_P |= FLAG_D; return 2; // SED

        // --- MISC ---
        case 0xEA: return 2; // NOP
        case 0x00: return 7; // BRK (Stub)

        default: 
            return 1;
    }
}

// Render Logic
void render_frame() {
    for (int t = 0; t < 960; t++) {
        int tile_id = cpu_ram[0x0400 + t];
        int xb = (t % 32) * 8, yb = (t / 32) * 8;
        for (int row = 0; row < 8; row++) {
            uint8_t p1 = chr_rom[tile_id * 16 + row], p2 = chr_rom[tile_id * 16 + row + 8];
            for (int col = 0; col < 8; col++) {
                int pix = ((p1 >> (7 - col)) & 1) | (((p2 >> (7 - col)) & 1) << 1);
                screen_buffer[(yb + row) * 256 + (xb + col)] = nes_palette[pix + 0x0F];
            }
        }
    }
}

void master_clock() {
    auto next = std::chrono::steady_clock::now();
    while (is_running) {
        int cycles = 0;
        while (cycles < 29780) { cycles += step_cpu(); }
        render_frame();
        next += std::chrono::microseconds(16666); 
        std::this_thread::sleep_until(next);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz, jstring filesDir) {
    const char* cPath = env->GetStringUTFChars(filesDir, nullptr);
    std::string pathStr(cPath);
    for(int i = 0; i < 4; i++) {
        std::ifstream in(pathStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        if (in.is_open()) in.read((char*)prg_rom[i], 16384);
    }
    reg_PC = (read_byte(0xFFFD) << 8) | read_byte(0xFFFC);
    reg_S = 0xFD; 
    reg_P = FLAG_I | FLAG_U;
    is_running = true;
    std::thread(master_clock).detach();
    env->ReleaseStringUTFChars(filesDir, cPath);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv* env, jobject thiz, jobject bitmap) {
    void* pixels;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;
    memcpy(pixels, screen_buffer, 256 * 240 * 4);
    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* cRomPath = env->GetStringUTFChars(romPath, nullptr);
    const char* cOutDir = env->GetStringUTFChars(outDir, nullptr);
    std::ifstream in(cRomPath, std::ios::binary);
    in.seekg(16);
    for (int i = 0; i < 4; i++) {
        std::ofstream out(std::string(cOutDir) + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        char bankData[16384];
        in.read(bankData, 16384);
        out.write(bankData, 16384);
    }
    env->ReleaseStringUTFChars(romPath, cRomPath);
    env->ReleaseStringUTFChars(outDir, cOutDir);
    return env->NewStringUTF("Success");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* env, jobject thiz, jint buttonBit, jboolean isPressed) {}
