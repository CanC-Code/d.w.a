#include "PPU.h"
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

uint16_t PPU::get_mirrored_addr(uint16_t addr) {
    // Vertical Mirroring for Dragon Warrior I
    return addr & 0x07FF;
}

uint8_t PPU::cpu_read(uint16_t addr, MapperMMC1& mapper) {
    uint8_t reg = addr % 8;
    if (reg == 2) { // PPUSTATUS
        uint8_t res = status;
        status &= ~0x80; // Clear VBlank flag on read
        addr_latch = 0;
        return res;
    }
    if (reg == 7) { // PPUDATA
        uint8_t data = data_buffer;
        uint16_t p_addr = addr_reg & 0x3FFF;
        if (p_addr < 0x2000) data_buffer = mapper.read_chr(p_addr);
        else if (p_addr < 0x3F00) data_buffer = vram[get_mirrored_addr(p_addr)];
        else {
            data = palette_ram[p_addr & 0x1F];
            data_buffer = vram[get_mirrored_addr(p_addr)];
        }
        addr_reg += (ctrl & 0x04) ? 32 : 1;
        return data;
    }
    return 0;
}

void PPU::cpu_write(uint16_t addr, uint8_t val, MapperMMC1& mapper) {
    uint8_t reg = addr % 8;
    if (reg == 0) ctrl = val;
    else if (reg == 1) mask = val;
    else if (reg == 3) oam_addr = val;
    else if (reg == 4) oam_ram[oam_addr++] = val;
    else if (reg == 6) { // PPUADDR
        if (addr_latch == 0) { addr_reg = (addr_reg & 0x00FF) | (val << 8); addr_latch = 1; }
        else { addr_reg = (addr_reg & 0xFF00) | val; addr_latch = 0; }
    }
    else if (reg == 7) { // PPUDATA
        uint16_t p_addr = addr_reg & 0x3FFF;
        if (p_addr < 0x2000) { /* CHR-ROM is usually read-only */ }
        else if (p_addr < 0x3F00) vram[get_mirrored_addr(p_addr)] = val;
        else {
            uint8_t p_idx = p_addr & 0x1F;
            if (p_idx >= 0x10 && (p_idx % 4 == 0)) p_idx -= 0x10;
            palette_ram[p_idx] = val;
        }
        addr_reg += (ctrl & 0x04) ? 32 : 1;
    }
}

void PPU::render_frame(MapperMMC1& mapper, const uint32_t* nes_palette) {
    uint16_t bg_pat_base = (ctrl & 0x10) ? 0x1000 : 0x0000;
    for (int ty = 0; ty < 30; ty++) {
        for (int tx = 0; tx < 32; tx++) {
            uint8_t tile = vram[get_mirrored_addr(0x2000 + ty * 32 + tx)];
            for (int y = 0; y < 8; y++) {
                uint8_t p1 = mapper.read_chr(bg_pat_base + tile * 16 + y);
                uint8_t p2 = mapper.read_chr(bg_pat_base + tile * 16 + y + 8);
                for (int x = 0; x < 8; x++) {
                    uint8_t pix = ((p1 >> (7 - x)) & 0x01) | (((p2 >> (7 - x)) & 0x01) << 1);
                    uint8_t p_idx = palette_ram[pix]; // Simple background palette mapping
                    screen_buffer[(ty * 8 + y) * 256 + (tx * 8 + x)] = nes_palette[p_idx & 0x3F];
                }
            }
        }
    }
}
