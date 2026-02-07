#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <vector>
#include "MapperMMC1.h"

/**
 * NES PPU (Picture Processing Unit)
 */
class PPU {
public:
    uint8_t vram[2048];
    uint8_t palette_ram[32];
    uint8_t oam_ram[256];
    uint32_t screen_buffer[256 * 240];

    // Registers
    uint8_t ctrl;
    uint8_t mask;
    uint8_t status;
    uint8_t oam_addr;

    // Fixed: Added missing members required by PPU.cpp
    uint16_t addr_reg;   // Internal VRAM address
    uint8_t addr_latch;  // Write toggle (w)
    uint8_t scroll_x;    // Horizontal scroll
    uint8_t scroll_y;    // Vertical scroll
    uint8_t data_buffer; // Read buffer for $2007
    bool nmi_pending;

    PPU();
    void reset();

    uint8_t cpu_read(uint16_t addr, MapperMMC1& mapper);
    void cpu_write(uint16_t addr, uint8_t val, MapperMMC1& mapper);
    void render_frame(MapperMMC1& mapper, const uint32_t* nes_palette);
    
    void do_dma(const uint8_t* cpu_ram_page) {
        std::memcpy(oam_ram, cpu_ram_page, 256);
    }

private:
    uint16_t get_mirrored_addr(uint16_t addr, uint8_t mirror_mode);
};

#endif
