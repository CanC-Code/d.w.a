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
#include <math.h>

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

// --- Audio State (Square Wave 1) ---
float audio_phase = 0;
float audio_frequency = 0.0f;
float audio_amplitude = 0.0f;
AAudioStream *audio_stream = nullptr;

// --- 6502 CPU Registers ---
uint16_t reg_PC;
uint8_t  reg_A, reg_X, reg_Y, reg_S, reg_P;
bool is_running = false;

#define FLAG_I (1 << 2)
#define FLAG_D (1 << 3)
#define FLAG_U (1 << 5)
#define FLAG_N (1 << 7)
#define FLAG_Z (1 << 1)

// --- MMC1 Mapper & APU Hooks ---
uint8_t current_prg_bank = 0;
uint8_t mmc1_shift_reg = 0x10;
#define ADDR_JOYPAD1 0x00F5 

// --- Audio Callback (The "Synth") ---
aaudio_data_callback_result_t audio_cb(AAudioStream *s, void *u, void *d, int32_t num) {
    float *buf = (float *)d;
    for (int i = 0; i < num; i++) {
        buf[i] = (sinf(audio_phase) > 0) ? audio_amplitude : -audio_amplitude;
        audio_phase += (2.0f * M_PI * audio_frequency) / 48000.0f;
        if (audio_phase > 2.0f * M_PI) audio_phase -= 2.0f * M_PI;
    }
    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

// --- Memory Access with APU Hooking ---
uint8_t read_byte(uint16_t addr) {
    if (addr < 0x2000) return cpu_ram[addr % 0x0800];
    if (addr >= 0x8000 && addr < 0xC000) return prg_rom[current_prg_bank][addr - 0x8000];
    if (addr >= 0xC000) return prg_rom[3][addr - 0xC000]; 
    return 0;
}

void write_byte(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) { cpu_ram[addr % 0x0800] = val; }
    // APU Hook: Detect frequency change for Square 1
    else if (addr == 0x4002 || addr == 0x4003) {
        uint16_t timer = (read_byte(0x4003) & 0x07) << 8 | read_byte(0x4002);
        audio_frequency = 1789773.0f / (16.0f * (timer + 1));
        audio_amplitude = (timer > 0) ? 0.05f : 0.0f;
    }
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

// --- PPU & CPU Logic ---
void render_frame() {
    for (int t = 0; t < 960; t++) {
        int tile_id = cpu_ram[0x0400 + t];
        int xb = (t % 32) * 8, yb = (t / 32) * 8;
        for (int row = 0; row < 8; row++) {
            uint8_t p1 = chr_rom[tile_id * 16 + row], p2 = chr_rom[tile_id * 16 + row + 8];
            for (int col = 0; col < 8; col++) {
                int pix = ((p1 >> (7 - col)) & 1) | (((p2 >> (7 - col)) & 1) << 1);
                screen_buffer[(yb + row) * 256 + (xb + col)] = nes_palette[pix + 10];
            }
        }
    }
}

void set_zn(uint8_t val) {
    if (val == 0) reg_P |= FLAG_Z; else reg_P &= ~FLAG_Z;
    if (val & 0x80) reg_P |= FLAG_N; else reg_P &= ~FLAG_N;
}

int step_cpu() {
    uint8_t op = read_byte(reg_PC++);
    switch (op) {
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

// --- JNI Implementation ---
extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv* env, jobject thiz, jobject bitmap) {
    AndroidBitmapInfo info; void* pixels;
    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0) return;
    AndroidBitmap_lockPixels(env, bitmap, &pixels);
    memcpy(pixels, screen_buffer, 256 * 240 * 4);
    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz, jstring filesDir) {
    const char* cPath = env->GetStringUTFChars(filesDir, nullptr);
    for(int i = 0; i < 4; i++) {
        std::ifstream in(std::string(cPath) + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        in.read(reinterpret_cast<char*>(prg_rom[i]), 16384);
    }
    
    // Audio Setup
    AAudioStreamBuilder *b; AAudio_createStreamBuilder(&b);
    AAudioStreamBuilder_setFormat(b, AAUDIO_FORMAT_PCM_FLOAT);
    AAudioStreamBuilder_setDataCallback(b, audio_cb, nullptr);
    AAudioStreamBuilder_openStream(b, &audio_stream);
    AAudioStream_requestStart(audio_stream);
    AAudioStreamBuilder_delete(b);

    uint16_t low = read_byte(0xFFFC), high = read_byte(0xFFFD);
    reg_PC = (high << 8) | low;
