#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>

class MapperMMC1 {
public:
    uint8_t prg_rom[4][16384]; 
    uint8_t chr_rom[4][4096];  
    uint8_t shift_register = 0x10, control = 0x0C, chr_bank_0 = 0, chr_bank_1 = 0, prg_bank = 0;

    void write(uint16_t addr, uint8_t val) {
        if (val & 0x80) {
            shift_register = 0x10;
            control |= 0x0C;
        } else {
            bool complete = (shift_register & 0x01);
            shift_register >>= 1;
            shift_register |= ((val & 0x01) << 4);
            if (complete) {
                uint8_t data = shift_register & 0x1F;
                if (addr >= 0x8000 && addr <= 0x9FFF) control = data;
                else if (addr >= 0xA000 && addr <= 0xBFFF) chr_bank_0 = data;
                else if (addr >= 0xC000 && addr <= 0xDFFF) chr_bank_1 = data;
                else if (addr >= 0xE000) prg_bank = data & 0x0F;
                shift_register = 0x10;
            }
        }
    }

    uint8_t read_prg(uint16_t addr) {
        uint8_t mode = (control >> 2) & 0x03;
        if (mode <= 1) return prg_rom[(prg_bank & 0x0E) + ((addr >> 14) & 1)][addr & 0x3FFF];
        if (mode == 2) return (addr < 0xC000) ? prg_rom[0][addr - 0x8000] : prg_rom[prg_bank % 4][addr - 0xC000];
        return (addr < 0xC000) ? prg_rom[prg_bank % 4][addr - 0x8000] : prg_rom[3][addr - 0xC000];
    }

    uint8_t read_chr(uint16_t addr) {
        if (control & 0x10) return (addr < 0x1000) ? chr_rom[chr_bank_0 % 4][addr] : chr_rom[chr_bank_1 % 4][addr - 0x1000];
        uint8_t bank = (chr_bank_0 & 0x1E);
        return (addr < 0x1000) ? chr_rom[bank % 4][addr] : chr_rom[(bank + 1) % 4][addr - 0x1000];
    }
};

#endif
