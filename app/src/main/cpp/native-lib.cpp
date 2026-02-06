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

// --- Forward Declarations ---
// This prevents the "undeclared identifier" error in engine_loop
void draw_frame(); 

// --- Global Hardware State ---
uint8_t cpu_ram[0x0800] = {0};
uint8_t ppu_vram[2048] = {0}; 
uint8_t palette_ram[32] = {0};
uint8_t oam_ram[256] = {0}; 
uint32_t screen_buffer[256 * 240] = {0};
uint8_t controller_state = 0;
uint8_t controller_shift = 0;
uint8_t ppu_data_buffer = 0;
std::mutex buffer_mutex;
MapperMMC1 mapper;

// Basic NES Palette (standard colors)
uint32_t nes_palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00,
    0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFEFB0FF, 0xFF64B0FF, 0xFF9290FF, 0xFFC676FF, 0xFFF36AFF, 0xFFFE6ECC, 0xFFFE8170, 0xFFEA9E22,
    0xFFBCBE00, 0xFF88D100, 0xFF5CE430, 0xFF45E082, 0xFF48CDDE, 0xFF4F4F4F, 0xFF000000, 0xFF000000
};

// --- Shared 6502 CPU State ---
extern "C" {
    uint16_t reg_PC = 0;
    uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;
    void execute_instruction(); 
    uint8_t bus_read(uint16_t addr);
    void bus_write(uint16_t addr, uint8_t val);
    void push_stack(uint8_t val) { cpu_ram[0x0100 | reg_S] = val; reg_S--; }
    uint8_t pop_stack() { reg_S++; return cpu_ram[0x0100 | reg_S]; }
    void update_nz(uint8_t val) { 
        reg_P &= ~0x82; 
        if (val == 0) reg_P |= 0x02;
        if (val & 0x80) reg_P |= 0x80;
    }
    void update_flags_cmp(uint8_t reg, uint8_t val) {
        reg_P &= ~0x83; 
        if (reg >= val) reg_P |= 0x01; 
        if (reg == val) reg_P |= 0x02; 
        if ((uint8_t)(reg - val) & 0x80) reg_P |= 0x80; 
    }
    void cpu_adc(uint8_t val) {
        uint16_t carry = (reg_P & 0x01);
        uint16_t sum = reg_A + val + carry;
        if (~(reg_A ^ val) & (reg_A ^ sum) & 0x80) reg_P |= 0x40; else reg_P &= ~0x40;
        if (sum > 0xFF) reg_P |= 0x01; else reg_P &= ~0x01;
        reg_A = (uint8_t)sum;
        update_nz(reg_A);
    }
    void cpu_sbc(uint8_t val) { cpu_adc(~val); }
    void cpu_bit(uint8_t val) {
        reg_P &= ~0xC2; 
        if ((val & reg_A) == 0) reg_P |= 0x02;
        reg_P |= (val & 0xC0); 
    }
    uint16_t read_pointer(uint16_t addr) {
        uint8_t lo = bus_read(addr);
        uint8_t hi = bus_read((addr & 0xFF00) | ((addr + 1) & 0x00FF));
        return (hi << 8) | lo;
    }
}

// --- PPU & Mirroring ---
uint8_t ppu_status = 0, ppu_ctrl = 0, ppu_mask = 0, ppu_addr_latch = 0, oam_addr = 0;
uint16_t ppu_addr_reg = 0;
bool is_running = false;
bool is_paused = false;

uint16_t ntable_mirror(uint16_t addr) {
    addr = (addr - 0x2000) % 0x1000;
    return addr % 0x0800; 
}

uint8_t read_palette(uint16_t addr) {
    uint8_t p_idx = addr & 0x1F;
    if (p_idx >= 0x10 && (p_idx % 4 == 0)) p_idx -= 0x10; 
    return palette_ram[p_idx];
}

extern "C" uint8_t bus_read(uint16_t addr) {
    if (addr < 0x2000) return cpu_ram[addr % 0x0800];
    if (addr >= 0x2000 && addr <= 0x3FFF) {
        uint16_t reg = addr % 8;
        if (reg == 2) { 
            uint8_t s = ppu_status; 
            ppu_status &= ~0x80; 
            ppu_addr_latch = 0; 
            return s; 
        }
        if (reg == 7) {
            uint8_t data = ppu_data_buffer;
            uint16_t p_addr = ppu_addr_reg & 0x3FFF;
            if (p_addr < 0x2000) ppu_data_buffer = mapper.read_chr(p_addr);
            else if (p_addr < 0x3F00) ppu_data_buffer = ppu_vram[ntable_mirror(p_addr)];
            else data = read_palette(p_addr);
            ppu_addr_reg += (ppu_ctrl & 0x04) ? 32 : 1;
            return data;
        }
    }
    if (addr >= 0x6000) return mapper.read_prg(addr);
    if (addr == 0x4016) { 
        uint8_t ret = (controller_shift & 0x80) >> 7; 
        controller_shift <<= 1; 
        return ret; 
    }
    return 0;
}

extern "C" void bus_write(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) cpu_ram[addr % 0x0800] = val;
    else if (addr >= 0x2000 && addr <= 0x3FFF) {
        uint16_t reg = addr % 8;
        if (reg == 0) ppu_ctrl = val;
        else if (reg == 1) ppu_mask = val;
        else if (reg == 3) oam_addr = val;
        else if (reg == 4) oam_ram[oam_addr++] = val;
        else if (reg == 6) {
            if (ppu_addr_latch == 0) { ppu_addr_reg = (ppu_addr_reg & 0x00FF) | (val << 8); ppu_addr_latch = 1; }
            else { ppu_addr_reg = (ppu_addr_reg & 0xFF00) | val; ppu_addr_latch = 0; }
        }
        else if (reg == 7) {
            uint16_t p_addr = ppu_addr_reg & 0x3FFF;
            if (p_addr >= 0x2000 && p_addr < 0x3F00) ppu_vram[ntable_mirror(p_addr)] = val;
            else if (p_addr >= 0x3F00) {
                uint8_t p_idx = p_addr & 0x1F;
                if (p_idx >= 0x10 && (p_idx % 4 == 0)) p_idx -= 0x10;
                palette_ram[p_idx] = val;
            }
            ppu_addr_reg += (ppu_ctrl & 0x04) ? 32 : 1;
        }
    }
    else if (addr == 0x4014) { 
        uint16_t base = val << 8;
        for (int i = 0; i < 256; i++) oam_ram[i] = bus_read(base + i);
    }
    else if (addr == 0x4016) { if ((val & 0x01) == 0) controller_shift = controller_state; }
    else if (addr >= 0x6000) mapper.write(addr, val);
}

// --- Implementation of Rendering ---
void draw_frame() {
    std::lock_guard<std::mutex> lock(buffer_mutex);
    uint16_t nt_base = (ppu_ctrl & 0x03) * 1024;
    uint16_t bg_pat_base = (ppu_ctrl & 0x10) ? 0x1000 : 0x0000;

    for (int ty = 0; ty < 30; ty++) {
        for (int tx = 0; tx < 32; tx++) {
            uint8_t tile = ppu_vram[ntable_mirror(0x2000 + nt_base + ty * 32 + tx)];
            for (int y = 0; y < 8; y++) {
                uint8_t p1 = mapper.read_chr(bg_pat_base + tile * 16 + y);
                uint8_t p2 = mapper.read_chr(bg_pat_base + tile * 16 + y + 8);
                for (int x = 0; x < 8; x++) {
                    uint8_t pix = ((p1 >> (7 - x)) & 0x01) | (((p2 >> (7 - x)) & 0x01) << 1);
                    uint8_t pal_entry = read_palette(pix);
                    screen_buffer[(ty * 8 + y) * 256 + (tx * 8 + x)] = nes_palette[pal_entry & 0x3F];
                }
            }
        }
    }
}

void power_on_reset() {
    reg_PC = (bus_read(0xFFFD) << 8) | bus_read(0xFFFC);
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Reset! PC set to: %04X", reg_PC);
}

void nmi_handler() {
    push_stack(reg_PC >> 8);
    push_stack(reg_PC & 0xFF);
    push_stack(reg_P);
    reg_PC = (bus_read(0xFFFB) << 8) | bus_read(0xFFFA);
}

void engine_loop() {
    power_on_reset();
    while (is_running) {
        if (is_paused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
            continue;
        }
        auto start_time = std::chrono::steady_clock::now();
        for (int i = 0; i < 15000; i++) { execute_instruction(); }
        ppu_status |= 0x80; 
        if (ppu_ctrl & 0x80) nmi_handler();
        draw_frame();
        std::this_thread::sleep_until(start_time + std::chrono::microseconds(16666));
    }
}

// ... Rest of your JNI methods (nativeInitEngine, nativeUpdateSurface, etc.)
