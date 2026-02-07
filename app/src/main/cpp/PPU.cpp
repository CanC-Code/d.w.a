#include "PPU.h"
#include <cstring>
#include <algorithm>

/**
 * NES PPU (Picture Processing Unit)
 * This implementation focuses on the Dragon Warrior requirements: 
 * MMC1 scrolling, 8x8 sprites, and accurate palette mirroring.
 */

PPU::PPU() { reset(); }

void PPU::reset() {
    std::memset(vram, 0, sizeof(vram));
    std::memset(palette_ram, 0, sizeof(palette_ram));
    std::memset(oam_ram, 0, sizeof(oam_ram));
    std::fill(screen_buffer, screen_buffer + (256 * 240), 0xFF000000); 
    ctrl = mask = status = oam_addr = 0;
    addr_reg = 0; addr_latch = 0; data_buffer = 0;
    scroll_x = 0; scroll_y = 0;
    nmi_pending = false;
}

/**
 * Handles NES Nametable Mirroring.
 * Dragon Warrior (MMC1) primarily uses Horizontal and Vertical mirroring.
 */
uint16_t PPU::get_mirrored_addr(uint16_t addr, Mirroring mode) {
    uint16_t a = (addr - 0x2000) & 0x0FFF;
    switch (mode) {
        case Mirroring::ONE_SCREEN_LOW:  return a & 0x03FF;
        case Mirroring::ONE_SCREEN_HIGH: return (a & 0x03FF) | 0x0400;
        case Mirroring::VERTICAL:        return a & 0x07FF;
        case Mirroring::HORIZONTAL:      return ((a / 2) & 0x0400) | (a & 0x03FF);
        default:                         return a & 0x07FF;
    }
}

uint8_t PPU::cpu_read(uint16_t addr, MapperMMC1& mapper) {
    uint8_t reg = addr % 8;
    switch (reg) {
        case 2: { // PPUSTATUS ($2002)
            uint8_t res = (status & 0xE0);
            status &= ~0x80; // Clear VBlank flag on read
            addr_latch = 0; 
            return res;
        }
        case 4: return oam_ram[oam_addr]; // OAMDATA
        case 7: { // PPUDATA
            uint16_t p_addr = addr_reg & 0x3FFF;
            uint8_t data = data_buffer;
            if (p_addr < 0x2000) data_buffer = mapper.read_chr(p_addr);
            else if (p_addr < 0x3F00) data_buffer = vram[get_mirrored_addr(p_addr, mapper.get_mirroring()) % 2048];
            else {
                data = palette_ram[p_addr & 0x1F]; // Palettes are not buffered
                data_buffer = vram[get_mirrored_addr(p_addr, mapper.get_mirroring()) % 2048];
            }
            addr_reg += (ctrl & 0x04) ? 32 : 1;
            return data;
        }
    }
    return 0;
}

void PPU::cpu_write(uint16_t addr, uint8_t val, MapperMMC1& mapper) {
    uint8_t reg = addr % 8;
    switch (reg) {
        case 0: ctrl = val; break;
        case 1: mask = val; break;
        case 3: oam_addr = val; break;
        case 4: oam_ram[oam_addr++] = val; break;
        case 5: // PPUSCROLL
            if (addr_latch == 0) { scroll_x = val; addr_latch = 1; }
            else { scroll_y = val; addr_latch = 0; }
            break;
        case 6: // PPUADDR
            if (addr_latch == 0) { addr_reg = (uint16_t)((addr_reg & 0x00FF) | ((val & 0x3F) << 8)); addr_latch = 1; }
            else { addr_reg = (addr_reg & 0xFF00) | val; addr_latch = 0; }
            break;
        case 7: { // PPUDATA
            uint16_t p_addr = addr_reg & 0x3FFF;
            if (p_addr < 0x2000) mapper.write_chr(p_addr, val);
            else if (p_addr < 0x3F00) vram[get_mirrored_addr(p_addr, mapper.get_mirroring()) % 2048] = val;
            else {
                uint16_t p_idx = p_addr & 0x1F;
                // Palette Mirroring: $3F10, $3F14, $3F18, $3F1C mirror background colors
                if ((p_idx & 0x13) == 0x10) p_idx &= ~0x10;
                palette_ram[p_idx] = val;
            }
            addr_reg += (ctrl & 0x04) ? 32 : 1;
            break;
        }
    }
}

/**
 * Performs a Full Frame Render.
 * Optimized for recompiled execution by processing layers with scroll offsets.
 */
void PPU::render_frame(MapperMMC1& mapper, const uint32_t* nes_palette) {
    status |= 0x80; // Set VBlank bit
    if (!(mask & 0x18)) return; // Rendering disabled

    Mirroring m_mode = mapper.get_mirroring();
    uint32_t bg_color = nes_palette[palette_ram[0] & 0x3F];
    std::fill(screen_buffer, screen_buffer + (256 * 240), bg_color);

    // 1. Render Background with Scrolling
    if (mask & 0x08) {
        uint16_t pt_base = (ctrl & 0x10) ? 0x1000 : 0x0000;
        int global_x = scroll_x + ((ctrl & 0x01) ? 256 : 0);
        int global_y = scroll_y + ((ctrl & 0x02) ? 240 : 0);

        for (int py = 0; py < 240; py++) {
            for (int px = 0; px < 256; px++) {
                // Calculate which Nametable and tile we are in
                int curr_x = (global_x + px) % 512;
                int curr_y = (global_y + py) % 480;

                uint16_t nt_base = 0x2000 + (curr_y / 240) * 0x0800 + (curr_x / 256) * 0x0400;
                uint16_t tile_x = (curr_x % 256) / 8;
                uint16_t tile_y = (curr_y % 240) / 8;
                
                uint16_t nt_addr = nt_base + (tile_y * 32) + tile_x;
                uint8_t tile_id = vram[get_mirrored_addr(nt_addr, m_mode) % 2048];

                // Attribute lookup for palette
                uint16_t attr_addr = nt_base + 960 + ((tile_y / 4) * 8) + (tile_x / 4);
                uint8_t attr = vram[get_mirrored_addr(attr_addr, m_mode) % 2048];
                uint8_t pal_shift = ((tile_y & 2) ? 4 : 0) | (tile_x & 2);
                uint8_t pal_id = (attr >> pal_shift) & 0x03;

                // Pixel Fetch
                int tx = curr_x % 8;
                int ty = curr_y % 8;
                uint8_t p0 = mapper.read_chr(pt_base + (tile_id * 16) + ty);
                uint8_t p1 = mapper.read_chr(pt_base + (tile_id * 16) + ty + 8);
                uint8_t color = ((p0 >> (7 - tx)) & 1) | (((p1 >> (7 - tx)) & 1) << 1);

                if (color != 0) {
                    uint8_t pal_val = palette_ram[(pal_id << 2) + color] & 0x3F;
                    screen_buffer[py * 256 + px] = nes_palette[pal_val];
                }
            }
        }
    }

    // 2. Render Sprites (8x8 mode)
    if (mask & 0x10) {
        uint16_t pt_base = (ctrl & 0x08) ? 0x1000 : 0x0000;
        for (int i = 63; i >= 0; i--) {
            uint8_t sy = oam_ram[i * 4];
            uint8_t tile = oam_ram[i * 4 + 1];
            uint8_t attr = oam_ram[i * 4 + 2];
            uint8_t sx = oam_ram[i * 4 + 3];

            if (sy >= 239) continue;

            for (int y = 0; y < 8; y++) {
                uint8_t lo = mapper.read_chr(pt_base + (tile * 16) + ((attr & 0x80) ? (7 - y) : y));
                uint8_t hi = mapper.read_chr(pt_base + (tile * 16) + ((attr & 0x80) ? (7 - y) : y) + 8);
                
                for (int x = 0; x < 8; x++) {
                    uint8_t bit = (attr & 0x40) ? x : (7 - x);
                    uint8_t color = ((lo >> bit) & 1) | (((hi >> bit) & 1) << 1);
                    
                    if (color == 0) continue; // Transparent

                    int dx = sx + x;
                    int dy = sy + y + 1;
                    if (dx < 256 && dy < 240) {
                        // Priority: If bit 5 is set, sprite is behind background
                        if ((attr & 0x20) && screen_buffer[dy * 256 + dx] != bg_color) continue;

                        uint8_t pal_idx = palette_ram[0x10 + ((attr & 0x03) << 2) + color] & 0x3F;
                        screen_buffer[dy * 256 + dx] = nes_palette[pal_idx];
                    }
                }
            }
        }
    }
}
