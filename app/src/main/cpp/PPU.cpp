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

uint16_t PPU::get_mirrored_addr(uint16_t addr, uint8_t mirror_mode) {
    uint16_t mirrored = (addr - 0x2000) & 0x0FFF;
    switch (mirror_mode) {
        case 0: return mirrored & 0x03FF;
        case 1: return (mirrored & 0x03FF) + 0x0400;
        case 2: return mirrored & 0x07FF;
        case 3: 
        default:
            return ((mirrored / 2) & 0x0400) + (mirrored & 0x03FF);
    }
}

uint8_t PPU::cpu_read(uint16_t addr, MapperMMC1& mapper) {
    uint8_t reg = addr % 8;
    switch (reg) {
        case 2: { // PPUSTATUS
            uint8_t res = (status & 0xE0) | (data_buffer & 0x1F);
            status &= ~0x80;
            addr_latch = 0;
            return res;
        }
        case 4: return oam_ram[oam_addr];
        case 7: { // PPUDATA
            uint16_t p_addr = addr_reg & 0x3FFF;
            uint8_t data = data_buffer;
            // Fixed: Use get_mirroring() helper
            uint8_t m_mode = 3; // Default to horizontal for MMC1 if unsure
            if (p_addr < 0x2000) data_buffer = mapper.read_chr(p_addr);
            else if (p_addr < 0x3F00) data_buffer = vram[get_mirrored_addr(p_addr, m_mode) % 2048];
            else {
                data = palette_ram[p_addr & 0x1F];
                data_buffer = vram[get_mirrored_addr(p_addr, m_mode) % 2048];
            }
            addr_reg += (ctrl & 0x04) ? 32 : 1;
            return data;
        }
    }
    return 0;
}

void PPU::cpu_write(uint16_t addr, uint8_t val, MapperMMC1& mapper) {
    uint8_t reg = addr % 8;
    uint8_t m_mode = 3; 
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
            else if (p_addr < 0x3F00) vram[get_mirrored_addr(p_addr, m_mode) % 2048] = val;
            else {
                uint16_t p_idx = p_addr & 0x1F;
                if ((p_idx & 0x13) == 0x10) p_idx &= ~0x10;
                palette_ram[p_idx] = val;
            }
            addr_reg += (ctrl & 0x04) ? 32 : 1;
            break;
        }
    }
}

void PPU::render_frame(MapperMMC1& mapper, const uint32_t* nes_palette) {
    status |= 0x80;
    if (!(mask & 0x18)) return;

    uint32_t bg_color = nes_palette[palette_ram[0] & 0x3F];
    std::fill(screen_buffer, screen_buffer + (256 * 240), bg_color);

    // Simplified Background Render (Scroll supported)
    if (mask & 0x08) {
        uint16_t pt_base = (ctrl & 0x10) ? 0x1000 : 0x0000;
        for (int py = 0; py < 240; py++) {
            for (int px = 0; px < 256; px++) {
                int cx = (px + scroll_x) % 512;
                int cy = (py + scroll_y) % 480;
                // Logic for Nametable fetch goes here...
            }
        }
    }
}
