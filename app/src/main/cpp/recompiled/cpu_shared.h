#ifndef CPU_SHARED_H
#define CPU_SHARED_H

#include <stdint.h>
#include <android/log.h>

// Log helper for debugging recompiled execution
#define LOG_CPU(...) __android_log_print(ANDROID_LOG_DEBUG, "DWA_RECOMPILED", __VA_ARGS__)

// --- NES Hardware Constants & Memory Map ---
// These resolve the "invalid suffix" errors for labels used as addresses
#define SpriteRAM          0x0200
#define PPU_CTRL           0x2000
#define PPU_MASK           0x2001
#define PPU_STATUS         0x2002
#define PPU_SCROLL         0x2005
#define PPU_ADDR           0x2006
#define PPU_DATA           0x2007
#define SND_CHN            0x4015
#define JOYPAD1            0x4016

// --- Dragon Warrior Specific Aliases ---
// Add these based on your Dragon_Warrior_Defines.asm
#define IntroPalPtr1       0x83D6 
#define IntroPalPtr2       0x83E1
#define StarburstPtrTbl    0x8420
#define MusicalNotesTbl    0x9600
#define RegSPPalPtr        0xBC00
#define OverworldPalPtr    0xBC10

// --- Function Entry Points (Targets for JSR/JMP) ---
#define SetBlackBackDrop   0x8353
#define ShowIntroText      0x8313
#define GetNextNote        0x9100
#define ProcessAudioByte   0x9150
#define CalcAttribAddr     0xC000
#define ClrWindowBlock     0xB869

extern "C" {
    // --- CPU Registers ---
    extern uint16_t reg_PC;
    extern uint8_t reg_A, reg_X, reg_Y, reg_P, reg_S;

    // --- Memory Bus ---
    extern uint8_t bus_read(uint16_t addr);
    extern void bus_write(uint16_t addr, uint8_t val);

    // --- Stack Operations ---
    extern void push_stack(uint8_t val);
    extern uint8_t pop_stack();

    // --- Flag Helpers ---
    extern void update_nz(uint8_t val);
    extern void update_flags_cmp(uint8_t reg, uint8_t val);

    // --- Math & Logic Helpers ---
    extern void cpu_adc(uint8_t val);
    extern void cpu_sbc(uint8_t val);
    extern void cpu_bit(uint8_t val);

    // --- Addressing Helpers ---
    extern uint16_t read_pointer(uint16_t addr);
    extern uint16_t read_pointer_x(uint16_t addr);

    // --- Execution Control ---
    void execute_instruction();
    void power_on_reset();
    void nmi_handler();
}

#endif // CPU_SHARED_H
