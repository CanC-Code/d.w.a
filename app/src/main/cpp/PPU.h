#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <vector>
#include "MapperMMC1.h"

/**
 * NES PPU (Picture Processing Unit)
 * Optimized for recompiled Dragon Warrior execution.
 */
class PPU {
public:
    // --- Memory Layout ---
    uint8_t vram[2048];                // Internal Name Tables (2KB)
    uint8_t palette_ram[32];           // Internal Palette Memory (Background + Sprites)
    uint8_t oam_ram[256];              // Object Attribute Memory (64 sprites)
    uint32_t screen_buffer[256 * 240]; // Final 32-bit RGBA output for Android Surface

    // --- Standard PPU Registers ($2000-$2007) ---
    uint8_t ctrl;     // $2000: Control
    uint8_t mask;     // $2001: Mask
    uint8_t status;   // $2002: Status
    uint8_t oam_addr; // $2003: OAM Address

    // --- Loopy's Internal Registers (Essential for Scrolling) ---
    // The NES doesn't just use a simple X/Y scroll; it uses a complex
    // internal address system to track where it is in a frame.
    union {
        struct {
            uint16_t coarse_x : 5;
            uint16_t coarse_y : 5;
            uint16_t nametable_x : 1;
            uint16_t nametable_y : 1;
            uint16_t fine_y : 3;
            uint16_t unused : 1;
        };
        uint16_t raw;
    } v_addr, t_addr; // v: Current VRAM address, t: Temporary VRAM address

    uint8_t fine_x;     // Fine X scroll (3 bits)
    uint8_t addr_latch; // 1-bit write toggle (w)

    // --- State & Interrupts ---
    uint8_t data_buffer;  // Delayed read buffer for $2007
    bool nmi_pending;     // Trigger for CPU NMI vector
    int scanline;         // Current rendering line (0-261)
    int cycle;            // Current pixel clock (0-340)

    PPU();
    void reset();

    // --- Bus Communication ---
    uint8_t cpu_read(uint16_t addr, MapperMMC1& mapper);
    void cpu_write(uint16_t addr, uint8_t val, MapperMMC1& mapper);

    // --- OAM DMA ---
    /**
     * Rapidly fills OAM memory from CPU RAM ($4014).
     * This is how the game moves the Hero and NPCs every frame.
     */
    void do_dma(const uint8_t* cpu_ram_page) {
        std::memcpy(oam_ram, cpu_ram_page, 256);
    }

    // --- Rendering Pipeline ---
    /**
     * Renders the frame according to NES hardware timings.
     * Integrates with the Android Choreographer via the JNI layer.
     */
    void render_frame(MapperMMC1& mapper, const uint32_t* nes_palette);

    /**
     * Signals the end of the visible frame.
     * Dragon Warrior uses this to trigger the 'MainLoop' for game logic.
     */
    void trigger_vblank() { 
        status |= 0x80; 
        if (ctrl & 0x80) nmi_pending = true; 
    }

private:
    /**
     * Handles hardware-level mirroring.
     * Maps $2000-$3EFF to the physical 2KB VRAM array.
     */
    uint16_t get_mirrored_addr(uint16_t addr, Mirroring mode);

    /**
     * Internal helper to fetch tile data from the Mapper.
     */
    uint8_t fetch_tile_pixel(MapperMMC1& mapper, uint16_t pt_base, uint8_t tile_id, int x, int y);
};

#endif // PPU_H
