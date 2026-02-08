#ifndef CPU_SHARED_H
#define CPU_SHARED_H

#include <stdint.h>
#include <stdbool.h>

// ... (Keep flags and memory defines as they were) ...

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
// Corrected signatures to handle both Accumulator and Memory
extern void cpu_asl(uint16_t addr, bool is_reg);
extern void cpu_lsr(uint16_t addr, bool is_reg);
extern void cpu_rol(uint16_t addr, bool is_reg);
extern void cpu_ror(uint16_t addr, bool is_reg);

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
#endif
