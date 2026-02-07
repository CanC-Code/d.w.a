#ifndef CPU_SHARED_H
#define CPU_SHARED_H

#include <stdint.h>
#include <stdbool.h>

/**
 * 6502 Status Register Flags
 * These bitmasks represent the state of the P register.
 * Essential for the NZ (Negative/Zero) updates and branching logic.
 */
#define FLAG_C 0x01 // Carry: Set if the last operation caused an overflow from bit 7 or underflow from bit 0
#define FLAG_Z 0x02 // Zero: Set if the result of the last operation was 0
#define FLAG_I 0x04 // Interrupt Disable: Set to disable maskable interrupts
#define FLAG_D 0x08 // Decimal Mode: (Not used in NES, but defined for 6502 compatibility)
#define FLAG_B 0x10 // Break: Set when a BRK instruction is executed
#define FLAG_U 0x20 // Unused: Always set to 1 on the NES
#define FLAG_V 0x40 // Overflow: Set if an arithmetic operation resulted in an invalid signed result
#define FLAG_N 0x80 // Negative: Set if bit 7 of the result is 1



#ifdef __cplusplus
extern "C" {
#endif

// --- Global CPU Registers and State ---
// These are defined in native-lib.cpp and shared with the recompiled dispatcher
extern uint16_t reg_PC;
extern uint8_t reg_A, reg_X, reg_Y, reg_S, reg_P;
extern uint8_t cpu_ram[0x0800];
extern bool is_running;
extern bool is_paused;

// --- Bus and Memory Functions ---
// Used by the recompiled code to interact with Mapper and PPU registers
uint8_t bus_read(uint16_t addr);
void bus_write(uint16_t addr, uint8_t val);

// --- Addressing Mode Helpers ---
// Helpers for Indirect and Indexed addressing modes (e.g., LDA ($FF),Y)
uint16_t read_pointer(uint16_t addr);
uint16_t read_pointer_indexed_x(uint16_t zp_addr);
uint16_t read_pointer_indexed_y(uint16_t zp_addr);

// --- Stack Operations ---
void push_stack(uint8_t val);
uint8_t pop_stack();

// --- CPU Core Logic ---
/**
 * Main entry point for the instruction dispatcher.
 * This will jump into the recompiled code block.
 */
void execute_instruction();

// Flag update helpers
void update_nz(uint8_t val);
void update_flags_cmp(uint8_t reg, uint8_t val);

// Arithmetic logic
void cpu_adc(uint8_t val);
void cpu_sbc(uint8_t val);
void cpu_bit(uint8_t val);

// Bit manipulation (Shift/Rotate)
uint8_t cpu_asl(uint8_t val);
uint8_t cpu_lsr(uint8_t val);
uint8_t cpu_rol(uint8_t val);
uint8_t cpu_ror(uint8_t val);

// --- Hardware Signal Helpers ---
/**
 * Triggers a Power-On Reset.
 * Sets the PC to the address found at $FFFC-$FFFD.
 */
void power_on_reset();

/**
 * Triggers a Non-Maskable Interrupt (NMI).
 * Used by the PPU to signal the start of VBlank.
 */
void nmi_handler();

#ifdef __cplusplus
}
#endif

#endif // CPU_SHARED_H
