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

// --- HARDWARE ARCHITECTURE ---
uint8_t cpu_ram[0x0800];
uint8_t prg_rom[4][16384];             
uint8_t chr_rom[16384];
uint32_t screen_buffer[256 * 240];
std::mutex buffer_mutex;

// --- CPU REGISTERS & FLAGS ---
uint16_t reg_PC;
uint8_t reg_A, reg_X, reg_Y, reg_S, reg_P;
bool is_running = false;

// --- PPU & MAPPER STATE ---
uint8_t ppu_status = 0;
uint16_t ppu_addr_reg = 0;
bool addr_latch = false;
uint8_t mmc1_sr = 0x10, current_bank = 0;
uint8_t input_bits = 0, input_latched = 0;

// Standard NES Palette (64 Colors)
uint32_t nes_palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00,
    0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFFEFEFE, 0xFF64B0FF, 0xFF9290FF, 0xFFC676FF, 0xFFF36AFF, 0xFFFE6ECC, 0xFFFE8170, 0xFFEA9E22,
    0xFFBCBE00, 0xFF88D800, 0xFF5CE430, 0xFF45E082, 0xFF48CDDE, 0xFF4F4F4F, 0xFF000000, 0xFF000000,
    0xFFFEFEFE, 0xFFC0DFFF, 0xFFD3D2FF, 0xFFE8BEFF, 0xFFF9B7FF, 0xFFFEBCE5, 0xFFFEC7C2, 0xFFFAD19F,
    0xFFE8E97C, 0xFFD2F072, 0xFFBFF69E, 0xFFB5F1CE, 0xFFB6EAF2, 0xFFB8B8B8, 0xFF000000, 0xFF000000
};

// --- BUS & ALU LOGIC ---
void update_nz(uint8_t val) {
    reg_P = (reg_P & 0x7D) | (val & 0x80) | ((val == 0) << 1);
}

uint8_t bus_read(uint16_t addr) {
    if (addr < 0x2000) return cpu_ram[addr % 0x0800];
    if (addr == 0x2002) { 
        uint8_t s = ppu_status; 
        ppu_status &= ~0x80; 
        addr_latch = false; 
        return s; 
    }
    if (addr == 0x4016) { 
        uint8_t b = (input_latched & 0x80) >> 7; 
        input_latched <<= 1; 
        return b | 0x40; 
    }
    if (addr >= 0x8000 && addr < 0xC000) return prg_rom[current_bank][addr - 0x8000];
    if (addr >= 0xC000) return prg_rom[3][addr - 0xC000];
    return 0;
}

void bus_write(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) {
        cpu_ram[addr % 0x0800] = val;
    } else if (addr == 0x2006) {
        if (!addr_latch) { ppu_addr_reg = (ppu_addr_reg & 0x00FF) | (val << 8); addr_latch = true; }
        else { ppu_addr_reg = (ppu_addr_reg & 0xFF00) | val; addr_latch = false; }
    } else if (addr == 0x4016) {
        if ((val & 0x01) == 0) input_latched = input_bits;
    } else if (addr >= 0x8000) {
        if (val & 0x80) { 
            mmc1_sr = 0x10; 
        } else {
            bool done = (mmc1_sr & 0x01);
            mmc1_sr >>= 1; mmc1_sr |= ((val & 0x01) << 4);
            if (done) { 
                if (((addr >> 13) & 0x03) == 3) current_bank = mmc1_sr & 0x0F; 
                mmc1_sr = 0x10; 
            }
        }
    }
}



// --- FULL CPU CORE ---
void execute_instruction() {
    uint8_t opcode = bus_read(reg_PC++);
    switch (opcode) {
        // --- Arithmetic ---
        case 0x69: { // ADC Imm
            uint8_t val = bus_read(reg_PC++);
            uint16_t sum = reg_A + val + (reg_P & 0x01);
            reg_P = (reg_P & 0xFE) | (sum > 0xFF);
            reg_A = (uint8_t)sum; update_nz(reg_A);
            break;
        }
        case 0xE9: { // SBC Imm
            uint8_t val = bus_read(reg_PC++) ^ 0xFF;
            uint16_t sum = reg_A + val + (reg_P & 0x01);
            reg_P = (reg_P & 0xFE) | (sum > 0xFF);
            reg_A = (uint8_t)sum; update_nz(reg_A);
            break;
        }
        // --- Load/Store ---
        case 0xA9: reg_A = bus_read(reg_PC++); update_nz(reg_A); break; 
        case 0xA2: reg_X = bus_read(reg_PC++); update_nz(reg_X); break; 
        case 0xA0: reg_Y = bus_read(reg_PC++); update_nz(reg_Y); break; 
        case 0x85: bus_write(bus_read(reg_PC++), reg_A); break;         
        case 0x8D: { uint16_t l = bus_read(reg_PC++), h = bus_read(reg_PC++); bus_write((h<<8)|l, reg_A); break; }
        // --- Increments ---
        case 0xE8: reg_X++; update_nz(reg_X); break; 
        case 0xC8: reg_Y++; update_nz(reg_Y); break;
        case 0xCA: reg_X--; update_nz(reg_X); break;
        case 0x88: reg_Y--; update_nz(reg_Y); break;
        // --- Jumps ---
        case 0x4C: { uint16_t l = bus_read(reg_PC++), h = bus_read(reg_PC++); reg_PC = (h<<8)|l; break; }
        case 0x20: { // JSR
            uint16_t l = bus_read(reg_PC++), h = bus_read(reg_PC++);
            uint16_t ret = reg_PC - 1;
            bus_write(0x0100 + reg_S--, (ret >> 8) & 0xFF); bus_write(0x0100 + reg_S--, ret & 0xFF);
            reg_PC = (h << 8) | l; break;
        }
        case 0x60: { // RTS
            uint16_t l = bus_read(0x0100 + ++reg_S), h = bus_read(0x0100 + ++reg_S);
            reg_PC = ((h << 8) | l) + 1; break;
        }
        // --- Branches ---
        case 0xD0: { int8_t rel = (int8_t)bus_read(reg_PC++); if (!(reg_P & 0x02)) reg_PC += rel; break; }
        case 0xF0: { int8_t rel = (int8_t)bus_read(reg_PC++); if (reg_P & 0x02) reg_PC += rel; break; }
        // --- Misc ---
        case 0x9A: reg_S = reg_X; break;
        case 0x78: reg_P |= 0x04; break;
        case 0xD8: reg_P &= ~0x08; break;
        case 0xEA: break;
    }
}

// --- RENDERING ENGINE ---
void draw_frame() {
    std::lock_guard<std::mutex> lock(buffer_mutex);
    // Render from CHR-ROM using current Mapper state
    for (int ty = 0; ty < 30; ty++) {
        for (int tx = 0; tx < 32; tx++) {
            uint16_t tile = (ty * 32 + tx) % 256; 
            for (int y = 0; y < 8; y++) {
                uint8_t p1 = chr_rom[tile * 16 + y];
                uint8_t p2 = chr_rom[tile * 16 + y + 8];
                for (int x = 0; x < 8; x++) {
                    uint8_t px = ((p1 >> (7-x)) & 1) | (((p2 >> (7-x)) & 1) << 1);
                    screen_buffer[(ty*8+y)*256 + (tx*8+x)] = nes_palette[px ? px + 0x10 : 0x0D];
                }
            }
        }
    }
}



void engine_loop() {
    while (is_running) {
        auto frame_start = std::chrono::steady_clock::now();
        for (int i = 0; i < 29780; i++) execute_instruction();
        ppu_status |= 0x80; // Signal VBlank
        draw_frame();
        std::this_thread::sleep_until(frame_start + std::chrono::milliseconds(16));
    }
}

// --- JNI EXPORTS ---
extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* cPath = env->GetStringUTFChars(romPath, 0);
    const char* cOut = env->GetStringUTFChars(outDir, 0);
    std::string outDirStr(cOut);
    std::ifstream nes(cPath, std::ios::binary);
    if (!nes) return env->NewStringUTF("File Error");
    nes.seekg(16); // Skip iNES Header
    for (int i = 0; i < 4; i++) {
        std::vector<char> buf(16384); nes.read(buf.data(), 16384);
        std::ofstream out(outDirStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        out.write(buf.data(), 16384);
    }
    std::vector<char> chr(16384); nes.read(chr.data(), 16384);
    std::ofstream c_out(outDirStr + "/chr_rom.bin", std::ios::binary);
    c_out.write(chr.data(), 16384);
    env->ReleaseStringUTFChars(romPath, cPath); env->ReleaseStringUTFChars(outDir, cOut);
    return env->NewStringUTF("Success");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz, jstring filesDir) {
    if (is_running) return;
    const char* cPath = env->GetStringUTFChars(filesDir, 0);
    std::string pathStr(cPath);
    for(int i = 0; i < 4; i++) {
        std::ifstream in(pathStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        if (in) in.read((char*)prg_rom[i], 16384);
    }
    std::ifstream c_in(pathStr + "/chr_rom.bin", std::ios::binary);
    if (c_in) c_in.read((char*)chr_rom, 16384);
    reg_S = 0xFD; reg_P = 0x24;
    reg_PC = (prg_rom[3][0x3FFD] << 8) | prg_rom[3][0x3FFC];
    is_running = true;
    std::thread(engine_loop).detach();
    env->ReleaseStringUTFChars(filesDir, cPath);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv* env, jobject thiz, jobject bitmap) {
    void* pixels;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;
    { std::lock_guard<std::mutex> lock(buffer_mutex); memcpy(pixels, screen_buffer, 256 * 240 * 4); }
    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* env, jobject thiz, jint bit, jboolean pressed) {
    if (pressed) input_bits |= bit; else input_bits &= ~bit;
}
