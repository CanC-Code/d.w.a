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
    // MMC1 default: PRG mode 3 ($C000 fixed to last), Vertical Mirroring
    control = 0x1C; 
    mirroring = 0x02; // Vertical
    prg_bank = 0;
    chr_bank_0 = 0;
    chr_bank_1 = 0;
}



void MapperMMC1::write(uint16_t addr, uint8_t val) {
    if (val & 0x80) {
        // Reset Signal
        shift_register = 0x10;
        write_count = 0;
        control |= 0x0C; // Set PRG mode to 3
    } else {
        // Shift bit into register (LSB first)
        bool bit = val & 0x01;
        shift_register = (shift_register >> 1) | (bit << 4);
        write_count++;

        if (write_count == 5) {
            uint8_t data = shift_register & 0x1F;
            
            if (addr >= 0x8000 && addr <= 0x9FFF) {
                control = data;
                // Update mirroring immediately for PPU synchronization
                mirroring = control & 0x03; 
            }
            else if (addr >= 0xA000 && addr <= 0xBFFF) {
                chr_bank_0 = data;
            }
            else if (addr >= 0xC000 && addr <= 0xDFFF) {
                chr_bank_1 = data;
            }
            else if (addr >= 0xE000) {
                prg_bank = data & 0x0F;
                // Bit 4 of PRG bank register often enables/disables RAM
            }

            // Reset for next command
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
    uint8_t mode = get_prg_mode();

    switch (mode) {
        case 0: 
        case 1: // 32KB Mode: Switch 32KB at $8000
        {
            uint8_t bank = (prg_bank & 0x0E); 
            if (addr < 0xC000) return prg_rom[bank % 4][offset];
            return prg_rom[(bank + 1) % 4][offset];
        }
        case 2: // Fix first bank ($8000), switch second ($C000)
            if (addr < 0xC000) return prg_rom[0][offset];
            return prg_rom[prg_bank % 4][offset];
        case 3: // Switch first ($8000), fix last bank ($C000)
        default:
            if (addr < 0xC000) return prg_rom[prg_bank % 4][offset];
            return prg_rom[3][offset]; 
    }
}



uint8_t MapperMMC1::read_chr(uint16_t addr) {
    uint16_t offset = addr & 0x0FFF;

    if (get_chr_mode()) { 
        // 4KB Mode: Switch two independent 4KB banks
        if (addr < 0x1000) return chr_rom[chr_bank_0 % 4][offset];
        return chr_rom[chr_bank_1 % 4][offset];
    } else { 
        // 8KB Mode: Switch 8KB at a time
        uint8_t bank = (chr_bank_0 & 0x0E);
        if (addr < 0x1000) return chr_rom[bank % 4][offset];
        return chr_rom[(bank + 1) % 4][offset];
    }
}

void MapperMMC1::write_chr(uint16_t addr, uint8_t val) {
    uint16_t offset = addr & 0x0FFF;
    // Note: DW1 is CHR-ROM, but we allow writes for dev/hacks
    if (get_chr_mode()) {
        if (addr < 0x1000) chr_rom[chr_bank_0 % 4][offset] = val;
        else chr_rom[chr_bank_1 % 4][offset] = val;
    } else {
        uint8_t bank = (chr_bank_0 & 0x0E);
        if (addr < 0x1000) chr_rom[bank % 4][offset] = val;
        else chr_rom[(bank + 1) % 4][offset] = val;
    }
}
