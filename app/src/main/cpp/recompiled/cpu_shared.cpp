#include "cpu_shared.h"

extern "C" {

    // These must be inside extern "C" so the Linker finds them
    void update_nz(uint8_t v) {
        reg_P &= ~(FLAG_N | FLAG_Z);
        if (v == 0) reg_P |= FLAG_Z;
        if (v & 0x80) reg_P |= FLAG_N;
    }

    void update_flags_cmp(uint8_t reg, uint8_t val) {
        reg_P &= ~(FLAG_C | FLAG_Z | FLAG_N);
        if (reg >= val) reg_P |= FLAG_C;
        if (reg == val) reg_P |= FLAG_Z;
        if ((uint8_t)(reg - val) & 0x80) reg_P |= FLAG_N;
    }

    void cpu_adc(uint8_t v) {
        uint16_t temp = (uint16_t)reg_A + (uint16_t)v + (uint16_t)(reg_P & FLAG_C);
        reg_P &= ~(FLAG_C | FLAG_V | FLAG_Z | FLAG_N);
        if (temp > 255) reg_P |= FLAG_C;
        if (~(reg_A ^ v) & (reg_A ^ temp) & 0x80) reg_P |= FLAG_V;
        reg_A = (uint8_t)temp;
        update_nz(reg_A);
    }

    void cpu_sbc(uint8_t v) {
        cpu_adc(~v);
    }

    void cpu_bit(uint8_t v) {
        reg_P &= ~(FLAG_Z | FLAG_V | FLAG_N);
        if (!(reg_A & v)) reg_P |= FLAG_Z;
        reg_P |= (v & 0xC0);
    }

    void cpu_asl_impl(uint16_t addr, bool is_reg) {
        uint8_t v = is_reg ? reg_A : bus_read(addr);
        if (v & 0x80) reg_P |= FLAG_C; else reg_P &= ~FLAG_C;
        v <<= 1;
        update_nz(v);
        if (is_reg) reg_A = v; else bus_write(addr, v);
    }

    void cpu_lsr_impl(uint16_t addr, bool is_reg) {
        uint8_t v = is_reg ? reg_A : bus_read(addr);
        if (v & 0x01) reg_P |= FLAG_C; else reg_P &= ~FLAG_C;
        v >>= 1;
        update_nz(v);
        if (is_reg) reg_A = v; else bus_write(addr, v);
    }

    void cpu_rol_impl(uint16_t addr, bool is_reg) {
        uint8_t v = is_reg ? reg_A : bus_read(addr);
        uint8_t old_c = (reg_P & FLAG_C);
        if (v & 0x80) reg_P |= FLAG_C; else reg_P &= ~FLAG_C;
        v = (v << 1) | old_c;
        update_nz(v);
        if (is_reg) reg_A = v; else bus_write(addr, v);
    }

    void cpu_ror_impl(uint16_t addr, bool is_reg) {
        uint8_t v = is_reg ? reg_A : bus_read(addr);
        uint8_t old_c = (reg_P & FLAG_C) ? 0x80 : 0x00;
        if (v & 0x01) reg_P |= FLAG_C; else reg_P &= ~FLAG_C;
        v = (v >> 1) | old_c;
        update_nz(v);
        if (is_reg) reg_A = v; else bus_write(addr, v);
    }

    uint16_t cpu_read_pointer(uint16_t addr) {
        return bus_read(addr) | (bus_read(addr + 1) << 8);
    }

    uint16_t cpu_read_jmp_indirect(uint16_t addr) {
        uint16_t low = bus_read(addr);
        uint16_t high = bus_read((addr & 0xFF00) | ((addr + 1) & 0x00FF));
        return low | (high << 8);
    }

    uint16_t read_pointer_indexed_x(uint16_t zp_addr) {
        uint8_t ptr = (uint8_t)(bus_read(zp_addr) + reg_X);
        return bus_read(ptr) | (bus_read((uint8_t)(ptr + 1)) << 8);
    }

    uint16_t read_pointer_indexed_y(uint16_t zp_addr, bool* page_crossed) {
        uint16_t base = bus_read(zp_addr) | (bus_read((uint8_t)(zp_addr + 1)) << 8);
        uint16_t final = base + reg_Y;
        if (page_crossed && (final & 0xFF00) != (base & 0xFF00)) *page_crossed = true;
        return final;
    }

    uint16_t addr_abs_x(uint16_t base, bool* page_crossed) {
        uint16_t final = base + reg_X;
        if (page_crossed && (final & 0xFF00) != (base & 0xFF00)) *page_crossed = true;
        return final;
    }

    uint16_t addr_abs_y(uint16_t base, bool* page_crossed) {
        uint16_t final = base + reg_Y;
        if (page_crossed && (final & 0xFF00) != (base & 0xFF00)) *page_crossed = true;
        return final;
    }
}
