#include "PPU.h"
#include <cstring>
#include <algorithm>

/**
 * PPU Lifecycle
 */
PPU::PPU() {
    std::memset(vram, 0, sizeof(vram));
    std::memset(oam_ram, 0, sizeof(oam_ram));
    std::memset(palette_ram, 0, sizeof(palette_ram));
    std::memset(screen_buffer, 0, sizeof(screen_buffer));
    reset();
}

void PPU::reset() {
    ctrl = 0;
    mask = 0;
    status = 0;
    oam_addr = 0;
    scroll_x = 0;
    scroll_y = 0;
    ppu_addr = 0;
    ppu_data_buffer = 0;
    latch = 0;
    addr_latch = 0; // The 'w' register
    nmi_pending = false;
}

/**
 * Register Communication (CPU Bus <-> PPU)
 */
uint8_t PPU::cpu_read(uint16_t addr, MapperMMC1& mapper) {
    uint8_t res = latch; // Default to open bus
    switch (0x2000 + (addr % 8)) {
        case 0x2002: // Status
            res = (status & 0xE0) | (latch & 0x1F);
            status &= ~0x80; // Clear VBlank bit
            addr_latch = 0;  // Reset address latch (w)
            break;
        case 0x2004: // OAM Data
            res = oam_ram[oam_addr];
            break;
        case 0x2007: // PPU Data
            res = ppu_data_buffer;
            ppu_data_buffer = vram_read(ppu_addr, mapper);
            // Palette reads are not buffered
            if (ppu_addr >= 0x3F00) res = ppu_data_buffer; 
            ppu_addr += (ctrl & 0x04) ? 32 : 1;
            ppu_addr &= 0x3FFF;
            break;
    }
    return res;
}

void PPU::cpu_write(uint16_t addr, uint8_t val, MapperMMC1& mapper) {
    latch = val;
    switch (0x2000 + (addr % 8)) {
        case 0x2000: ctrl = val; break;
        case 0x2001: mask = val; break;
        case 0x2003: oam_addr = val; break;
        case 0x2004: oam_ram[oam_addr++] = val; break;
        case 0x2005: // Scroll
            if (addr_latch == 0) { scroll_x = val; addr_latch = 1; }
            else { scroll_y = val; addr_latch = 0; }
            break;
        case 0x2006: // PPU Addr
            if (addr_latch == 0) { ppu_addr = (ppu_addr & 0x00FF) | ((val & 0x3F) << 8); addr_latch = 1; }
            else { ppu_addr = (ppu_addr & 0xFF00) | val; addr_latch = 0; }
            break;
        case 0x2007: // PPU Data
            vram_write(ppu_addr, val, mapper);
            ppu_addr += (ctrl & 0x04) ? 32 : 1;
            ppu_addr &= 0x3FFF;
            break;
    }
}

/**
 * Internal Memory Access with Palette Mirroring Fix
 */
uint8_t PPU::vram_read(uint16_t addr, MapperMMC1& mapper) {
    addr &= 0x3FFF;
    if (addr < 0x2000) return mapper.read_chr(addr);
    if (addr < 0x3F00) return vram[get_mirrored_addr(addr, mapper.get_mirroring())];
    
    // Palette Mirroring: 0x3F10, 0x3F14, 0x3F18, 0x3F1C are mirrors of 0x3F00, 0x3F04, etc.
    uint16_t p_addr = addr & 0x1F;
    if (p_addr >= 0x10 && (p_addr & 0x03) == 0) p_addr -= 0x10;
    return palette_ram[p_addr];
}

void PPU::vram_write(uint16_t addr, uint8_t val, MapperMMC1& mapper) {
    addr &= 0x3FFF;
    if (addr < 0x2000) {
        mapper.write_chr(addr, val);
    } else if (addr < 0x3F00) {
        vram[get_mirrored_addr(addr, mapper.get_mirroring())] = val;
    } else {
        uint16_t p_addr = addr & 0x1F;
        if (p_addr >= 0x10 && (p_addr & 0x03) == 0) p_addr -= 0x10;
        palette_ram[p_addr] = val;
    }
}



uint16_t PPU::get_mirrored_addr(uint16_t addr, Mirroring mirroring) {
    addr &= 0x0FFF; // Offset from 0x2000
    switch (mirroring) {
        case Mirroring::ONE_SCREEN_LOW:  return addr & 0x03FF;
        case Mirroring::ONE_SCREEN_HIGH: return (addr & 0x03FF) + 0x0400;
        case Mirroring::HORIZONTAL:
            // (0,1) maps to 0, (2,3) maps to 1
            return ((addr / 2) & 0x0400) + (addr % 0x0400);
        case Mirroring::VERTICAL:
            return addr & 0x07FF;
        default: return addr & 0x07FF;
    }
}

/**
 * Main Rendering Entry Point
 */
void PPU::render_frame(MapperMMC1& mapper, const uint32_t* nes_palette) {
    // If rendering is disabled, fill with backdrop color and exit
    if (!(mask & 0x18)) {
        uint32_t backdrop = nes_palette[palette_ram[0] & 0x3F];
        std::fill(screen_buffer, screen_buffer + (256 * 240), backdrop);
        return; 
    }

    uint8_t bg_pixel_map[256 * 240];
    std::memset(bg_pixel_map, 0, sizeof(bg_pixel_map));

    // 1. RENDER BACKGROUND
    if (mask & 0x08) {
        uint16_t bg_pt_base = (ctrl & 0x10) ? 0x1000 : 0x0000;
        Mirroring mirroring = mapper.get_mirroring();
        uint16_t base_nt_idx = (ctrl & 0x03); 

        for (int y = 0; y < 240; y++) {
            for (int x = 0; x < 256; x++) {
                int abs_x = x + scroll_x;
                int abs_y = y + scroll_y;
                
                // Nametable selection based on scroll
                int nt_x = (abs_x / 256) % 2;
                int nt_y = (abs_y / 240) % 2;
                uint16_t current_nt_base = 0x2000 + ((base_nt_idx ^ nt_x ^ (nt_y << 1)) * 0x0400);

                int tx = (abs_x % 256) / 8;
                int ty = (abs_y % 240) / 8;

                uint16_t nt_addr = current_nt_base + (ty * 32) + tx;
                uint8_t tile_idx = vram[get_mirrored_addr(nt_addr, mirroring)];

                uint16_t attr_addr = current_nt_base + 0x3C0 + (ty / 4) * 8 + (tx / 4);
                uint8_t attr = vram[get_mirrored_addr(attr_addr, mirroring)];
                uint8_t palette_idx = (attr >> (((ty & 2) ? 4 : 0) | ((tx & 2) ? 2 : 0))) & 0x03;

                uint8_t p_low = mapper.read_chr(bg_pt_base + (tile_idx * 16) + (abs_y % 8));
                uint8_t p_high = mapper.read_chr(bg_pt_base + (tile_idx * 16) + (abs_y % 8) + 8);
                
                int bit_pos = 7 - (abs_x % 8);
                uint8_t pixel = ((p_low >> bit_pos) & 0x01) | (((p_high >> bit_pos) & 0x01) << 1);

                bg_pixel_map[y * 256 + x] = pixel; 
                uint8_t color_final = (pixel == 0) ? palette_ram[0] : palette_ram[(palette_idx << 2) | pixel];
                screen_buffer[y * 256 + x] = nes_palette[color_final & 0x3F];
            }
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
            sy++; // NES Sprites are delayed by one scanline

            for (int py = 0; py < 8; py++) {
                int target_y = sy + py;
                if (target_y >= 240) continue;
                
                int line = (attr & 0x80) ? (7 - py) : py;
                uint8_t low = mapper.read_chr(sp_pt_base + (tile * 16) + line);
                uint8_t high = mapper.read_chr(sp_pt_base + (tile * 16) + line + 8);

                for (int px = 0; px < 8; px++) {
                    int target_x = sx + px;
                    if (target_x >= 256) continue;
                    
                    int bit = (attr & 0x40) ? px : (7 - px);
                    uint8_t pixel = ((low >> bit) & 0x01) | (((high >> bit) & 0x01) << 1);

                    if (pixel == 0) continue; 

                    // Sprite 0 Hit Logic
                    if (i == 0 && bg_pixel_map[target_y * 256 + target_x] != 0) {
                        status |= 0x40; 
                    }

                    // Priority Logic
                    if ((attr & 0x20) && bg_pixel_map[target_y * 256 + target_x] != 0) continue;

                    uint8_t p_idx = (attr & 0x03) + 4;
                    uint8_t color_final = palette_ram[(p_idx << 2) | pixel];
                    screen_buffer[target_y * 256 + target_x] = nes_palette[color_final & 0x3F];
                }
            }
        }
    }
}
