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

// Hardware
uint8_t cpu_ram[0x0800];
uint8_t ppu_vram[2048];      // Nametables for tile placement
uint8_t palette_ram[32]; 
uint32_t screen_buffer[256 * 240];
std::mutex buffer_mutex;
MapperMMC1 mapper;

// Registers
uint16_t reg_PC;
uint8_t reg_A, reg_X, reg_Y, reg_S, reg_P;
uint8_t ppu_status, ppu_ctrl, ppu_addr_latch;
uint16_t ppu_addr_reg;
bool is_running = false;

uint32_t nes_palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00,
    0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000
};

// --- Bus Access ---
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

// --- Rendering Logic ---
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

// ... execute_instruction() implementation remains similar to previous version ...

void engine_loop() {
    while (is_running) {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < 29780; i++) {
            // Placeholder: Full CPU Step
        }
        ppu_status |= 0x80; 
        draw_frame();
        std::this_thread::sleep_until(start + std::chrono::milliseconds(16));
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz, jstring filesDir) {
    if (is_running) return;
    const char* cPath = env->GetStringUTFChars(filesDir, 0);
    std::string pathStr(cPath);
    for(int i = 0; i < 4; i++) {
        std::ifstream in(pathStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        if (in) in.read((char*)mapper.prg_rom[i], 16384);
    }
    std::ifstream c_in(pathStr + "/chr_rom.bin", std::ios::binary);
    if (c_in) c_in.read((char*)mapper.chr_rom[0], 4096);
    
    reg_PC = (mapper.read_prg(0xFFFD) << 8) | mapper.read_prg(0xFFFC);
    is_running = true;
    std::thread(engine_loop).detach();
    env->ReleaseStringUTFChars(filesDir, cPath);
}
