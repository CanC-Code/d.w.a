#ifndef CPU_SHARED_H
#define CPU_SHARED_H

#include <stdint.h>

// ============================================================================
// 6502 STATUS FLAGS (P REGISTER)
// ============================================================================
#define FLAG_C 0x01  // Carry
#define FLAG_Z 0x02  // Zero
#define FLAG_I 0x04  // Interrupt Disable
#define FLAG_D 0x08  // Decimal Mode (Not used on NES 2A03)
#define FLAG_B 0x10  // Break
#define FLAG_U 0x20  // Unused (Always 1)
#define FLAG_V 0x40  // Overflow
#define FLAG_N 0x80  // Negative

// ============================================================================
// SHARED INTERFACE
// ============================================================================
#ifdef __cplusplus
extern "C" {
#endif

// CPU Registers
extern uint16_t reg_PC;
extern uint8_t reg_A, reg_X, reg_Y, reg_P, reg_S;

// Global Hardware State (Declared in native-lib.cpp)
extern uint8_t ppu_status;
extern uint8_t ppu_ctrl;
extern uint8_t ppu_addr_latch;

// Memory Bus Interface
extern uint8_t bus_read(uint16_t addr);
extern void bus_write(uint16_t addr, uint8_t val);

// --- INTERRUPT & LIFECYCLE HANDLERS ---
extern void power_on_reset();
extern void nmi_handler();
extern void irq_handler();

// --- STACK HELPERS ---
inline void push_stack(uint8_t val) {
    bus_write(0x0100 | reg_S, val);
    reg_S--;
}

inline uint8_t pop_stack() {
    reg_S++;
    return bus_read(0x0100 | reg_S);
}

// --- ARITHMETIC & FLAG HELPERS ---
// Updates Negative and Zero flags based on val
inline void update_nz(uint8_t val) {
    reg_P &= ~(FLAG_N | FLAG_Z);
    if (val == 0) reg_P |= FLAG_Z;
    if (val & 0x80) reg_P |= FLAG_N;
}

// ADC: Add with Carry (Handles Overflow and Carry flags)
inline void cpu_adc(uint8_t val) {
    uint16_t temp = reg_A + val + (reg_P & FLAG_C);
    reg_P &= ~(FLAG_V | FLAG_C);
    if (temp > 255) reg_P |= FLAG_C;
    if (~(reg_A ^ val) & (reg_A ^ (uint8_t)temp) & 0x80) reg_P |= FLAG_V;
    reg_A = (uint8_t)temp;
    update_nz(reg_A);
}

// SBC: Subtract with Carry
inline void cpu_sbc(uint8_t val) {
    cpu_adc(~val);
}

// BIT: Bit Test (Updates N, V, and Z flags)
inline void cpu_bit(uint8_t val) {
    reg_P &= ~(FLAG_N | FLAG_V | FLAG_Z);
    if ((reg_A & val) == 0) reg_P |= FLAG_Z;
    if (val & 0x80) reg_P |= FLAG_N;
    if (val & 0x40) reg_P |= FLAG_V;
}

// --- COMPARISON HELPERS ---
inline void update_flags_cmp(uint8_t reg, uint8_t val) {
    reg_P &= ~(FLAG_N | FLAG_Z | FLAG_C);
    if (reg >= val) reg_P |= FLAG_C;
    if (reg == val) reg_P |= FLAG_Z;
    if ((uint8_t)(reg - val) & 0x80) reg_P |= FLAG_N;
}

// --- SHIFT/ROTATE HELPERS ---
inline uint8_t cpu_asl(uint8_t val) {
    reg_P &= ~FLAG_C;
    if (val & 0x80) reg_P |= FLAG_C;
    val <<= 1;
    update_nz(val);
    return val;
}

inline uint8_t cpu_lsr(uint8_t val) {
    reg_P &= ~FLAG_C;
    if (val & 0x01) reg_P |= FLAG_C;
    val >>= 1;
    update_nz(val);
    return val;
}

inline uint8_t cpu_rol(uint8_t val) {
    uint8_t old_c = (reg_P & FLAG_C) ? 1 : 0;
    reg_P &= ~FLAG_C;
    if (val & 0x80) reg_P |= FLAG_C;
    val = (val << 1) | old_c;
    update_nz(val);
    return val;
}

inline uint8_t cpu_ror(uint8_t val) {
    uint8_t old_c = (reg_P & FLAG_C) ? 0x80 : 0;
    reg_P &= ~FLAG_C;
    if (val & 0x01) reg_P |= FLAG_C;
    val = (val >> 1) | old_c;
    update_nz(val);
    return val;
}

// --- ADDRESSING HELPERS ---
// Used for JMP ($addr) or recompiled data access
inline uint16_t read_pointer(uint16_t addr) {
    uint8_t low = bus_read(addr);
    uint8_t high = bus_read(addr + 1);
    return (uint16_t)(high << 8) | low;
}

// Indirect Indexed Y: [($zp), Y]
inline uint16_t read_pointer_indexed_y(uint16_t zp_addr) {
    uint16_t base = read_pointer(zp_addr & 0xFF);
    return base + reg_Y;
}

// Indexed Indirect X: [($zp + X)]
inline uint16_t read_pointer_indexed_x(uint16_t zp_addr) {
    return read_pointer((zp_addr + reg_X) & 0xFF);
}

#ifdef __cplusplus
}
#endif

#endif // CPU_SHARED_H
