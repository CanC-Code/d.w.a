#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>

/**
 * MMC1 Mapper (iNES Mapper 1)
 * Optimized for Dragon Warrior I (64KB PRG / 16KB CHR)
 */
class MapperMMC1 {
public:
    // --- Memory Storage ---
    // Dragon Warrior 1 uses 4 banks of 16KB PRG and 4 banks of 4KB CHR.
    uint8_t prg_rom[4][16384];  
    uint8_t chr_rom[4][4096];   
    uint8_t prg_ram[8192];      

    // --- Internal MMC1 State ---
    uint8_t shift_register;     
    uint8_t control;            // Reg 0: $8000-$9FFF (Control)
    uint8_t chr_bank0;          // Reg 1: $A000-$BFFF (CHR Bank 0)
    uint8_t chr_bank1;          // Reg 2: $C000-$DFFF (CHR Bank 1)
    uint8_t prg_bank;           // Reg 3: $E000-$FFFF (PRG Bank)

    // Mirroring state: 0: OneScreen-Lower, 1: OneScreen-Upper, 2: Vertical, 3: Horizontal
    uint8_t mirroring; 

    MapperMMC1();

    /**
     * Resets the mapper to its power-on state.
     * MMC1 usually starts with the last PRG bank fixed at $C000.
     */
    void reset();

    /**
     * Handles writes to the mapper registers via the shift register.
     */
    void write(uint16_t addr, uint8_t val);

    /**
     * Redirects CPU memory reads ($8000-$FFFF) to the correct PRG-ROM bank.
     */
    uint8_t read_prg(uint16_t addr);

    /**
     * Redirects PPU memory reads ($0000-$1FFF) to the correct CHR-ROM bank.
     */
    uint8_t read_chr(uint16_t addr);

    /**
     * Handles PPU writes to the CHR address space ($0000-$1FFF).
     * Necessary for games using CHR-RAM or for mapper-controlled CHR access.
     */
    void write_chr(uint16_t addr, uint8_t val);

    // Helpers to interpret the control register
    uint8_t get_prg_mode() const { return (control >> 2) & 0x03; }
    uint8_t get_chr_mode() const { return (control >> 4) & 0x01; }
};

#endif // MAPPERMMC1_H
