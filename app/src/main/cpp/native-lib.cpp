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

// --- Hardware State ---
[span_64](start_span)uint8_t cpu_ram[0x0800];[span_64](end_span)
[span_65](start_span)uint8_t prg_rom[4][16384];[span_65](end_span)
[span_66](start_span)uint8_t chr_rom[8192];[span_66](end_span)
[span_67](start_span)uint8_t sram[0x2000];[span_67](end_span)
[span_68](start_span)uint32_t screen_buffer[256 * 240];[span_68](end_span)
[span_69](start_span)uint32_t nes_palette[64] = {[span_69](end_span)
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4142FF, 0xFF7C1AFF, 0xFFB513F9, 0xFFD210BE, 0xFFD42C44, 0xFFB84B00,
    0xFF816D00, 0xFF458800, 0xFF129800, 0xFF009923, 0xFF008985, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFFFFFFF, 0xFF64B0FF, 0xFF9290FF, 0xFFC676FF, 0xFFF35AFF, 0xFFFE66CC, 0xFFFE7373, 0xFFF38913,
    0xFFD3AB00, 0xFFA1D000, 0xFF7FDE00, 0xFF72E248, 0xFF72D1AF, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFFFFFFF, 0xFFC0DFFF, 0xFFD3D2FF, 0xFFE8C8FF, 0xFFFBC2FF, 0xFFFEC4EA, 0xFFFECCC5, 0xFFF7D8A5,
    0xFFE4E594, 0xFFCFEF96, 0xFFBDF4AB, 0xFFB3F3CC, 0xFFB5EBF2, 0xFF000000, 0xFF000000, 0xFF000000
};

// --- CPU Registers & Flags ---
[span_70](start_span)uint16_t reg_PC;[span_70](end_span)
[span_71](start_span)uint8_t  reg_A, reg_X, reg_Y, reg_S, reg_P;[span_71](end_span)
[span_72](start_span)bool is_running = false;[span_72](end_span)
[span_73](start_span)#define FLAG_Z (1 << 1)[span_73](end_span)
[span_74](start_span)#define FLAG_I (1 << 2)[span_74](end_span)
[span_75](start_span)#define FLAG_D (1 << 3)[span_75](end_span)
[span_76](start_span)#define FLAG_U (1 << 5)[span_76](end_span)
[span_77](start_span)#define FLAG_N (1 << 7)[span_77](end_span)

[span_78](start_span)uint8_t current_prg_bank = 0;[span_78](end_span)

// --- Memory Access ---
uint8_t read_byte(uint16_t addr) {
    [span_79](start_span)if (addr < 0x2000) return cpu_ram[addr % 0x0800];[span_79](end_span)
    [span_80](start_span)if (addr >= 0x8000 && addr < 0xC000) return prg_rom[current_prg_bank][addr - 0x8000];[span_80](end_span)
    [span_81](start_span)if (addr >= 0xC000) return prg_rom[3][addr - 0xC000];[span_81](end_span)
    return 0;
}

void write_byte(uint16_t addr, uint8_t val) {
    [span_82](start_span)if (addr < 0x2000) cpu_ram[addr % 0x0800] = val;[span_82](end_span)
}

// --- CPU Execution Step ---
int step_cpu() {
    [span_83](start_span)uint8_t op = read_byte(reg_PC++);[span_83](end_span)
    switch (op) {
        case 0x78: reg_P |= FLAG_I; return 2; [span_84](start_span)// SEI[span_84](end_span)
        case 0xD8: reg_P &= ~FLAG_D; return 2; [span_85](start_span)// CLD[span_85](end_span)
        [span_86](start_span)case 0xA9: // LDA Immediate[span_86](end_span)
            reg_A = read_byte(reg_PC++);
            reg_P = (reg_A == 0) ? (reg_P | FLAG_Z) : (reg_P & ~FLAG_Z);
            reg_P = (reg_A & 0x80) ? (reg_P | FLAG_N) : (reg_P & ~FLAG_N);
            return 2;
        [span_87](start_span)case 0x8D: { // STA Absolute[span_87](end_span)
            uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++);
            write_byte((h << 8) | l, reg_A);
            return 4;
        }
        case 0x20: { // JSR (Jump to Subroutine) - Critical for Booting
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
        [span_88](start_span)case 0x4C: { // JMP Absolute[span_88](end_span)
            uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++);
            reg_PC = (h << 8) | l;
            return 3;
        }
        default: return 1; 
    }
}

void master_clock() {
    [span_89](start_span)auto next = std::chrono::steady_clock::now();[span_89](end_span)
    [span_90](start_span)while (is_running) {[span_90](end_span)
        int cycles = 0;
        while (cycles < 29780) { cycles += step_cpu(); [span_91](start_span)}
        
        // --- Visual Test Pattern ---
        // If the CPU is ticking, you'll see a changing color pattern
        for (int i = 0; i < 256 * 240; i++) screen_buffer[i] = nes_palette[rand() % 64];[span_91](end_span)
        
        next += std::chrono::microseconds(16666); 
        [span_92](start_span)std::this_thread::sleep_until(next);[span_92](end_span)
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz, jstring filesDir) {
    [span_93](start_span)const char* cPath = env->GetStringUTFChars(filesDir, nullptr);[span_93](end_span)
    std::string pathStr(cPath);
    
    [span_94](start_span)// Load extracted Banks[span_94](end_span)
    for(int i = 0; i < 4; i++) {
        std::ifstream in(pathStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        if (in.is_open()) in.read((char*)prg_rom[i], 16384);
    }

    [span_95](start_span)// Set CPU Reset Vector[span_95](end_span)
    reg_PC = (read_byte(0xFFFD) << 8) | read_byte(0xFFFC);
    reg_S = 0xFD; 
    reg_P = FLAG_I | FLAG_U;

    is_running = true;
    [span_96](start_span)std::thread(master_clock).detach();[span_96](end_span)
    env->ReleaseStringUTFChars(filesDir, cPath);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv* env, jobject thiz, jobject bitmap) {
    void* pixels;
    [span_97](start_span)if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;[span_97](end_span)
    [span_98](start_span)memcpy(pixels, screen_buffer, 256 * 240 * 4);[span_98](end_span)
    [span_99](start_span)AndroidBitmap_unlockPixels(env, bitmap);[span_99](end_span)
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
