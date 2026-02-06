#include "MapperMMC1.h"
#include <cstring>

MapperMMC1::MapperMMC1() {
    // Clear all memory to ensure no uninitialized data causes logic hangs
    std::memset(prg_rom, 0, sizeof(prg_rom));
    std::memset(chr_rom, 0, sizeof(chr_rom));
    std::memset(prg_ram, 0, sizeof(prg_ram));
    reset();
}

void MapperMMC1::reset() {
    shift_register = 0x10;
    write_count = 0;
    // Control Mode 3 ($1C): $C000 fixed to last bank, $8000 switchable
    control = 0x1C; 
    prg_bank = 0;
    chr_bank_0 = 0;
    chr_bank_1 = 0;
    ram_enabled = true;
}



void MapperMMC1::write(uint16_t addr, uint8_t val) {
    if (val & 0x80) {
        // Reset bit 7: Clears shift register and sets PRG mode 3
        shift_register = 0x10;
        write_count = 0;
        control |= 0x0C;
    } else {
        // MMC1 expects 5 serial writes to fill a register (LSB first)
        shift_register = ((val & 0x01) << 4) | (shift_register >> 1);
        write_count++;

        if (write_count == 5) {
            uint8_t data = shift_register & 0x1F;
            
            // Map the write to one of the four internal MMC1 registers
            if (addr >= 0x8000 && addr <= 0x9FFF) {
                control = data; // Mirroring and Bank Modes
            } else if (addr >= 0xA000 && addr <= 0xBFFF) {
                chr_bank_0 = data; // CHR Bank 0 (or 8KB bank)
            } else if (addr >= 0xC000 && addr <= 0xDFFF) {
                chr_bank_1 = data; // CHR Bank 1 (ignored in 8KB mode)
            } else if (addr >= 0xE000) {
                // PRG Bank: Bits 0-3 select bank, Bit 4 disables PRG RAM
                prg_bank = data & 0x0F;
                ram_enabled = !(data & 0x10); 
            }

            shift_register = 0x10;
            write_count = 0;
        }
    }
}

uint8_t MapperMMC1::read_prg(uint16_t addr) {
    // PRG RAM access ($6000-$7FFF) - used for save games
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        return ram_enabled ? prg_ram[addr - 0x6000] : 0x00;
    }

    uint16_t offset = addr & 0x3FFF; // 16KB offset
    uint8_t mode = (control >> 2) & 0x03;

    

    switch (mode) {
        case 0: 
        case 1: // 32KB Mode: Switch both $8000 and $C000 together
        {
            uint8_t bank = (prg_bank & 0x0E); 
            if (addr >= 0xC000) bank++;
            return prg_rom[bank & 0x03][offset];
        }
        case 2: // Fixed Bank 0 at $8000, Switchable at $C000
            if (addr < 0xC000) return prg_rom[0][offset];
            return prg_rom[prg_bank & 0x03][offset];
        case 3: // Switchable at $8000, Fixed Last Bank at $C000 (Standard for DW1)
        default:
            if (addr < 0xC000) return prg_rom[prg_bank & 0x03][offset];
            return prg_rom[3][offset]; 
    }
}

uint8_t MapperMMC1::read_chr(uint16_t addr) {
    uint16_t offset = addr & 0x0FFF; // 4KB offset
    
    

    // Dragon Warrior 1 uses 8KB CHR total (2 banks of 4KB)
    if (control & 0x10) { // 4KB Separate Bank Switching
        if (addr < 0x1000) return chr_rom[chr_bank_0 & 0x01][offset];
        return chr_rom[chr_bank_1 & 0x01][offset];
    } else { // 8KB Continuous Bank Switching
        uint8_t bank = (chr_bank_0 & 0x0E);
        if (addr >= 0x1000) bank++;
        return chr_rom[bank & 0x01][offset];
    }
}
