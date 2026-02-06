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
    uint16_t mirrored = (addr - 0x2000) % 0x1000;
    // Vertical Mirroring: 
    // $2000-$23FF (NT0) and $2800-$2BFF (NT2) are VRAM 0
    // $2400-$27FF (NT1) and $2C00-$2FFF (NT3) are VRAM 1
    if (mirrored >= 0x0000 && mirrored < 0x0400) return mirrored;      // NT0
    if (mirrored >= 0x0400 && mirrored < 0x0800) return mirrored;      // NT1
    if (mirrored >= 0x0800 && mirrored < 0x0C00) return mirrored - 0x0800; // NT2 -> NT0
    return mirrored - 0x0800; // NT3 -> NT1
}

uint8_t PPU::cpu_read(uint16_t addr, MapperMMC1& mapper) {
    uint8_t reg = addr % 8;
    if (reg == 2) { // PPUSTATUS
        uint8_t res = (status & 0xE0) | (data_buffer & 0x1F);
        status &= ~0x80; // Clear VBlank flag
        addr_latch = 0;
        return res;
    }
    if (reg == 7) { // PPUDATA
        uint8_t data = data_buffer;
        uint16_t p_addr = addr_reg & 0x3FFF;
        
        if (p_addr < 0x2000) data_buffer = mapper.read_chr(p_addr);
        else if (p_addr < 0x3F00) data_buffer = vram[get_mirrored_addr(p_addr)];
        else {
            // Palette reads are immediate, but buffer gets filled with VRAM "under" it
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
            // Some MMC1 boards allow CHR-RAM writing. 
            // In DW1 it is ROM, but adding this for stability.
            mapper.chr_rom[p_addr / 4096][p_addr % 4096] = val;
        }
        else if (p_addr < 0x3F00) {
            vram[get_mirrored_addr(p_addr)] = val;
        }
        else {
            uint8_t p_idx = p_addr & 0x1F;
            // Mirroring for $3F10, $3F14, $3F18, $3F1C to background colors
            if (p_idx >= 0x10 && (p_idx % 4 == 0)) p_idx -= 0x10;
            palette_ram[p_idx] = val;
        }
        addr_reg += (ctrl & 0x04) ? 32 : 1;
    }
}

void PPU::render_frame(MapperMMC1& mapper, const uint32_t* nes_palette) {
    // If rendering is disabled in PPUMASK, just fill with background color
    if (!(mask & 0x08)) {
        uint32_t bg_color = nes_palette[palette_ram[0] & 0x3F];
        for(int i=0; i<256*240; i++) screen_buffer[i] = bg_color;
        return;
    }

    uint16_t bg_pat_base = (ctrl & 0x10) ? 0x1000 : 0x0000;

    for (int ty = 0; ty < 30; ty++) {
        for (int tx = 0; tx < 32; tx++) {
            uint16_t nt_addr = 0x2000 + (ty * 32) + tx;
            uint8_t tile = vram[get_mirrored_addr(nt_addr)];

            // Attribute Table lookup (simplified for speed)
            // This determines which palette (0-3) the 16x16 area uses
            uint16_t attr_addr = 0x23C0 + (ty / 4) * 8 + (tx / 4);
            uint8_t attr_byte = vram[get_mirrored_addr(attr_addr)];
            uint8_t palette_shift = ((ty & 2) << 1) | (tx & 2);
            uint8_t palette_id = (attr_byte >> palette_shift) & 0x03;

            for (int y = 0; y < 8; y++) {
                uint8_t p1 = mapper.read_chr(bg_pat_base + tile * 16 + y);
                uint8_t p2 = mapper.read_chr(bg_pat_base + tile * 16 + y + 8);
                
                for (int x = 0; x < 8; x++) {
                    uint8_t color_bits = ((p1 >> (7 - x)) & 0x01) | (((p2 >> (7 - x)) & 0x01) << 1);
                    
                    uint32_t final_pixel;
                    if (color_bits == 0) {
                        final_pixel = nes_palette[palette_ram[0] & 0x3F]; // Universal Background
                    } else {
                        // Palette RAM is organized as: 
                        // 0x00-0x03 (Palette 0), 0x04-0x07 (Palette 1)...
                        uint8_t p_idx = palette_ram[(palette_id << 2) + color_bits];
                        final_pixel = nes_palette[p_idx & 0x3F];
                    }
                    screen_buffer[(ty * 8 + y) * 256 + (tx * 8 + x)] = final_pixel;
                }
            }
        }
    }
}
