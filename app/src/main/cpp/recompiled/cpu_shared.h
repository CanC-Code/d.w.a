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

// --- Dragon Warrior Internal RAM Aliases (Critical for Game Logic) ---
#define MapWidth           0x0013 
#define MapHeight          0x0014
#define VBlankFlag         0x002D // CRITICAL: Game loops on this ($2D) until it's 1
#define FrameCounter       0x0012 // Incremented every frame
#define ScrollX            0x00FC 
#define ScrollY            0x00FD
#define PlayerFlags        0x00CF 
#define MusicTrack         0x00F0 // Used to trigger audio in native engine

// --- Directional & Audio Engine Aliases ---
#define DIR_UP             0x00
#define DIR_RIGHT          0x01
#define DIR_DOWN           0x02
#define DIR_LEFT           0x03

// --- Map Data Pointers (Required for Bank 00 / 01 Data lookups) ---
#define DLCstlGFDat        0x80B0
#define HauksnessDat       0x8178
#define TantGFDat          0x8240
#define KolDat             0x85F6
#define BrecconaryDat      0x8716
#define CantlinDat         0x88D8
#define GarinhamDat        0x8A9A
#define RimuldarDat        0x8B62

// --- Global Function Entry Points (Address-to-Function Mapping) ---
#define ModAttribBits      0xC006
#define PalFadeOut         0xC212
#define PalFadeIn          0xC529
#define UpdateRandNum      0xC55B
#define AddPPUBufEntry     0xC690
#define ClearSpriteRAM     0xC6BB
#define WaitForNMI         0xFF74

// Use extern "C" for linkage consistency between C++ files and recompiled C code
extern "C" {
    // --- CPU Registers (Shared Global State) ---
    extern uint16_t reg_PC;
    extern uint8_t reg_A, reg_X, reg_Y, reg_P, reg_S;

    // --- Memory Bus (The HAL Bridge) ---
    extern uint8_t bus_read(uint16_t addr);
    extern void bus_write(uint16_t addr, uint8_t val);

    // --- Stack Management ---
    extern void push_stack(uint8_t val);
    extern uint8_t pop_stack();

    // --- Core Flag Logic ---
    extern void update_nz(uint8_t val);
    extern void update_flags_cmp(uint8_t reg, uint8_t val);

    // --- Native 6502 Instruction Helpers ---
    extern void cpu_adc(uint8_t val);
    extern void cpu_sbc(uint8_t val);
    extern void cpu_bit(uint8_t val);
    extern uint8_t cpu_asl(uint8_t val);
    extern uint8_t cpu_lsr(uint8_t val);
    extern uint8_t cpu_rol(uint8_t val);
    extern uint8_t cpu_ror(uint8_t val);

    // --- Native Addressing Mode Helpers ---
    extern uint16_t read_pointer(uint16_t addr);
    extern uint16_t read_pointer_indexed_x(uint16_t addr);
    extern uint16_t read_pointer_indexed_y(uint16_t zp_addr);

    // --- Execution Hooks ---
    void power_on_reset();
    void nmi_handler();
    void execute_instruction();
}

#endif // CPU_SHARED_H
