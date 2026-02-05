#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>

class MapperMMC1 {
public:
    // Dragon Warrior uses 4 PRG banks (64KB) and 4 CHR banks (16KB)
    uint8_t prg_rom[4][16384]; 
    uint8_t chr_rom[4][4096];  
    
    uint8_t shift_register = 0x10;
    uint8_t control = 0x0C; // Default: PRG 16KB swap at $8000, fixed at $C000
    uint8_t chr_bank_0 = 0;
    uint8_t chr_bank_1 = 0;
    uint8_t prg_bank = 0;

    void write(uint16_t addr, uint8_t val) {
        // Bit 7 reset: Resets shift register and sets PRG mode to 3
        if (val & 0x80) {
            shift_register = 0x10;
            control |= 0x0C;
        } else {
            bool complete = (shift_register & 0x01);
            shift_register >>= 1;
            shift_register |= ((val & 0x01) << 4);

            if (complete) {
                uint8_t data = shift_register & 0x1F;
                if (addr >= 0x8000 && addr <= 0x9FFF) {
                    control = data;
                } else if (addr >= 0xA000 && addr <= 0xBFFF) {
                    chr_bank_0 = data;
                } else if (addr >= 0xC000 && addr <= 0xDFFF) {
                    chr_bank_1 = data;
                } else if (addr >= 0xE000) {
                    prg_bank = data & 0x0F;
                }
                shift_register = 0x10;
            }
        }
    }

    uint8_t read_prg(uint16_t addr) {
        uint8_t mode = (control >> 2) & 0x03;
        // Mode 0, 1: 32KB at $8000
        if (mode <= 1) {
            uint8_t bank = (prg_bank & 0x0E);
            return prg_rom[bank % 4][addr - 0x8000];
        }
        // Mode 2: Fix first bank at $8000, switch second bank at $C000
        if (mode == 2) {
            if (addr < 0xC000) return prg_rom[0][addr - 0x8000];
            return prg_rom[prg_bank % 4][addr - 0xC000];
        }
        // Mode 3: Fix last bank at $C000, switch first bank at $8000
        if (addr < 0xC000) return prg_rom[prg_bank % 4][addr - 0x8000];
        return prg_rom[3][addr - 0xC000];
    }

    uint8_t read_chr(uint16_t addr) {
        // CHR Mode (Bit 4 of Control): 0 = 8KB mode, 1 = 4KB mode
        if (control & 0x10) {
            if (addr < 0x1000) return chr_rom[chr_bank_0 % 4][addr];
            return chr_rom[chr_bank_1 % 4][addr - 0x1000];
        } else {
            uint8_t bank = (chr_bank_0 & 0x1E);
            if (addr < 0x1000) return chr_rom[bank % 4][addr];
            return chr_rom[(bank + 1) % 4][addr - 0x1000];
        }
    }
};

#endif
