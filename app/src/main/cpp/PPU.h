#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <cstring>
#include <vector>
#include "MapperMMC1.h"

/**
 * NES PPU (Picture Processing Unit)
 * Handles the generation of the 256x240 video signal.
 */
class PPU {
public:
    // --- MEMORY STORAGE ---
    uint8_t vram[2048];        // 2KB Internal Nametable RAM
    uint8_t palette_ram[32];   // Internal Palette RAM
    uint8_t oam_ram[256];      // Object Attribute Memory (Sprites)
    uint32_t screen_buffer[256 * 240]; // Final RGBA output buffer

    // --- PPU REGISTERS ($2000 - $2007) ---
    uint8_t ctrl;      // $2000: PPU Control
    uint8_t mask;      // $2001: PPU Mask (Rendering/Tint)
    uint8_t status;    // $2002: PPU Status
    uint8_t oam_addr;  // $2003: OAM Address

    // --- INTERNAL STATE REGISTERS ---
    uint16_t addr_reg;    // Internal VRAM address pointer (v)
    uint8_t addr_latch;   // Address/Scroll latch toggle (w)
    uint8_t scroll_x;     // X-Scroll offset
    uint8_t scroll_y;     // Y-Scroll offset
    uint8_t data_buffer;  // Delayed read buffer for PPUDATA
    bool nmi_pending;     // Becomes true when V-Blank starts

    PPU();
    void reset();

    /**
     * CPU Interface: Handles memory-mapped I/O from the 6502.
     */
    uint8_t cpu_read(uint16_t addr, MapperMMC1& mapper);
    void cpu_write(uint16_t addr, uint8_t val, MapperMMC1& mapper);

    /**
     * Frame Generation: Draws background tiles and sprites into screen_buffer.
     */
    void render_frame(MapperMMC1& mapper, const uint32_t* nes_palette);

    /**
     * OAM DMA: Fast transfer of sprite data from CPU RAM.
     * Dragon Warrior uses this at the start of every frame.
     */
    void do_dma(const uint8_t* cpu_ram_page) {
        std::memcpy(oam_ram, cpu_ram_page, 256);
    }

    /**
     * NMI Check: Used by the main engine loop to trigger the CPU interrupt.
     */
    bool check_nmi() {
        bool result = nmi_pending;
        nmi_pending = false; // Acknowledge NMI
        return result;
    }

private:
    /**
     * Address Mirroring: Resolves logical PPU addresses to physical VRAM indices.
     */
    uint16_t get_mirrored_addr(uint16_t addr, Mirroring mode);
};

#endif // PPU_H
