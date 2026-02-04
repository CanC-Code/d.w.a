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
#include "MapperMMC1.h" // Ensure this is in your project

#define LOG_TAG "DWA_NATIVE"

// --- Hardware State ---
uint8_t cpu_ram[0x0800];
uint8_t ppu_vram[2048];      [span_3](start_span)// Nametables[span_3](end_span)
uint8_t palette_ram[32];     [span_4](start_span)// Palette memory[span_4](end_span)
uint32_t screen_buffer[256 * 240];
std::mutex buffer_mutex;

MapperMMC1 mapper; [span_5](start_span)[span_6](start_span)// Using your updated Mapper[span_5](end_span)[span_6](end_span)

// --- CPU State ---
uint16_t reg_PC;
uint8_t reg_A, reg_X, reg_Y, reg_S, reg_P;
bool is_running = false;

// --- PPU State ---
uint8_t ppu_status = 0;
uint8_t ppu_ctrl = 0;
uint16_t ppu_addr_reg = 0;
bool addr_latch = false;
uint8_t input_bits = 0, input_latched = 0;

uint32_t nes_palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00,
    0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000
};

// --- Memory Mapping ---
uint16_t mirror_address(uint16_t addr) {
    addr = (addr - 0x2000) % 2048; [span_7](start_span)// Simplified Mirroring[span_7](end_span)
    return addr;
}

uint8_t bus_read(uint16_t addr) {
    [span_8](start_span)if (addr < 0x2000) return cpu_ram[addr % 0x0800];[span_8](end_span)
    if (addr == 0x2002) { 
        [span_9](start_span)uint8_t s = ppu_status; ppu_status &= ~0x80; addr_latch = false; return s;[span_9](end_span)
    }
    if (addr == 0x4016) { 
        uint8_t b = (input_latched & 0x80) >> 7; input_latched <<= 1; return b | [span_10](start_span)0x40;[span_10](end_span)
    }
    if (addr >= 0x8000) return mapper.read_prg(addr); [span_11](start_span)// Use updated Mapper[span_11](end_span)
    return 0;
}

void bus_write(uint16_t addr, uint8_t val) {
    [span_12](start_span)if (addr < 0x2000) cpu_ram[addr % 0x0800] = val;[span_12](end_span)
    [span_13](start_span)else if (addr == 0x2000) ppu_ctrl = val;[span_13](end_span)
    else if (addr == 0x2006) {
        if (!addr_latch) { ppu_addr_reg = (ppu_addr_reg & 0x00FF) | (val << 8); addr_latch = true; [span_14](start_span)}
        else { ppu_addr_reg = (ppu_addr_reg & 0xFF00) | val; addr_latch = false; }
    }
    else if (addr == 0x2007) {
        uint16_t paddr = ppu_addr_reg & 0x3FFF;
        if (paddr >= 0x2000 && paddr < 0x3F00) ppu_vram[mirror_address(paddr)] = val;[span_14](end_span)
        [span_15](start_span)else if (paddr >= 0x3F00) palette_ram[paddr & 0x1F] = val;[span_15](end_span)
        ppu_addr_reg += (ppu_ctrl & 0x04) ? [span_16](start_span)32 : 1;[span_16](end_span)
    }
    [span_17](start_span)else if (addr == 0x4016) { if (val & 0x01) input_latched = input_bits;[span_17](end_span) }
    else if (addr >= 0x8000) mapper.write(addr, val); [span_18](start_span)[span_19](start_span)// Pass to MMC1[span_18](end_span)[span_19](end_span)
}

// --- RENDERING FIX ---
void draw_frame() {
    std::lock_guard<std::mutex> lock(buffer_mutex);
    [span_20](start_span)// 1. Read Nametable to find which tile to draw where[span_20](end_span)
    for (int ty = 0; ty < 30; ty++) {
        for (int tx = 0; tx < 32; tx++) {
            uint16_t nt_addr = ty * 32 + tx;
            uint8_t tile_id = ppu_vram[nt_addr]; [span_21](start_span)// Which tile[span_21](end_span)

            [span_22](start_span)// 2. Fetch tile graphics from CHR banks via Mapper[span_22](end_span)
            for (int y = 0; y < 8; y++) {
                uint8_t p1 = mapper.read_chr(tile_id * 16 + y);
                uint8_t p2 = mapper.read_chr(tile_id * 16 + y + 8);
                
                for (int x = 0; x < 8; x++) {
                    uint8_t pixel = ((p1 >> (7-x)) & 0x01) | (((p2 >> (7-x)[span_23](start_span)) & 0x01) << 1);[span_23](end_span)
                    [span_24](start_span)// Standard background color logic[span_24](end_span)
                    uint32_t color = pixel ? nes_palette[palette_ram[pixel] & 0x3F] : 0xFF000000;
                    [span_25](start_span)screen_buffer[(ty*8+y)*256 + (tx*8+x)] = color;[span_25](end_span)
                }
            }
        }
    }
}

// --- CPU Core ---
void update_nz(uint8_t val) { reg_P = (reg_P & 0x7D) | (val & 0x80) | ((val == 0) << 1); [span_26](start_span)}

void execute_instruction() {
    uint8_t opcode = bus_read(reg_PC++);
    switch (opcode) {
        case 0xA9: reg_A = bus_read(reg_PC++); update_nz(reg_A); break;[span_26](end_span)
        [span_27](start_span)case 0x85: bus_write(bus_read(reg_PC++), reg_A); break;[span_27](end_span)
        case 0x4C: { uint16_t l = bus_read(reg_PC++), h = bus_read(reg_PC++); reg_PC = (h<<8)|l; break; }
        [span_28](start_span)case 0x20: { // JSR[span_28](end_span)
            uint16_t l = bus_read(reg_PC++), h = bus_read(reg_PC++);
            uint16_t ret = reg_PC - 1;
            [span_29](start_span)bus_write(0x0100 + reg_S--, (ret >> 8) & 0xFF); bus_write(0x0100 + reg_S--, ret & 0xFF);[span_29](end_span)
            reg_PC = (h << 8) | l; break;
        }
        [span_30](start_span)case 0x60: { // RTS[span_30](end_span)
            uint16_t l = bus_read(0x0100 + ++reg_S), h = bus_read(0x0100 + ++reg_S);
            [span_31](start_span)reg_PC = ((h << 8) | l) + 1; break;[span_31](end_span)
        }
        case 0xD0: { int8_t rel = (int8_t)bus_read(reg_PC++); if (!(reg_P & 0x02)) reg_PC += rel; break; [span_32](start_span)}
        case 0xEA: break; // NOP[span_32](end_span)
        // Add more opcodes as needed for full boot
    }
}

void engine_loop() {
    while (is_running) {
        auto frame_start = std::chrono::steady_clock::now();
        [span_33](start_span)for (int i = 0; i < 29780; i++) execute_instruction();[span_33](end_span)
        ppu_status |= 0x80; [span_34](start_span)// VBlank[span_34](end_span)
        draw_frame();
        [span_35](start_span)std::this_thread::sleep_until(frame_start + std::chrono::milliseconds(16));[span_35](end_span)
    }
}

// ... JNI Functions calling mapper.prg_rom and mapper.chr_rom instead of global arrays ...
