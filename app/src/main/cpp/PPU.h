#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <cstring>
#include <vector>
#include "MapperMMC1.h"

/**
 * NES PPU (Picture Processing Unit)
 * Handles the generation of the 256x240 video signal.
 * Precision note: Dragon Warrior 1 uses Nametable scrolling and 
 * Sprite 0 hits for UI, requiring accurate register state.
 */
class PPU {
public:
    // --- MEMORY STORAGE ---
    // Physical VRAM is 2KB, mirrored across the 4KB Nametable space ($2000-$2FFF)
    uint8_t vram[2048];        
    
    // Palette RAM: 32 bytes ($3F00-$3F1F)
    uint8_t palette_ram[32];   
    
    // OAM: 256 bytes for 64 sprites (4 bytes each)
    uint8_t oam_ram[256];      
    
    // Output: 32-bit RGBA pixels for Android Bitmap
    uint32_t screen_buffer[256 * 240]; 

    // --- PPU REGISTERS ($2000 - $2007) ---
    uint8_t ctrl;      // $2000: Control (NMI, Master/Slave, Sprite Size, etc.)
    uint8_t mask;      // $2001: Mask (Greyscale, Background/Sprite visibility)
    uint8_t status;    // $2002: Status (VBlank, Sprite 0 Hit, Overflow)
    uint8_t oam_addr;  // $2003: OAM Address

    // --- INTERNAL STATE REGISTERS ---
    uint16_t addr_reg;    // Current VRAM address (v)
    uint8_t addr_latch;   // Write toggle (w) - affects PPUADDR and PPUSCROLL
    uint8_t scroll_x;     // Fine X scroll
    uint8_t scroll_y;     // Fine Y scroll
    uint8_t data_buffer;  // Buffered read for PPUDATA (Read delay)
    bool nmi_pending;     // Trigger for NMI interrupt handler

    PPU();
    void reset();

    /**
     * CPU Interface: Accessed via bus_read/bus_write in native-lib.cpp.
     */
    uint8_t cpu_read(uint16_t addr, MapperMMC1& mapper);
    void cpu_write(uint16_t addr, uint8_t val, MapperMMC1& mapper);

    /**
     * Frame Generation: Converts VRAM and CHR-ROM data into pixels.
     */
    void render_frame(MapperMMC1& mapper, const uint32_t* nes_palette);

    /**
     * OAM DMA: Fast transfer from CPU RAM.
     * Dragon Warrior uses this to update the Hero and NPC sprites every frame.
     */
    void do_dma(const uint8_t* source_data) {
        std::memcpy(oam_ram, source_data, 256);
    }

    /**
     * NMI Check: Polled by the engine loop to determine if the CPU should 
     * jump to the NMI vector ($FFFA).
     */
    bool check_nmi() {
        if (nmi_pending) {
            nmi_pending = false; 
            return true;
        }
        return false;
    }

private:
    /**
     * Address Mirroring: Resolves 6502-side PPU addresses using MMC1 mode.
     */
    
    uint16_t get_mirrored_addr(uint16_t addr, Mirroring mode);
};

#endif // PPU_H
