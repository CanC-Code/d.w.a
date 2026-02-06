#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <vector>
#include "MapperMMC1.h"

class PPU {
public:
    // --- Memory Layout ---
    uint8_t vram[2048];                // Internal Name Tables (2KB)
    uint8_t palette_ram[32];           // Internal Palette Memory
    uint8_t oam_ram[256];              // Object Attribute Memory (Sprites)
    uint32_t screen_buffer[256 * 240]; // Final RGBA pixels (Output for JNI)

    // --- Standard PPU Registers ---
    uint8_t ctrl;     // $2000: Control
    uint8_t mask;     // $2001: Mask
    uint8_t status;   // $2002: Status
    uint8_t oam_addr; // $2003: OAM Address

    // --- Internal State (Loopy Registers) ---
    // These track the VRAM address and the temporary address for scrolling
    uint16_t addr_reg;    // Current VRAM address (15 bits)
    uint16_t temp_reg;    // Temporary VRAM address (for $2005/$2006)
    uint8_t fine_x;       // Fine X scroll (3 bits)
    uint8_t addr_latch;   // Toggle for high/low byte writes
    uint8_t data_buffer;  // Delayed read buffer for PPUDATA ($2007)

    PPU();
    void reset();

    // --- CPU Interface ---
    uint8_t cpu_read(uint16_t addr, MapperMMC1& mapper);
    void cpu_write(uint16_t addr, uint8_t val, MapperMMC1& mapper);

    // --- Frame Generation ---
    void render_frame(MapperMMC1& mapper, const uint32_t* nes_palette);

private:
    /**
     * Translates a CPU-side VRAM address ($2000-$3FFF) to internal 2KB VRAM offset.
     * Takes mirror_mode from the Mapper (MMC1 dynamic mirroring).
     */
    uint16_t get_mirrored_addr(uint16_t addr, uint8_t mirror_mode);
};

#endif
