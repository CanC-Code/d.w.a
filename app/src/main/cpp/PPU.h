#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <vector>
#include "MapperMMC1.h"

/**
 * NES PPU (Picture Processing Unit)
 * Handles Nametables, Palettes, OAM (Sprites), and Rendering.
 */
class PPU {
public:
    // --- Memory Layout ---
    uint8_t vram[2048];                // Internal Name Tables (2KB)
    uint8_t palette_ram[32];           // Internal Palette Memory
    uint8_t oam_ram[256];              // Object Attribute Memory (Sprites)
    uint32_t screen_buffer[256 * 240]; // Final RGBA pixels (Output for JNI)

    // --- Standard PPU Registers ($2000-$2007) ---
    uint8_t ctrl;     // $2000: Control (NMI, Sprite Size, PT base)
    uint8_t mask;     // $2001: Mask (Color effects, show/hide layers)
    uint8_t status;   // $2002: Status (VBlank, Sprite 0 Hit)
    uint8_t oam_addr; // $2003: OAM Address

    // --- Internal State (Scrolling & VRAM) ---
    uint16_t addr_reg;    // Current VRAM address (Looped via $2006)
    uint8_t addr_latch;   // Toggle for high/low byte writes
    uint8_t data_buffer;  // Delayed read buffer for PPUDATA

    // Scroll tracking (Essential for Dragon Warrior map movement)
    uint8_t scroll_x;     
    uint8_t scroll_y;

    // Interrupt handling
    // Updated: nmi_pending is used by the CPU loop to trigger the NMI routine
    bool nmi_pending;

    PPU();
    
    /**
     * Resets PPU state to power-on defaults.
     */
    void reset();

    // --- CPU Interface ---
    /**
     * Handles CPU reads from PPU registers $2000-$2007.
     * Note: PPUSTATUS ($2002) and PPUDATA ($2007) have side effects.
     */
    uint8_t cpu_read(uint16_t addr, MapperMMC1& mapper);

    /**
     * Handles CPU writes to PPU registers $2000-$2007.
     */
    void cpu_write(uint16_t addr, uint8_t val, MapperMMC1& mapper);

    // --- Frame Generation ---
    /**
     * Renders a full 256x240 frame into the screen_buffer.
     * Uses the Mapper to fetch CHR data for background and sprites.
     */
    void render_frame(MapperMMC1& mapper, const uint32_t* nes_palette);

    /**
     * Helper to signal VBlank and prepare for NMI.
     * In Dragon Warrior, this is usually called at the end of the frame
     * to trigger the music and input polling routines.
     */
    void trigger_vblank() { 
        status |= 0x80; 
        if (ctrl & 0x80) nmi_pending = true; 
    }

private:
    /**
     * Handles MMC1-controlled Mirroring (Horizontal, Vertical, One-Screen).
     * Maps addresses $2000-$3EFF to the 2KB internal VRAM.
     */
    uint16_t get_mirrored_addr(uint16_t addr, uint8_t mirror_mode);
};

#endif // PPU_H
