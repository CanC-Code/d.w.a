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

// Standard NES Palette
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

// --- Memory Access Helpers ---
uint8_t bus_read(uint16_t addr) {
    if (addr < 0x2000) return cpu_ram[addr % 0x0800];
    if (addr == 0x2002) { 
        uint8_t s = ppu_status; 
        ppu_status &= ~0x80; 
        ppu_addr_latch = 0; 
        return s; 
    }
    if (addr >= 0x8000) return mapper.read_prg(addr);
    return 0;
}

void bus_write(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) {
        cpu_ram[addr % 0x0800] = val;
    } else if (addr == 0x2000) {
        ppu_ctrl = val;
    } else if (addr == 0x2006) {
        if (ppu_addr_latch == 0) {
            ppu_addr_reg = (ppu_addr_reg & 0x00FF) | (val << 8);
            ppu_addr_latch = 1;
        } else {
            ppu_addr_reg = (ppu_addr_reg & 0xFF00) | val;
            ppu_addr_latch = 0;
        }
    } else if (addr == 0x2007) {
        uint16_t p_addr = ppu_addr_reg & 0x3FFF;
        if (p_addr >= 0x2000 && p_addr < 0x3F00) {
            ppu_vram[(p_addr - 0x2000) % 2048] = val;
        } else if (p_addr >= 0x3F00) {
            palette_ram[p_addr & 0x1F] = val;
        }
        ppu_addr_reg += (ppu_ctrl & 0x04) ? 32 : 1;
    } else if (addr >= 0x8000) {
        mapper.write(addr, val);
    }
}

// --- CPU Instruction Implementation ---
void update_nz(uint8_t val) {
    reg_P = (reg_P & 0x7D) | (val & 0x80) | ((val == 0) << 1);
}

void execute_instruction() {
    // Safety check to prevent running into empty space
    if (reg_PC < 0x8000 && reg_PC >= 0x2000) {
        reg_PC = 0xFFFC; // Force reset if lost
        return;
    }

    uint8_t opcode = bus_read(reg_PC++);
    switch (opcode) {
        case 0xA9: reg_A = bus_read(reg_PC++); update_nz(reg_A); break; // LDA Imm
        case 0x85: bus_write(bus_read(reg_PC++), reg_A); break;         // STA ZP
        case 0x4C: { // JMP Abs
            uint16_t l = bus_read(reg_PC++);
            uint16_t h = bus_read(reg_PC++);
            reg_PC = (h << 8) | l; 
            break; 
        }
        case 0x20: { // JSR
            uint16_t l = bus_read(reg_PC++);
            uint16_t h = bus_read(reg_PC++);
            uint16_t ret = reg_PC - 1;
            bus_write(0x0100 + reg_S--, (ret >> 8) & 0xFF);
            bus_write(0x0100 + reg_S--, ret & 0xFF);
            reg_PC = (h << 8) | l;
            break;
        }
        case 0x60: { // RTS
            uint16_t l = bus_read(0x0100 + ++reg_S);
            uint16_t h = bus_read(0x0100 + ++reg_S);
            reg_PC = ((h << 8) | l) + 1;
            break;
        }
        case 0x78: reg_P |= 0x04; break; // SEI
        case 0xD8: reg_P &= ~0x08; break; // CLD
        case 0xEA: break; // NOP
        default: break; // Handle undefined opcodes safely
    }
}

// --- Frame Loop ---
void draw_frame() {
    std::lock_guard<std::mutex> lock(buffer_mutex);
    for (int ty = 0; ty < 30; ty++) {
        for (int tx = 0; tx < 32; tx++) {
            uint8_t tile_id = ppu_vram[ty * 32 + tx];
            for (int y = 0; y < 8; y++) {
                uint8_t p1 = mapper.read_chr(tile_id * 16 + y);
                uint8_t p2 = mapper.read_chr(tile_id * 16 + y + 8);
                for (int x = 0; x < 8; x++) {
                    uint8_t pix = ((p1 >> (7-x)) & 0x01) | (((p2 >> (7-x)) & 0x01) << 1);
                    uint32_t color = pix ? nes_palette[palette_ram[pix] & 0x3F] : 0xFF000000;
                    screen_buffer[(ty*8+y)*256 + (tx*8+x)] = color;
                }
            }
        }
    }
}

void engine_loop() {
    while (is_running) {
        auto start = std::chrono::steady_clock::now();
        // 29780 cycles per frame
        for (int i = 0; i < 29780; i++) {
            execute_instruction();
        }
        ppu_status |= 0x80; 
        draw_frame();
        std::this_thread::sleep_until(start + std::chrono::milliseconds(16));
    }
}

// --- JNI Interface ---
extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz, jstring filesDir) {
    if (is_running) return;
    const char* cPath = env->GetStringUTFChars(filesDir, 0);
    std::string pathStr(cPath);

    // Safety: Clear RAM and buffers
    memset(cpu_ram, 0, 0x0800);
    memset(ppu_vram, 0, 2048);

    // Load ROMs into Mapper
    for(int i = 0; i < 4; i++) {
        std::ifstream in(pathStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        if (in) in.read((char*)mapper.prg_rom[i], 16384);
    }
    std::ifstream c_in(pathStr + "/chr_rom.bin", std::ios::binary);
    if (c_in) c_in.read((char*)mapper.chr_rom[0], 4096);
    
    // Set Reset Vector
    uint8_t low = mapper.read_prg(0xFFFC);
    uint8_t high = mapper.read_prg(0xFFFD);
    reg_PC = (high << 8) | low;

    if (reg_PC == 0) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "CRITICAL: Reset Vector is 0! ROM banks failed to load.");
        env->ReleaseStringUTFChars(filesDir, cPath);
        return;
    }

    is_running = true;
    std::thread(engine_loop).detach();
    env->ReleaseStringUTFChars(filesDir, cPath);
}

// Ensure other JNI exports (nativeExtractRom, nativeUpdateSurface, etc.) are present below...
