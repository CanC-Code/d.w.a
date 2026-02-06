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

// --- Global Hardware State ---
uint8_t cpu_ram[0x0800] = {0};
uint8_t ppu_vram[2048] = {0}; // 2KB for Nametables
uint8_t palette_ram[32] = {0};
uint8_t oam_ram[256] = {0}; 
uint32_t screen_buffer[256 * 240] = {0};
uint8_t controller_state = 0;
uint8_t controller_shift = 0;
uint8_t ppu_data_buffer = 0;
std::mutex buffer_mutex;
MapperMMC1 mapper;

// --- Shared 6502 CPU State ---
extern "C" {
    uint16_t reg_PC = 0; // Program Counter
    uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;
    
    // IMPORT from recompiled code
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

// --- PPU & Mirroring Helpers ---
uint8_t ppu_status = 0, ppu_ctrl = 0, ppu_mask = 0, ppu_addr_latch = 0, oam_addr = 0;
uint16_t ppu_addr_reg = 0;
bool is_running = false;
bool is_paused = false;

// NES hardware uses mirroring to map 4 Nametables into 2KB of RAM
uint16_t ntable_mirror(uint16_t addr) {
    addr = (addr - 0x2000) % 0x1000;
    // Dragon Warrior (MMC1) typically uses Vertical or Horizontal mirroring
    // This is a simplified Vertical Mirroring check
    return addr % 0x0800; 
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
            else data = palette_ram[p_addr & 0x1F];
            
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
            if (p_addr < 0x2000) { /* CHR ROM is usually read only */ }
            else if (p_addr < 0x3F00) ppu_vram[ntable_mirror(p_addr)] = val;
            else palette_ram[p_addr & 0x1F] = val;
            ppu_addr_reg += (ppu_ctrl & 0x04) ? 32 : 1;
        }
    }
    else if (addr == 0x4014) { // OAM DMA
        uint16_t base = val << 8;
        for (int i = 0; i < 256; i++) oam_ram[i] = bus_read(base + i);
    }
    else if (addr == 0x4016) { if ((val & 0x01) == 0) controller_shift = controller_state; }
    else if (addr >= 0x6000) mapper.write(addr, val);
}

// --- Core Loop Enhancements ---

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

        // 1. Execute CPU instructions for one frame (approx 29780 cycles)
        // For recompiled logic, we execute until the code hits a "Wait for VBlank" loop
        // or a specific cycle count.
        for (int i = 0; i < 15000; i++) { 
            execute_instruction(); 
        }

        // 2. Trigger VBlank / NMI
        ppu_status |= 0x80; 
        if (ppu_ctrl & 0x80) nmi_handler();

        // 3. Render
        draw_frame();

        // 4. Sync to 60 FPS
        std::this_thread::sleep_until(start_time + std::chrono::microseconds(16666));
    }
}
