#include <jni.h>
#include <string>
#include <vector>
#include <fstream>
#include <android/log.h>
#include <android/bitmap.h>
#include <thread>
#include <mutex>
#include <cstring>

#define LOG_TAG "DWA_NATIVE"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// --- FIXED NES Hardware State ---
uint8_t cpu_ram[0x0800];
uint8_t prg_rom[4][16384];             
uint8_t chr_rom[16384]; // FIXED: Increased to 16KB to match Header.asm
uint8_t controller_state = 0;

// --- CPU State ---
uint16_t reg_PC = 0;
bool is_running = false;
uint32_t screen_buffer[256 * 240];
std::mutex buffer_mutex;

// --- Memory Mapping Logic (MMC1 Lite) ---
uint8_t read_byte(uint16_t addr) {
    if (addr < 0x2000) return cpu_ram[addr % 0x0800];
    // Bank 0-2 switchable, Bank 3 fixed at $C000
    if (addr >= 0x8000 && addr < 0xC000) return prg_rom[0][addr - 0x8000];
    if (addr >= 0xC000) return prg_rom[3][addr - 0xC000];
    return 0;
}



extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* cPath = env->GetStringUTFChars(romPath, nullptr);
    const char* cOut = env->GetStringUTFChars(outDir, nullptr);
    std::string outDirStr(cOut);

    std::ifstream nes(cPath, std::ios::binary);
    if (!nes.is_open()) return env->NewStringUTF("Cannot open source ROM");

    // 1. Verify iNES Header ($4E $45 $53 $1A)
    uint8_t header[16];
    nes.read((char*)header, 16);
    if (header[0] != 'N' || header[1] != 'E' || header[2] != 'S') {
        return env->NewStringUTF("Invalid iNES Header");
    }

    // 2. Extract PRG Banks (4 Banks = 64KB)
    for (int i = 0; i < 4; i++) {
        std::vector<char> buffer(16384);
        nes.read(buffer.data(), 16384);
        std::ofstream out(outDirStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        out.write(buffer.data(), 16384);
    }

    // 3. Extract CHR Banks (2 Banks = 16KB)
    // Your Header.asm Byte 5 is $02
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
    if (is_running) return;

    const char* cPath = env->GetStringUTFChars(filesDir, nullptr);
    std::string pathStr(cPath);

    // Load binaries into memory with bounds checking
    for(int i = 0; i < 4; i++) {
        std::ifstream in(pathStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        if (in.is_open()) in.read((char*)prg_rom[i], 16384);
    }
    
    std::ifstream chr_in(pathStr + "/chr_rom.bin", std::ios::binary);
    if (chr_in.is_open()) chr_in.read((char*)chr_rom, 16384);

    // CRITICAL: Set PC from the Reset Vector at $FFFC-$FFFD (End of Bank 3)
    uint16_t lo = prg_rom[3][0x3FFC];
    uint16_t hi = prg_rom[3][0x3FFD];
    reg_PC = (hi << 8) | lo;

    LOGI("Engine initialized at PC: %04X", reg_PC);
    
    is_running = true;
    // Note: Ensure your master_clock() function is implemented below
    env->ReleaseStringUTFChars(filesDir, cPath);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv* env, jobject thiz, jobject bitmap) {
    void* pixels;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;
    {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        memcpy(pixels, screen_buffer, 256 * 240 * 4);
    }
    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* env, jobject thiz, jint button_bit, jboolean is_pressed) {
    if (is_pressed) controller_state |= button_bit;
    else controller_state &= ~button_bit;
}
