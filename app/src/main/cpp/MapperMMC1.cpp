#include "MapperMMC1.h"
#include <cstring>

MapperMMC1::MapperMMC1() {
    std::memset(prg_rom, 0, sizeof(prg_rom));
    std::memset(chr_rom, 0, sizeof(chr_rom));
    std::memset(prg_ram, 0, sizeof(prg_ram));
    reset();
}

void MapperMMC1::reset() {
    shift_register = 0x10;
    write_count = 0;
    // Mode 3: $C000 fixed to last bank (3), $8000 switchable
    control = 0x1C; 
    prg_bank = 0;
    chr_bank_0 = 0;
    chr_bank_1 = 0;
    ram_enabled = true;
}

void MapperMMC1::write(uint16_t addr, uint8_t val) {
    if (val & 0x80) {
        shift_register = 0x10;
        write_count = 0;
        control |= 0x0C; // Force PRG Mode 3
    } else {
        // MMC1 shifts in the LSB first
        shift_register = ((val & 0x01) << 4) | (shift_register >> 1);
        write_count++;

        if (write_count == 5) {
            uint8_t data = shift_register & 0x1F;

            // MMC1 Register Selection based on CPU Address
            if (addr >= 0x8000 && addr <= 0x9FFF) {
                control = data;
            } else if (addr >= 0xA000 && addr <= 0xBFFF) {
                chr_bank_0 = data;
            } else if (addr >= 0xC000 && addr <= 0xDFFF) {
                chr_bank_1 = data;
            } else if (addr >= 0xE000) {
                // PRG Bank: bit 4 is RAM chip disable
                prg_bank = data & 0x0F;
                ram_enabled = !(data & 0x10); 
            }

            shift_register = 0x10;
            write_count = 0;
        }
    }
}

uint8_t MapperMMC1::read_prg(uint16_t addr) {
    // PRG-RAM (SRAM) for Save Games
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        return ram_enabled ? prg_ram[addr - 0x6000] : 0x00;
    }

    uint16_t offset = addr & 0x3FFF;
    uint8_t mode = (control >> 2) & 0x03;

    // Dragon Warrior 1: 4 PRG Banks total
    switch (mode) {
        case 0: 
        case 1: // 32KB Mode: $8000 switches both banks
        {
            uint8_t bank = (prg_bank & 0x0E); 
            if (addr >= 0xC000) bank++;
            return prg_rom[bank & 0x03][offset];
        }
        case 2: // Fix first bank ($8000), switch second ($C000)
            if (addr < 0xC000) return prg_rom[0][offset];
            return prg_rom[prg_bank & 0x03][offset];
        case 3: // Switch first bank ($8000), fix last ($C000)
        default:
            if (addr < 0xC000) return prg_rom[prg_bank & 0x03][offset];
            return prg_rom[3][offset]; // Fixed to Bank 03
    }
}

uint8_t MapperMMC1::read_chr(uint16_t addr) {
    uint16_t offset = addr & 0x0FFF;
    // Dragon Warrior uses 8KB CHR-ROM total
    // (Pattern Table 0 and 1)
    
    
    
    if (control & 0x10) { // 4KB Switching Mode
        if (addr < 0x1000) return chr_rom[chr_bank_0 & 0x01][offset];
        return chr_rom[chr_bank_1 & 0x01][offset];
    } else { // 8KB Switching Mode
        uint8_t bank = (chr_bank_0 & 0x0E);
        if (addr >= 0x1000) bank++;
        return chr_rom[bank & 0x01][offset];
    }
}
