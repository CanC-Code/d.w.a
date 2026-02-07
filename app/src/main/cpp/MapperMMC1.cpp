#include "MapperMMC1.h"
#include <cstring>
#include "recompiled/cpu_shared.h"

/**
 * MMC1 Implementation for Dragon Warrior (NES-DW)
 * Stable initialization is required to ensure the Reset Vector ($FFFC)
 * is read from the correct physical ROM bank.
 */
MapperMMC1::MapperMMC1() {
    std::memset(prg_rom, 0, sizeof(prg_rom));
    std::memset(chr_rom, 0, sizeof(chr_rom));
    std::memset(prg_ram, 0, sizeof(prg_ram));
    reset();
}

void MapperMMC1::reset() {
    // MMC1 Shift register starts with a "1" in the 5th bit to detect 5 writes
    shift_register = 0x10; 
    write_count = 0;

    // --- CRITICAL INITIALIZATION ---
    // 0x1F: Mode 3, 8KB CHR, Horizontal Mirroring.
    // This ensures the game boots into the fixed bank ($C000-$FFFF).
    control = 0x1F; 

    prg_bank = 0;
    chr_bank_0 = 0;
    chr_bank_1 = 0;
}

/**
 * get_bank_ptr: Used by the recompiled dispatcher for high-speed access.
 * Added modulo safety to prevent segfaults on uninitialized bank writes.
 */
const uint8_t* MapperMMC1::get_bank_ptr(uint16_t addr) const {
    uint8_t mode = (control >> 2) & 0x03;
    uint8_t bank_select = prg_bank & 0x0F;

    if (addr >= 0x8000 && addr < 0xC000) {
        // Lower Bank Window ($8000-$BFFF)
        switch (mode) {
            case 0: case 1: return prg_rom[(bank_select & 0x0E) % 4]; // 32KB mode
            case 2: return prg_rom[0];                               // Fixed to first
            case 3: return prg_rom[bank_select % 4];                 // Switchable
            default: return prg_rom[0];
        }
    } else if (addr >= 0xC000) {
        // Upper Bank Window ($C000-$FFFF)
        switch (mode) {
            case 0: case 1: return prg_rom[(bank_select | 0x01) % 4]; // 32KB mode
            case 2: return prg_rom[bank_select % 4];                 // Switchable
            case 3: return prg_rom[3];                               // Fixed to last
            default: return prg_rom[3];
        }
    }
    return nullptr;
}



void MapperMMC1::write(uint16_t addr, uint8_t val) {
    // 1. PRG-RAM (SRAM) Writes: $6000-$7FFF
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        // Bit 4 of prg_bank is the RAM Disable bit
        if (!(prg_bank & 0x10)) {
            prg_ram[addr - 0x6000] = val;
        }
        return;
    }

    // MMC1 registers are mapped to $8000-$FFFF
    if (addr < 0x8000) return;

    // 2. Serial Reset logic: Writing any value with Bit 7 set resets the mapper
    if (val & 0x80) {
        shift_register = 0x10;
        write_count = 0;
        control |= 0x0C; // Mode 3 is the standard power-on state
        return;
    }

    // 3. Shift bit 0 of 'val' into our 5-bit register
    // Shift is LSB first.
    bool bit0 = (val & 0x01);
    
    // We clear the bit at the current position and shift in the new LSB at the top (Bit 4)
    shift_register = (shift_register >> 1) | (bit0 ? 0x10 : 0x00);
    write_count++;

    // 4. On the 5th write, commit the shift register to the target internal register
    if (write_count == 5) {
        uint8_t data = shift_register & 0x1F;

        if (addr >= 0x8000 && addr <= 0x9FFF) {
            control = data;
        } else if (addr >= 0xA000 && addr <= 0xBFFF) {
            chr_bank_0 = data;
        } else if (addr >= 0xC000 && addr <= 0xDFFF) {
            chr_bank_1 = data;
        } else if (addr >= 0xE000 && addr <= 0xFFFF) {
            prg_bank = data;
        }

        // Reset for the next 5-bit sequence
        shift_register = 0x10;
        write_count = 0;
    }
}

uint8_t MapperMMC1::read_prg(uint16_t addr) {
    if (addr >= 0x6000 && addr < 0x8000) return prg_ram[addr - 0x6000];
    if (addr < 0x8000) return 0;

    const uint8_t* bank = get_bank_ptr(addr);
    return bank ? bank[addr & 0x3FFF] : 0;
}

uint8_t MapperMMC1::read_chr(uint16_t addr) {
    bool mode_4kb = (control & 0x10);
    if (mode_4kb) {
        if (addr < 0x1000) return chr_rom[chr_bank_0 % 2][addr & 0x0FFF];
        return chr_rom[chr_bank_1 % 2][addr & 0x0FFF];
    } else {
        // 8KB Mode: Uses chr_bank_0 and ignores its LSB
        uint8_t base = (chr_bank_0 & 0xFE) % 2;
        return chr_rom[(addr < 0x1000 ? base : (base + 1)) % 2][addr & 0x0FFF];
    }
}

void MapperMMC1::write_chr(uint16_t addr, uint8_t val) {
    // Dragon Warrior (NES) has CHR-ROM, but the recompiler often treats 
    // it as CHR-RAM to allow for dynamic tile updates.
    uint8_t bank = (addr < 0x1000) ? (chr_bank_0 % 2) : (chr_bank_1 % 2);
    chr_rom[bank][addr & 0x0FFF] = val;
}
