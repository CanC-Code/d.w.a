#include <jni.h>
#include <string>
#include <vector>
#include <fstream>
#include <android/log.h>
#include <android/bitmap.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstring>

#define LOG_TAG "DWA_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// --- NES Hardware State ---
uint8_t cpu_ram[0x0800];
uint8_t prg_rom[4][16384];             
uint8_t chr_rom[16384]; // FIXED: Increased to 16KB per Header.asm
uint8_t controller_state = 0, controller_shift = 0;

// --- PPU & CPU Registers ---
uint8_t ppu_vram[2048]; 
uint8_t palette_ram[32]; 
uint16_t ppu_addr_reg;    
bool ppu_addr_latch = false;
uint8_t ppu_ctrl, ppu_status, ppu_data_buffer; 
uint16_t reg_PC;
uint8_t  reg_A, reg_X, reg_Y, reg_S, reg_P;
bool is_running = false;
uint32_t screen_buffer[256 * 240];
std::mutex buffer_mutex;

// --- MMC1 State ---
uint8_t current_prg_bank = 0; 

uint8_t read_byte(uint16_t addr) {
    if (addr < 0x2000) return cpu_ram[addr % 0x0800];
    // MMC1 Mapping: $8000-$BFFF is switchable, $C000-$FFFF is fixed to Bank 3
    if (addr >= 0x8000 && addr < 0xC000) return prg_rom[current_prg_bank][addr - 0x8000];
    if (addr >= 0xC000) return prg_rom[3][addr - 0xC000];
    return 0;
}



extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* cPath = env->GetStringUTFChars(romPath, nullptr);
    const char* cOut = env->GetStringUTFChars(outDir, nullptr);
    std::string outDirStr(cOut);
    std::ifstream nes(cPath, std::ios::binary);
    
    if (!nes.is_open()) return env->NewStringUTF("File Error");

    uint8_t header[16];
    nes.read((char*)header, 16);

    // FIXED: Skip the 16-byte header to align instructions correctly
    // Extract 4 PRG banks (64KB)
    for (int i = 0; i < 4; i++) {
        std::vector<char> buffer(16384);
        nes.read(buffer.data(), 16384);
        std::ofstream out(outDirStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        out.write(buffer.data(), 16384);
    }

    // FIXED: Extract 2 CHR banks (16KB) as defined in Byte 5 of your Header.asm
    std::vector<char> chrBuffer(16384);
    nes.read(chrBuffer.data(), 16384);
    std::ofstream chrOut(outDirStr + "/chr_rom.bin", std::ios::binary);
    chrOut.write(chrBuffer.data(), 16384);

    env->ReleaseStringUTFChars(romPath, cPath);
    env->ReleaseStringUTFChars(outDir, cOut);
    return env->NewStringUTF("Success");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz, jstring filesDir) {
    const char* cPath = env->GetStringUTFChars(filesDir, nullptr);
    std::string pathStr(cPath);
    
    for(int i = 0; i < 4; i++) {
        std::ifstream in(pathStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        if (in.is_open()) in.read((char*)prg_rom[i], 16384);
    }
    std::ifstream chr_in(pathStr + "/chr_rom.bin", std::ios::binary);
    if (chr_in.is_open()) chr_in.read((char*)chr_rom, 16384);

    // FIXED: Read Reset Vector from Bank 3 ($FFFC-$FFFD)
    uint16_t lo = prg_rom[3][0x3FFC];
    uint16_t hi = prg_rom[3][0x3FFD];
    reg_PC = (hi << 8) | lo;

    is_running = true;
    // Launch master_clock thread...
    env->ReleaseStringUTFChars(filesDir, cPath);
}
