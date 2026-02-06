#include "PPU.h"
#include <cstring>

PPU::PPU() { reset(); }

void PPU::reset() {
    std::memset(vram, 0, sizeof(vram));
    std::memset(palette_ram, 0, sizeof(palette_ram));
    std::memset(oam_ram, 0, sizeof(oam_ram));
    // Initialize screen with a non-black color (e.g., dark blue) to detect engine life
    std::fill(screen_buffer, screen_buffer + (256 * 240), 0xFF000033); 
    ctrl = mask = status = oam_addr = 0;
    addr_reg = 0; addr_latch = 0; data_buffer = 0;
}

uint16_t PPU::get_mirrored_addr(uint16_t addr) {
    uint16_t mirrored = (addr - 0x2000) & 0x0FFF;
    // Dragon Warrior (MMC1) usually uses Vertical Mirroring:
    // $2000-$23FF -> Bank 0
    // $2400-$27FF -> Bank 1
    // $2800-$2BFF -> Mirror of Bank 0
    // $2C00-$2FFF -> Mirror of Bank 1
    if (mirrored >= 0x0800) mirrored -= 0x0800;
    return mirrored;
}

uint8_t PPU::cpu_read(uint16_t addr, MapperMMC1& mapper) {
    uint8_t reg = addr % 8;
    if (reg == 2) { // PPUSTATUS
        uint8_t res = status;
        status &= ~0x80; // Clear VBlank flag on read
        addr_latch = 0;  // Reset address latch for $2006
        return res;
    }
    if (reg == 4) return oam_ram[oam_addr]; // OAMDATA
    if (reg == 7) { // PPUDATA
        uint16_t p_addr = addr_reg & 0x3FFF;
        uint8_t data = data_buffer;

        if (p_addr < 0x2000) {
            data_buffer = mapper.read_chr(p_addr);
        } else if (p_addr < 0x3F00) {
            data_buffer = vram[get_mirrored_addr(p_addr)];
        } else {
            // Palette reads are immediate, but buffer is filled with VRAM underneath
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
            mapper.write_chr(p_addr, val);
        } else if (p_addr < 0x3F00) {
            vram[get_mirrored_addr(p_addr)] = val;
        } else {
            // Critical Palette Mirroring: $3F10, $3F14, $3F18, $3F1C are mirrors of $3F00, $3F04, $3F08, $3F0C
            uint16_t p_idx = p_addr & 0x1F;
            if (p_idx >= 0x10 && (p_idx % 4 == 0)) p_idx -= 0x10;
            palette_ram[p_idx] = val;
        }
        addr_reg += (ctrl & 0x04) ? 32 : 1;
    }
}

void PPU::render_frame(MapperMMC1& mapper, const uint32_t* nes_palette) {
    // 1. Clear Frame with Backdrop
    uint32_t bg_color = nes_palette[palette_ram[0] & 0x3F];
    std::fill(screen_buffer, screen_buffer + (256 * 240), bg_color);

    if (!(mask & 0x08)) return; // Background rendering disabled

    uint16_t bg_table = (ctrl & 0x10) ? 0x1000 : 0x0000;
    uint16_t sprite_table = (ctrl & 0x08) ? 0x1000 : 0x0000;

    // 2. Render Background
    for (int ty = 0; ty < 30; ty++) {
        for (int tx = 0; tx < 32; tx++) {
            uint16_t nt_addr = 0x2000 + (ty * 32) + tx;
            uint8_t tile_index = vram[get_mirrored_addr(nt_addr)];

            uint16_t attr_addr = 0x23C0 + (ty / 4) * 8 + (tx / 4);
            uint8_t attr_byte = vram[get_mirrored_addr(attr_addr)];
            // Determine which 2 bits of the attribute byte to use
            int shift = ((ty % 4) / 2) * 4 + ((tx % 4) / 2) * 2;
            uint8_t palette_id = (attr_byte >> shift) & 0x03;

            for (int y = 0; y < 8; y++) {
                uint8_t lo = mapper.read_chr(bg_table + (tile_index * 16) + y);
                uint8_t hi = mapper.read_chr(bg_table + (tile_index * 16) + y + 8);

                for (int x = 0; x < 8; x++) {
                    uint8_t bit = 7 - x;
                    uint8_t color = ((lo >> bit) & 0x01) | (((hi >> bit) & 0x01) << 1);
                    if (color == 0) continue;

                    uint32_t pixel_color = nes_palette[palette_ram[(palette_id << 2) + color] & 0x3F];
                    screen_buffer[(ty * 8 + y) * 256 + (tx * 8 + x)] = pixel_color;
                }
            }
        }
    }

    // 3. Render Sprites (Dragon Warrior uses 8x8 sprites)
    if (!(mask & 0x10)) return; 

    for (int i = 63; i >= 0; i--) { // Render in reverse to handle priority correctly
        uint8_t y_pos = oam_ram[i * 4] + 1;
        uint8_t tile_idx = oam_ram[i * 4 + 1];
        uint8_t attributes = oam_ram[i * 4 + 2];
        uint8_t x_pos = oam_ram[i * 4 + 3];

        if (y_pos >= 240) continue;

        uint8_t pal_id = (attributes & 0x03) + 4; // Sprite palettes start at index 4
        bool flip_v = attributes & 0x80;
        bool flip_h = attributes & 0x40;

        for (int py = 0; py < 8; py++) {
            int row = flip_v ? (7 - py) : py;
            uint8_t lo = mapper.read_chr(sprite_table + (tile_idx * 16) + row);
            uint8_t hi = mapper.read_chr(sprite_table + (tile_idx * 16) + row + 8);

            for (int px = 0; px < 8; px++) {
                int col = flip_h ? px : (7 - px);
                uint8_t color = ((lo >> col) & 0x01) | (((hi >> col) & 0x01) << 1);
                
                if (color == 0) continue; // Transparent
                if (x_pos + px >= 256 || y_pos + py >= 240) continue;

                screen_buffer[(y_pos + py) * 256 + (x_pos + px)] = nes_palette[palette_ram[(pal_id << 2) + color] & 0x3F];
            }
        }
    }
}
