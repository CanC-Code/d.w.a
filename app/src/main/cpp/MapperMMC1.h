#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>

class MapperMMC1 {
public:
    uint8_t prg_rom[4][16384]; // 4 banks of 16KB
    uint8_t chr_rom[4][4096];  // 4 banks of 4KB (16KB total)

    uint8_t shift_register = 0x10;
    uint8_t control = 0x0C;
    uint8_t chr_bank_0 = 0;
    uint8_t chr_bank_1 = 0;
    uint8_t prg_bank = 0;
    uint8_t mirroring = 0;

    void write(uint16_t addr, uint8_t val) {
        if (val & 0x80) {
            shift_register = 0x10;
            control |= 0x0C;
        } else {
            bool complete = (shift_register & 0x10); // Check if bit 4 is set to see if 5 bits are loaded
            bool bit_to_shift = (val & 0x01);
            
            if (val & 0x01) {
                // Correct logic for shift register completion
            }
            // Standard MMC1 Shift logic:
            if (shift_register & 0x01) {
                uint8_t data = ((shift_register >> 1) | ((val & 0x01) << 4)) & 0x1F;
                if (addr >= 0x8000 && addr <= 0x9FFF) {
                    control = data;
                    mirroring = data & 0x03;
                } else if (addr >= 0xA000 && addr <= 0xBFFF) {
                    chr_bank_0 = data;
                } else if (addr >= 0xC000 && addr <= 0xDFFF) {
                    chr_bank_1 = data;
                } else if (addr >= 0xE000) {
                    prg_bank = data & 0x0F;
                }
                shift_register = 0x10;
            } else {
                shift_register >>= 1;
                shift_register |= ((val & 0x01) << 4);
            }
        }
    }

    uint8_t read_prg(uint16_t addr) {
        // PRG Bank Mode (Control bits 2-3)
        uint8_t mode = (control >> 2) & 0x03;
        if (mode <= 1) {
            // 32KB Mode
            return prg_rom[(prg_bank & 0x0E) % 4 + ((addr >> 14) & 1)][addr & 0x3FFF];
        } else if (mode == 2) {
            // Fix first bank at $8000, switch second bank at $C000
            if (addr < 0xC000) return prg_rom[0][addr - 0x8000];
            return prg_rom[prg_bank % 4][addr - 0xC000];
        } else {
            // Fix last bank at $C000, switch first bank at $8000
            if (addr < 0xC000) return prg_rom[prg_bank % 4][addr - 0x8000];
            return prg_rom[3][addr - 0xC000];
        }
    }

    uint8_t read_chr(uint16_t addr) {
        // CHR Bank Mode (Control bit 4)
        if (control & 0x10) {
            // 4KB Mode: Independent banks
            if (addr < 0x1000) return chr_rom[chr_bank_0 % 4][addr];
            return chr_rom[chr_bank_1 % 4][addr - 0x1000];
        } else {
            // 8KB Mode: Both banks switched together
            // When in 8KB mode, the low bit of chr_bank_0 is ignored
            uint8_t bank = (chr_bank_0 & 0x1E); 
            if (addr < 0x1000) return chr_rom[bank % 4][addr];
            return chr_rom[(bank + 1) % 4][addr - 0x1000];
        }
    }
};

#endif
