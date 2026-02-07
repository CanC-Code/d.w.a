#include "MapperMMC1.h"
#include <cstring>
#include "recompiled/cpu_shared.h"

/**
 * MMC1 Implementation
 * Note: Dragon Warrior (NES-DW) uses the MMC1B2 revision. 
 * It requires stable bank switching for $C000-$FFFF kernels.
 */
MapperMMC1::MapperMMC1() {
    std::memset(prg_rom, 0, sizeof(prg_rom));
    std::memset(chr_rom, 0, sizeof(chr_rom));
    std::memset(prg_ram, 0, sizeof(prg_ram));
    reset();
}

void MapperMMC1::reset() {
    shift_register = 0x10; 
    write_count = 0;

    // Mode 3: $8000 switchable, $C000 fixed to LAST bank.
    // Mirroring: Horizontal (Common for DW)
    control = 0x1C; 
    prg_bank = 0;
    chr_bank_0 = 0;
    chr_bank_1 = 0;
}

void MapperMMC1::write(uint16_t addr, uint8_t val) {
    // 1. PRG-RAM Access ($6000-$7FFF)
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        // Bit 4 of prg_bank is the RAM Disable bit on many MMC1 boards.
        // 0 = Enabled, 1 = Disabled.
        if (!(prg_bank & 0x10)) {
            prg_ram[addr - 0x6000] = val;
        }
        return;
    }

    if (addr < 0x8000) return;

    // 2. MMC1 Reset Logic (Bit 7 of write value)
    if (val & 0x80) {
        shift_register = 0x10;
        write_count = 0;
        control |= 0x0C; // Reset to PRG Mode 3
        return;
    }

    // 3. Serial Shifting
    // MMC1 takes 5 writes to update a register.
    bool bit0 = (val & 0x01);
    
    // In actual hardware, a write to the same address on the very next 
    // cycle is ignored. In recompilation, we just ensure 5-bit integrity.
    shift_register = ((bit0 ? 0x01 : 0x00) << 4) | (shift_register >> 1);
    write_count++;

    if (write_count == 5) {
        // Shift Register is now full (5 bits)
        uint8_t data = (shift_register >> 0) & 0x1F;

        if (addr >= 0x8000 && addr <= 0x9FFF) {
            // Reg 0: Control
            // [..CPPMM] C=CHR Mode, P=PRG Mode, M=Mirroring
            control = data;
        } else if (addr >= 0xA000 && addr <= 0xBFFF) {
            // Reg 1: CHR Bank 0
            chr_bank_0 = data;
        } else if (addr >= 0xC000 && addr <= 0xDFFF) {
            // Reg 2: CHR Bank 1
            chr_bank_1 = data;
        } else if (addr >= 0xE000 && addr <= 0xFFFF) {
            // Reg 3: PRG Bank
            // [..RPPPP] R=RAM Enable, P=PRG Bank
            prg_bank = data;
        }

        // Reset Shift Register for next sequence
        shift_register = 0x10;
        write_count = 0;
    }
}

uint8_t MapperMMC1::read_prg(uint16_t addr) {
    // Standard SRAM Read
    if (addr >= 0x6000 && addr < 0x8000) {
        return prg_ram[addr - 0x6000];
    }

    uint16_t offset = addr & 0x3FFF;
    uint8_t mode = (control >> 2) & 0x03;
    uint8_t bank_select = prg_bank & 0x0F;

    // MMC1 PRG Mirroring/Banking Modes
    switch (mode) {
        case 0:
        case 1:
            // 32KB Mode: Switch $8000-$FFFF as one block
            {
                uint8_t base = (bank_select & 0x0E);
                return prg_rom[(base | ((addr >> 14) & 1)) & 0x03][offset];
            }
        case 2:
            // Fixed first bank ($8000-$BFFF) to Bank 0, Switch $C000-$FFFF
            if (addr < 0xC000) return prg_rom[0][offset];
            return prg_rom[bank_select & 0x03][offset];
        case 3:
        default:
            // Fixed last bank ($C000-$FFFF) to Bank 3, Switch $8000-$BFFF
            // Dragon Warrior expects kernel/NMI handlers here.
            if (addr < 0xC000) return prg_rom[bank_select & 0x03][offset];
            return prg_rom[3][offset];
    }
}

uint8_t MapperMMC1::read_chr(uint16_t addr) {
    // addr is 0x0000-0x1FFF (8KB total)
    bool mode_4kb = (control & 0x10);

    if (mode_4kb) {
        if (addr < 0x1000) return chr_rom[chr_bank_0 & 0x03][addr & 0x0FFF];
        return chr_rom[chr_bank_1 & 0x03][addr & 0x0FFF];
    } else {
        // 8KB Mode
        uint8_t base = (chr_bank_0 & 0x02);
        return chr_rom[(base | (addr >> 12)) & 0x03][addr & 0x0FFF];
    }
}

void MapperMMC1::write_chr(uint16_t addr, uint8_t val) {
    // If the board uses CHR-RAM (common in MMC1)
    bool mode_4kb = (control & 0x10);
    uint8_t bank;
    if (mode_4kb) {
        bank = (addr < 0x1000) ? (chr_bank_0 & 0x03) : (chr_bank_1 & 0x03);
    } else {
        bank = (chr_bank_0 & 0x02) | (addr >> 12);
    }
    chr_rom[bank & 0x03][addr & 0x0FFF] = val;
}
