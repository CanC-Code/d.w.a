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
    // Control Mode 3 ($1C): Fix last bank at $C000, 4KB CHR mode, Vertical Mirroring
    control = 0x1C; 
    prg_bank = 0;
    chr_bank_0 = 0;
    chr_bank_1 = 0;
    ram_enabled = true;
}

void MapperMMC1::write(uint16_t addr, uint8_t val) {
    // If bit 7 is set, reset the shift register
    if (val & 0x80) {
        shift_register = 0x10;
        write_count = 0;
        control |= 0x0C; 
    } else {
        // Shift bit into register
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
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        return prg_ram[addr - 0x6000];
    }

    uint16_t offset = addr & 0x3FFF;
    uint8_t mode = (control >> 2) & 0x03;

    switch (mode) {
        case 0: 
        case 1: // 32KB switching
        {
            uint8_t bank = (prg_bank & 0x0E); 
            if (addr >= 0xC000) bank++;
            return prg_rom[bank % 4][offset];
        }
        case 2: // Fix first bank at $8000, switch $C000
            if (addr < 0xC000) return prg_rom[0][offset];
            return prg_rom[prg_bank % 4][offset];
        case 3: // Switch $8000, fix last bank at $C000
        default:
            if (addr < 0xC000) return prg_rom[prg_bank % 4][offset];
            return prg_rom[3][offset]; 
    }
}

uint8_t MapperMMC1::read_chr(uint16_t addr) {
    uint16_t offset = addr & 0x0FFF;

    if (control & 0x10) { // 4KB banks mode
        if (addr < 0x1000) return chr_rom[chr_bank_0 % 4][offset];
        return chr_rom[chr_bank_1 % 4][offset];
    } else { // 8KB bank mode
        uint8_t bank = (chr_bank_0 & 0x0E);
        if (addr >= 0x1000) bank++;
        return chr_rom[bank % 4][offset];
    }
}

// IMPLEMENTATION TO FIX PPU.CPP ERROR
void MapperMMC1::write_chr(uint16_t addr, uint8_t val) {
    uint16_t offset = addr & 0x0FFF;
    
    // While Dragon Warrior is CHR-ROM, this allows the PPU 
    // bus logic to function without crashing if a write is attempted.
    if (control & 0x10) { // 4KB banks
        if (addr < 0x1000) chr_rom[chr_bank_0 % 4][offset] = val;
        else chr_rom[chr_bank_1 % 4][offset] = val;
    } else { // 8KB bank
        uint8_t bank = (chr_bank_0 & 0x0E);
        if (addr >= 0x1000) bank++;
        chr_rom[bank % 4][offset] = val;
    }
}
