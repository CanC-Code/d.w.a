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

// --- MEMORY & HARDWARE STATE ---
uint8_t cpu_ram[0x0800];
uint8_t prg_rom[4][16384];             
uint8_t chr_rom[16384]; // 2 Banks (16KB)
uint32_t screen_buffer[256 * 240];
std::mutex buffer_mutex;

// --- CPU REGISTERS ---
uint16_t reg_PC;
uint8_t reg_A, reg_X, reg_Y, reg_S, reg_P;
bool is_running = false;

// --- PPU & MAPPER REGISTERS ---
uint8_t ppu_status = 0;
uint16_t ppu_addr;
bool address_latch = false;
uint8_t mmc1_shift = 0x10, current_prg = 0;
uint8_t ctrl_bits = 0, ctrl_latched = 0;

// NES Palette (64 Colors - Subset shown for title screen)
uint32_t palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00,
    0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFFEFEFE, 0xFF64B0FF, 0xFF9290FF, 0xFFC676FF, 0xFFF36AFF, 0xFFFE6ECC, 0xFFFE8170, 0xFFEA9E22,
    0xFFBCBE00, 0xFF88D800, 0xFF5CE430, 0xFF45E082, 0xFF48CDDE, 0xFF4F4F4F, 0xFF000000, 0xFF000000,
    0xFFFEFEFE, 0xFFC0DFFF, 0xFFD3D2FF, 0xFFE8BEFF, 0xFFF9B7FF, 0xFFFEBCE5, 0xFFFEC7C2, 0xFFFAD19F,
    0xFFE8E97C, 0xFFD2F072, 0xFFBFF69E, 0xFFB5F1CE, 0xFFB6EAF2, 0xFFB8B8B8, 0xFF000000, 0xFF000000
};

// --- CORE CPU HELPERS ---
void set_nz(uint8_t val) {
    reg_P = (reg_P & 0x7D) | (val & 0x80) | ((val == 0) << 1);
}

uint8_t read_byte(uint16_t addr) {
    if (addr < 0x2000) return cpu_ram[addr % 0x0800];
    if (addr == 0x2002) { 
        uint8_t s = ppu_status; 
        ppu_status &= ~0x80; 
        address_latch = false; 
        return s; 
    }
    if (addr == 0x4016) { 
        uint8_t b = (ctrl_latched & 0x80) >> 7; 
        ctrl_latched <<= 1; 
        return b | 0x40; 
    }
    if (addr >= 0x8000 && addr < 0xC000) return prg_rom[current_prg][addr - 0x8000];
    if (addr >= 0xC000) return prg_rom[3][addr - 0xC000];
    return 0;
}

void write_byte(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) {
        cpu_ram[addr % 0x0800] = val;
    } else if (addr == 0x2006) {
        if (!address_latch) { ppu_addr = (ppu_addr & 0x00FF) | (val << 8); address_latch = true; }
        else { ppu_addr = (ppu_addr & 0xFF00) | val; address_latch = false; }
    } else if (addr == 0x4016) {
        if ((val & 0x01) == 0) ctrl_latched = ctrl_bits;
    } else if (addr >= 0x8000) {
        if (val & 0x80) { 
            mmc1_shift = 0x10; 
        } else {
            bool done = (mmc1_shift & 0x01);
            mmc1_shift >>= 1; mmc1_shift |= ((val & 0x01) << 4);
            if (done) { 
                if (((addr >> 13) & 0x03) == 3) current_prg = mmc1_shift & 0x0F; 
                mmc1_shift = 0x10; 
            }
        }
    }
}

// --- CPU DISPATCHER ---
void step_cpu() {
    uint8_t op = read_byte(reg_PC++);
    switch (op) {
        case 0x78: break; // SEI
        case 0xD8: reg_P &= ~0x08; break; // CLD
        case 0xA9: reg_A = read_byte(reg_PC++); set_nz(reg_A); break; // LDA Imm
        case 0xA2: reg_X = read_byte(reg_PC++); set_nz(reg_X); break; // LDX Imm
        case 0x8D: { uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++); write_byte((h<<8)|l, reg_A); break; }
        case 0x9A: reg_S = reg_X; break; // TXS
        case 0x20: { // JSR
            uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++);
            uint16_t r = reg_PC - 1;
            write_byte(0x0100 + reg_S--, (r >> 8) & 0xFF); write_byte(0x0100 + reg_S--, r & 0xFF);
            reg_PC = (h << 8) | l; break;
        }
        case 0x60: { // RTS
            uint16_t l = read_byte(0x0100 + ++reg_S), h = read_byte(0x0100 + ++reg_S);
            reg_PC = ((h << 8) | l) + 1; break;
        }
        case 0x4C: { uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++); reg_PC = (h<<8)|l; break; }
        case 0xD0: { int8_t rel = (int8_t)read_byte(reg_PC++); if (!(reg_P & 0x02)) reg_PC += rel; break; } // BNE
        case 0xF0: { int8_t rel = (int8_t)read_byte(reg_PC++); if (reg_P & 0x02) reg_PC += rel; break; }  // BEQ
        case 0xEA: break; // NOP
    }
}

// --- GRAPHICS PIPELINE ---
void render_frame() {
    std::lock_guard<std::mutex> lock(buffer_mutex);
    // Draw tiles from CHR-ROM (First 256 tiles)
    for (int ty = 0; ty < 30; ty++) {
        for (int tx = 0; tx < 32; tx++) {
            uint16_t tile_idx = (ty * 32 + tx) % 256;
            for (int y = 0; y < 8; y++) {
                uint8_t p1 = chr_rom[tile_idx * 16 + y];
                uint8_t p2 = chr_rom[tile_idx * 16 + y + 8];
                for (int x = 0; x < 8; x++) {
                    uint8_t pixel = ((p1 >> (7-x)) & 1) | (((p2 >> (7-x)) & 1) << 1);
                    uint32_t color = palette[pixel ? pixel + 0x10 : 0x0D]; // Basic offset for visible color
                    screen_buffer[(ty*8+y)*256 + (tx*8+x)] = color; 
                }
            }
        }
    }
}

void master_clock() {
    while (is_running) {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < 29780; i++) step_cpu();
        ppu_status |= 0x80; // Set VBlank flag
        render_frame();
        std::this_thread::sleep_until(start + std::chrono::milliseconds(16));
    }
}

// --- JNI INTERFACE ---
extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* cPath = env->GetStringUTFChars(romPath, nullptr);
    const char* cOut = env->GetStringUTFChars(outDir, nullptr);
    std::string outDirStr(cOut);
    std::ifstream nes(cPath, std::ios::binary);

    if (!nes.is_open()) return env->NewStringUTF("File Error");
    uint8_t header[16];
    nes.read((char*)header, 16);

    for (int i = 0; i < 4; i++) {
        std::vector<char> buffer(16384);
        nes.read(buffer.data(), 16384);
        std::ofstream out(outDirStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        out.write(buffer.data(), 16384);
    }

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

    reg_S = 0xFD; reg_P = 0x24;
    reg_PC = (prg_rom[3][0x3FFD] << 8) | prg_rom[3][0x3FFC];

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
    if (isPressed) ctrl_bits |= buttonBit;
    else ctrl_bits &= ~buttonBit;
}
