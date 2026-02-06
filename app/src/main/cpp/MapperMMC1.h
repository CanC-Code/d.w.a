#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>

class MapperMMC1 {
public:
    // Dragon Warrior I: 64KB PRG (4x16KB) and 16KB CHR
    // Memory arrays used by the loader and the bus
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
    bool ram_enabled; // Added to fix "use of undeclared identifier" error

    // Constructor: Declaration only
    MapperMMC1();

    // Hardware Interface: Declarations only
    // Logic is moved to MapperMMC1.cpp to prevent redefinition errors
    void reset();
    void write(uint16_t addr, uint8_t val);
    uint8_t read_prg(uint16_t addr);
    uint8_t read_chr(uint16_t addr);
};

#endif // MAPPERMMC1_H
