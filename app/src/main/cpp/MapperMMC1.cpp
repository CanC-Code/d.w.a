#include "MapperMMC1.h"
#include <cstring>
#include "recompiled/cpu_shared.h"

MapperMMC1::MapperMMC1() {
    // Zero out all memory on initialization
    std::memset(prg_rom, 0, sizeof(prg_rom));
    std::memset(chr_rom, 0, sizeof(chr_rom));
    std::memset(prg_ram, 0, sizeof(prg_ram));
    reset();
}

void MapperMMC1::reset() {
    shift_register = 0x10;
    write_count = 0;
    // Initial Control: Mode 3 (Fix $C000 to bank 3, $8000 switchable)
    control = 0x0C; 
    prg_bank = 0;
    chr_bank_0 = 0;
    chr_bank_1 = 0;
}

/**
 * MMC1 uses a serial write protocol. 
 * It takes 5 writes to any address >= $8000 to update one internal register.
 */
void MapperMMC1::write(uint16_t addr, uint8_t val) {
    // PRG-RAM Write ($6000-$7FFF) - Dragon Warrior Save Data
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        prg_ram[addr - 0x6000] = val;
        return;
    }

    if (addr < 0x8000) return;

    // Bit 7 is a reset flag for the mapper's internal state
    if (val & 0x80) {
        shift_register = 0x10;
        write_count = 0;
        control |= 0x0C;
    } else {
        // Shift bit 0 of val into the 5th bit of our register
        bool bit0 = (val & 0x01);
        shift_register = (shift_register >> 1) | (bit0 ? 0x10 : 0x00);
        write_count++;

        // On the 5th write, apply the accumulated 5 bits to the target register
        if (write_count == 5) {
            uint8_t data = shift_register & 0x1F;
            
            // Determine register by address bits 13 and 14
            if (addr <= 0x9FFF) {
                control = data; // $8000-$9FFF: Control
            } else if (addr <= 0xBFFF) {
                chr_bank_0 = data; // $A000-$BFFF: CHR Bank 0
            } else if (addr <= 0xDFFF) {
                chr_bank_1 = data; // $C000-$DFFF: CHR Bank 1
            } else {
                prg_bank = data & 0x0F; // $E000-$FFFF: PRG Bank
            }

            // Reset for next sequence
            shift_register = 0x10;
            write_count = 0;
        }
    }
}

uint8_t MapperMMC1::read_prg(uint16_t addr) {
    // Handle Save RAM
    if (addr < 0x8000) {
        return (addr >= 0x6000) ? prg_ram[addr - 0x6000] : 0;
    }

    uint16_t offset = addr & 0x3FFF;
    uint8_t mode = (control >> 2) & 0x03;

    switch (mode) {
        case 0:
        case 1: // 32KB switching mode: Use bits 1-3 of prg_bank
        {
            uint8_t bank = (prg_bank & 0x0E);
            if (addr >= 0xC000) bank++;
            return prg_rom[bank & 0x03][offset];
        }
        case 2: // Fixed first bank ($8000) to 0, switch second ($C000)
            if (addr < 0xC000) return prg_rom[0][offset];
            return prg_rom[prg_bank & 0x03][offset];
        case 3: // Switch first bank ($8000), fix second ($C000) to Bank 3
        default:
            if (addr < 0xC000) return prg_rom[prg_bank & 0x03][offset];
            return prg_rom[3][offset];
    }
}

uint8_t MapperMMC1::read_chr(uint16_t addr) {
    bool mode_4kb = control & 0x10; // Bit 4 of control defines CHR mode
    
    if (mode_4kb) {
        // 4KB Bank switching
        if (addr < 0x1000) return chr_rom[0][addr & 0x0FFF];
        return chr_rom[1][addr & 0x0FFF];
    } else {
        // 8KB Bank switching (Dragon Warrior standard)
        return (addr < 0x1000) ? chr_rom[0][addr] : chr_rom[1][addr - 0x1000];
    }
}

// Support for CHR-RAM if the ROM uses it (Dragon Warrior typically uses CHR-ROM)
void MapperMMC1::write_chr(uint16_t addr, uint8_t val) {
    if (addr < 0x1000) chr_rom[0][addr & 0x0FFF] = val;
    else chr_rom[1][addr & 0x0FFF] = val;
}
