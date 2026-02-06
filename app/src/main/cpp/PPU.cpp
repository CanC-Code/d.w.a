#include "PPU.h"
#include <cstring>
#include <algorithm>

PPU::PPU() { reset(); }

void PPU::reset() {
    std::memset(vram, 0, sizeof(vram));
    std::memset(palette_ram, 0, sizeof(palette_ram));
    std::memset(oam_ram, 0, sizeof(oam_ram));
    // Default to NES Black
    std::fill(screen_buffer, screen_buffer + (256 * 240), 0xFF000000); 
    ctrl = mask = status = oam_addr = 0;
    addr_reg = 0; addr_latch = 0; data_buffer = 0;
}

/**
 * MMC1 can switch mirroring modes. Dragon Warrior typically uses 
 * Horizontal or Vertical mirroring depending on the scene.
 */
uint16_t PPU::get_mirrored_addr(uint16_t addr, uint8_t mirror_mode) {
    uint16_t mirrored = (addr - 0x2000) & 0x0FFF;
    
    // mirror_mode: 0 = OneScreen Lower, 1 = OneScreen Upper, 2 = Vertical, 3 = Horizontal
    switch (mirror_mode) {
        case 0: return mirrored & 0x03FF;          // OneScreen Lower
        case 1: return (mirrored & 0x03FF) + 0x0400; // OneScreen Upper
        case 2: // Vertical Mirroring
            return mirrored & 0x07FF; 
        case 3: // Horizontal Mirroring
        default:
            if (mirrored < 0x0800) return mirrored & 0x03FF;
            return (mirrored & 0x03FF) + 0x0400;
    }
}

uint8_t PPU::cpu_read(uint16_t addr, MapperMMC1& mapper) {
    uint8_t reg = addr % 8;
    if (reg == 2) { // PPUSTATUS
        uint8_t res = (status & 0xE0) | (data_buffer & 0x1F);
        status &= ~0x80; // Clear VBlank flag
        addr_latch = 0; 
        return res;
    }
    if (reg == 4) return oam_ram[oam_addr]; // OAMDATA
    if (reg == 7) { // PPUDATA
        uint16_t p_addr = addr_reg & 0x3FFF;
        uint8_t data = data_buffer;

        if (p_addr < 0x2000) {
            data_buffer = mapper.read_chr(p_addr);
        } else if (p_addr < 0x3F00) {
            data_buffer = vram[get_mirrored_addr(p_addr, mapper.mirroring)];
        } else {
            // Palette reads are not buffered
            data = palette_ram[p_addr & 0x1F];
            data_buffer = vram[get_mirrored_addr(p_addr, mapper.mirroring)];
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
            mapper.write_chr(p_addr, val);
        } else if (p_addr < 0x3F00) {
            vram[get_mirrored_addr(p_addr, mapper.mirroring)] = val;
        } else {
            uint16_t p_idx = p_addr & 0x1F;
            // Palette Mirroring: $3F10, $3F14, $3F18, $3F1C mirror down to $3F00...
            if (p_idx >= 0x10 && (p_idx % 4 == 0)) p_idx -= 0x10;
            palette_ram[p_idx] = val;
        }
        addr_reg += (ctrl & 0x04) ? 32 : 1;
    }
}

void PPU::render_frame(MapperMMC1& mapper, const uint32_t* nes_palette) {
    // Fill with backdrop color (Palette index 0)
    uint32_t backdrop = nes_palette[palette_ram[0] & 0x3F];
    std::fill(screen_buffer, screen_buffer + (256 * 240), backdrop);

    // If BG and Sprites are hidden, return early (Grey Screen check)
    if (!(mask & 0x18)) return;

    // 1. Background Rendering
    if (mask & 0x08) {
        uint16_t bg_table = (ctrl & 0x10) ? 0x1000 : 0x0000;
        
        for (int ty = 0; ty < 30; ty++) {
            for (int tx = 0; tx < 32; tx++) {
                // Determine Nametable based on PPUCTRL (Base NT address)
                uint16_t base_nt = 0x2000 + ((ctrl & 0x03) * 0x0400);
                uint16_t nt_addr = base_nt + (ty * 32) + tx;
                
                uint8_t tile_index = vram[get_mirrored_addr(nt_addr, mapper.mirroring)];

                // Attribute Table (Palette selection for 2x2 tile blocks)
                uint16_t nt_offset = (nt_addr - 0x2000) % 0x0400;
                uint16_t attr_addr = (nt_addr & 0x2C00) + 0x03C0 + ((ty / 4) * 8) + (tx / 4);
                uint8_t attr_byte = vram[get_mirrored_addr(attr_addr, mapper.mirroring)];
                int shift = ((ty % 4) / 2) * 4 + ((tx % 4) / 2) * 2;
                uint8_t palette_id = (attr_byte >> shift) & 0x03;

                for (int y = 0; y < 8; y++) {
                    uint8_t lo = mapper.read_chr(bg_table + (tile_index * 16) + y);
                    uint8_t hi = mapper.read_chr(bg_table + (tile_index * 16) + y + 8);

                    for (int x = 0; x < 8; x++) {
                        uint8_t bit = 7 - x;
                        uint8_t color_idx = ((lo >> bit) & 0x01) | (((hi >> bit) & 0x01) << 1);

                        if (color_idx != 0) {
                            uint32_t color = nes_palette[palette_ram[(palette_id << 2) + color_idx] & 0x3F];
                            screen_buffer[(ty * 8 + y) * 256 + (tx * 8 + x)] = color;
                        }
                    }
                }
            }
        }
    }

    // 2. Sprite Rendering
    if (mask & 0x10) {
        uint16_t sprite_table = (ctrl & 0x08) ? 0x1000 : 0x0000;

        // Render in reverse to handle sprite priority (0 is highest)
        for (int i = 63; i >= 0; i--) {
            uint8_t y_pos = oam_ram[i * 4];
            uint8_t tile_idx = oam_ram[i * 4 + 1];
            uint8_t attributes = oam_ram[i * 4 + 2];
            uint8_t x_pos = oam_ram[i * 4 + 3];

            if (y_pos >= 239) continue;

            uint8_t pal_id = (attributes & 0x03) + 4;
            bool flip_v = attributes & 0x80;
            bool flip_h = attributes & 0x40;

            for (int py = 0; py < 8; py++) {
                int row = flip_v ? (7 - py) : py;
                uint8_t lo = mapper.read_chr(sprite_table + (tile_idx * 16) + row);
                uint8_t hi = mapper.read_chr(sprite_table + (tile_idx * 16) + row + 8);

                for (int px = 0; px < 8; px++) {
                    int col = flip_h ? px : (7 - px);
                    uint8_t color_idx = ((lo >> col) & 0x01) | (((hi >> col) & 0x01) << 1);

                    if (color_idx == 0) continue;

                    int sx = x_pos + px;
                    int sy = y_pos + py + 1;

                    if (sx < 256 && sy < 240) {
                        screen_buffer[sy * 256 + sx] = nes_palette[palette_ram[(pal_id << 2) + color_idx] & 0x3F];
                    }
                }
            }
        }
    }
}
