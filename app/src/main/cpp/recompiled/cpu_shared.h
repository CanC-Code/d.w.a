#ifndef CPU_SHARED_H
#define CPU_SHARED_H

#include <stdint.h>

// 6502 STATUS FLAGS
#define FLAG_C 0x01  // Carry
#define FLAG_Z 0x02  // Zero
#define FLAG_I 0x04  // Interrupt Disable
#define FLAG_D 0x08  // Decimal (Unused on NES)
#define FLAG_B 0x10  // Break
#define FLAG_U 0x20  // Unused
#define FLAG_V 0x40  // Overflow
#define FLAG_N 0x80  // Negative

// These must be visible to both C and C++ components
#ifdef __cplusplus
extern "C" {
#endif

// Shared Registers
extern uint16_t reg_PC;
extern uint8_t reg_A, reg_X, reg_Y, reg_P, reg_S;

// Bus Interface (Defined in native-lib.cpp)
extern uint8_t bus_read(uint16_t addr);
extern void bus_write(uint16_t addr, uint8_t val);

#ifdef __cplusplus
}
#endif

// --- HELPER FUNCTIONS ---
// We use 'static inline' to allow these to be included in multiple .cpp 
// files (every recompiled bank) without causing duplicate symbol errors.

static inline void push_stack(uint8_t val) {
    bus_write(0x0100 | reg_S, val);
    reg_S--;
}

static inline uint8_t pop_stack() {
    reg_S++;
    return bus_read(0x0100 | reg_S);
}

static inline void update_nz(uint8_t val) {
    reg_P &= ~(FLAG_N | FLAG_Z);
    if (val == 0) reg_P |= FLAG_Z;
    if (val & 0x80) reg_P |= FLAG_N;
}

static inline void update_flags_cmp(uint8_t reg, uint8_t val) {
    reg_P &= ~(FLAG_N | FLAG_Z | FLAG_C);
    if (reg >= val) reg_P |= FLAG_C;
    if (reg == val) reg_P |= FLAG_Z;
    if ((uint8_t)(reg - val) & 0x80) reg_P |= FLAG_N;
}

static inline void cpu_adc(uint8_t val) {
    uint16_t carry = (reg_P & FLAG_C) ? 1 : 0;
    uint16_t temp = (uint16_t)reg_A + (uint16_t)val + carry;
    reg_P &= ~(FLAG_V | FLAG_C);
    if (temp > 0xFF) reg_P |= FLAG_C;
    if (~(reg_A ^ val) & (reg_A ^ (uint8_t)temp) & 0x80) reg_P |= FLAG_V;
    reg_A = (uint8_t)temp;
    update_nz(reg_A);
}

static inline void cpu_sbc(uint8_t val) {
    cpu_adc(~val); // 6502 SBC is ADC with inverted bits
}

static inline void cpu_bit(uint8_t val) {
    reg_P &= ~(FLAG_N | FLAG_V | FLAG_Z);
    if ((reg_A & val) == 0) reg_P |= FLAG_Z;
    if (val & 0x80) reg_P |= FLAG_N;
    if (val & 0x40) reg_P |= FLAG_V;
}

static inline uint8_t cpu_asl(uint8_t val) {
    reg_P &= ~FLAG_C;
    if (val & 0x80) reg_P |= FLAG_C;
    uint8_t result = val << 1;
    update_nz(result);
    return result;
}

static inline uint8_t cpu_lsr(uint8_t val) {
    reg_P &= ~FLAG_C;
    if (val & 0x01) reg_P |= FLAG_C;
    uint8_t result = val >> 1;
    update_nz(result);
    return result;
}

static inline uint8_t cpu_rol(uint8_t val) {
    uint8_t old_c = (reg_P & FLAG_C) ? 1 : 0;
    reg_P &= ~FLAG_C;
    if (val & 0x80) reg_P |= FLAG_C;
    uint8_t result = (val << 1) | old_c;
    update_nz(result);
    return result;
}

static inline uint8_t cpu_ror(uint8_t val) {
    uint8_t old_c = (reg_P & FLAG_C) ? 0x80 : 0;
    reg_P &= ~FLAG_C;
    if (val & 0x01) reg_P |= FLAG_C;
    uint8_t result = (val >> 1) | old_c;
    update_nz(result);
    return result;
}

// ADDRESSING HELPERS


static inline uint16_t read_pointer(uint16_t addr) {
    uint8_t low = bus_read(addr);
    // Page wrap bug: JMP ($xxFF) fetches high byte from $xx00, not $xx+1
    uint16_t next_addr = (addr & 0xFF00) | ((addr + 1) & 0x00FF);
    uint8_t high = bus_read(next_addr);
    return (uint16_t)(high << 8) | low;
}

static inline uint16_t read_pointer_indexed_y(uint16_t zp_addr) {
    // This is the (Indirect), Y mode used for almost all DW1 data access
    uint16_t base = read_pointer(zp_addr & 0xFF);
    return base + reg_Y;
}

static inline uint16_t read_pointer_indexed_x(uint16_t zp_addr) {
    return read_pointer((zp_addr + reg_X) & 0xFF);
}

#endif // CPU_SHARED_H
