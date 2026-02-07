#ifndef CPU_SHARED_H
#define CPU_SHARED_H

#include <stdint.h>

// ============================================================================
// 6502 STATUS FLAGS (P REGISTER)
// ============================================================================
#define FLAG_C 0x01  // Carry
#define FLAG_Z 0x02  // Zero
#define FLAG_I 0x04  // Interrupt Disable
#define FLAG_D 0x08  // Decimal Mode (Present but ignored on NES 2A03)
#define FLAG_B 0x10  // Break
#define FLAG_U 0x20  // Unused (Always 1)
#define FLAG_V 0x40  // Overflow
#define FLAG_N 0x80  // Negative

#ifdef __cplusplus
extern "C" {
#endif

// --- CPU REGISTERS ---
extern uint16_t reg_PC;
extern uint8_t reg_A, reg_X, reg_Y, reg_P, reg_S;

// --- BUS INTERFACE ---
// These are the hooks to your Mapper and PPU implementations
extern uint8_t bus_read(uint16_t addr);
extern void bus_write(uint16_t addr, uint8_t val);

// --- INTERRUPTS ---
void power_on_reset();
void nmi_handler();
void irq_handler();

// ============================================================================
// STACK OPERATIONS
// ============================================================================
// The stack in 6502 is hardcoded to Page 1 ($0100-$01FF)
inline void push_stack(uint8_t val) {
    bus_write(0x0100 | reg_S, val);
    reg_S--;
}

inline uint8_t pop_stack() {
    reg_S++;
    return bus_read(0x0100 | reg_S);
}

// ============================================================================
// ALU HELPERS
// ============================================================================

// Updates Negative and Zero flags (The most frequent flag update)
inline void update_nz(uint8_t val) {
    reg_P &= ~(FLAG_N | FLAG_Z);
    if (val == 0) reg_P |= FLAG_Z;
    if (val & 0x80) reg_P |= FLAG_N;
}

// ADC: Add with Carry
// Formula for Overflow: If (Input1.sign == Input2.sign) AND (Input1.sign != Result.sign)
inline void cpu_adc(uint8_t val) {
    uint16_t temp = (uint16_t)reg_A + (uint16_t)val + (uint16_t)(reg_P & FLAG_C);
    
    reg_P &= ~(FLAG_V | FLAG_C);
    
    // Carry: Result > 255
    if (temp > 0xFF) reg_P |= FLAG_C;
    
    // Overflow: logic checks for signed magnitude flip
    if (~(reg_A ^ val) & (reg_A ^ (uint8_t)temp) & 0x80) reg_P |= FLAG_V;
    
    reg_A = (uint8_t)temp;
    update_nz(reg_A);
}

// SBC: Subtract with Carry
// On 6502, SBC is literally ADC with the input bits inverted.
inline void cpu_sbc(uint8_t val) {
    cpu_adc(~val);
}

// BIT: Bit Test (Unique logic: Z comes from AND, N/V come from bits 7/6 of operand)
inline void cpu_bit(uint8_t val) {
    reg_P &= ~(FLAG_N | FLAG_V | FLAG_Z);
    if ((reg_A & val) == 0) reg_P |= FLAG_Z;
    if (val & 0x80) reg_P |= FLAG_N;
    if (val & 0x40) reg_P |= FLAG_V;
}

// ============================================================================
// ADDRESSING HELPERS
// ============================================================================

/**
 * Implements the 6502 Indirect JMP bug.
 * If a pointer falls on a page boundary ($xxFF), the MSB is fetched from 
 * the start of the current page ($xx00) instead of the next page.
 */
inline uint16_t read_pointer_buggy(uint16_t addr) {
    uint8_t low = bus_read(addr);
    uint16_t high_addr = (addr & 0xFF00) | ((addr + 1) & 0x00FF);
    uint8_t high = bus_read(high_addr);
    return (uint16_t)(high << 8) | low;
}

// Standard Zero-Page Pointer
inline uint16_t read_zp_ptr(uint8_t zp_addr) {
    uint8_t low = bus_read(zp_addr);
    uint8_t high = bus_read((uint8_t)(zp_addr + 1)); // Wrap within ZP
    return (uint16_t)(high << 8) | low;
}

#ifdef __cplusplus
}
#endif

#endif // CPU_SHARED_H
