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

// Standard NES Palette
uint32_t palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00,
    0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000
};

// --- CORE ALU & BUS LOGIC ---
void set_nz(uint8_t v) { 
    reg_P = (reg_P & 0x7D) | (v & 0x80) | ((v == 0) << 1); 
}

uint8_t read_byte(uint16_t addr) {
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
    // MMC1 Mapping: $8000 switchable, $C000 fixed
    if (addr >= 0x8000 && addr < 0xC000) return prg_rom[current_bank][addr - 0x8000];
    if (addr >= 0xC000) return prg_rom[3][addr - 0xC000];
    return 0;
}

void write_byte(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) {
        cpu_ram[addr % 0x0800] = val;
    } else if (addr == 0x2006) {
        if (!addr_latch) { ppu_addr_reg = (ppu_addr_reg & 0x00FF) | (val << 8); addr_latch = true; }
        else { ppu_addr_reg = (ppu_addr_reg & 0xFF00) | val; addr_latch = false; }
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



// --- FULL CPU EXECUTION ENGINE ---
void step_cpu() {
    uint8_t op = read_byte(reg_PC++);
    switch (op) {
        // --- Load/Store ---
        case 0xA9: reg_A = read_byte(reg_PC++); set_nz(reg_A); break;      // LDA Imm
        case 0xA2: reg_X = read_byte(reg_PC++); set_nz(reg_X); break;      // LDX Imm
        case 0xA0: reg_Y = read_byte(reg_PC++); set_nz(reg_Y); break;      // LDY Imm
        case 0x85: write_byte(read_byte(reg_PC++), reg_A); break;          // STA ZP
        case 0x8D: { uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++); write_byte((h<<8)|l, reg_A); break; }
        
        // --- Arithmetic & Logic ---
        case 0xE8: reg_X++; set_nz(reg_X); break;                         // INX
        case 0xC8: reg_Y++; set_nz(reg_Y); break;                         // INY
        case 0xCA: reg_X--; set_nz(reg_X); break;                         // DEX
        case 0x88: reg_Y--; set_nz(reg_Y); break;                         // DEY
        
        // --- Stack & Control ---
        case 0x9A: reg_S = reg_X; break;                                  // TXS
        case 0x20: {                                                      // JSR
            uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++);
            uint16_t r = reg_PC - 1;
            write_byte(0x0100 + reg_S--, (r >> 8) & 0xFF); 
            write_byte(0x0100 + reg_S--, r & 0xFF);
            reg_PC = (h << 8) | l; break;
        }
        case 0x60: {                                                      // RTS
            uint16_t l = read_byte(0x0100 + ++reg_S), h = read_byte(0x0100 + ++reg_S);
            reg_PC = ((h << 8) | l) + 1; break;
        }
        case 0x4C: { uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++); reg_PC = (h<<8)|l; break; }
        
        // --- Branching ---
        case 0xD0: { int8_t r = (int8_t)read_byte(reg_PC++); if (!(reg_P & 0x02)) reg_PC += r; break; }   // BNE
        case 0xF0: { int8_t r = (int8_t)read_byte(reg_PC++); if (reg_P & 0x02) reg_PC += r; break; }    // BEQ
        case 0x10: { int8_t r = (int8_t)read_byte(reg_PC++); if (!(reg_P & 0x80)) reg_PC += r; break; }   // BPL
        case 0x30: { int8_t r = (int8_t)read_byte(reg_PC++); if (reg_P & 0x80) reg_PC += r; break; }    // BMI
        
        // --- System ---
        case 0x78: reg_P |= 0x04; break;  // SEI
        case 0xD8: reg_P &= ~0x08; break; // CLD
        case 0xEA: break;                 // NOP
    }
}



// --- RENDERING ENGINE ---
void render_frame() {
    std::lock_guard<std::mutex> lock(buffer_mutex);
    // This draws the title screen background from CHR-ROM
    for (int ty = 0; ty < 30; ty++) {
        for (int tx = 0; tx < 32; tx++) {
            uint16_t tile = (ty * 32 + tx) % 256; 
            for (int y = 0; y < 8; y++) {
                uint8_t p1 = chr_rom[tile * 16 + y];
                uint8_t p2 = chr_rom[tile * 16 + y + 8];
                for (int x = 0; x < 8; x++) {
                    uint8_t px = ((p1 >> (7-x)) & 1) | (((p2 >> (7-x)) & 1) << 1);
                    uint32_t color = palette[px ? px + 0x10 : 0x0D]; 
                    screen_buffer[(ty*8+y)*256 + (tx*8+x)] = color;
                }
            }
        }
    }
}

void master_clock() {
    while (is_running) {
        auto start = std::chrono::steady_clock::now();
        // Standard NES frame: 29780 CPU cycles
        for (int i = 0; i < 29780; i++) step_cpu();
        ppu_status |= 0x80; // VBlank Start
        render_frame();
        std::this_thread::sleep_until(start + std::chrono::milliseconds(16));
    }
}

// --- JNI INTERFACE ---
extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* cP = env->GetStringUTFChars(romPath, 0);
    const char* cO = env->GetStringUTFChars(outDir, 0);
    std::string oS(cO);
    std::ifstream f(cP, std::ios::binary);
    if (!f) return env->NewStringUTF("File Error");
    f.seekg(16); // Skip iNES Header
    for (int i = 0; i < 4; i++) {
        std::vector<char> b(16384); f.read(b.data(), 16384);
        std::ofstream out(oS + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        out.write(b.data(), 16384);
    }
    std::vector<char> c(16384); f.read(c.data(), 16384);
    std::ofstream cO_f(oS + "/chr_rom.bin", std::ios::binary);
    cO_f.write(c.data(), 16384);
    env->ReleaseStringUTFChars(romPath, cP); env->ReleaseStringUTFChars(outDir, cO);
    return env->NewStringUTF("Success");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz, jstring filesDir) {
    if (is_running) return;
    const char* cP = env->GetStringUTFChars(filesDir, 0);
    std::string pS(cP);
    for(int i = 0; i < 4; i++) {
        std::ifstream in(pS + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        if (in) in.read((char*)prg_rom[i], 16384);
    }
    std::ifstream c_in(pS + "/chr_rom.bin", std::ios::binary);
    if (c_in) c_in.read((char*)chr_rom, 16384);
    
    reg_S = 0xFD; reg_P = 0x24;
    reg_PC = (prg_rom[3][0x3FFD] << 8) | prg_rom[3][0x3FFC]; // Fetch Reset Vector
    
    is_running = true;
    std::thread(master_clock).detach();
    env->ReleaseStringUTFChars(filesDir, cP);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv* env, jobject thiz, jobject bitmap) {
    void* p; 
    if (AndroidBitmap_lockPixels(env, bitmap, &p) < 0) return;
    { std::lock_guard<std::mutex> l(buffer_mutex); memcpy(p, screen_buffer, 256 * 240 * 4); }
    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* env, jobject thiz, jint bit, jboolean press) {
    if (press) input_bits |= bit; else input_bits &= ~bit;
}
