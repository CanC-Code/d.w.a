#include "PPU.h"
#include "MapperMMC1.h"
#include <cstring>

PPU::PPU() { reset(); }

void PPU::reset() {
    std::memset(vram, 0, sizeof(vram));
    std::memset(palette_ram, 0, sizeof(palette_ram));
    std::memset(oam_ram, 0, sizeof(oam_ram));
    std::memset(screen_buffer, 0, sizeof(screen_buffer));
    ctrl = mask = status = oam_addr = 0;
    addr_reg = 0; addr_latch = 0; data_buffer = 0;
}

uint8_t PPU::cpu_read(uint16_t addr) {
    uint8_t reg = addr % 8;
    if (reg == 2) { // PPUSTATUS
        uint8_t res = status;
        status &= ~0x80; // Clear VBlank on read
        addr_latch = 0;  // Reset 2006 latch
        return res;
    }
    if (reg == 7) { // PPUDATA
        uint8_t data = data_buffer;
        // In reality, PPUDATA reads are buffered
        // (code for reading VRAM/CHR omitted for brevity)
        addr_reg += (ctrl & 0x04) ? 32 : 1;
        return data;
    }
    return 0;
}

void PPU::cpu_write(uint16_t addr, uint8_t val) {
    uint8_t reg = addr % 8;
    if (reg == 0) ctrl = val;
    else if (reg == 1) mask = val;
    else if (reg == 6) { // PPUADDR (Dual write)
        if (addr_latch == 0) {
            addr_reg = (addr_reg & 0x00FF) | (val << 8);
            addr_latch = 1;
        } else {
            addr_reg = (addr_reg & 0xFF00) | val;
            addr_latch = 0;
        }
    }
    // ... other register writes ...
}
