#include <cstdint>
#include <cstring>
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "DWA_PPU", __VA_ARGS__)

// PPU Memory
static uint8_t vram[0x800];        // 2KB nametable RAM
static uint8_t palette_ram[32];    // Palette RAM
static uint8_t oam[256];           // Object Attribute Memory (sprites)

// PPU Registers
static uint8_t ppu_ctrl = 0;       // $2000
static uint8_t ppu_mask = 0;       // $2001
static uint8_t ppu_status = 0;     // $2002
static uint8_t oam_addr = 0;       // $2003
static uint16_t vram_addr = 0;     // Current VRAM address
static uint16_t temp_addr = 0;     // Temporary VRAM address
static uint8_t fine_x = 0;         // Fine X scroll
static bool write_toggle = false;  // First/second write toggle
static uint8_t ppu_data_buffer = 0;// Read buffer

// PPU State
static int scanline = 0;
static int dot = 0;
static uint64_t frame_count = 0;
static bool prev_vblank_status = false;  // NEW: Track VBlank edge

// External CHR-ROM access
extern uint8_t mapper_read_chr(uint16_t addr);
extern void mapper_write_chr(uint16_t addr, uint8_t value);

// NES Color Palette (RGB)
static const uint32_t nes_palette[64] = {
    0xFF545454, 0xFF001E74, 0xFF081090, 0xFF300088, 0xFF440064, 0xFF5C0030, 0xFF540400, 0xFF3C1800,
    0xFF202A00, 0xFF083A00, 0xFF004000, 0xFF003C00, 0xFF00323C, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFF989698, 0xFF084CC4, 0xFF3032EC, 0xFF5C1EE4, 0xFF8814B0, 0xFFA01464, 0xFF982220, 0xFF783C00,
    0xFF545A00, 0xFF287200, 0xFF087C00, 0xFF007628, 0xFF006678, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFECEEEC, 0xFF4C9AEC, 0xFF787CEC, 0xFFB062EC, 0xFFE454EC, 0xFFEC58B4, 0xFFEC6A64, 0xFFD48820,
    0xFFA0AA00, 0xFF74C400, 0xFF4CD020, 0xFF38CC6C, 0xFF38B4CC, 0xFF3C3C3C, 0xFF000000, 0xFF000000,
    0xFFECEEEC, 0xFFA8CCEC, 0xFFBCBCEC, 0xFFD4B2EC, 0xFFECAEEC, 0xFFECAED4, 0xFFECB4B0, 0xFFE4C490,
    0xFFCCD278, 0xFFB4DE78, 0xFFA8E290, 0xFF98E2B4, 0xFFA0D6E4, 0xFFA0A2A0, 0xFF000000, 0xFF000000
};

void ppu_init() {
    memset(vram, 0, sizeof(vram));
    memset(palette_ram, 0, sizeof(palette_ram));
    memset(oam, 0, sizeof(oam));
    LOGI("PPU initialized");
}

void ppu_reset() {
    ppu_ctrl = 0;
    ppu_mask = 0;
    ppu_status = 0xA0;  // VBlank flag set initially
    oam_addr = 0;
    vram_addr = 0;
    temp_addr = 0;
    fine_x = 0;
    write_toggle = false;
    ppu_data_buffer = 0;
    scanline = 241;  // Start in VBlank
    dot = 0;
    frame_count = 0;
    prev_vblank_status = true;
    LOGI("PPU reset");
}

// NEW: Check if PPU just entered VBlank (for NMI triggering)
bool ppu_in_vblank() {
    bool current_vblank = (ppu_status & 0x80) != 0;
    bool just_entered = current_vblank && !prev_vblank_status;
    prev_vblank_status = current_vblank;
    
    // Only trigger NMI if enabled in PPUCTRL
    return just_entered && (ppu_ctrl & 0x80);
}

// Read from VRAM
static uint8_t vram_read(uint16_t addr) {
    addr &= 0x3FFF;
    
    if (addr < 0x2000) {
        return mapper_read_chr(addr);
    } else if (addr < 0x3F00) {
        uint16_t mirror_addr = addr & 0x7FF;
        return vram[mirror_addr];
    } else {
        addr &= 0x1F;
        if (addr == 0x10) addr = 0x00;
        if (addr == 0x14) addr = 0x04;
        if (addr == 0x18) addr = 0x08;
        if (addr == 0x1C) addr = 0x0C;
        return palette_ram[addr];
    }
}

// Write to VRAM
static void vram_write(uint16_t addr, uint8_t value) {
    addr &= 0x3FFF;
    
    if (addr < 0x2000) {
        mapper_write_chr(addr, value);
    } else if (addr < 0x3F00) {
        uint16_t mirror_addr = addr & 0x7FF;
        vram[mirror_addr] = value;
    } else {
        addr &= 0x1F;
        if (addr == 0x10) addr = 0x00;
        if (addr == 0x14) addr = 0x04;
        if (addr == 0x18) addr = 0x08;
        if (addr == 0x1C) addr = 0x0C;
        palette_ram[addr] = value;
    }
}

// PPU Register Read
uint8_t ppu_read_register(uint16_t addr) {
    addr &= 0x2007;
    
    switch (addr) {
        case 0x2002:  // PPUSTATUS
        {
            uint8_t result = ppu_status;
            ppu_status &= 0x7F;  // Clear VBlank flag
            write_toggle = false;
            prev_vblank_status = false;  // Reset edge detection
            return result;
        }
        case 0x2004:  // OAMDATA
            return oam[oam_addr];
            
        case 0x2007:  // PPUDATA
        {
            uint8_t result = ppu_data_buffer;
            ppu_data_buffer = vram_read(vram_addr);
            
            // Palette reads are immediate
            if ((vram_addr & 0x3F00) == 0x3F00) {
                result = ppu_data_buffer;
            }
            
            vram_addr += (ppu_ctrl & 0x04) ? 32 : 1;
            vram_addr &= 0x3FFF;
            return result;
        }
        default:
            return 0;
    }
}

// PPU Register Write
void ppu_write_register(uint16_t addr, uint8_t value) {
    addr &= 0x2007;
    
    switch (addr) {
        case 0x2000:  // PPUCTRL
            ppu_ctrl = value;
            temp_addr = (temp_addr & 0xF3FF) | ((value & 0x03) << 10);
            break;
            
        case 0x2001:  // PPUMASK
            ppu_mask = value;
            break;
            
        case 0x2003:  // OAMADDR
            oam_addr = value;
            break;
            
        case 0x2004:  // OAMDATA
            oam[oam_addr++] = value;
            break;
            
        case 0x2005:  // PPUSCROLL
            if (!write_toggle) {
                temp_addr = (temp_addr & 0xFFE0) | (value >> 3);
                fine_x = value & 0x07;
                write_toggle = true;
            } else {
                temp_addr = (temp_addr & 0x8C1F) | ((value & 0x07) << 12) | ((value & 0xF8) << 2);
                write_toggle = false;
            }
            break;
            
        case 0x2006:  // PPUADDR
            if (!write_toggle) {
                temp_addr = (temp_addr & 0x00FF) | ((value & 0x3F) << 8);
                write_toggle = true;
            } else {
                temp_addr = (temp_addr & 0xFF00) | value;
                vram_addr = temp_addr;
                write_toggle = false;
            }
            break;
            
        case 0x2007:  // PPUDATA
            vram_write(vram_addr, value);
            vram_addr += (ppu_ctrl & 0x04) ? 32 : 1;
            vram_addr &= 0x3FFF;
            break;
    }
}

// Step PPU by one dot
void ppu_step() {
    dot++;
    
    if (dot >= 341) {
        dot = 0;
        scanline++;
        
        if (scanline >= 262) {
            scanline = 0;
            frame_count++;
        }
        
        // VBlank start (scanline 241)
        if (scanline == 241) {
            ppu_status |= 0x80;
        }
        
        // Pre-render scanline (261)
        if (scanline == 261) {
            ppu_status &= 0x7F;  // Clear VBlank
            ppu_status &= 0x5F;  // Clear sprite 0 hit and overflow
        }
    }
}

// Render a complete frame
void ppu_render_frame(uint32_t* pixels) {
    // Clear to backdrop color
    uint32_t backdrop = nes_palette[palette_ram[0] & 0x3F];
    for (int i = 0; i < 256 * 240; i++) {
        pixels[i] = backdrop;
    }
    
    // Render background if enabled
    if (ppu_mask & 0x08) {
        uint16_t nametable_base = 0x2000 | ((ppu_ctrl & 0x03) << 10);
        uint16_t pattern_base = (ppu_ctrl & 0x10) ? 0x1000 : 0x0000;
        
        for (int ty = 0; ty < 30; ty++) {
            for (int tx = 0; tx < 32; tx++) {
                uint16_t tile_addr = nametable_base | (ty << 5) | tx;
                uint8_t tile_index = vram_read(tile_addr);
                
                uint16_t attr_addr = nametable_base | 0x3C0 | ((ty >> 2) << 3) | (tx >> 2);
                uint8_t attr_byte = vram_read(attr_addr);
                
                int shift = ((ty & 2) << 1) | (tx & 2);
                uint8_t palette_num = (attr_byte >> shift) & 0x03;
                
                for (int py = 0; py < 8; py++) {
                    uint16_t tile_row = pattern_base | (tile_index << 4) | py;
                    uint8_t low_byte = vram_read(tile_row);
                    uint8_t high_byte = vram_read(tile_row + 8);
                    
                    for (int px = 0; px < 8; px++) {
                        int bit = 7 - px;
                        uint8_t pixel_val = ((low_byte >> bit) & 1) | (((high_byte >> bit) & 1) << 1);
                        
                        if (pixel_val != 0) {
                            uint8_t palette_index = palette_ram[(palette_num << 2) | pixel_val];
                            uint32_t color = nes_palette[palette_index & 0x3F];
                            
                            int screen_x = (tx << 3) | px;
                            int screen_y = (ty << 3) | py;
                            if (screen_x < 256 && screen_y < 240) {
                                pixels[screen_y * 256 + screen_x] = color;
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Render sprites if enabled
    if (ppu_mask & 0x10) {
        uint16_t sprite_pattern_base = (ppu_ctrl & 0x08) ? 0x1000 : 0x0000;
        
        for (int i = 0; i < 64; i++) {
            uint8_t sprite_y = oam[i * 4];
            uint8_t sprite_tile = oam[i * 4 + 1];
            uint8_t sprite_attr = oam[i * 4 + 2];
            uint8_t sprite_x = oam[i * 4 + 3];
            
            if (sprite_y >= 0xEF) continue;
            
            uint8_t palette_num = (sprite_attr & 0x03) + 4;
            bool flip_h = sprite_attr & 0x40;
            bool flip_v = sprite_attr & 0x80;
            
            for (int py = 0; py < 8; py++) {
                int actual_py = flip_v ? (7 - py) : py;
                uint16_t tile_row = sprite_pattern_base | (sprite_tile << 4) | actual_py;
                uint8_t low_byte = vram_read(tile_row);
                uint8_t high_byte = vram_read(tile_row + 8);
                
                for (int px = 0; px < 8; px++) {
                    int actual_px = flip_h ? px : (7 - px);
                    uint8_t pixel_val = ((low_byte >> actual_px) & 1) | (((high_byte >> actual_px) & 1) << 1);
                    
                    if (pixel_val != 0) {
                        uint8_t palette_index = palette_ram[(palette_num << 2) | pixel_val];
                        uint32_t color = nes_palette[palette_index & 0x3F];
                        
                        int screen_x = sprite_x + px;
                        int screen_y = sprite_y + py + 1;
                        if (screen_x < 256 && screen_y < 240) {
                            pixels[screen_y * 256 + screen_x] = color;
                        }
                    }
                }
            }
        }
    }
}