#include "PPU.h"
#include <cstring>
#include <algorithm>

PPU::PPU() { reset(); }

void PPU::reset() {
    std::memset(vram, 0, sizeof(vram));
    std::memset(palette_ram, 0, sizeof(palette_ram));
    std::memset(oam_ram, 0, sizeof(oam_ram));
    std::fill(screen_buffer, screen_buffer + (256 * 240), 0xFF000000); 
    ctrl = mask = status = oam_addr = 0;
    addr_reg = 0; addr_latch = 0; data_buffer = 0;
    scroll_x = 0; scroll_y = 0;
}

uint16_t PPU::get_mirrored_addr(uint16_t addr, uint8_t mirror_mode) {
    uint16_t mirrored = (addr - 0x2000) & 0x0FFF;
    // mirror_mode: 0=OneScreen Lower, 1=OneScreen Upper, 2=Vertical, 3=Horizontal
    switch (mirror_mode) {
        case 0: return mirrored & 0x03FF;
        case 1: return (mirrored & 0x03FF) + 0x0400;
        case 2: return mirrored & 0x07FF; // Vertical
        case 3: 
        default:
            if (mirrored < 0x0800) return mirrored & 0x03FF;
            return (mirrored & 0x03FF) + 0x0400; // Horizontal
    }
}

uint8_t PPU::cpu_read(uint16_t addr, MapperMMC1& mapper) {
    uint8_t reg = addr % 8;
    switch (reg) {
        case 2: { // PPUSTATUS
            uint8_t res = (status & 0xE0) | (data_buffer & 0x1F);
            status &= ~0x80; // Clear VBlank flag on read
            addr_latch = 0; 
            return res;
        }
        case 4: return oam_ram[oam_addr]; // OAMDATA
        case 7: { // PPUDATA
            uint16_t p_addr = addr_reg & 0x3FFF;
            uint8_t data = data_buffer;
            if (p_addr < 0x2000) data_buffer = mapper.read_chr(p_addr);
            else if (p_addr < 0x3F00) data_buffer = vram[get_mirrored_addr(p_addr, mapper.mirroring)];
            else {
                data = palette_ram[p_addr & 0x1F];
                data_buffer = vram[get_mirrored_addr(p_addr, mapper.mirroring)];
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
            if (addr_latch == 0) {
                addr_reg = (uint16_t)((addr_reg & 0x00FF) | ((val & 0x3F) << 8));
                addr_latch = 1;
            } else {
                addr_reg = (addr_reg & 0xFF00) | val;
                addr_latch = 0;
            }
            break;
        case 7: { // PPUDATA
            uint16_t p_addr = addr_reg & 0x3FFF;
            if (p_addr < 0x2000) mapper.write_chr(p_addr, val);
            else if (p_addr < 0x3F00) vram[get_mirrored_addr(p_addr, mapper.mirroring)] = val;
            else {
                uint16_t p_idx = p_addr & 0x1F;
                if (p_idx >= 0x10 && (p_idx % 4 == 0)) p_idx -= 0x10;
                palette_ram[p_idx] = val;
            }
            addr_reg += (ctrl & 0x04) ? 32 : 1;
            break;
        }
    }
}

/**
 * Enhanced Rendering Loop
 * Implements Nametable switching and better Background logic.
 */
void PPU::render_frame(MapperMMC1& mapper, const uint32_t* nes_palette) {
    // Set VBlank status (In a real NES this happens mid-frame, 
    // but for static recompilation we set it here for the next CPU cycle)
    status |= 0x80;

    uint32_t backdrop = nes_palette[palette_ram[0] & 0x3F];
    std::fill(screen_buffer, screen_buffer + (256 * 240), backdrop);

    if (!(mask & 0x18)) return; // Screen is off

    // 1. Background (Tile-based with Scroll)
    if (mask & 0x08) {
        uint16_t bg_pt_base = (ctrl & 0x10) ? 0x1000 : 0x0000;
        
        for (int row = 0; row < 30; row++) {
            for (int col = 0; col < 32; col++) {
                // Determine base Nametable from PPUCTRL
                uint16_t base_nt = 0x2000 + ((ctrl & 0x03) * 0x0400);
                uint16_t nt_addr = base_nt + (row * 32) + col;

                uint8_t tile_index = vram[get_mirrored_addr(nt_addr, mapper.mirroring)];
                
                // Attribute Table Logic
                uint16_t attr_addr = (nt_addr & 0x2C00) + 0x03C0 + ((row / 4) * 8) + (col / 4);
                uint8_t attr_byte = vram[get_mirrored_addr(attr_addr, mapper.mirroring)];
                uint8_t palette_id = (attr_byte >> (((row % 4) / 2) * 4 + ((col % 4) / 2) * 2)) & 0x03;

                // Render 8x8 Tile
                for (int y = 0; y < 8; y++) {
                    uint8_t lo = mapper.read_chr(bg_pt_base + (tile_index * 16) + y);
                    uint8_t hi = mapper.read_chr(bg_pt_base + (tile_index * 16) + y + 8);
                    for (int x = 0; x < 8; x++) {
                        uint8_t color_idx = ((lo >> (7 - x)) & 0x01) | (((hi >> (7 - x)) & 0x01) << 1);
                        if (color_idx == 0) continue;

                        int px = (col * 8) + x;
                        int py = (row * 8) + y;
                        if (px < 256 && py < 240) {
                            screen_buffer[py * 256 + px] = nes_palette[palette_ram[(palette_id << 2) + color_idx] & 0x3F];
                        }
                    }
                }
            }
        }
    }

    // 2. Sprites (OAM)
    if (mask & 0x10) {
        uint16_t sp_pt_base = (ctrl & 0x08) ? 0x1000 : 0x0000;
        for (int i = 63; i >= 0; i--) {
            uint8_t y = oam_ram[i * 4];
            uint8_t tile = oam_ram[i * 4 + 1];
            uint8_t attr = oam_ram[i * 4 + 2];
            uint8_t x = oam_ram[i * 4 + 3];

            if (y >= 239) continue;

            for (int py = 0; py < 8; py++) {
                int row = (attr & 0x80) ? (7 - py) : py;
                uint8_t lo = mapper.read_chr(sp_pt_base + (tile * 16) + row);
                uint8_t hi = mapper.read_chr(sp_pt_base + (tile * 16) + row + 8);
                for (int px = 0; px < 8; px++) {
                    int col = (attr & 0x40) ? px : (7 - px);
                    uint8_t color_idx = ((lo >> col) & 0x01) | (((hi >> col) & 0x01) << 1);
                    if (color_idx == 0) continue;

                    int screen_x = x + px;
                    int screen_y = y + py + 1;
                    if (screen_x < 256 && screen_y < 240) {
                        screen_buffer[screen_y * 256 + screen_x] = nes_palette[palette_ram[((attr & 0x03) + 4) * 4 + color_idx] & 0x3F];
                    }
                }
            }
        }
    }
}
