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
    // addr is 0x2000 - 0x3EFF
    uint16_t mirrored = (addr - 0x2000) & 0x0FFF;
    
    // Vertical Mirroring for Dragon Warrior (MMC1):
    // $2000-$23FF (NT0) maps to VRAM 0x000
    // $2400-$27FF (NT1) maps to VRAM 0x400
    // $2800-$2BFF (NT2) maps to VRAM 0x000 (Mirror of NT0)
    // $2C00-$2FFF (NT3) maps to VRAM 0x400 (Mirror of NT1)
    if (mirrored < 0x0800) return mirrored;
    return mirrored - 0x0800;
}

uint8_t PPU::cpu_read(uint16_t addr, MapperMMC1& mapper) {
    uint8_t reg = addr % 8;
    if (reg == 2) { // PPUSTATUS
        uint8_t res = (status & 0xE0); // Return top 3 bits
        status &= ~0x80; // CLEAR VBlank flag on read
        addr_latch = 0;
        return res;
    }
    if (reg == 7) { // PPUDATA
        uint16_t p_addr = addr_reg & 0x3FFF;
        uint8_t data = data_buffer;

        if (p_addr < 0x2000) data_buffer = mapper.read_chr(p_addr);
        else if (p_addr < 0x3F00) data_buffer = vram[get_mirrored_addr(p_addr)];
        else {
            // Palette reads are immediate
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
        if (addr_latch == 0) {
            addr_reg = (uint16_t)((addr_reg & 0x00FF) | ((val & 0x3F) << 8));
            addr_latch = 1;
        } else {
            addr_reg = (addr_reg & 0xFF00) | val;
            addr_latch = 0;
        }
    }
    else if (reg == 7) { // PPUDATA
        uint16_t p_addr = addr_reg & 0x3FFF;
        if (p_addr < 0x2000) {
            // Writing to CHR (Only if Mapper allows RAM)
            mapper.write_chr(p_addr, val);
        }
        else if (p_addr < 0x3F00) {
            vram[get_mirrored_addr(p_addr)] = val;
        }
        else {
            // Palette mirror logic: $3F10, $3F14, $3F18, $3F1C mirror down to $3F00...
            uint16_t p_idx = p_addr & 0x1F;
            if (p_idx >= 0x10 && (p_idx % 4 == 0)) p_idx -= 0x10;
            palette_ram[p_idx] = val;
        }
        addr_reg += (ctrl & 0x04) ? 32 : 1;
    }
}

void PPU::render_frame(MapperMMC1& mapper, const uint32_t* nes_palette) {
    // If background rendering is disabled, show the backdrop color
    if (!(mask & 0x08)) {
        uint32_t bg_color = nes_palette[palette_ram[0] & 0x3F];
        std::fill(screen_buffer, screen_buffer + (256 * 240), bg_color);
        return;
    }

    uint16_t bg_table = (ctrl & 0x10) ? 0x1000 : 0x0000;

    // Background Rendering
    for (int ty = 0; ty < 30; ty++) {
        for (int tx = 0; tx < 32; tx++) {
            uint16_t nt_addr = 0x2000 + (ty * 32) + tx;
            uint8_t tile_index = vram[get_mirrored_addr(nt_addr)];

            uint16_t attr_addr = 0x23C0 + (ty / 4) * 8 + (tx / 4);
            uint8_t attr_byte = vram[get_mirrored_addr(attr_addr)];
            uint8_t shift = ((ty & 2) << 1) | (tx & 2);
            uint8_t palette_id = (attr_byte >> shift) & 0x03;

            // Render the 8x8 tile
            for (int y = 0; y < 8; y++) {
                uint8_t lo = mapper.read_chr(bg_table + (tile_index * 16) + y);
                uint8_t hi = mapper.read_chr(bg_table + (tile_index * 16) + y + 8);

                for (int x = 0; x < 8; x++) {
                    uint8_t bit = 7 - x;
                    uint8_t color = ((lo >> bit) & 0x01) | (((hi >> bit) & 0x01) << 1);
                    
                    uint32_t pixel_color;
                    if (color == 0) {
                        pixel_color = nes_palette[palette_ram[0] & 0x3F];
                    } else {
                        pixel_color = nes_palette[palette_ram[(palette_id << 2) + color] & 0x3F];
                    }
                    screen_buffer[(ty * 8 + y) * 256 + (tx * 8 + x)] = pixel_color;
                }
            }
        }
    }
}
