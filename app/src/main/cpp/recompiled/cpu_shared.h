#ifndef CPU_SHARED_H
#define CPU_SHARED_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// --- Registers and State ---
extern uint16_t reg_PC;
extern uint8_t reg_A, reg_X, reg_Y, reg_S, reg_P;
extern uint8_t cpu_ram[0x0800];
extern bool is_running;

// --- Bus and Memory Functions (Declarations Only) ---
uint8_t bus_read(uint16_t addr);
void bus_write(uint16_t addr, uint8_t val);
uint16_t read_pointer(uint16_t addr);
uint16_t read_pointer_indexed_x(uint16_t zp_addr);
uint16_t read_pointer_indexed_y(uint16_t zp_addr);

// --- Stack Operations (Declarations Only) ---
void push_stack(uint8_t val);
uint8_t pop_stack();

// --- CPU Core Logic (Declarations Only) ---
void execute_instruction();
void update_nz(uint8_t val);
void update_flags_cmp(uint8_t reg, uint8_t val);
void cpu_adc(uint8_t val);
void cpu_sbc(uint8_t val);
void cpu_bit(uint8_t val);
void cpu_asl(uint16_t addr, bool is_reg);
void cpu_lsr(uint16_t addr, bool is_reg);
void cpu_rol(uint16_t addr, bool is_reg);
void cpu_ror(uint16_t addr, bool is_reg);

// --- PPU / Video Helpers (Declarations Only) ---
uint16_t ntable_mirror(uint16_t addr);
uint8_t read_palette(uint16_t addr);
void clear_screen_to_black();

#ifdef __cplusplus
}
#endif

#endif // CPU_SHARED_H
