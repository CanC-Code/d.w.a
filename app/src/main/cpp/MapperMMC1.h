#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>
#include <cstring>

/**
 * MMC1 Mapper (iNES Mapper 1)
 * Dragon Warrior I Specs: 
 * PRG-ROM: 64KB (4 banks of 16KB)
 * CHR-ROM: 16KB (4 banks of 4KB)
 */
class MapperMMC1 {
public:
    // --- Memory Storage ---
    // Increased to 8 banks for safety, though DW1 uses 4.
    uint8_t prg_rom[8][16384];  
    uint8_t chr_rom[8][4096];   
    uint8_t prg_ram[8192];      

    // --- Internal State ---
    uint8_t shift_register;     
    uint8_t write_count;        

    // --- Configuration Registers ---
    uint8_t control;            // $8000
    uint8_t chr_bank_0;         // $A000
    uint8_t chr_bank_1;         // $C000
    uint8_t prg_bank;           // $E000

    uint8_t mirroring; 

    MapperMMC1();
    void reset();
    void write(uint16_t addr, uint8_t val);
    uint8_t read_prg(uint16_t addr);
    uint8_t read_chr(uint16_t addr);
    void write_chr(uint16_t addr, uint8_t val);

    uint8_t get_prg_mode() const { return (control >> 2) & 0x03; }
    uint8_t get_chr_mode() const { return (control >> 4) & 0x01; }
};

#endif
