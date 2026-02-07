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
 * NES Mirroring Logic: Resolves VRAM addresses $2000-$2FFF into a 2KB buffer.
 */
uint16_t PPU::get_mirrored_addr(uint16_t addr, Mirroring mode) {
    uint16_t mirrored = addr & 0x0FFF;
    switch (mode) {
        case Mirroring::ONE_SCREEN_LOW:  return mirrored & 0x03FF;
        case Mirroring::ONE_SCREEN_HIGH: return (mirrored & 0x03FF) + 0x0400;
        case Mirroring::VERTICAL:        return mirrored & 0x07FF;
        case Mirroring::HORIZONTAL:      
        default:
            // (A B) over (C D) -> (A A) over (B B)
            return ((mirrored / 2) & 0x0400) | (mirrored & 0x03FF);
    }
}

uint8_t PPU::cpu_read(uint16_t addr, MapperMMC1& mapper) {
    uint8_t reg = addr & 0x0007;
    switch (reg) {
        case 2: { // PPUSTATUS
            uint8_t res = (status & 0xE0) | (data_buffer & 0x1F);
            status &= ~0x80; // Clear VBlank flag
            addr_latch = 0;  // Reset address/scroll latch
            return res;
        }
        case 4: return oam_ram[oam_addr];
        case 7: { // PPUDATA
            uint16_t p_addr = addr_reg & 0x3FFF;
            uint8_t data = data_buffer;
            
            if (p_addr < 0x2000) {
                data_buffer = mapper.read_chr(p_addr);
            } else if (p_addr < 0x3F00) {
                data_buffer = vram[get_mirrored_addr(p_addr, mapper.get_mirroring()) % 2048];
            } else {
                // Palette reads are immediate, but the buffer is filled with VRAM data underneath
                data = palette_ram[p_addr & 0x1F];
                data_buffer = vram[get_mirrored_addr(p_addr, mapper.get_mirroring()) % 2048];
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
            if (addr_latch == 0) { addr_reg = (uint16_t)((addr_reg & 0x00FF) | ((val & 0x3F) << 8)); addr_latch = 1; }
            else { addr_reg = (addr_reg & 0xFF00) | val; addr_latch = 0; }
            break;
        case 7: { // PPUDATA
            uint16_t p_addr = addr_reg & 0x3FFF;
            if (p_addr < 0x2000) {
                mapper.write_chr(p_addr, val);
            } else if (p_addr < 0x3F00) {
                vram[get_mirrored_addr(p_addr, mapper.get_mirroring()) % 2048] = val;
            } else {
                uint16_t p_idx = p_addr & 0x1F;
                // Palette Mirroring: 0x3F10/14/18/1C mirror to 0x3F00/04/08/0C
                if ((p_idx & 0x03) == 0 && (p_idx & 0x10)) p_idx &= ~0x10;
                palette_ram[p_idx] = val;
            }
            addr_reg += (ctrl & 0x04) ? 32 : 1;
            break;
        }
    }
}

/**
 * Basic Frame Rendering Logic
 * Dragon Warrior relies on accurate Background tile fetching from CHR-ROM.
 */
void PPU::render_frame(MapperMMC1& mapper, const uint32_t* nes_palette) {
    status |= 0x80; // Set VBlank
    if (!(mask & 0x18)) return; // Rendering disabled

    uint16_t pt_base = (ctrl & 0x10) ? 0x1000 : 0x0000;
    Mirroring mirroring = mapper.get_mirroring();

    for (int ty = 0; ty < 30; ty++) {
        for (int tx = 0; tx < 32; tx++) {
            // 1. Fetch Nametable Tile
            uint16_t nt_addr = 0x2000 + (ty * 32) + tx;
            uint8_t tile_idx = vram[get_mirrored_addr(nt_addr, mirroring) % 2048];

            // 2. Fetch Attribute Byte (Palette selection)
            uint16_t attr_addr = 0x23C0 + (ty / 4) * 8 + (tx / 4);
            uint8_t attr = vram[get_mirrored_addr(attr_addr, mirroring) % 2048];
            // Determine which 2 bits of the attribute byte to use
            uint8_t palette_idx = (attr >> (((ty & 2) ? 4 : 0) | ((tx & 2) ? 2 : 0))) & 0x03;

            // 3. Draw 8x8 Tile
            for (int y = 0; y < 8; y++) {
                uint8_t low = mapper.read_chr(pt_base + (tile_idx * 16) + y);
                uint8_t high = mapper.read_chr(pt_base + (tile_idx * 16) + y + 8);

                for (int x = 0; x < 8; x++) {
                    uint8_t pixel = ((low >> (7 - x)) & 0x01) | (((high >> (7 - x)) & 0x01) << 1);
                    int screen_x = tx * 8 + x;
                    int screen_y = ty * 8 + y;

                    if (screen_x < 256 && screen_y < 240) {
                        // Resolve color through palette ram
                        uint8_t color_final = palette_ram[(palette_idx << 2) | pixel];
                        screen_buffer[screen_y * 256 + screen_x] = nes_palette[color_final & 0x3F];
                    }
                }
            }
        }
    }
}
