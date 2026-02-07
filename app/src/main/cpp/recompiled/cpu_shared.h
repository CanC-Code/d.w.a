#ifndef CPU_SHARED_H
#define CPU_SHARED_H

#include <stdint.h>

// 6502 Status Flags
#define FLAG_C 0x01  // Carry
#define FLAG_Z 0x02  // Zero
#define FLAG_I 0x04  // Interrupt Disable
#define FLAG_D 0x08  // Decimal Mode (not used on NES)
#define FLAG_B 0x10  // Break
#define FLAG_U 0x20  // Unused (always 1)
#define FLAG_V 0x40  // Overflow
#define FLAG_N 0x80  // Negative

// PPU Registers
#define PPU_CTRL   0x2000
#define PPU_MASK   0x2001
#define PPU_STATUS 0x2002
#define OAM_ADDR   0x2003
#define OAM_DATA   0x2004
#define PPU_SCROLL 0x2005
#define PPU_ADDR   0x2006
#define PPU_DATA   0x2007

// APU/IO Registers
#define SND_CHN    0x4015
#define OAM_DMA    0x4014
#define JOYPAD1    0x4016
#define JOYPAD2    0x4017

// Use extern "C" for C++ linkage
#ifdef __cplusplus
extern "C" {
#endif

// CPU Registers
extern uint16_t reg_PC;
extern uint8_t reg_A, reg_X, reg_Y, reg_P, reg_S;

// Memory Bus
extern uint8_t bus_read(uint16_t addr);
extern void bus_write(uint16_t addr, uint8_t val);

// Stack Operations
extern void push_stack(uint8_t val);
extern uint8_t pop_stack();

// Flag Updates
extern void update_nz(uint8_t val);
extern void update_flags_cmp(uint8_t reg, uint8_t val);

// Arithmetic Operations
extern void cpu_adc(uint8_t val);
extern void cpu_sbc(uint8_t val);
extern void cpu_bit(uint8_t val);

// Shift/Rotate Operations
extern uint8_t cpu_asl(uint8_t val);
extern uint8_t cpu_lsr(uint8_t val);
extern uint8_t cpu_rol(uint8_t val);
extern uint8_t cpu_ror(uint8_t val);

// Pointer Reads
extern uint16_t read_pointer(uint16_t addr);
extern uint16_t read_pointer_indexed_y(uint16_t zp_addr);
extern uint16_t read_pointer_indexed_x(uint16_t zp_addr);

// System Control
extern void power_on_reset();
extern void nmi_handler();
extern void execute_instruction();

#ifdef __cplusplus
}
#endif

#endif // CPU_SHARED_H
