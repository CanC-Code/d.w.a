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

// --- Audio Engine ---
float audio_phase = 0, audio_frequency = 0, audio_amplitude = 0;
AAudioStream *audio_stream = nullptr;

aaudio_data_callback_result_t audio_callback(AAudioStream *stream, void *user_data, void *audio_data, int32_t num_frames) {
    float *buffer = (float *)audio_data;
    for (int i = 0; i < num_frames; i++) {
        buffer[i] = (sinf(audio_phase) > 0) ? audio_amplitude : -audio_amplitude;
        audio_phase += (2.0f * M_PI * audio_frequency) / 48000.0f;
        if (audio_phase > 2.0f * M_PI) audio_phase -= 2.0f * M_PI;
    }
    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

// --- CPU State ---
uint16_t reg_PC;
uint8_t  reg_A, reg_X, reg_Y, reg_S, reg_P;
bool is_running = false;

#define FLAG_Z (1 << 1)
#define FLAG_I (1 << 2)
#define FLAG_D (1 << 3)
#define FLAG_U (1 << 5)
#define FLAG_N (1 << 7)

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

// CPU Logic
int step_cpu() {
    uint8_t op = read_byte(reg_PC++);
    switch (op) {
        case 0x78: reg_P |= FLAG_I; return 2; // SEI
        case 0xD8: reg_P &= ~FLAG_D; return 2; // CLD
        case 0xA9: // LDA Immediate
            reg_A = read_byte(reg_PC++);
            reg_P = (reg_A == 0) ? (reg_P | FLAG_Z) : (reg_P & ~FLAG_Z);
            reg_P = (reg_A & 0x80) ? (reg_P | FLAG_N) : (reg_P & ~FLAG_N);
            return 2;
        case 0x8D: { // STA Absolute
            uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++);
            write_byte((h << 8) | l, reg_A);
            return 4;
        }
        case 0x20: { // JSR (Jump to Subroutine)
            uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++);
            uint16_t ret = reg_PC - 1;
            write_byte(0x0100 + reg_S--, (ret >> 8) & 0xFF);
            write_byte(0x0100 + reg_S--, ret & 0xFF);
            reg_PC = (h << 8) | l;
            return 6;
        }
        case 0x60: { // RTS (Return from Subroutine)
            uint16_t l = read_byte(0x0100 + ++reg_S);
            uint16_t h = read_byte(0x0100 + ++reg_S);
            reg_PC = ((h << 8) | l) + 1;
            return 6;
        }
        case 0x4C: { // JMP Absolute
            uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++);
            reg_PC = (h << 8) | l;
            return 3;
        }
        default: return 1; // NOP-like for unknown
    }
}

void render_frame() {
    // Basic test pattern to confirm the screen isn't just one color
    for (int y = 0; y < 240; y++) {
        for (int x = 0; x < 256; x++) {
            screen_buffer[y * 256 + x] = nes_palette[(x + y) % 64];
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
    global_save_path = pathStr + "/hero.sav";

    for(int i = 0; i < 4; i++) {
        std::ifstream in(pathStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        if (in.is_open()) { in.read((char*)prg_rom[i], 16384); in.close(); }
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
    std::string outDirStr(cOutDir);

    std::ifstream in(cRomPath, std::ios::binary);
    if (!in.is_open()) return env->NewStringUTF("Error: Could not open ROM");

    in.seekg(16); // Skip iNES Header
    for (int i = 0; i < 4; i++) {
        std::ofstream out(outDirStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        char bankData[16384];
        in.read(bankData, 16384);
        out.write(bankData, 16384);
        out.close();
    }

    env->ReleaseStringUTFChars(romPath, cRomPath);
    env->ReleaseStringUTFChars(outDir, cOutDir);
    return env->NewStringUTF("Success: Banks extracted");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* env, jobject thiz, jint buttonBit, jboolean isPressed) {}
