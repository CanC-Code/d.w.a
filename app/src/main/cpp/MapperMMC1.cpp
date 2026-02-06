#include "MapperMMC1.h"
#include <cstring>

MapperMMC1::MapperMMC1() {
    // Zero out memory arrays to ensure no garbage data affects the game
    std::memset(prg_rom, 0, sizeof(prg_rom));
    std::memset(chr_rom, 0, sizeof(chr_rom));
    std::memset(prg_ram, 0, sizeof(prg_ram));
    reset();
}

void MapperMMC1::reset() {
    shift_register = 0x10;
    write_count = 0;
    // Mode 3: $C000 fixed to last bank (3), $8000 switchable
    // This is the default state for most MMC1 games like Dragon Warrior
    control = 0x1C; 
    prg_bank = 0;
    chr_bank_0 = 0;
    chr_bank_1 = 0;
    ram_enabled = true;
}



void MapperMMC1::write(uint16_t addr, uint8_t val) {
    if (val & 0x80) {
        // Reset the shift register if the 7th bit is set
        shift_register = 0x10;
        write_count = 0;
        control |= 0x0C; // PRG Mode 3 (Fixed $C000)
    } else {
        // MMC1 shifts in one bit at a time, LSB first
        shift_register = ((val & 0x01) << 4) | (shift_register >> 1);
        write_count++;

        if (write_count == 5) {
            uint8_t data = shift_register & 0x1F;

            // Determine target register based on the address range
            if (addr >= 0x8000 && addr <= 0x9FFF) {
                control = data;
            } else if (addr >= 0xA000 && addr <= 0xBFFF) {
                chr_bank_0 = data;
            } else if (addr >= 0xC000 && addr <= 0xDFFF) {
                chr_bank_1 = data;
            } else if (addr >= 0xE000) {
                // PRG Bank: bit 4 controls RAM enable on some MMC1 boards
                prg_bank = data & 0x0F;
                ram_enabled = !(data & 0x10); 
            }

            // Reset for the next 5-bit write cycle
            shift_register = 0x10;
            write_count = 0;
        }
    }
}

uint8_t MapperMMC1::read_prg(uint16_t addr) {
    // Handle PRG-RAM (SRAM) access for battery-backed saves
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        return ram_enabled ? prg_ram[addr - 0x6000] : 0x00;
    }

    uint16_t offset = addr & 0x3FFF;
    uint8_t mode = (control >> 2) & 0x03;

    // Dragon Warrior I specific: 4 PRG banks total
    
    
    switch (mode) {
        case 0: 
        case 1: // 32KB Mode: $8000 switches both 16KB slots
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
            return prg_rom[3][offset]; // Standard DW1 configuration
    }
}

uint8_t MapperMMC1::read_chr(uint16_t addr) {
    uint16_t offset = addr & 0x0FFF;
    
    

    // Dragon Warrior uses 8KB CHR-ROM (effectively 2 pages of 4KB)
    if (control & 0x10) { // 4KB Switching Mode
        if (addr < 0x1000) return chr_rom[chr_bank_0 & 0x07][offset];
        return chr_rom[chr_bank_1 & 0x07][offset];
    } else { // 8KB Switching Mode
        uint8_t bank = (chr_bank_0 & 0x06);
        if (addr >= 0x1000) bank++;
        return chr_rom[bank & 0x07][offset];
    }
}
