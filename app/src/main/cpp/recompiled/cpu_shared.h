#ifndef CPU_SHARED_H
#define CPU_SHARED_H

#include <stdint.h>

// ============================================================================
// 6502 STATUS FLAGS (P REGISTER)
// ============================================================================
#define FLAG_C 0x01  // Carry
#define FLAG_Z 0x02  // Zero
#define FLAG_I 0x04  // Interrupt Disable
#define FLAG_D 0x08  // Decimal Mode (Unused on NES, but logic stays for parity)
#define FLAG_B 0x10  // Break
#define FLAG_U 0x20  // Unused (Always 1)
#define FLAG_V 0x40  // Overflow
#define FLAG_N 0x80  // Negative

#ifdef __cplusplus
extern "C" {
#endif

// --- CPU REGISTERS (Declared in native-lib.cpp) ---
extern uint16_t reg_PC;
extern uint8_t reg_A, reg_X, reg_Y, reg_P, reg_S;

// --- BUS INTERFACE ---
extern uint8_t bus_read(uint16_t addr);
extern void bus_write(uint16_t addr, uint8_t val);

// --- STACK HELPERS ---
// The NES stack lives in page 0x01 (0x0100 - 0x01FF)
inline void push_stack(uint8_t val) {
    bus_write(0x0100 | reg_S, val);
    reg_S--;
}

inline uint8_t pop_stack() {
    reg_S++;
    return bus_read(0x0100 | reg_S);
}

// --- ALU & FLAG HELPERS ---
inline void update_nz(uint8_t val) {
    reg_P &= ~(FLAG_N | FLAG_Z);
    if (val == 0) reg_P |= FLAG_Z;
    if (val & 0x80) reg_P |= FLAG_N;
}

inline void update_flags_cmp(uint8_t reg, uint8_t val) {
    reg_P &= ~(FLAG_N | FLAG_Z | FLAG_C);
    if (reg >= val) reg_P |= FLAG_C;
    if (reg == val) reg_P |= FLAG_Z;
    if ((uint8_t)(reg - val) & 0x80) reg_P |= FLAG_N;
}

inline void cpu_adc(uint8_t val) {
    // ADC logic includes carry: A + M + C
    uint16_t temp = (uint16_t)reg_A + (uint16_t)val + (uint16_t)(reg_P & FLAG_C);
    
    // Overflow: set if (A^result) & (M^result) has bit 7 set
    reg_P &= ~(FLAG_V | FLAG_C);
    if (temp > 0xFF) reg_P |= FLAG_C;
    if (~(reg_A ^ val) & (reg_A ^ (uint8_t)temp) & 0x80) reg_P |= FLAG_V;
    
    reg_A = (uint8_t)temp;
    update_nz(reg_A);
}

inline void cpu_sbc(uint8_t val) {
    // SBC is ADC with the bits inverted: A - M - (1 - C)
    cpu_adc(~val);
}

inline void cpu_bit(uint8_t val) {
    reg_P &= ~(FLAG_N | FLAG_V | FLAG_Z);
    if ((reg_A & val) == 0) reg_P |= FLAG_Z;
    // BIT copies bits 7 and 6 of memory directly to N and V flags
    if (val & 0x80) reg_P |= FLAG_N;
    if (val & 0x40) reg_P |= FLAG_V;
}

// --- SHIFT/ROTATE HELPERS ---
inline uint8_t cpu_asl(uint8_t val) {
    reg_P &= ~FLAG_C;
    if (val & 0x80) reg_P |= FLAG_C;
    uint8_t result = val << 1;
    update_nz(result);
    return result;
}

inline uint8_t cpu_lsr(uint8_t val) {
    reg_P &= ~FLAG_C;
    if (val & 0x01) reg_P |= FLAG_C;
    uint8_t result = val >> 1;
    update_nz(result);
    return result;
}

inline uint8_t cpu_rol(uint8_t val) {
    uint8_t old_c = (reg_P & FLAG_C) ? 1 : 0;
    reg_P &= ~FLAG_C;
    if (val & 0x80) reg_P |= FLAG_C;
    uint8_t result = (val << 1) | old_c;
    update_nz(result);
    return result;
}

inline uint8_t cpu_ror(uint8_t val) {
    uint8_t old_c = (reg_P & FLAG_C) ? 0x80 : 0;
    reg_P &= ~FLAG_C;
    if (val & 0x01) reg_P |= FLAG_C;
    uint8_t result = (val >> 1) | old_c;
    update_nz(result);
    return result;
}

// --- BRANCHING HELPERS ---
// Relative offset is a signed 8-bit integer
inline void cpu_branch(int8_t offset) {
    reg_PC += offset;
}

// --- ADDRESSING HELPERS ---
// Important: NES Zero Page wraps ($00FF + 1 = $0000)
inline uint16_t read_pointer(uint16_t addr) {
    uint8_t low = bus_read(addr);
    // Explicitly handle page wrapping for Zero Page pointers
    uint16_t next_addr = (addr & 0xFF00) | ((addr + 1) & 0x00FF);
    uint8_t high = bus_read(next_addr);
    return (uint16_t)(high << 8) | low;
}

inline uint16_t read_pointer_indexed_y(uint16_t zp_addr) {
    uint16_t base = read_pointer(zp_addr & 0xFF);
    return base + reg_Y;
}

inline uint16_t read_pointer_indexed_x(uint16_t zp_addr) {
    // (Indirect, X) wraps within zero page before the fetch
    return read_pointer((zp_addr + reg_X) & 0xFF);
}

// --- INTERRUPT HARDWARE ---
inline void cpu_interrupt_nmi() {
    push_stack((uint8_t)((reg_PC >> 8) & 0xFF));
    push_stack((uint8_t)(reg_PC & 0xFF));
    push_stack(reg_P | FLAG_U); // Hardware interrupts push with U set, B clear
    reg_P |= FLAG_I; // Disable further IRQs
    reg_PC = (uint16_t)bus_read(0xFFFA) | ((uint16_t)bus_read(0xFFFB) << 8);
}

#ifdef __cplusplus
}
#endif

#endif // CPU_SHARED_H
