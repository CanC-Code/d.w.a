#include <jni.h>
#include <string>
#include <vector>
#include <fstream>
#include <android/log.h>
#include <android/bitmap.h>
#include <thread>
#include <chrono>
#include <mutex>

#define LOG_TAG "DWA_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// --- Hardware State ---
uint8_t cpu_ram[0x0800];
uint8_t prg_rom[4][16384];             
uint8_t chr_rom[16384];
uint32_t screen_buffer[256 * 240];
std::mutex buffer_mutex;

// --- CPU Registers ---
uint16_t reg_PC;
uint8_t reg_A, reg_X, reg_Y, reg_S, reg_P;
bool is_running = false;

// --- PPU & Mapper State ---
uint8_t ppu_status = 0;
uint8_t mmc1_shift_reg = 0x10;
uint8_t mmc1_control = 0x0C; // Default MMC1 state
uint8_t current_prg_bank = 0;

// --- Memory Access Logic ---
uint8_t read_byte(uint16_t addr) {
    if (addr < 0x2000) return cpu_ram[addr % 0x0800];
    if (addr == 0x2002) {
        uint8_t s = ppu_status;
        ppu_status &= ~0x80; // Reading clears VBlank
        return s;
    }
    // MMC1 Logic: $8000-$BFFF is switchable, $C000-$FFFF is fixed (Bank 3)
    if (addr >= 0x8000 && addr < 0xC000) return prg_rom[current_prg_bank][addr - 0x8000];
    if (addr >= 0xC000) return prg_rom[3][addr - 0xC000];
    return 0;
}

void write_byte(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) cpu_ram[addr % 0x0800] = val;
    else if (addr >= 0x8000) {
        // MMC1 Serial Write Logic
        if (val & 0x80) {
            mmc1_shift_reg = 0x10;
        } else {
            bool complete = (mmc1_shift_reg & 0x01);
            mmc1_shift_reg >>= 1;
            mmc1_shift_reg |= ((val & 0x01) << 4);
            if (complete) {
                // Determine which MMC1 register to write to based on addr
                uint8_t target = (addr >> 13) & 0x03;
                if (target == 3) current_prg_bank = mmc1_shift_reg & 0x0F;
                mmc1_shift_reg = 0x10;
            }
        }
    }
}



// --- Core CPU Execution (Simplified Dispatcher) ---
void step_cpu() {
    uint8_t opcode = read_byte(reg_PC++);
    switch (opcode) {
        case 0x78: break; // SEI (Ignore Interrupts)
        case 0xD8: break; // CLD (Clear Decimal)
        case 0xA9: reg_A = read_byte(reg_PC++); break; // LDA Immediate
        case 0xAD: { // LDA Absolute
            uint16_t lo = read_byte(reg_PC++);
            uint16_t hi = read_byte(reg_PC++);
            reg_A = read_byte((hi << 8) | lo);
            break;
        }
        case 0x4C: { // JMP Absolute
            uint16_t lo = read_byte(reg_PC++);
            uint16_t hi = read_byte(reg_PC++);
            reg_PC = (hi << 8) | lo;
            break;
        }
        // Additional 6502 opcodes would follow here...
    }
    
    // Safety: Simulation of VBlank to satisfy initialization loops
    static int cycle_count = 0;
    if (++cycle_count > 2000) {
        ppu_status |= 0x80;
        cycle_count = 0;
    }
}

void master_clock() {
    while (is_running) {
        auto start = std::chrono::steady_clock::now();
        
        // Execute one frame's worth of instructions
        for (int i = 0; i < 29780; i++) step_cpu();

        // Render logic: Basic background clear
        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            for (int i = 0; i < 256 * 240; i++) screen_buffer[i] = 0xFF1412A7; // DW Blue
        }

        std::this_thread::sleep_until(start + std::chrono::milliseconds(16));
    }
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

    // Skip 16-byte header, extract 4 PRG banks
    for (int i = 0; i < 4; i++) {
        std::vector<char> buffer(16384);
        nes.read(buffer.data(), 16384);
        std::ofstream out(outDirStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        out.write(buffer.data(), 16384);
    }

    // Extract 16KB CHR
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

    for(int i = 0; i < 4; i++) {
        std::ifstream in(pathStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        if (in.is_open()) in.read((char*)prg_rom[i], 16384);
    }
    std::ifstream chr_in(pathStr + "/chr_rom.bin", std::ios::binary);
    if (chr_in.is_open()) chr_in.read((char*)chr_rom, 16384);

    // Read Reset Vector from the end of Bank 3
    uint16_t lo = prg_rom[3][0x3FFC];
    uint16_t hi = prg_rom[3][0x3FFD];
    reg_PC = (hi << 8) | lo;

    is_running = true;
    std::thread(master_clock).detach();
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
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* env, jobject thiz, jint buttonBit, jboolean isPressed) {
    // Basic controller input placeholder
}
