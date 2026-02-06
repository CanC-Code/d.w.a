#ifndef CPU_SHARED_H
#define CPU_SHARED_H

#include <stdint.h>
#include <android/log.h>

// Log helper for debugging recompiled execution
#define LOG_CPU(...) __android_log_print(ANDROID_LOG_DEBUG, "DWA_RECOMPILED", __VA_ARGS__)

// --- 6502 Status Flag Bitmask ---
// These must match the bit positions in the hardware P register
#define FLAG_C 0x01 // Carry: Set if last operation caused an overflow/underflow
#define FLAG_Z 0x02 // Zero: Set if result of last operation was 0
#define FLAG_I 0x04 // Interrupt Disable: If set, NMIs/IRQs are ignored
#define FLAG_D 0x08 // Decimal Mode: (Unused in NES, but retained for logic)
#define FLAG_B 0x10 // Break: Set when BRK is executed
#define FLAG_U 0x20 // Unused/Always 1: Hardware always reports this bit as high
#define FLAG_V 0x40 // Overflow: Set if signed overflow occurred
#define FLAG_N 0x80 // Negative: Set if bit 7 of result is 1

// --- NES Hardware Constants ---
#define RAM_STACK_BASE     0x0100
#define PPU_CTRL           0x2000
#define PPU_MASK           0x2001
#define PPU_STATUS         0x2002
#define OAM_ADDR           0x2003
#define OAM_DATA           0x2004
#define PPU_SCROLL         0x2005
#define PPU_ADDR           0x2006
#define PPU_DATA           0x2007
#define APU_STATUS         0x4015
#define JOYPAD1            0x4016
#define JOYPAD2            0x4017

// --- Dragon Warrior Internal RAM Aliases ---
// These allow recompiled code to use names instead of raw hex
#define DW_VBLANK_FLAG     0x002D // The "engine heartbeat" address
#define DW_FRAME_CNT       0x0012 
#define DW_SCROLL_X        0x00FC 
#define DW_MAP_ID          0x0045



// Wrap in extern "C" to prevent C++ name mangling, allowing the 
// recompiled .c or .o files to link to your native-lib.cpp functions.
extern "C" {
    // --- CPU Registers (Shared Global State) ---
    extern uint16_t reg_PC;
    extern uint8_t reg_A, reg_X, reg_Y, reg_P, reg_S;

    // --- Memory Bus (The HAL Bridge) ---
    uint8_t bus_read(uint16_t addr);
    void bus_write(uint16_t addr, uint8_t val);

    // --- Stack Management ---
    void push_stack(uint8_t val);
    uint8_t pop_stack();

    // --- Core Flag Logic ---
    void update_nz(uint8_t val);
    void update_flags_cmp(uint8_t reg, uint8_t val);

    // --- Native 6502 Instruction Helpers ---
    // These are implemented in native-lib.cpp and called by recompiled instructions
    void cpu_adc(uint8_t val);
    void cpu_sbc(uint8_t val);
    void cpu_bit(uint8_t val);
    uint8_t cpu_asl(uint8_t val);
    uint8_t cpu_lsr(uint8_t val);
    uint8_t cpu_rol(uint8_t val);
    uint8_t cpu_ror(uint8_t val);

    // --- Addressing Mode Pointer Helpers ---
    // Little-endian address fetching
    uint16_t read_pointer(uint16_t addr);
    uint16_t read_pointer_indexed_x(uint16_t addr);
    uint16_t read_pointer_indexed_y(uint16_t zp_addr);

    // --- Main Dispatch Hook ---
    void execute_instruction();
}

#endif // CPU_SHARED_H
