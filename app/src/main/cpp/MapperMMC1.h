#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>

class MapperMMC1 {
public:
    [span_6](start_span)// Dragon Warrior uses 4 PRG banks (64KB) and 4 CHR banks (16KB)[span_6](end_span)
    uint8_t prg_rom[4][16384]; 
    uint8_t chr_rom[4][4096];  
    
    uint8_t shift_register = 0x10;
    uint8_t control = 0x0C; [span_7](start_span)// Default: PRG 16KB swap at $8000, fixed at $C000[span_7](end_span)
    uint8_t chr_bank_0 = 0;
    uint8_t chr_bank_1 = 0;
    uint8_t prg_bank = 0;

    void write(uint16_t addr, uint8_t val) {
        [span_8](start_span)// Bit 7 reset: Resets shift register and sets PRG mode to 3[span_8](end_span)
        if (val & 0x80) {
            shift_register = 0x10;
            control |= 0x0C;
        } else {
            bool complete = (shift_register & 0x01);
            shift_register >>= 1;
            shift_register |= ((val & 0x01) << 4);

            if (complete) {
                uint8_t data = shift_register & 0x1F;
                [span_9](start_span)// MMC1 Internal Register Mapping[span_9](end_span)
                if (addr >= 0x8000 && addr <= 0x9FFF) {
                    control = data;
                } else if (addr >= 0xA000 && addr <= 0xBFFF) {
                    chr_bank_0 = data;
                } else if (addr >= 0xC000 && addr <= 0xDFFF) {
                    chr_bank_1 = data;
                } else if (addr >= 0xE000) {
                    [span_10](start_span)// PRG bank selection (only bottom 4 bits used)[span_10](end_span)
                    prg_bank = data & 0x0F;
                }
                shift_register = 0x10;
            }
        }
    }

    uint8_t read_prg(uint16_t addr) {
        uint8_t mode = (control >> 2) & 0x03;
        [span_11](start_span)// Mode 0, 1: 32KB at $8000[span_11](end_span)
        if (mode <= 1) {
            uint8_t bank = (prg_bank & 0x0E);
            return prg_rom[bank % 4][addr - 0x8000];
        }
        [span_12](start_span)// Mode 2: Fix first bank at $8000, switch second bank at $C000[span_12](end_span)
        if (mode == 2) {
            if (addr < 0xC000) return prg_rom[0][addr - 0x8000];
            return prg_rom[prg_bank % 4][addr - 0xC000];
        }
        [span_13](start_span)// Mode 3: Fix last bank at $C000, switch first bank at $8000[span_13](end_span)
        if (addr < 0xC000) return prg_rom[prg_bank % 4][addr - 0x8000];
        return prg_rom[3][addr - 0xC000];
    }

    uint8_t read_chr(uint16_t addr) {
        [span_14](start_span)// CHR Mode (Bit 4 of Control): 0 = 8KB mode, 1 = 4KB mode[span_14](end_span)
        if (control & 0x10) {
            [span_15](start_span)// 4KB independent banking[span_15](end_span)
            if (addr < 0x1000) return chr_rom[chr_bank_0 % 4][addr];
            return chr_rom[chr_bank_1 % 4][addr - 0x1000];
        } else {
            [span_16](start_span)// 8KB switched together[span_16](end_span)
            uint8_t bank = (chr_bank_0 & 0x1E);
            if (addr < 0x1000) return chr_rom[bank % 4][addr];
            return chr_rom[(bank + 1) % 4][addr - 0x1000];
        }
    }
};

#endif
