#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>

class MapperMMC1 {
public:
    uint8_t prg_rom[4][16384];  
    uint8_t chr_rom[4][4096];   
    uint8_t prg_ram[8192];      

    uint8_t shift_register;     
    uint8_t control;            
    uint8_t chr_bank0;          
    uint8_t chr_bank1;          
    uint8_t prg_bank;           
    uint8_t mirroring; 

    MapperMMC1();
    void reset();
    
    // This is the method the CPU calls to interact with the mapper
    void write(uint16_t addr, uint8_t val);

    uint8_t read_prg(uint16_t addr);
    uint8_t read_chr(uint16_t addr);
    void write_chr(uint16_t addr, uint8_t val);

    uint8_t get_prg_mode() const { return (control >> 2) & 0x03; }
    uint8_t get_chr_mode() const { return (control >> 4) & 0x01; }
};

#endif
