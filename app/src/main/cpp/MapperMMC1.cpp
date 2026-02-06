#include "MapperMMC1.h"
#include <cstring>

MapperMMC1::MapperMMC1() {
    // Zero out memory to prevent garbage values
    std::memset(prg_rom, 0, sizeof(prg_rom));
    std::memset(chr_rom, 0, sizeof(chr_rom));
    std::memset(prg_ram, 0, sizeof(prg_ram));
    reset();
}

void MapperMMC1::reset() {
    shift_register = 0x10;
    write_count = 0;
    // Mode 3: $C000 fixed to last bank, $8000 switchable
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
        control |= 0x0C; 
    } else {
        // MMC1 is serial; it takes 5 writes to fill the register
        shift_register = ((val & 0x01) << 4) | (shift_register >> 1);
        write_count++;

        if (write_count == 5) {
            uint8_t data = shift_register & 0x1F;

            if (addr >= 0x8000 && addr <= 0x9FFF) {
                control = data;
            } else if (addr >= 0xA000 && addr <= 0xBFFF) {
                chr_bank_0 = data;
            } else if (addr >= 0xC000 && addr <= 0xDFFF) {
                chr_bank_1 = data;
            } else if (addr >= 0xE000) {
                prg_bank = data & 0x0F;
                ram_enabled = !(data & 0x10); 
            }

            shift_register = 0x10;
            write_count = 0;
        }
    }
}

uint8_t MapperMMC1::read_prg(uint16_t addr) {
    // Handle PRG RAM (SRAM) access
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        return ram_enabled ? prg_ram[addr - 0x6000] : 0x00;
    }

    uint16_t offset = addr & 0x3FFF;
    uint8_t mode = (control >> 2) & 0x03;

    switch (mode) {
        case 0: case 1: // 32KB switching mode
        {
            uint8_t bank = (prg_bank & 0x0E); 
            if (addr >= 0xC000) bank++;
            return prg_rom[bank % 4][offset];
        }
        case 2: // Fix first bank ($8000) to 0, switch second ($C000)
            if (addr < 0xC000) return prg_rom[0][offset];
            return prg_rom[prg_bank % 4][offset];
        case 3: // Switch first bank ($8000), fix last ($C000) to 3 (Dragon Warrior style)
            if (addr < 0xC000) return prg_rom[prg_bank % 4][offset];
            return prg_rom[3][offset];
    }
    return 0;
}

uint8_t MapperMMC1::read_chr(uint16_t addr) {
    uint16_t offset = addr & 0x0FFF;
    if (control & 0x10) { // 4KB Mode
        if (addr < 0x1000) return chr_rom[chr_bank_0 % 8][offset];
        return chr_rom[chr_bank_1 % 8][offset];
    } else { // 8KB Mode
        uint8_t bank = (chr_bank_0 & 0x1E);
        if (addr >= 0x1000) bank++;
        return chr_rom[bank % 8][offset];
    }
}
