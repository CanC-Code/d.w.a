#ifndef CPU_SHARED_H
#define CPU_SHARED_H

#include <cstdint>

// CPU Flag bits
#define FLAG_C 0x01  // Carry
#define FLAG_Z 0x02  // Zero
#define FLAG_I 0x04  // IRQ Disable
#define FLAG_D 0x08  // Decimal (unused on NES)
#define FLAG_B 0x10  // Break
#define FLAG_U 0x20  // Unused (always 1)
#define FLAG_V 0x40  // Overflow
#define FLAG_N 0x80  // Negative

// CPU Registers (externally defined)
extern uint8_t reg_A;
extern uint8_t reg_X;
extern uint8_t reg_Y;
extern uint8_t reg_P;
extern uint8_t reg_SP;
extern uint16_t reg_PC;
extern int cycles_to_run;

// Memory access
extern uint8_t bus_read(uint16_t addr);
extern void bus_write(uint16_t addr, uint8_t value);

// CPU helper functions
void update_nz(uint8_t value);
void update_flags_cmp(uint8_t reg, uint8_t value);
void cpu_adc(uint8_t value);
void cpu_sbc(uint8_t value);
void cpu_bit(uint8_t value);
uint8_t cpu_asl(uint8_t value);
uint8_t cpu_lsr(uint8_t value);
uint8_t cpu_rol(uint8_t value);
uint8_t cpu_ror(uint8_t value);
void cpu_push(uint8_t value);
uint8_t cpu_pop();
uint16_t cpu_read_pointer(uint16_t addr);

// Addressing modes
uint16_t addr_abs_x(uint16_t addr, bool* page_crossed);
uint16_t addr_abs_y(uint16_t addr, bool* page_crossed);
uint16_t read_pointer_indexed_x(uint16_t zp_addr);
uint16_t read_pointer_indexed_y(uint16_t zp_addr, bool* page_crossed);

#endif // CPU_SHARED_H