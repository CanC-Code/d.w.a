#include <jni.h>
#include <string>
#include <fstream>
#include <android/log.h>
#include <android/bitmap.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstring>
#include "MapperMMC1.h"
#include "recompiled/cpu_shared.h"

#define LOG_TAG "DWA_NATIVE"

// --- Global Hardware State ---
uint8_t cpu_ram[0x0800] = {0};
uint8_t ppu_vram[2048] = {0};
uint8_t palette_ram[32] = {0};
uint8_t oam_ram[256] = {0};
uint32_t screen_buffer[256 * 240] = {0};
uint8_t controller_state = 0;
uint8_t controller_shift = 0;
uint8_t last_strobe = 0;
uint8_t ppu_data_buffer = 0;
std::mutex buffer_mutex;
MapperMMC1 mapper;

// PPU Internal State
uint8_t ppu_status = 0, ppu_ctrl = 0, ppu_mask = 0, ppu_addr_latch = 0, oam_addr = 0;
uint16_t ppu_addr_reg = 0;
bool is_running = false;
bool is_paused = false;

// Recompiled Dispatcher Entrance
namespace Dispatcher { void execute(); }

// --- Satisfying cpu_shared.h Externs ---
extern "C" {
    uint16_t reg_PC = 0;
    uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x34;

    void update_nz(uint8_t val) {
        reg_P &= ~(FLAG_N | FLAG_Z);
        if (val == 0) reg_P |= FLAG_Z;
        if (val & 0x80) reg_P |= FLAG_N;
    }

    void update_flags_cmp(uint8_t reg, uint8_t val) {
        reg_P &= ~(FLAG_N | FLAG_Z | FLAG_C);
        if (reg >= val) reg_P |= FLAG_C;
        if (reg == val) reg_P |= FLAG_Z;
        if ((uint8_t)(reg - val) & 0x80) reg_P |= FLAG_N;
    }

    void cpu_adc(uint8_t val) {
        uint16_t carry = (reg_P & FLAG_C) ? 1 : 0;
        uint16_t sum = reg_A + val + carry;
        if (~(reg_A ^ val) & (reg_A ^ sum) & 0x80) reg_P |= FLAG_V; else reg_P &= ~FLAG_V;
        if (sum > 0xFF) reg_P |= FLAG_C; else reg_P &= ~FLAG_C;
        reg_A = (uint8_t)sum;
        update_nz(reg_A);
    }

    void cpu_sbc(uint8_t val) { cpu_adc(~val); }

    void cpu_bit(uint8_t val) {
        reg_P &= ~(FLAG_Z | FLAG_V | FLAG_N);
        if ((val & reg_A) == 0) reg_P |= FLAG_Z;
        reg_P |= (val & 0xC0);
    }

    uint8_t cpu_asl(uint8_t val) {
        reg_P = (reg_P & ~FLAG_C) | ((val >> 7) & 0x01);
        uint8_t res = val << 1;
        update_nz(res);
        return res;
    }

    uint8_t cpu_lsr(uint8_t val) {
        reg_P = (reg_P & ~FLAG_C) | (val & 0x01);
        uint8_t res = val >> 1;
        update_nz(res);
        return res;
    }

    uint8_t cpu_rol(uint8_t val) {
        uint8_t old_c = (reg_P & FLAG_C) ? 1 : 0;
        reg_P = (reg_P & ~FLAG_C) | ((val >> 7) & 0x01);
        uint8_t res = (val << 1) | old_c;
        update_nz(res);
        return res;
    }

    uint8_t cpu_ror(uint8_t val) {
        uint8_t old_c = (reg_P & FLAG_C) ? 1 : 0;
        reg_P = (reg_P & ~FLAG_C) | (val & 0x01);
        uint8_t res = (val >> 1) | (old_c << 7);
        update_nz(res);
        return res;
    }

    // --- Critical Addressing Helpers ---
    uint16_t read_pointer(uint16_t addr) {
        return bus_read(addr) | (bus_read(addr + 1) << 8);
    }

    uint16_t read_pointer_x(uint16_t addr) {
        return bus_read((addr + reg_X) & 0xFF) | (bus_read((addr + reg_X + 1) & 0xFF) << 8);
    }

    uint16_t read_pointer_indexed_y(uint16_t zp_addr) {
        uint16_t lo = bus_read(zp_addr);
        uint16_t hi = bus_read((zp_addr + 1) & 0xFF);
        return (lo | (hi << 8)) + reg_Y;
    }

    void push_stack(uint8_t val) { cpu_ram[0x0100 | reg_S] = val; reg_S--; }
    uint8_t pop_stack() { reg_S++; return cpu_ram[0x0100 | reg_S]; }
}

// --- Bus Access ---
extern "C" uint8_t bus_read(uint16_t addr) {
    if (addr < 0x0800) return cpu_ram[addr];
    if (addr >= 0x2000 && addr <= 0x3FFF) {
        if ((addr % 8) == 2) {
            uint8_t s = ppu_status | 0x80; // Always signal VBlank to keep logic moving
            ppu_status &= ~0x80; ppu_addr_latch = 0; return s;
        }
        // ... other PPU regs ...
    }
    if (addr >= 0x8000) return mapper.read_prg(addr);
    return 0;
}

extern "C" void bus_write(uint16_t addr, uint8_t val) {
    if (addr < 0x0800) cpu_ram[addr] = val;
    else if (addr >= 0x8000) mapper.write(addr, val);
    // ... PPU write logic ...
}

// --- Entry Points ---
extern "C" void power_on_reset() {
    mapper.reset();
    uint16_t v_lo = mapper.read_prg(0xFFFC);
    uint16_t v_hi = mapper.read_prg(0xFFFD);
    reg_PC = (v_hi << 8) | v_lo;
    
    // Failsafe: Dragon Warrior US entry
    if (reg_PC == 0x0000 || reg_PC == 0xFFFF) reg_PC = 0xFF8E; 
    
    reg_S = 0xFD;
    reg_P = 0x34;
    LOG_CPU("Native Game Started at PC: %04X", reg_PC);
}

extern "C" void nmi_handler() {
    ppu_status |= 0x80;
    push_stack(reg_PC >> 8);
    push_stack(reg_PC & 0xFF);
    push_stack(reg_P);
    reg_PC = read_pointer(0xFFFA);
}

void engine_loop() {
    power_on_reset();
    is_running = true;
    while (is_running) {
        if (is_paused) { std::this_thread::sleep_for(std::chrono::milliseconds(16)); continue; }
        
        // Execute block of recompiled instructions
        for (int i = 0; i < 20000; i++) {
            Dispatcher::execute();
        }
        
        // Trigger recompiled VBlank logic
        if (ppu_ctrl & 0x80) nmi_handler();
        
        // Render current VRAM state
        // draw_frame(); 
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

// --- JNI Surface Update ---
extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv *env, jobject thiz, jobject bitmap) {
    AndroidBitmapInfo info;
    void* pixels;
    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0) return;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;
    
    // Simple copy to surface
    std::lock_guard<std::mutex> lock(buffer_mutex);
    memcpy(pixels, screen_buffer, 256 * 240 * 4);
    
    AndroidBitmap_unlockPixels(env, bitmap);
}
