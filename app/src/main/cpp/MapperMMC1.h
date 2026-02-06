#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>

/**
 * MMC1 Mapper (iNES Mapper 1)
 * Dragon Warrior I Specs: 
 * PRG-ROM: 64KB (4 banks of 16KB)
 * CHR-ROM: 16KB (4 banks of 4KB to support flexible switching)
 */
class MapperMMC1 {
public:
    // --- Memory Storage ---
    uint8_t prg_rom[4][16384];  // 64KB PRG
    uint8_t chr_rom[4][4096];   // 16KB CHR (allows for 4KB bank swapping)
    uint8_t prg_ram[8192];      // 8KB Save RAM (SRAM)

    // --- Internal State ---
    uint8_t shift_register;     // MMC1 uses a serial shift register for writes
    uint8_t write_count;        // Tracks the 5 bits needed for a full write
    
    // --- Configuration Registers ---
    uint8_t control;            // $8000: Mirroring, PRG/CHR bank modes
    uint8_t chr_bank_0;         // $A000: CHR Bank 0
    uint8_t chr_bank_1;         // $C000: CHR Bank 1
    uint8_t prg_bank;           // $E000: PRG Bank selection
    
    // --- PPU Sync State ---
    // This is read by PPU.cpp every frame to handle Nametable mirroring
    // 0: OneScreen Lower, 1: OneScreen Upper, 2: Vertical, 3: Horizontal
    uint8_t mirroring; 

    MapperMMC1();

    /**
     * Resets mapper to power-on state.
     * Sets PRG mode 3 (fixed last bank) and Vertical mirroring.
     */
    void reset();

    /**
     * CPU Write ($8000-$FFFF)
     * Handles the serial 5-bit write protocol to the shift register.
     */
    void write(uint16_t addr, uint8_t val);

    /**
     * PRG Read ($6000-$FFFF)
     * Includes Save RAM ($6000-$7FFF) and Banked ROM ($8000-$FFFF).
     */
    uint8_t read_prg(uint16_t addr);

    /**
     * CHR Read ($0000-$1FFF)
     * Provides 8KB of pattern table data to the PPU.
     */
    uint8_t read_chr(uint16_t addr);

    /**
     * CHR Write
     * Usually ignored for ROM, but used if the game utilizes CHR-RAM.
     */
    void write_chr(uint16_t addr, uint8_t val);

    /**
     * Helper to determine current PRG banking mode.
     * 0, 1: switch 32 KB at $8000
     * 2: fix first bank at $8000, switch 16 KB at $C000
     * 3: switch 16 KB at $8000, fix last bank at $C000
     */
    uint8_t get_prg_mode() const { return (control >> 2) & 0x03; }
    
    /**
     * Helper to determine CHR banking mode.
     * 0: switch 8 KB at a time; 1: switch two separate 4 KB banks
     */
    uint8_t get_chr_mode() const { return (control >> 4) & 0x01; }
};

#endif // MAPPERMMC1_H
