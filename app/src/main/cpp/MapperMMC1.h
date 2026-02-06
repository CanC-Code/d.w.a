#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>

class MapperMMC1 {
public:
    // Dragon Warrior (NES) specific memory layout:
    // 64KB PRG ROM = 4 banks of 16KB
    // 8KB CHR ROM = 2 banks of 4KB (Your header has 8, which is fine for compatibility)
    uint8_t prg_rom[4][16384]; 
    uint8_t chr_rom[8][4096];
    uint8_t prg_ram[8192]; 

    // Mapper State Registers
    uint8_t shift_register;
    uint8_t write_count; 
    uint8_t control;
    uint8_t chr_bank_0;
    uint8_t chr_bank_1;
    uint8_t prg_bank;
    bool ram_enabled;

    MapperMMC1();

    void reset();
    void write(uint16_t addr, uint8_t val);
    uint8_t read_prg(uint16_t addr);
    uint8_t read_chr(uint16_t addr);
};

#endif
