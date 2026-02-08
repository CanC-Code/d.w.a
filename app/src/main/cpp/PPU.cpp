/**
 * Finalized PPU Rendering for Dragon Warrior
 */
void PPU::render_frame(MapperMMC1& mapper, const uint32_t* nes_palette) {
    if (!(mask & 0x18)) {
        std::fill(screen_buffer, screen_buffer + (256 * 240), nes_palette[palette_ram[0] & 0x3F]);
        return; 
    }

    // Temporary buffer to track background transparency for Sprite-Zero Hit
    uint8_t bg_pixel_map[256 * 240] = {0};

    // 1. RENDER BACKGROUND
    uint16_t bg_pt_base = (ctrl & 0x10) ? 0x1000 : 0x0000;
    Mirroring mirroring = mapper.get_mirroring();

    for (int y = 0; y < 240; y++) {
        for (int x = 0; x < 256; x++) {
            // Scrolling Logic
            int fine_x = (x + scroll_x) % 512;
            int fine_y = (y + scroll_y) % 480;
            
            uint16_t nt_base = 0x2000 + ((ctrl & 0x03) * 0x0400);
            int tx = fine_x / 8;
            int ty = fine_y / 8;
            
            // Handle Nametable wrap-around correctly for MMC1
            uint16_t current_nt = nt_base;
            if (tx >= 32) { current_nt ^= 0x0400; tx -= 32; }
            if (ty >= 30) { current_nt ^= 0x0800; ty -= 30; }
            
            uint16_t nt_addr = current_nt + (ty * 32) + tx;
            uint8_t tile_idx = vram[get_mirrored_addr(nt_addr, mirroring)];

            uint16_t attr_addr = current_nt + 0x3C0 + (ty / 4) * 8 + (tx / 4);
            uint8_t attr = vram[get_mirrored_addr(attr_addr, mirroring)];
            uint8_t palette_idx = (attr >> (((ty & 2) ? 4 : 0) | ((tx & 2) ? 2 : 0))) & 0x03;

            uint8_t p_low = mapper.read_chr(bg_pt_base + (tile_idx * 16) + (fine_y % 8));
            uint8_t p_high = mapper.read_chr(bg_pt_base + (tile_idx * 16) + (fine_y % 8) + 8);
            uint8_t pixel = ((p_low >> (7 - (fine_x % 8))) & 0x01) | (((p_high >> (7 - (fine_x % 8))) & 0x01) << 1);

            bg_pixel_map[y * 256 + x] = pixel; // Store for Sprite-Zero check
            
            uint8_t color_final = (pixel == 0) ? palette_ram[0] : palette_ram[(palette_idx << 2) | pixel];
            screen_buffer[y * 256 + x] = nes_palette[color_final & 0x3F];
        }
    }

    // 2. RENDER SPRITES
    if (mask & 0x10) {
        uint16_t sp_pt_base = (ctrl & 0x08) ? 0x1000 : 0x0000;
        for (int i = 63; i >= 0; i--) {
            uint8_t sy = oam_ram[i * 4];
            uint8_t tile = oam_ram[i * 4 + 1];
            uint8_t attr = oam_ram[i * 4 + 2];
            uint8_t sx = oam_ram[i * 4 + 3];

            if (sy >= 239) continue;
            sy++; // Sprites are offset by one scanline vertically

            for (int py = 0; py < 8; py++) {
                if (sy + py >= 240) continue;
                
                int line = (attr & 0x80) ? (7 - py) : py;
                uint8_t low = mapper.read_chr(sp_pt_base + (tile * 16) + line);
                uint8_t high = mapper.read_chr(sp_pt_base + (tile * 16) + line + 8);

                for (int px = 0; px < 8; px++) {
                    if (sx + px >= 256) continue;
                    
                    int bit = (attr & 0x40) ? px : (7 - px);
                    uint8_t pixel = ((low >> bit) & 0x01) | (((high >> bit) & 0x01) << 1);

                    if (pixel == 0) continue; 

                    // --- NEW: Sprite-Zero Hit Logic ---
                    if (i == 0 && bg_pixel_map[(sy + py) * 256 + (sx + px)] != 0) {
                        status |= 0x40; // Set Sprite-Zero flag
                    }

                    // --- NEW: Priority Check (Behind Background) ---
                    if ((attr & 0x20) && bg_pixel_map[(sy + py) * 256 + (sx + px)] != 0) continue;

                    uint8_t p_idx = (attr & 0x03) + 4;
                    // Palette Mirroring: Correct for $3F10, $3F14...
                    uint16_t pal_addr = (p_idx << 2) | pixel;
                    uint8_t color_final = palette_ram[pal_addr];
                    screen_buffer[(sy + py) * 256 + (sx + px)] = nes_palette[color_final & 0x3F];
                }
            }
        }
    }
}
