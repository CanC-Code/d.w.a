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

// --- Dragon Warrior Internal RAM Aliases ---
#define MapWidth           0x0013 
#define MapHeight          0x0014
#define PlayerFlags        0x00CF 

// --- Missing Directional & Audio Aliases (from Bank01/Bank03 errors) ---
#define DIR_UP             0x00
#define DIR_RIGHT          0x01
#define DIR_DOWN           0x02
#define DIR_LEFT           0x03

#define MCTL_SQ1_SW        0x00
#define MCTL_SQ1_HW        0x01
#define MCTL_SQ2_SW        0x02
#define MCTL_SQ2_HW        0x03
#define MCTL_TRI_SW        0x04
#define MCTL_TRI_HW        0x05
#define MCTL_NOIS_SW       0x06
#define MCTL_DMC_HW        0x07

// --- Function Entry Points (Targets for JSR/JMP) ---
// These resolve the "undeclared identifier" errors in Bank00-Bank03
#define BankPointers       0x8000
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

// Missing labels discovered during compilation
#define C5E0               0xC5E0
#define C009               0xC009
#define CalcAttribAddr     0xAF70
#define SetAttribBits      0xAFB0
#define AttribLoop         0xAF8C
#define NPCNewDir          0xB1A0
#define ChkPlayerRight     0xB1C4
#define UpdateSound        0x8040
#define GetNextNote        0x80C0
#define SoundUpdateEnd     0x8110
#define MusicReturn        0x8140
#define ProcessAudioByte   0x8150

// Data Pointers (Bank00)
#define MapDatTbl          0x8042
#define BrecCvrdDatPtr     0x8026
#define GarinCvrdDatPtr    0x802A
#define CantCvrdDatPtr     0x802E
#define RimCvrdDatPtr      0x8032
#define DLCstlGFDat        0x8284
#define HauksnessDat       0x8326
#define TantGFDat          0x8368
#define ThrnRoomDat        0x8430
#define DgnLrdBLDat        0x8452
#define KolDat             0x8514
#define BrecconaryDat      0x8564
#define CantlinDat         0x8626
#define GarinhamDat        0x8688
#define RimuldarDat        0x8730
#define TantSLDat          0x8792
#define RainCaveDat        0x8814
#define DropCaveDat        0x8836

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
    
    // FIX: cpu_asl must return uint8_t to support "reg_A = cpu_asl(reg_A);"
    extern uint8_t cpu_asl(uint8_t val);
    extern uint8_t cpu_lsr(uint8_t val);
    extern uint8_t cpu_rol(uint8_t val);
    extern uint8_t cpu_ror(uint8_t val);

    // --- Addressing Helpers ---
    extern uint16_t read_pointer(uint16_t addr);
    extern uint16_t read_pointer_x(uint16_t addr);
    
    // FIX: Added for "LDA ($ZP),Y" support in recompiled files
    extern uint16_t read_pointer_indexed_y(uint16_t zp_addr);

    // --- Execution Control ---
    void execute_instruction();
    void power_on_reset();
    void nmi_handler();
}

#endif // CPU_SHARED_H
