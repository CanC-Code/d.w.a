#include "MapperMMC1.h"
#include <cstring>
#include "recompiled/cpu_shared.h"

/**
 * MMC1 is one of the most complex NES mappers due to its serial write interface.
 * It requires 5 consecutive writes to shift a single 5-bit value into its registers.
 */
MapperMMC1::MapperMMC1() {
    std::memset(prg_rom, 0, sizeof(prg_rom));
    std::memset(chr_rom, 0, sizeof(chr_rom));
    std::memset(prg_ram, 0, sizeof(prg_ram));
    reset();
}

void MapperMMC1::reset() {
    shift_register = 0x10; // The 5th bit is used as a "completion" marker
    write_count = 0;
    
    // Default: Mode 3 (Fixed high bank), 8KB CHR mode
    // Dragon Warrior relies on Mode 3 to keep the kernel in $C000-$FFFF
    control = 0x0C; 
    prg_bank = 0;
    chr_bank_0 = 0;
    chr_bank_1 = 0;
}

void MapperMMC1::write(uint16_t addr, uint8_t val) {
    // 1. Handle PRG-RAM (SRAM) Writes ($6000-$7FFF)
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        // QOL: Bit 4 of prg_bank is the Chip Enable (CE) for RAM on some MMC1 revisions
        // We allow the write if the bit is 0 (standard) or if we ignore protection.
        if (!(prg_bank & 0x10)) {
            prg_ram[addr - 0x6000] = val;
        }
        return;
    }

    // 2. Ignore writes below $8000 (standard bus behavior)
    if (addr < 0x8000) return;

    // 3. Handle Serial Reset (Bit 7)
    if (val & 0x80) {
        shift_register = 0x10;
        write_count = 0;
        control |= 0x0C; // Reset to Mode 3
    } else {
        // 4. Shift logic: Shift val.bit0 into shift_register.bit4
        bool bit0 = (val & 0x01);
        shift_register = (shift_register >> 1) | (bit0 ? 0x10 : 0x00);
        write_count++;

        // 5. On the 5th write, copy the shift register to the target MMC1 register
        if (write_count == 5) {
            uint8_t data = shift_register & 0x1F;

            if (addr <= 0x9FFF) {
                // Register 0: Control ($8000-$9FFF)
                // Bits 0-1: Mirroring (0: 1-Screen Low, 1: 1-Screen High, 2: Vert, 3: Horiz)
                // Bits 2-3: PRG Mode, Bit 4: CHR Mode
                control = data;
            } else if (addr <= 0xBFFF) {
                // Register 1: CHR Bank 0 ($A000-$BFFF)
                chr_bank_0 = data;
            } else if (addr <= 0xDFFF) {
                // Register 2: CHR Bank 1 ($C000-$DFFF)
                chr_bank_1 = data;
            } else {
                // Register 3: PRG Bank ($E000-$FFFF)
                // Bits 0-3: Select Bank, Bit 4: RAM Chip Enable
                prg_bank = data; 
            }

            // Reset shift state
            shift_register = 0x10;
            write_count = 0;
        }
    }
}

uint8_t MapperMMC1::read_prg(uint16_t addr) {
    // Map $6000-$7FFF to PRG-RAM
    if (addr < 0x8000) {
        return (addr >= 0x6000) ? prg_ram[addr - 0x6000] : 0;
    }

    uint16_t offset = addr & 0x3FFF;
    uint8_t mode = (control >> 2) & 0x03;
    uint8_t bank_select = prg_bank & 0x0F;

    switch (mode) {
        case 0:
        case 1:
            // 32KB Mode: Switch $8000-$FFFF at once. Ignore bit 0 of bank_select.
            {
                uint8_t base_bank = (bank_select & 0x0E);
                if (addr >= 0xC000) base_bank++;
                return prg_rom[base_bank & 0x03][offset];
            }
        case 2:
            // Fixed first bank ($8000-$BFFF) to Bank 0. Switch second bank.
            if (addr < 0xC000) return prg_rom[0][offset];
            return prg_rom[bank_select & 0x03][offset];
        case 3:
        default:
            // Fixed last bank ($C000-$FFFF) to Bank 3. Switch first bank.
            // THIS IS THE MODE DRAGON WARRIOR USES.
            if (addr < 0xC000) return prg_rom[bank_select & 0x03][offset];
            return prg_rom[3][offset];
    }
}

uint8_t MapperMMC1::read_chr(uint16_t addr) {
    bool mode_4kb = control & 0x10;

    if (mode_4kb) {
        // 4KB Mode: Two independent banks
        if (addr < 0x1000) return chr_rom[chr_bank_0 & 0x03][addr & 0x0FFF];
        return chr_rom[chr_bank_1 & 0x03][addr & 0x0FFF];
    } else {
        // 8KB Mode: Use CHR Bank 0, ignore bit 0 of the register
        uint8_t base_bank = (chr_bank_0 & 0x02);
        if (addr >= 0x1000) base_bank++;
        return chr_rom[base_bank & 0x03][addr & 0x0FFF];
    }
}

void MapperMMC1::write_chr(uint16_t addr, uint8_t val) {
    // Useful if the game uses CHR-RAM instead of CHR-ROM.
    uint8_t bank = (addr < 0x1000) ? (chr_bank_0 & 0x03) : (chr_bank_1 & 0x03);
    chr_rom[bank][addr & 0x0FFF] = val;
}
