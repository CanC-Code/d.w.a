#ifndef CPU_SHARED_H
#define CPU_SHARED_H

#include <stdint.h>
#include <stdbool.h>

// --- 6502 Status Register Flags ---
// Required to fix "undeclared identifier" errors in native-lib.cpp
#define FLAG_C 0x01 // Carry
#define FLAG_Z 0x02 // Zero
#define FLAG_I 0x04 // Interrupt Disable
#define FLAG_D 0x08 // Decimal Mode
#define FLAG_B 0x10 // Break
#define FLAG_U 0x20 // Unused
#define FLAG_V 0x40 // Overflow
#define FLAG_N 0x80 // Negative



#ifdef __cplusplus
extern "C" {
#endif

// --- Global CPU Registers and State ---
// 'extern' tells the compiler these are defined in native-lib.cpp
extern uint16_t reg_PC;
extern uint8_t reg_A, reg_X, reg_Y, reg_S, reg_P;
extern uint8_t cpu_ram[0x0800];
extern bool is_running;
extern bool is_paused;

// --- Bus and Memory Functions ---
uint8_t bus_read(uint16_t addr);
void bus_write(uint16_t addr, uint8_t val);

// --- Addressing Mode Helpers ---
uint16_t read_pointer(uint16_t addr);
uint16_t read_pointer_indexed_x(uint16_t zp_addr);
uint16_t read_pointer_indexed_y(uint16_t zp_addr);

// --- Stack Operations ---
void push_stack(uint8_t val);
uint8_t pop_stack();

// --- CPU Core Logic ---
void execute_instruction();
void update_nz(uint8_t val);
void update_flags_cmp(uint8_t reg, uint8_t val);
void cpu_adc(uint8_t val);
void cpu_sbc(uint8_t val);
void cpu_bit(uint8_t val);

// Optimized signatures to match native-lib implementation
uint8_t cpu_asl(uint8_t val);
uint8_t cpu_lsr(uint8_t val);
uint8_t cpu_rol(uint8_t val);
uint8_t cpu_ror(uint8_t val);

// --- PPU / Video Helpers ---
uint16_t ntable_mirror(uint16_t addr);
uint8_t read_palette(uint16_t addr);
void clear_screen_to_black();

#ifdef __cplusplus
}
#endif

#endif // CPU_SHARED_H
