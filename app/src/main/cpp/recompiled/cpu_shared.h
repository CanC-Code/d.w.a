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
#define VBlankFlag         0x002D // Set to 1 inside NMI, game waits for this
#define FrameCounter       0x0012 // Incremented every frame
#define ScrollX            0x00FC 
#define ScrollY            0x00FD
#define PlayerFlags        0x00CF 

// --- Directional & Audio Engine Aliases ---
#define DIR_UP             0x00
#define DIR_RIGHT          0x01
#define DIR_DOWN           0x02
#define DIR_LEFT           0x03

// --- Map Data Pointers (Required for Bank 00) ---
#define DLCstlGFDat        0x80B0
#define HauksnessDat       0x8178
#define TantGFDat          0x8240
#define ThrnRoomDat        0x8402
#define DgnLrdBLDat        0x8434
#define KolDat             0x85F6
#define BrecconaryDat      0x8716
#define CantlinDat         0x88D8
#define GarinhamDat        0x8A9A
#define RimuldarDat        0x8B62
#define TantSLDat          0x8D24
#define RainCaveDat        0x8D56
#define DropCaveDat        0x8D88
#define DgnLrdSL1Dat       0x8DBA
#define DgnLrdSL2Dat       0x8E82
#define DgnLrdSL3Dat       0x8EB4
#define DgnLrdSL4Dat       0x8EE6
#define DgnLrdSL5Dat       0x8F18
#define DgnLrdSL6Dat       0x8F4A
#define SwampCaveDat       0x8F7C
#define RckMtnB1Dat        0x8FD6
#define RckMtnB2Dat        0x9038
#define GarinCaveB1Dat     0x909A
#define GarinCaveB2Dat     0x925C
#define GarinCaveB3Dat     0x9162
#define GarinCaveB4Dat     0x922A
#define ErdCaveB1Dat       0x92B0
#define ErdCaveB2Dat       0x92E2

// --- Global Function Entry Points (Address-to-Function Mapping) ---
#define ModAttribBits      0xC006
#define GetNPCSpriteIndex  0xC0F4
#define WordMultiply       0xC1C9
#define ByteDivide         0xC1F0
#define PalFadeOut         0xC212
#define PalFadeIn          0xC529
#define ClearAttribByte    0xC244
#define UpdateRandNum      0xC55B
#define CalcPPUBufAddr     0xC596
#define PrepSPPalLoad      0xC632
#define PrepBGPalLoad      0xC63D
#define AddPPUBufEntry     0xC690
#define ClearSpriteRAM     0xC6BB
#define IdleUpdate         0xCB30
#define CheckForTriggers   0xCBF7
#define ChangeMaps         0xD9E2
#define WaitForNMI         0xFF74

// Use extern "C" for linkage consistency between C++ files and recompiled C
extern "C" {
    // --- CPU Registers (State) ---
    extern uint16_t reg_PC;
    extern uint8_t reg_A, reg_X, reg_Y, reg_P, reg_S;

    // --- Memory Bus (HAL) ---
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
    // These are often used by recompiled code to resolve pointers
    extern uint16_t read_pointer(uint16_t addr);
    extern uint16_t read_pointer_indexed_x(uint16_t addr); // (Indirect, X)
    extern uint16_t read_pointer_indexed_y(uint16_t zp_addr); // (Indirect), Y

    // --- Execution Hooks ---
    void power_on_reset();
    void nmi_handler();
    void execute_instruction();
}

#endif // CPU_SHARED_H
