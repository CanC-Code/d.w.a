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
    uint8_t vram[2048];        
    uint8_t palette_ram[32];   
    uint8_t oam_ram[256];      
    uint32_t screen_buffer[256 * 240]; 

    // --- PPU REGISTERS ($2000 - $2007) ---
    uint8_t ctrl;      // $2000: Control
    uint8_t mask;      // $2001: Mask
    uint8_t status;    // $2002: Status
    uint8_t oam_addr;  // $2003: OAM Address

    // --- INTERNAL STATE ---
    uint16_t ppu_addr;        // Current VRAM address (v)
    uint8_t ppu_data_buffer;  // Buffered read for $2007
    uint8_t latch;            // Last value written to PPU bus
    uint8_t addr_latch;       // Write toggle (w)
    
    uint8_t scroll_x;         
    uint8_t scroll_y;     

    bool nmi_pending;     

    PPU();

    /**
     * Resets PPU to power-on state.
     */
    void reset();

    /**
     * CPU Interface: Called from bus_read/bus_write.
     */
    uint8_t cpu_read(uint16_t addr, MapperMMC1& mapper);
    void cpu_write(uint16_t addr, uint8_t val, MapperMMC1& mapper);

    /**
     * Frame Generation: Renders background and sprites.
     */
    void render_frame(MapperMMC1& mapper, const uint32_t* nes_palette);

    /**
     * OAM DMA: Fast transfer of 256 bytes to OAM RAM.
     */
    void do_dma(uint8_t* source_data);

    /**
     * Interrupt Logic.
     */
    bool check_nmi();
    void clear_status();

private:
    /**
     * Internal VRAM Access: Handles mapping/mirroring logic for the PPU.
     */
    uint8_t vram_read(uint16_t addr, MapperMMC1& mapper);
    void vram_write(uint16_t addr, uint8_t val, MapperMMC1& mapper);

    /**
     * Address Mirroring: Maps $2000-$2FFF to physical VRAM offsets.
     */
    uint16_t get_mirrored_addr(uint16_t addr, Mirroring mode);
};

#endif // PPU_H
