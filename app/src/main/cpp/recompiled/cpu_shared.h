#ifndef CPU_SHARED_H
#define CPU_SHARED_H

#include <stdint.h>
#include <android/log.h>

// Log helper for debugging recompiled execution
#define LOG_CPU(...) __android_log_print(ANDROID_LOG_DEBUG, "DWA_RECOMPILED", __VA_ARGS__)

// --- NES Hardware Constants & Memory Map ---
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

// --- Dragon Warrior Specific Aliases & RAM ---
#define MapWidth           0x0013 
#define MapHeight          0x0014
#define CoverDatPtr        0x0017
#define PlayerFlags        0x00CF 

// --- Function Entry Points (Targets for JSR/JMP) ---
[span_2](start_span)// These addresses are verified from the BankPointers table and Forward Declarations[span_2](end_span)
[span_3](start_span)#define LoadStartPals      0xAA7E //[span_3](end_span)
[span_4](start_span)#define LoadEndBossGFX     0xBABD //[span_4](end_span)
[span_5](start_span)#define DoSprites          0xB6DA //[span_5](end_span)
[span_6](start_span)#define RemoveWindow       0xA7A2 //[span_6](end_span)
[span_7](start_span)#define DoPalFadeIn        0xAA3D //[span_7](end_span)
[span_8](start_span)#define DoPalFadeOut       0xAA43 //[span_8](end_span)

[span_9](start_span)// Verified Aliases[span_9](end_span)
#define ModAttribBits      0xC006
#define GetNPCSpriteIndex  0xC0F4
#define UpdateRandNum      0xC55B
#define CalcPPUBufAddr     0xC596
#define ClearSpriteRAM     0xC6BB

// Additional Common Routines
#define GoldToBCD          0xA8BA 
#define GetJoypadStatus    0xC608 

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
