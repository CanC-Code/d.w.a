#include "PPU.h"
#include <cstring>
#include <algorithm>

/**
 * Resolves NES Nametable mirroring for MMC1.
 * Maps a logical PPU address ($2000-$2FFF) to a physical offset in 2KB VRAM.
 */
uint16_t PPU::get_mirrored_addr(uint16_t addr, Mirroring mirroring) {
    addr &= 0x0FFF; // Map to 0-$FFF range
    switch (mirroring) {
        case Mirroring::ONE_SCREEN_LOW:  return addr % 0x0400;
        case Mirroring::ONE_SCREEN_HIGH: return (addr % 0x0400) + 0x0400;
        case Mirroring::HORIZONTAL:
            // Standard Horizontal: AABB pattern
            if (addr < 0x0800) return addr % 0x0400;
            else               return (addr % 0x0400) + 0x0400;
        case Mirroring::VERTICAL:
            // Standard Vertical: ABAB pattern
            return addr % 0x0800;
        default: return addr % 0x0800;
    }
}

/**
 * Main Rendering Entry Point
 * Called once per frame by the engine loop.
 */
void PPU::render_frame(MapperMMC1& mapper, const uint32_t* nes_palette) {
    // 0. Reset frame status bits
    // Clearing Sprite-Zero hit allows the game to detect it fresh this frame
    status &= ~0x40; 

    // If rendering is disabled (mask bits 3 & 4), just fill with backdrop color
    if (!(mask & 0x18)) {
        std::fill(screen_buffer, screen_buffer + (256 * 240), nes_palette[palette_ram[0] & 0x3F]);
        return; 
    }

    // Temporary map to track which background pixels are opaque (non-zero)
    // Required for Sprite-Zero Hit detection and Sprite Priority
    uint8_t bg_pixel_map[256 * 240];
    std::memset(bg_pixel_map, 0, sizeof(bg_pixel_map));

    // 1. RENDER BACKGROUND
    uint16_t bg_pt_base = (ctrl & 0x10) ? 0x1000 : 0x0000;
    Mirroring mirroring = mapper.get_mirroring();
    uint16_t base_nt_idx = (ctrl & 0x03); 

    for (int y = 0; y < 240; y++) {
        for (int x = 0; x < 256; x++) {
            // Calculate absolute coordinates in the 512x480 space
            int abs_x = x + scroll_x;
            int abs_y = y + scroll_y;

            // Determine which of the 4 nametables we are currently in
            int nt_x = (abs_x / 256) % 2;
            int nt_y = (abs_y / 240) % 2;
            
            // Logic to pick the correct 1KB quadrant
            uint16_t current_nt_base = 0x2000 + ((base_nt_idx ^ nt_x ^ (nt_y << 1)) * 0x0400);

            int tx = (abs_x % 256) / 8;
            int ty = (abs_y % 240) / 8;

            // Fetch Tile Index from Nametable
            uint16_t nt_addr = current_nt_base + (ty * 32) + tx;
            uint8_t tile_idx = vram[get_mirrored_addr(nt_addr, mirroring)];

            // Attribute Table (Palette selection for 16x16 areas)
            uint16_t attr_addr = current_nt_base + 0x3C0 + (ty / 4) * 8 + (tx / 4);
            uint8_t attr = vram[get_mirrored_addr(attr_addr, mirroring)];
            uint8_t palette_idx = (attr >> (((ty & 2) ? 4 : 0) | ((tx & 2) ? 2 : 0))) & 0x03;

            // Fetch Pixel Data from CHR-ROM/RAM via Mapper
            uint8_t p_low = mapper.read_chr(bg_pt_base + (tile_idx * 16) + (abs_y % 8));
            uint8_t p_high = mapper.read_chr(bg_pt_base + (tile_idx * 16) + (abs_y % 8) + 8);
            uint8_t pixel = ((p_low >> (7 - (abs_x % 8))) & 0x01) | (((p_high >> (7 - (abs_x % 8))) & 0x01) << 1);

            bg_pixel_map[y * 256 + x] = pixel; 

            // Color selection logic
            uint8_t color_final = (pixel == 0) ? palette_ram[0] : palette_ram[(palette_idx << 2) | pixel];
            screen_buffer[y * 256 + x] = nes_palette[color_final & 0x3F];
        }
    }

    // 2. RENDER SPRITES
    if (mask & 0x10) {
        uint16_t sp_pt_base = (ctrl & 0x08) ? 0x1000 : 0x0000;
        
        // Render in reverse order (63 to 0) so Sprite 0 has highest priority
        for (int i = 63; i >= 0; i--) {
            uint8_t sy = oam_ram[i * 4];
            uint8_t tile = oam_ram[i * 4 + 1];
            uint8_t attr = oam_ram[i * 4 + 2];
            uint8_t sx = oam_ram[i * 4 + 3];

            if (sy >= 239) continue;
            sy++; // Vertical offset for NES sprites

            for (int py = 0; py < 8; py++) {
                int target_y = sy + py;
                if (target_y >= 240) continue;

                // Vertical Flip
                int line = (attr & 0x80) ? (7 - py) : py;
                uint8_t low = mapper.read_chr(sp_pt_base + (tile * 16) + line);
                uint8_t high = mapper.read_chr(sp_pt_base + (tile * 16) + line + 8);

                for (int px = 0; px < 8; px++) {
                    int target_x = sx + px;
                    if (target_x >= 256) continue;

                    // Horizontal Flip
                    int bit = (attr & 0x40) ? px : (7 - px);
                    uint8_t pixel = ((low >> bit) & 0x01) | (((high >> bit) & 0x01) << 1);

                    if (pixel == 0) continue; // Transparency

                    // Sprite-Zero Hit Detection
                    if (i == 0 && bg_pixel_map[target_y * 256 + target_x] != 0) {
                        status |= 0x40; 
                    }

                    // Background Priority (Draw behind background if bit 5 set)
                    if ((attr & 0x20) && bg_pixel_map[target_y * 256 + target_x] != 0) continue;

                    // Sprite Palette address ($3F10-$3F1F)
                    uint8_t p_idx = (attr & 0x03) + 4;
                    uint16_t pal_addr = (p_idx << 2) | pixel;
                    
                    // Palette Mirroring for transparency indices
                    uint8_t color_final = (pal_addr % 4 == 0) ? palette_ram[0] : palette_ram[pal_addr];
                    screen_buffer[target_y * 256 + target_x] = nes_palette[color_final & 0x3F];
                }
            }
        }
    }
}
