#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>

class MapperMMC1 {
public:
    // Dragon Warrior (NES) usually uses 64KB PRG (4 banks of 16KB)
    // and 16KB-32KB CHR (4-8 banks of 4KB)
    uint8_t prg_rom[4][16384]; 
    uint8_t chr_rom[8][4096];  

    uint8_t shift_register = 0x10;
    uint8_t control = 0x0C; // Initial: PRG mode 3 (fixed last bank at $C000)
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
                    // PRG Bank: Lower 4 bits select bank. Bit 4 (0x10) often used for 256KB+
                    prg_bank = data & 0x0F;
                }
                shift_register = 0x10;
            }
        }
    }

    uint8_t read_prg(uint16_t addr) {
        uint8_t mode = (control >> 2) & 0x03;
        uint16_t offset = addr & 0x3FFF;

        // Mode 0, 1: Switch 32KB at $8000, ignoring low bit of bank number
        if (mode <= 1) {
            uint8_t bank = (prg_bank & 0x0E);
            if (addr < 0xC000) return prg_rom[bank % 4][offset];
            return prg_rom[(bank + 1) % 4][offset];
        }

        // Mode 2: Fix first bank at $8000, switch second bank at $C000
        if (mode == 2) {
            if (addr < 0xC000) return prg_rom[0][offset];
            return prg_rom[prg_bank % 4][offset];
        }

        // Mode 3: Switch first bank at $8000, fix last bank (bank 3) at $C000
        // This is the most common mode for Dragon Warrior
        if (addr < 0xC000) return prg_rom[prg_bank % 4][offset];
        return prg_rom[3][offset];
    }

    uint8_t read_chr(uint16_t addr) {
        uint16_t offset = addr & 0x0FFF;
        // CHR Mode (Bit 4 of Control): 0 = 8KB mode, 1 = 4KB mode
        if (control & 0x10) {
            // 4KB Bank Switching
            if (addr < 0x1000) return chr_rom[chr_bank_0 % 8][offset];
            return chr_rom[chr_bank_1 % 8][offset];
        } else {
            // 8KB Bank Switching (low bit ignored)
            uint8_t bank = (chr_bank_0 & 0x1E);
            if (addr < 0x1000) return chr_rom[bank % 8][offset];
            return chr_rom[(bank + 1) % 8][offset];
        }
    }
};

#endif
