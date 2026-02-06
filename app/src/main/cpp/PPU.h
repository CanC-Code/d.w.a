#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <mutex>
#include "MapperMMC1.h"

class PPU {
public:
    uint8_t vram[2048];
    uint8_t palette_ram[32];
    uint8_t oam_ram[256];
    uint32_t screen_buffer[256 * 240];
    
    // Registers
    uint8_t ctrl;    // $2000
    uint8_t mask;    // $2001
    uint8_t status;  // $2002
    uint8_t oam_addr; // $2003
    
    uint16_t addr_reg;
    uint8_t addr_latch;
    uint8_t data_buffer;

    PPU();
    void reset();
    uint8_t cpu_read(uint16_t addr, MapperMMC1& mapper);
    void cpu_write(uint16_t addr, uint8_t val, MapperMMC1& mapper);
    void render_frame(MapperMMC1& mapper, const uint32_t* nes_palette);
    
private:
    uint16_t get_mirrored_addr(uint16_t addr);
};

#endif
