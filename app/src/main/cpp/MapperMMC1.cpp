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
    shift_register = 0x10; // Bit 4 is the "full" marker for the 5-bit shift
    write_count = 0;

    // --- CRITICAL INITIALIZATION ---
    // Mode 3 (0x0C): $8000 is switchable, $C000 is FIXED to the last bank.
    // We add 0x10 for 4KB CHR mode (often used) and 0x03 for Horizontal Mirroring.
    // Result 0x1F ensures safe booting and correct world map layout.
    control = 0x1F; 
    
    prg_bank = 0;
    chr_bank_0 = 0;
    chr_bank_1 = 0;
}

/**
 * get_bank_ptr: Used by the recompiled dispatcher for high-speed access.
 */
const uint8_t* MapperMMC1::get_bank_ptr(uint16_t addr) const {
    uint8_t mode = (control >> 2) & 0x03;
    uint8_t bank_select = prg_bank & 0x0F;

    if (addr >= 0x8000 && addr < 0xC000) {
        // Lower Bank Window
        if (mode == 2) return prg_rom[0]; // Fixed to first bank
        return prg_rom[bank_select % 4];  // Switchable
    } else if (addr >= 0xC000) {
        // Upper Bank Window
        if (mode == 3) return prg_rom[3]; // Fixed to last bank (DW Kernel)
        return prg_rom[bank_select % 4];  // Switchable
    }
    return nullptr;
}



void MapperMMC1::write(uint16_t addr, uint8_t val) {
    // 1. PRG-RAM (SRAM) Writes
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        // Only write if RAM is enabled (Bit 4 of prg_bank is 0)
        if (!(prg_bank & 0x10)) {
            prg_ram[addr - 0x6000] = val;
        }
        return;
    }

    if (addr < 0x8000) return;

    // 2. Serial Reset logic
    if (val & 0x80) {
        shift_register = 0x10;
        write_count = 0;
        control |= 0x0C; // Reset to Mode 3 for safety
        return;
    }

    // 3. Shift bit 0 of 'val' into our 5-bit register
    bool bit0 = (val & 0x01);
    shift_register = (bit0 ? 0x10 : 0x00) | (shift_register >> 1);
    write_count++;

    // 4. On the 5th write, commit the shift register to an internal register
    if (write_count == 5) {
        uint8_t data = (shift_register & 0x1F);

        if (addr <= 0x9FFF) {
            control = data; // Register 0: Control
        } else if (addr <= 0xBFFF) {
            chr_bank_0 = data; // Register 1: CHR Bank 0
        } else if (addr <= 0xDFFF) {
            chr_bank_1 = data; // Register 2: CHR Bank 1
        } else {
            prg_bank = data; // Register 3: PRG Bank Select
        }

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
        // 8KB Mode: Uses chr_bank_0 and ignores bit 0
        uint8_t base = (chr_bank_0 & 0xFE) % 2;
        return chr_rom[(base | (addr >> 12)) % 2][addr & 0x0FFF];
    }
}

void MapperMMC1::write_chr(uint16_t addr, uint8_t val) {
    // Standard CHR-RAM write (Dragon Warrior uses CHR-ROM, 
    // but some recompiled versions may use RAM for patches)
    uint8_t bank = (addr < 0x1000) ? (chr_bank_0 % 2) : (chr_bank_1 % 2);
    chr_rom[bank][addr & 0x0FFF] = val;
}
