#include "cpu_shared.h"

// CPU Registers (defined in native-lib.cpp, declared extern here)
extern uint8_t reg_A;
extern uint8_t reg_X;
extern uint8_t reg_Y;
extern uint8_t reg_P;
extern uint8_t reg_SP;
extern uint16_t reg_PC;
extern int cycles_to_run;

// Memory access (defined in native-lib.cpp)
extern uint8_t bus_read(uint16_t addr);
extern void bus_write(uint16_t addr, uint8_t value);

// Update N and Z flags
void update_nz(uint8_t value) {
    if (value == 0)
        reg_P |= FLAG_Z;
    else
        reg_P &= ~FLAG_Z;
    
    if (value & 0x80)
        reg_P |= FLAG_N;
    else
        reg_P &= ~FLAG_N;
}

// Compare instruction (CMP, CPX, CPY)
void update_flags_cmp(uint8_t reg, uint8_t value) {
    uint16_t result = reg - value;
    
    if (reg >= value)
        reg_P |= FLAG_C;
    else
        reg_P &= ~FLAG_C;
    
    update_nz(result & 0xFF);
}

// ADC (Add with Carry)
void cpu_adc(uint8_t value) {
    uint16_t result = reg_A + value + (reg_P & FLAG_C ? 1 : 0);
    
    // Overflow: (A^result) & (value^result) & 0x80
    if (((reg_A ^ result) & (value ^ result) & 0x80) != 0)
        reg_P |= FLAG_V;
    else
        reg_P &= ~FLAG_V;
    
    if (result > 0xFF)
        reg_P |= FLAG_C;
    else
        reg_P &= ~FLAG_C;
    
    reg_A = result & 0xFF;
    update_nz(reg_A);
}

// SBC (Subtract with Carry)
void cpu_sbc(uint8_t value) {
    value = ~value;  // SBC is ADC with inverted operand
    cpu_adc(value);
}

// BIT test
void cpu_bit(uint8_t value) {
    if ((reg_A & value) == 0)
        reg_P |= FLAG_Z;
    else
        reg_P &= ~FLAG_Z;
    
    if (value & 0x80)
        reg_P |= FLAG_N;
    else
        reg_P &= ~FLAG_N;
    
    if (value & 0x40)
        reg_P |= FLAG_V;
    else
        reg_P &= ~FLAG_V;
}

// ASL (Arithmetic Shift Left)
uint8_t cpu_asl(uint8_t value) {
    if (value & 0x80)
        reg_P |= FLAG_C;
    else
        reg_P &= ~FLAG_C;
    
    value <<= 1;
    update_nz(value);
    return value;
}

// LSR (Logical Shift Right)
uint8_t cpu_lsr(uint8_t value) {
    if (value & 0x01)
        reg_P |= FLAG_C;
    else
        reg_P &= ~FLAG_C;
    
    value >>= 1;
    update_nz(value);
    return value;
}

// ROL (Rotate Left)
uint8_t cpu_rol(uint8_t value) {
    uint8_t old_carry = (reg_P & FLAG_C) ? 1 : 0;
    
    if (value & 0x80)
        reg_P |= FLAG_C;
    else
        reg_P &= ~FLAG_C;
    
    value = (value << 1) | old_carry;
    update_nz(value);
    return value;
}

// ROR (Rotate Right)
uint8_t cpu_ror(uint8_t value) {
    uint8_t old_carry = (reg_P & FLAG_C) ? 0x80 : 0;
    
    if (value & 0x01)
        reg_P |= FLAG_C;
    else
        reg_P &= ~FLAG_C;
    
    value = (value >> 1) | old_carry;
    update_nz(value);
    return value;
}

// Stack operations
void cpu_push(uint8_t value) {
    bus_write(0x100 | reg_SP, value);
    reg_SP--;
}

uint8_t cpu_pop() {
    reg_SP++;
    return bus_read(0x100 | reg_SP);
}

// Read 16-bit pointer
uint16_t cpu_read_pointer(uint16_t addr) {
    uint8_t lo = bus_read(addr);
    uint8_t hi = bus_read(addr + 1);
    return lo | (hi << 8);
}

// Addressing modes with page crossing detection
uint16_t addr_abs_x(uint16_t addr, bool* page_crossed) {
    uint16_t result = addr + reg_X;
    if (page_crossed) {
        *page_crossed = ((addr & 0xFF00) != (result & 0xFF00));
    }
    return result;
}

uint16_t addr_abs_y(uint16_t addr, bool* page_crossed) {
    uint16_t result = addr + reg_Y;
    if (page_crossed) {
        *page_crossed = ((addr & 0xFF00) != (result & 0xFF00));
    }
    return result;
}

// Indexed indirect (X)
uint16_t read_pointer_indexed_x(uint16_t zp_addr) {
    uint8_t addr = (zp_addr + reg_X) & 0xFF;
    uint8_t lo = bus_read(addr);
    uint8_t hi = bus_read((addr + 1) & 0xFF);
    return lo | (hi << 8);
}

// Indirect indexed (Y)
uint16_t read_pointer_indexed_y(uint16_t zp_addr, bool* page_crossed) {
    uint8_t lo = bus_read(zp_addr & 0xFF);
    uint8_t hi = bus_read((zp_addr + 1) & 0xFF);
    uint16_t base = lo | (hi << 8);
    uint16_t result = base + reg_Y;
    if (page_crossed) {
        *page_crossed = ((base & 0xFF00) != (result & 0xFF00));
    }
    return result;
}