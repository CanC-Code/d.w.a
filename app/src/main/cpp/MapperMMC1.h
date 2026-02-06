#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>

/**
 * MMC1 Mapper (iNES Mapper 1)
 * Dragon Warrior I Specs: 
 * PRG-ROM: 64KB (4 banks of 16KB)
 * CHR-ROM: 8KB (2 banks of 4KB)
 */
class MapperMMC1 {
public:
    // Memory Storage
    uint8_t prg_rom[4][16384]; 
    uint8_t chr_rom[2][4096]; // Standard DW1 has 8KB CHR
    uint8_t prg_ram[8192]; 

    // Internal Shift Register
    uint8_t shift_register;
    uint8_t write_count; 

    // Configuration Registers
    uint8_t control;    // $8000-$9FFF
    uint8_t chr_bank_0; // $A000-$BFFF
    uint8_t chr_bank_1; // $C000-$DFFF
    uint8_t prg_bank;   // $E000-$FFFF
    bool ram_enabled;

    MapperMMC1();

    // Reset to power-on state
    void reset();

    // CPU interface for bank switching ($8000-$FFFF)
    void write(uint16_t addr, uint8_t val);

    // CPU read access ($6000-$FFFF)
    uint8_t read_prg(uint16_t addr);

    // PPU read access ($0000-$1FFF)
    uint8_t read_chr(uint16_t addr);

    // PPU write access (typically ignored for CHR-ROM)
    void write_chr(uint16_t addr, uint8_t val);

    /**
     * Mirroring Mode Helper
     * 0: One-Screen (Lower)
     * 1: One-Screen (Upper)
     * 2: Vertical (Dragon Warrior Default)
     * 3: Horizontal
     */
    uint8_t get_mirroring_mode() const {
        return control & 0x03;
    }
};

#endif // MAPPERMMC1_H
