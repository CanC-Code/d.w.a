#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <vector>
#include <mutex>

class PPU {
public:
    uint8_t vram[2048];
    uint8_t palette_ram[32];
    uint8_t oam_ram[256];
    uint32_t screen_buffer[256 * 240];
    
    // Internal Registers
    uint8_t ctrl;    // $2000
    uint8_t mask;    // $2001
    uint8_t status;  // $2002
    uint8_t oam_addr; // $2003
    
    // Address latching logic ($2006)
    uint16_t addr_reg;
    uint8_t addr_latch;
    uint8_t data_buffer;

    PPU();
    void reset();
    
    // Bus Interface
    uint8_t cpu_read(uint16_t addr);
    void cpu_write(uint16_t addr, uint8_t val);
    
    // Rendering
    void render_frame(class MapperMMC1* mapper);
    
private:
    uint16_t get_mirrored_addr(uint16_t addr);
};

#endif
