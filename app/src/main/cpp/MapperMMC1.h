#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>

class MapperMMC1 {
public:
    // Dragon Warrior I: 64KB PRG (4x16KB) and 16KB CHR (4x4KB)
    uint8_t prg_rom[4][16384]; 
    uint8_t chr_rom[4][4096]; // Corrected to 4 banks for 16KB CHR
    uint8_t prg_ram[8192]; 

    // Mapper State Registers
    uint8_t shift_register;
    uint8_t write_count; 
    uint8_t control;
    uint8_t chr_bank_0;
    uint8_t chr_bank_1;
    uint8_t prg_bank;
    bool ram_enabled;

    // Constructor
    MapperMMC1();

    // Hardware Interface
    void reset();
    
    // CPU writes to $8000-$FFFF to control the mapper
    void write(uint16_t addr, uint8_t val);
    
    // CPU reads from $8000-$FFFF
    uint8_t read_prg(uint16_t addr);
    
    // PPU reads from $0000-$1FFF
    uint8_t read_chr(uint16_t addr);

    // PPU writes to $0000-$1FFF (Required by PPU.cpp for stability)
    void write_chr(uint16_t addr, uint8_t val);
};

#endif // MAPPERMMC1_H
