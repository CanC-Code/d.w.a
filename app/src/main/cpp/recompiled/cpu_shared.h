#ifndef CPU_SHARED_H
#define CPU_SHARED_H

#include <stdint.h>
#include <stdbool.h>

/**
 * 6502 Status Flags
 */
#define FLAG_C 0x01  // Carry
#define FLAG_Z 0x02  // Zero
#define FLAG_I 0x04  // Interrupt Disable
#define FLAG_D 0x08  // Decimal (Disabled on NES hardware)
#define FLAG_B 0x10  // Break
#define FLAG_U 0x20  // Unused
#define FLAG_V 0x40  // Overflow
#define FLAG_N 0x80  // Negative

/**
 * NES Memory Mapped I/O
 */
#define PPU_CTRL   0x2000
#define PPU_MASK   0x2001
#define PPU_STATUS 0x2002
#define OAM_ADDR   0x2003
#define OAM_DATA   0x2004
#define PPU_SCROLL 0x2005
#define PPU_ADDR   0x2006
#define PPU_DATA   0x2007
#define OAM_DMA    0x4014
#define SND_CHN    0x4015
#define JOYPAD1    0x4016
#define JOYPAD2    0x4017

#ifdef __cplusplus
extern "C" {
#endif

// --- CPU ARCHITECTURE STATE ---
extern uint16_t reg_PC;
extern uint8_t reg_A, reg_X, reg_Y, reg_P, reg_S;

// --- EMULATION STATE & INTERRUPTS ---
extern bool is_running;      
extern bool nmi_pending;     
extern bool irq_pending;     
extern uint8_t joypad_state; 

// --- TIMING ---
extern int64_t total_cycles;  
extern int32_t cycles_to_run; 

static inline void add_cycles(int n) {
    total_cycles += n;
    cycles_to_run -= n;
}

// --- MEMORY BUS ---
extern uint8_t bus_read(uint16_t addr);
extern void bus_write(uint16_t addr, uint8_t val);
extern void cpu_push(uint8_t val);
extern uint8_t cpu_pop();

// --- FLAG MANAGEMENT ---
extern void update_nz(uint8_t val);
extern void update_flags_cmp(uint8_t reg, uint8_t val);

// --- RECOMPILED OPS ---
extern void cpu_adc(uint8_t val);
extern void cpu_sbc(uint8_t val);
extern void cpu_bit(uint8_t val);

// Core implementations for memory-based shifts
extern void cpu_asl_impl(uint16_t addr, bool is_reg);
extern void cpu_lsr_impl(uint16_t addr, bool is_reg);
extern void cpu_rol_impl(uint16_t addr, bool is_reg);
extern void cpu_ror_impl(uint16_t addr, bool is_reg);

// --- COMPATIBILITY WRAPPERS (Fixes BankXX.cpp errors) ---
// These allow 'reg_A = cpu_asl(reg_A);' to still work.
static inline uint8_t cpu_asl(uint8_t val) { cpu_asl_impl(0, true); return reg_A; }
static inline uint8_t cpu_lsr(uint8_t val) { cpu_lsr_impl(0, true); return reg_A; }
static inline uint8_t cpu_rol(uint8_t val) { cpu_rol_impl(0, true); return reg_A; }
static inline uint8_t cpu_ror(uint8_t val) { cpu_ror_impl(0, true); return reg_A; }

// --- ADDRESSING HELPERS ---
extern uint16_t addr_abs_x(uint16_t base, bool* page_crossed);
extern uint16_t addr_abs_y(uint16_t base, bool* page_crossed);
extern uint16_t cpu_read_pointer(uint16_t addr);
extern uint16_t cpu_read_jmp_indirect(uint16_t addr); 
extern uint16_t read_pointer_indexed_y(uint16_t zp_addr, bool* page_crossed);
extern uint16_t read_pointer_indexed_x(uint16_t zp_addr);

// --- SYSTEM CONTROL ---
extern void nmi_handler();
extern void irq_handler();
extern void execute_instruction();

#ifdef __cplusplus
}
#endif

#endif // CPU_SHARED_H
