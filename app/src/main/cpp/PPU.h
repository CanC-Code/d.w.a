#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <vector>
#include "MapperMMC1.h"

class PPU {
public:
    // Memory Layout
    uint8_t vram[2048];       // Internal Name Tables
    uint8_t palette_ram[32];  // Internal Palette Memory
    uint8_t oam_ram[256];     // Object Attribute Memory (Sprites)
    uint32_t screen_buffer[256 * 240]; // Final RGBA pixels

    // Standard PPU Registers
    uint8_t ctrl;    // $2000: Control (NMI, Master/Slave, Sprite Size, etc.)
    uint8_t mask;    // $2001: Mask (Color effects, show/hide BG/Sprites)
    uint8_t status;  // $2002: Status (VBlank, Sprite 0 Hit, Overflow)
    uint8_t oam_addr; // $2003: OAM Address

    // Internal Communication State
    uint16_t addr_reg;    // Internal address register (Loopy V)
    uint8_t addr_latch;   // Toggle for high/low byte writes to $2006
    uint8_t data_buffer;  // Delayed read buffer for $2007

    // NMI Signaling
    bool nmi_occurred;    // Flag to trigger CPU interrupt
    bool nmi_previous;    // Used to detect rising edge of NMI

    PPU();
    void reset();

    // CPU Interface
    uint8_t cpu_read(uint16_t addr, MapperMMC1& mapper);
    void cpu_write(uint16_t addr, uint8_t val, MapperMMC1& mapper);

    // Frame Generation
    void render_frame(MapperMMC1& mapper, const uint32_t* nes_palette);
    
    // Interrupt Sync
    void set_vblank(bool is_vblank) {
        if (is_vblank) {
            status |= 0x80; // Set VBlank flag
            if (ctrl & 0x80) nmi_occurred = true; // Trigger NMI if enabled
        } else {
            status &= ~0x80;
            nmi_occurred = false;
        }
    }

private:
    uint16_t get_mirrored_addr(uint16_t addr);
};

#endif
