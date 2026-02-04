#include <jni.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <android/log.h>
#include <android/bitmap.h>
#include <thread>
#include <chrono>

#define LOG_TAG "DWA_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// --- NES Hardware State ---
uint8_t cpu_ram[0x0800];               
uint8_t prg_rom[4][16384];             
uint8_t chr_rom[8192];                 
uint8_t sram[0x2000];                  

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

// --- 6502 CPU Registers ---
uint16_t reg_PC;
uint8_t  reg_A, reg_X, reg_Y, reg_S, reg_P;
bool is_running = false;

#define FLAG_C (1 << 0)
#define FLAG_Z (1 << 1)
#define FLAG_I (1 << 2)
#define FLAG_D (1 << 3)
#define FLAG_U (1 << 5)
#define FLAG_N (1 << 7)

// --- MMC1 Mapper ---
uint8_t current_prg_bank = 0;
uint8_t mmc1_shift_reg = 0x10;
#define ADDR_JOYPAD1 0x00F5 

// --- Memory Access ---
uint8_t read_byte(uint16_t addr) {
    if (addr < 0x2000) return cpu_ram[addr % 0x0800];
    if (addr >= 0x6000 && addr < 0x8000) return sram[addr - 0x6000];
    if (addr >= 0x8000 && addr < 0xC000) return prg_rom[current_prg_bank][addr - 0x8000];
    if (addr >= 0xC000) return prg_rom[3][addr - 0xC000]; 
    return 0;
}

void write_byte(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) { cpu_ram[addr % 0x0800] = val; }
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

// --- PPU Rendering Logic ---

void render_frame() {
    // Basic Background Rendering (Simplified for Native Port)
    // In a perfect port, we iterate through the nametable in RAM
    for (int t = 0; t < 960; t++) { // 32x30 tiles
        int tile_id = cpu_ram[0x0400 + t]; // Dummy offset for nametable
        int x_base = (t % 32) * 8;
        int y_base = (t / 32) * 8;

        for (int row = 0; row < 8; row++) {
            uint8_t p1 = chr_rom[tile_id * 16 + row];
            uint8_t p2 = chr_rom[tile_id * 16 + row + 8];
            for (int col = 0; col < 8; col++) {
                int pixel = ((p1 >> (7 - col)) & 1) | (((p2 >> (7 - col)) & 1) << 1);
                screen_buffer[(y_base + row) * 256 + (x_base + col)] = nes_palette[pixel * 10]; // Placeholder color logic
            }
        }
    }
}

// --- CPU Logic ---
void set_zn(uint8_t val) {
    if (val == 0) reg_P |= FLAG_Z; else reg_P &= ~FLAG_Z;
    if (val & 0x80) reg_P |= FLAG_N; else reg_P &= ~FLAG_N;
}

int step_cpu() {
    uint8_t opcode = read_byte(reg_PC++);
    switch (opcode) {
        case 0x78: reg_P |= FLAG_I; return 2;
        case 0xD8: reg_P &= ~FLAG_D; return 2;
        case 0xA9: reg_A = read_byte(reg_PC++); set_zn(reg_A); return 2;
        case 0x8D: { 
            uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++);
            write_byte((h << 8) | l, reg_A); return 4; 
        }
        case 0x4C: { 
            uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++);
            reg_PC = (h << 8) | l; return 3;
        }
        default: return 1;
    }
}

// --- Master Clock ---
void master_clock() {
    auto next_frame = std::chrono::steady_clock::now();
    while (is_running) {
        int cycles = 0;
        while (cycles < 29780) { cycles += step_cpu(); }
        
        render_frame(); // Draw the screen after CPU work

        next_frame += std::chrono::microseconds(16666); 
        std::this_thread::sleep_until(next_frame);
    }
}

// --- JNI Bridge ---
extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv* env, jobject thiz, jobject bitmap) {
    AndroidBitmapInfo info;
    void* pixels;
    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0) return;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;
    
    memcpy(pixels, screen_buffer, 256 * 240 * 4);
    
    AndroidBitmap_unlockPixels(env, bitmap);
}

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
    
    uint16_t low = read_byte(0xFFFC), high = read_byte(0xFFFD);
    reg_PC = (high << 8) | low;
    reg_S = 0xFD; reg_P = FLAG_I | FLAG_U;
    
    is_running = true;
    std::thread(master_clock).detach();
    env->ReleaseStringUTFChars(filesDir, cFilesDir);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* env, jobject thiz, jint buttonBit, jboolean isPressed) {
    if (isPressed) cpu_ram[ADDR_JOYPAD1] |= (uint8_t)buttonBit;
    else cpu_ram[ADDR_JOYPAD1] &= ~(uint8_t)buttonBit;
}
