#include "MapperMMC1.h"
#include <cstring>
#include "recompiled/cpu_shared.h"

MapperMMC1::MapperMMC1() {
    reset();
}

void MapperMMC1::reset() {
    shift_register = 0x10;
    control = 0x0C; // Fixed last bank at $C000, 16KB PRG switching
    prg_bank = 0;
    chr_bank0 = 0;
    chr_bank1 = 0;
    mirroring = 0;
}

uint8_t MapperMMC1::read_prg(uint16_t addr) {
    // PRG RAM Range
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        return prg_ram[addr - 0x6000];
    }

    // PRG ROM Range
    if (addr >= 0x8000) {
        uint16_t offset = addr & 0x3FFF;
        uint8_t mode = (control >> 2) & 0x03;

        switch (mode) {
            case 0: case 1: // 32KB switching
                if (addr < 0xC000) 
                    return prg_rom[prg_bank & 0xFE][offset];
                else 
                    return prg_rom[(prg_bank & 0xFE) | 1][offset];

            case 2: // Fix first bank at $8000, switch $C000
                if (addr < 0xC000) 
                    return prg_rom[0][offset];
                else 
                    return prg_rom[prg_bank][offset];

            case 3: // Fix last bank at $C000, switch $8000
            default:
                if (addr < 0xC000) 
                    return prg_rom[prg_bank][offset];
                else 
                    return prg_rom[3][offset]; // Dragon Warrior is 64KB (4 banks)
        }
    }
    return 0;
}

void MapperMMC1::write(uint16_t addr, uint8_t val) {
    if (addr < 0x8000) {
        if (addr >= 0x6000) prg_ram[addr - 0x6000] = val;
        return;
    }

    // MMC1 Shift Register Logic
    if (val & 0x80) {
        shift_register = 0x10;
        control |= 0x0C;
    } else {
        bool complete = (shift_register & 0x01);
        shift_register >>= 1;
        shift_register |= ((val & 0x01) << 4);

        if (complete) {
            uint8_t target_reg = (addr >> 13) & 0x03;
            uint8_t data = shift_register;

            switch (target_reg) {
                case 0: // Control
                    control = data;
                    mirroring = data & 0x03;
                    break;
                case 1: // CHR Bank 0
                    chr_bank0 = data;
                    break;
                case 2: // CHR Bank 1
                    chr_bank1 = data;
                    break;
                case 3: // PRG Bank
                    prg_bank = data & 0x0F;
                    break;
            }
            shift_register = 0x10;
        }
    }
}

uint8_t MapperMMC1::read_chr(uint16_t addr) {
    uint16_t offset = addr & 0x0FFF;
    if (control & 0x10) { // 4KB CHR mode
        if (addr < 0x1000) return chr_rom[chr_bank0][offset];
        else return chr_rom[chr_bank1][offset];
    } else { // 8KB CHR mode
        uint8_t bank = chr_bank0 & 0xFE;
        if (addr < 0x1000) return chr_rom[bank][offset];
        else return chr_rom[bank | 1][offset];
    }
}
