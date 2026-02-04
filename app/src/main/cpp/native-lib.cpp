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
#include "MapperMMC1.h"

#define LOG_TAG "DWA_NATIVE"

// Hardware State
uint8_t cpu_ram[0x0800] = {0};
uint8_t ppu_vram[2048] = {0};      
uint8_t palette_ram[32] = {0}; 
uint32_t screen_buffer[256 * 240] = {0};
std::mutex buffer_mutex;
MapperMMC1 mapper;

// Registers
uint16_t reg_PC = 0;
uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;
uint8_t ppu_status = 0, ppu_ctrl = 0, ppu_addr_latch = 0;
uint16_t ppu_addr_reg = 0;
bool is_running = false;

uint32_t nes_palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00,
    0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000
};

// --- Memory Access ---
uint8_t bus_read(uint16_t addr) {
    if (addr < 0x2000) return cpu_ram[addr % 0x0800];
    if (addr == 0x2002) { uint8_t s = ppu_status; ppu_status &= ~0x80; ppu_addr_latch = 0; return s; }
    if (addr >= 0x8000) return mapper.read_prg(addr);
    return 0;
}

void bus_write(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) cpu_ram[addr % 0x0800] = val;
    else if (addr == 0x2000) ppu_ctrl = val;
    else if (addr == 0x2006) {
        if (ppu_addr_latch == 0) { ppu_addr_reg = (ppu_addr_reg & 0x00FF) | (val << 8); ppu_addr_latch = 1; }
        else { ppu_addr_reg = (ppu_addr_reg & 0xFF00) | val; ppu_addr_latch = 0; }
    }
    else if (addr == 0x2007) {
        uint16_t p_addr = ppu_addr_reg & 0x3FFF;
        if (p_addr >= 0x2000 && p_addr < 0x3F00) ppu_vram[(p_addr - 0x2000) % 2048] = val;
        else if (p_addr >= 0x3F00) palette_ram[p_addr & 0x1F] = val;
        ppu_addr_reg += (ppu_ctrl & 0x04) ? 32 : 1;
    }
    else if (addr >= 0x8000) mapper.write(addr, val);
}

void update_nz(uint8_t val) {
    reg_P = (reg_P & 0x7D) | (val & 0x80) | ((val == 0) << 1);
}

// --- BOOT-CRITICAL CPU CORE ---
void execute_instruction() {
    uint8_t opcode = bus_read(reg_PC++);
    switch (opcode) {
        case 0xA9: reg_A = bus_read(reg_PC++); update_nz(reg_A); break; // LDA Imm
        case 0xA2: reg_X = bus_read(reg_PC++); update_nz(reg_X); break; // LDX Imm
        case 0xA0: reg_Y = bus_read(reg_PC++); update_nz(reg_Y); break; // LDY Imm
        case 0x85: bus_write(bus_read(reg_PC++), reg_A); break;         // STA ZP
        case 0x86: bus_write(bus_read(reg_PC++), reg_X); break;         // STX ZP
        case 0x9A: reg_S = reg_X; break;                                // TXS
        case 0x4C: {                                                    // JMP Abs
            uint16_t l = bus_read(reg_PC++);
            uint16_t h = bus_read(reg_PC++);
            reg_PC = (h << 8) | l; 
            break; 
        }
        case 0x20: {                                                    // JSR
            uint16_t l = bus_read(reg_PC++);
            uint16_t h = bus_read(reg_PC++);
            uint16_t ret = reg_PC - 1;
            bus_write(0x0100 + reg_S--, (ret >> 8) & 0xFF);
            bus_write(0x0100 + reg_S--, ret & 0xFF);
            reg_PC = (h << 8) | l;
            break;
        }
        case 0x60: {                                                    // RTS
            uint16_t l = bus_read(0x0100 + ++reg_S);
            uint16_t h = bus_read(0x0100 + ++reg_S);
            reg_PC = ((h << 8) | l) + 1;
            break;
        }
        case 0x78: reg_P |= 0x04; break;  // SEI
        case 0xD8: reg_P &= ~0x08; break; // CLD
        case 0xEA: break;                 // NOP
        default: 
            __android_log_print(ANDROID_LOG_WARN, LOG_TAG, "Unknown Opcode: %02X at %04X", opcode, reg_PC-1);
            break; 
    }
}

// ... draw_frame and engine_loop remain the same ...

// --- MISSING JNI EXPORTS ---

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

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* cPath = env->GetStringUTFChars(romPath, 0);
    const char* cOut = env->GetStringUTFChars(outDir, 0);
    std::string outDirStr(cOut);
    std::ifstream nes(cPath, std::ios::binary);
    
    if (!nes) return env->NewStringUTF("File Error");
    
    nes.seekg(16); // Skip iNES Header
    for (int i = 0; i < 4; i++) {
        std::vector<char> buf(16384);
        nes.read(buf.data(), 16384);
        std::ofstream out(outDirStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        out.write(buf.data(), 16384);
    }
    
    std::vector<char> chr(16384);
    nes.read(chr.data(), 16384);
    std::ofstream c_out(outDirStr + "/chr_rom.bin", std::ios::binary);
    c_out.write(chr.data(), 16384);
    
    env->ReleaseStringUTFChars(romPath, cPath);
    env->ReleaseStringUTFChars(outDir, cOut);
    return env->NewStringUTF("Success");
}
