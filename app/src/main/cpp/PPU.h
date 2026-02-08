#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <cstring>
#include <vector>
#include "MapperMMC1.h"

/**
 * NES PPU (Picture Processing Unit)
 * Handles the generation of the 256x240 video signal.
 * * Performance Note: This header supports a frame-based renderer optimized 
 * for recompiled execution while maintaining the register state Dragon Warrior
 * expects for its text boxes and menu overlays.
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
    uint8_t ctrl;      // $2000: Control (NMI, Sprite Size, etc.)
    uint8_t mask;      // $2001: Mask (Background/Sprite visibility)
    uint8_t status;    // $2002: Status (VBlank, Sprite 0 Hit)
    uint8_t oam_addr;  // $2003: OAM Address

    // --- INTERNAL STATE REGISTERS ---
    // addr_reg (v) and temp_addr (t) are used for complex scrolling and VRAM access
    uint16_t addr_reg;    
    uint16_t temp_addr;   // "t" register: holds scroll and address latches
    uint8_t fine_x;       // fine x scroll (3 bits)
    uint8_t addr_latch;   // write toggle (w)
    
    uint8_t scroll_x;     // Effectively fine_x + coarse
    uint8_t scroll_y;     
    
    uint8_t data_buffer;  // Buffered read for PPUDATA ($2007)
    bool nmi_pending;     

    PPU();
    
    /**
     * Resets PPU to power-on state.
     * Clears buffers and sets status to initial VBlank state.
     */
    void reset();

    /**
     * CPU Interface: These functions should be called from bus_read/bus_write
     * in your native-lib.cpp to handle register access.
     */
    uint8_t cpu_read(uint16_t addr, MapperMMC1& mapper);
    void cpu_write(uint16_t addr, uint8_t val, MapperMMC1& mapper);

    /**
     * Frame Generation: Converts VRAM and CHR data into pixels.
     * Synchronized with MapperMMC1 for bank-switched tile fetching.
     */
    void render_frame(MapperMMC1& mapper, const uint32_t* nes_palette);

    /**
     * OAM DMA: Fast transfer from CPU RAM.
     * Triggered by write to $4014.
     */
    void do_dma(const uint8_t* source_data) {
        std::memcpy(oam_ram, source_data, 256);
    }

    /**
     * NMI Check: Logic to trigger the CPU NMI interrupt.
     */
    bool check_nmi() {
        if (nmi_pending) {
            nmi_pending = false; 
            return true;
        }
        return false;
    }

    /**
     * Utility: Clears the Sprite-Zero and VBlank flags.
     * Usually called at the start of a new frame.
     */
    void clear_status() {
        status &= ~0xE0; // Clear bits 7, 6, 5
    }

private:
    /**
     * Address Mirroring: Resolves logical $2000-$2FFF to physical 2KB VRAM.
     */
    uint16_t get_mirrored_addr(uint16_t addr, Mirroring mode);
};

#endif // PPU_H
