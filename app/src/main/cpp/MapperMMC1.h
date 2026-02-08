#include "MapperMMC1.h"
#include <cstring>

MapperMMC1::MapperMMC1() {
    reset();
    // Initialize memory to avoid garbage data
    std::memset(prg_rom, 0, sizeof(prg_rom));
    std::memset(chr_rom, 0, sizeof(chr_rom));
    std::memset(prg_ram, 0, sizeof(prg_ram));
}

void MapperMMC1::reset() {
    shift_register = 0x10; // Initial state: bit 4 is set
    write_count = 0;
    control = 0x0C;        // PRG bank mode 3, Horizontal mirroring
    chr_bank_0 = 0;
    chr_bank_1 = 0;
    prg_bank = 0;
}

/**
 * MMC1 uses a serial write mechanism.
 * It takes 5 writes to bit 0 to fill the internal registers.
 */
void MapperMMC1::write(uint16_t addr, uint8_t val) {
    if (val & 0x80) {
        // Reset mapper
        shift_register = 0x10;
        write_count = 0;
        control |= 0x0C;
    } else {
        bool complete = (shift_register & 0x01);
        shift_register >>= 1;
        shift_register |= ((val & 0x01) << 4);
        write_count++;

        if (complete) {
            uint8_t target = (addr >> 13) & 0x03; // $8000, $A000, $C000, $E000
            switch (target) {
                case 0: control = shift_register; break;
                case 1: chr_bank_0 = shift_register; break;
                case 2: chr_bank_1 = shift_register; break;
                case 3: prg_bank = shift_register & 0x0F; break;
            }
            shift_register = 0x10;
            write_count = 0;
        }
    }
}

uint8_t MapperMMC1::read_prg(uint16_t addr) {
    if (addr >= 0x6000 && addr < 0x8000) {
        return prg_ram[addr - 0x6000];
    }
    
    // Dragon Warrior (MMC1) usually uses 16KB PRG switching.
    // Mode 3: Fix last bank at $C000, switch 16KB at $8000
    if (addr >= 0x8000) {
        uint16_t offset = addr & 0x3FFF;
        if (addr < 0xC000) {
            return prg_rom[prg_bank % 4][offset];
        } else {
            return prg_rom[3][offset]; // Fix last bank
        }
    }
    return 0;
}

uint8_t MapperMMC1::read_chr(uint16_t addr) {
    // Standard 8KB CHR for Dragon Warrior
    return chr_rom[addr & 0x1FFF];
}

void MapperMMC1::write_chr(uint16_t addr, uint8_t val) {
    // Only used if the game uses CHR-RAM instead of ROM
    chr_rom[addr & 0x1FFF] = val;
}

Mirroring MapperMMC1::get_mirroring() const {
    switch (control & 0x03) {
        case 0: return Mirroring::ONE_SCREEN_LOW;
        case 1: return Mirroring::ONE_SCREEN_HIGH;
        case 2: return Mirroring::VERTICAL;
        case 3: return Mirroring::HORIZONTAL;
    }
    return Mirroring::HORIZONTAL;
}
