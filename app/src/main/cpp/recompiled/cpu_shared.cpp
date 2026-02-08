#include "cpu_shared.h"

extern "C" {
    // Math Implementations
    void cpu_adc(uint8_t v) {
        uint16_t temp = (uint16_t)reg_A + (uint16_t)v + (uint16_t)(reg_P & FLAG_C);
        reg_P &= ~(FLAG_C | FLAG_V | FLAG_Z | FLAG_N);
        if (temp > 255) reg_P |= FLAG_C;
        if (~(reg_A ^ v) & (reg_A ^ temp) & 0x80) reg_P |= FLAG_V;
        reg_A = (uint8_t)temp;
        update_nz(reg_A);
    }

    void cpu_sbc(uint8_t v) {
        cpu_adc(~v); // SBC is ADC with bits inverted
    }

    void cpu_bit(uint8_t v) {
        reg_P &= ~(FLAG_Z | FLAG_V | FLAG_N);
        if (!(reg_A & v)) reg_P |= FLAG_Z;
        reg_P |= (v & 0xC0); // Copy bits 6 and 7 to V and N
    }

    // Shift Implementations
    void cpu_asl(uint16_t addr, bool is_reg) {
        uint8_t v = is_reg ? reg_A : bus_read(addr);
        reg_P = (reg_P & ~FLAG_C) | ((v >> 7) & FLAG_C);
        v <<= 1;
        update_nz(v);
        if (is_reg) reg_A = v; else bus_write(addr, v);
    }

    void cpu_lsr(uint16_t addr, bool is_reg) {
        uint8_t v = is_reg ? reg_A : bus_read(addr);
        reg_P = (reg_P & ~FLAG_C) | (v & FLAG_C);
        v >>= 1;
        update_nz(v);
        if (is_reg) reg_A = v; else bus_write(addr, v);
    }

    void cpu_rol(uint16_t addr, bool is_reg) {
        uint8_t v = is_reg ? reg_A : bus_read(addr);
        uint8_t c = (reg_P & FLAG_C);
        reg_P = (reg_P & ~FLAG_C) | ((v >> 7) & FLAG_C);
        v = (v << 1) | c;
        update_nz(v);
        if (is_reg) reg_A = v; else bus_write(addr, v);
    }

    void cpu_ror(uint16_t addr, bool is_reg) {
        uint8_t v = is_reg ? reg_A : bus_read(addr);
        uint8_t c = (reg_P & FLAG_C) << 7;
        reg_P = (reg_P & ~FLAG_C) | (v & FLAG_C);
        v = (v >> 1) | c;
        update_nz(v);
        if (is_reg) reg_A = v; else bus_write(addr, v);
    }

    // Pointer Helpers
    uint16_t read_pointer_indexed_x(uint16_t addr) {
        uint8_t ptr = (uint8_t)(bus_read(addr) + reg_X);
        return bus_read(ptr) | (bus_read((uint8_t)(ptr + 1)) << 8);
    }

    uint16_t read_pointer_indexed_y(uint16_t addr, bool* page_crossed) {
        uint16_t base = bus_read(addr) | (bus_read((uint8_t)(addr + 1)) << 8);
        uint16_t final = base + reg_Y;
        if (page_crossed && (final & 0xFF00) != (base & 0xFF00)) *page_crossed = true;
        return final;
    }

    uint16_t cpu_read_jmp_indirect(uint16_t addr) {
        uint16_t low = bus_read(addr);
        uint16_t high = bus_read((addr & 0xFF00) | ((addr + 1) & 0x00FF));
        return low | (high << 8);
    }
}
