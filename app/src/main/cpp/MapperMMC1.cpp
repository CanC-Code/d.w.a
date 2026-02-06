#include "MapperMMC1.h"
#include <cstring>

void MapperMMC1::reset() {
    shift_reg = 0x10;
    control = 0x0C;
    prg_bank = 0;
    chr_bank_0 = 0;
    chr_bank_1 = 0;
}

void MapperMMC1::write(uint16_t addr, uint8_t val) {
    if (val & 0x80) {
        shift_reg = 0x10;
        control |= 0x0C;
    } else {
        bool last_bit = (shift_reg & 0x01);
        shift_reg >>= 1;
        shift_reg |= ((val & 0x01) << 4);

        if (last_bit) {
            uint8_t register_num = (addr >> 13) & 0x03;
            if (register_num == 0) control = shift_reg;
            else if (register_num == 1) chr_bank_0 = shift_reg;
            else if (register_num == 2) chr_bank_1 = shift_reg;
            else if (register_num == 3) prg_bank = shift_reg & 0x0F;
            shift_reg = 0x10;
        }
    }
}

uint8_t MapperMMC1::read_prg(uint16_t addr) {
    // MMC1 logic for 16KB bank switching
    int bank = (addr < 0xC000) ? (prg_bank & 0xFE) : 3;
    return prg_rom[bank][addr % 16384];
}

uint8_t MapperMMC1::read_chr(uint16_t addr) {
    int bank = (addr < 0x1000) ? chr_bank_0 : chr_bank_1;
    return chr_rom[bank % 2][addr % 4096];
}

uint16_t MapperMMC1::get_current_bank() {
    return prg_bank;
}
