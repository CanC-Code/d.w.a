#ifndef CPU_SHARED_H
#define CPU_SHARED_H

#include <stdint.h>
#include <android/log.h>

// Log helper for debugging recompiled execution
#define LOG_CPU(...) __android_log_print(ANDROID_LOG_DEBUG, "DWA_RECOMPILED", __VA_ARGS__)

// --- 6502 Status Flag Bitmask ---
#define FLAG_C 0x01 // Carry
#define FLAG_Z 0x02 // Zero
#define FLAG_I 0x04 // Interrupt Disable
#define FLAG_D 0x08 // Decimal Mode
#define FLAG_B 0x10 // Break
#define FLAG_U 0x20 // Unused/Always 1
#define FLAG_V 0x40 // Overflow
#define FLAG_N 0x80 // Negative

// --- NES Hardware Constants ---
#define SpriteRAM          0x0200
#define PPU_CTRL           0x2000
#define PPU_MASK           0x2001
#define PPU_STATUS         0x2002
#define OAM_ADDR           0x2003
#define OAM_DATA           0x2004
#define PPU_SCROLL         0x2005
#define PPU_ADDR           0x2006
#define PPU_DATA           0x2007
#define SND_CHN            0x4015
#define JOYPAD1            0x4016
#define JOYPAD2            0x4017

// --- Dragon Warrior Internal RAM Aliases ---
#define MapWidth           0x0013 
#define MapHeight          0x0014
#define VBlankFlag         0x002D // CRITICAL: Game loops on this ($2D) until NMI sets it to 1
#define FrameCounter       0x0012 
#define ScrollX            0x00FC 
#define ScrollY            0x00FD

// Use extern "C" for linkage consistency between C++ and recompiled bank files
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

    // --- Native 6502 Instruction Helpers (CRITICAL: Fixes Linker Errors) ---
    void cpu_adc(uint8_t val);
    void cpu_sbc(uint8_t val);
    void cpu_bit(uint8_t val);
    uint8_t cpu_asl(uint8_t val);
    uint8_t cpu_lsr(uint8_t val);
    uint8_t cpu_rol(uint8_t val);
    uint8_t cpu_ror(uint8_t val);

    // --- Native Addressing Mode Helpers ---
    uint16_t read_pointer(uint16_t addr);
    uint16_t read_pointer_indexed_x(uint16_t addr);
    uint16_t read_pointer_indexed_y(uint16_t zp_addr);

    // --- Execution Hooks ---
    void execute_instruction();
}

#endif // CPU_SHARED_H
