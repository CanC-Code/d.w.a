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
    nmi_pending = false;
}

/**
 * NES Mirroring Logic: Maps the 4KB virtual space ($2000-$2FFF) into 2KB physical VRAM.
 */

uint16_t PPU::get_mirrored_addr(uint16_t addr, Mirroring mode) {
    uint16_t mirrored = addr & 0x0FFF; // Relative to $2000
    switch (mode) {
        case Mirroring::ONE_SCREEN_LOW:  return mirrored & 0x03FF;
        case Mirroring::ONE_SCREEN_HIGH: return (mirrored & 0x03FF) + 0x0400;
        case Mirroring::VERTICAL:        return mirrored & 0x07FF;
        case Mirroring::HORIZONTAL:      
            // Correct Horizontal: Maps NT0/NT1 to first 1KB, NT2/NT3 to second 1KB
            return ((mirrored & 0x0800) >> 1) | (mirrored & 0x03FF);
        default: return mirrored & 0x07FF;
    }
}

uint8_t PPU::cpu_read(uint16_t addr, MapperMMC1& mapper) {
    uint8_t reg = addr & 0x0007;
    switch (reg) {
        case 2: { // PPUSTATUS
            uint8_t res = (status & 0xE0) | (data_buffer & 0x1F);
            status &= ~0x80; // Clear VBlank flag on read
            addr_latch = 0;  // Reset address/scroll latch
            return res;
        }
        case 4: return oam_ram[oam_addr];
        case 7: { // PPUDATA
            uint16_t p_addr = addr_reg & 0x3FFF;
            uint8_t data = data_buffer; // Buffered read

            if (p_addr < 0x2000) {
                data_buffer = mapper.read_chr(p_addr);
            } else if (p_addr < 0x3F00) {
                data_buffer = vram[get_mirrored_addr(p_addr, mapper.get_mirroring())];
            } else {
                // Palette reads are immediate (no buffer delay)
                data = palette_ram[p_addr & 0x1F];
                // Buffer is filled with the mirrored VRAM data "underneath" the palette
                data_buffer = vram[get_mirrored_addr(p_addr, mapper.get_mirroring())];
            }
            addr_reg += (ctrl & 0x04) ? 32 : 1;
            return data;
        }
    }
    return 0;
}

void PPU::cpu_write(uint16_t addr, uint8_t val, MapperMMC1& mapper) {
    uint8_t reg = addr & 0x0007;
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
            if (p_addr < 0x2000) {
                mapper.write_chr(p_addr, val);
            } else if (p_addr < 0x3F00) {
                vram[get_mirrored_addr(p_addr, mapper.get_mirroring())] = val;
            } else {
                uint16_t p_idx = p_addr & 0x1F;
                // Palette Mirroring hardware behavior
                if ((p_idx & 0x03) == 0) {
                    palette_ram[0x00] = palette_ram[0x04] = palette_ram[0x08] = 
                    palette_ram[0x0C] = palette_ram[0x10] = palette_ram[0x14] = 
                    palette_ram[0x18] = palette_ram[0x1C] = val;
                }
                palette_ram[p_idx] = val;
            }
            addr_reg += (ctrl & 0x04) ? 32 : 1;
            break;
        }
    }
}

/**
 * Basic Frame Rendering
 * Handles Background Nametables and Attribute Palettes.
 */
void PPU::render_frame(MapperMMC1& mapper, const uint32_t* nes_palette) {
    if (!(mask & 0x18)) {
        std::fill(screen_buffer, screen_buffer + (256 * 240), 0xFF000000);
        return; 
    }

    uint16_t pt_base = (ctrl & 0x10) ? 0x1000 : 0x0000;
    Mirroring mirroring = mapper.get_mirroring();

    for (int ty = 0; ty < 30; ty++) {
        for (int tx = 0; tx < 32; tx++) {
            // 1. Fetch Nametable Tile index
            uint16_t nt_addr = 0x2000 + (ty * 32) + tx;
            uint8_t tile_idx = vram[get_mirrored_addr(nt_addr, mirroring)];

            // 2. Fetch Attribute Byte for palette selection
            uint16_t attr_addr = 0x23C0 + (ty / 4) * 8 + (tx / 4);
            uint8_t attr = vram[get_mirrored_addr(attr_addr, mirroring)];
            uint8_t palette_idx = (attr >> (((ty & 2) ? 4 : 0) | ((tx & 2) ? 2 : 0))) & 0x03;

            // 3. Render the 8x8 tile
            for (int py = 0; py < 8; py++) {
                uint8_t low = mapper.read_chr(pt_base + (tile_idx * 16) + py);
                uint8_t high = mapper.read_chr(pt_base + (tile_idx * 16) + py + 8);

                for (int px = 0; px < 8; px++) {
                    uint8_t pixel = ((low >> (7 - px)) & 0x01) | (((high >> (7 - px)) & 0x01) << 1);
                    
                    int screen_x = tx * 8 + px;
                    int screen_y = ty * 8 + py;

                    if (pixel != 0) { // Color 0 is transparent/background
                        uint8_t color_final = palette_ram[(palette_idx << 2) | pixel];
                        screen_buffer[screen_y * 256 + screen_x] = nes_palette[color_final & 0x3F];
                    } else {
                        // Background color (Universal Background)
                        screen_buffer[screen_y * 256 + screen_x] = nes_palette[palette_ram[0] & 0x3F];
                    }
                }
            }
        }
    }
}
