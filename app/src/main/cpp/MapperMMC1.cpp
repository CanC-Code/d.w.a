#include "MapperMMC1.h"
#include <cstring>
#include "recompiled/cpu_shared.h"

MapperMMC1::MapperMMC1() {
    reset();
}

void MapperMMC1::reset() {
    shift_register = 0x10;
    control = 0x0C; // Initial state: Fix last bank at $C000, switchable $8000
    prg_bank = 0;
    chr_bank0 = 0;
    chr_bank1 = 0;
    mirroring = 0;
    
    // Clear PRG RAM for Dragon Warrior save state compatibility
    std::memset(prg_ram, 0, sizeof(prg_ram));
}

uint8_t MapperMMC1::read_prg(uint16_t addr) {
    // PRG RAM Range ($6000-$7FFF) - Used for save data in Dragon Warrior
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        return prg_ram[addr - 0x6000];
    }

    // PRG ROM Range ($8000-$FFFF)
    if (addr >= 0x8000) {
        uint16_t offset = addr & 0x3FFF;
        uint8_t mode = (control >> 2) & 0x03;

        switch (mode) {
            case 0: case 1: // 32KB switching mode
                if (addr < 0xC000) 
                    return prg_rom[(prg_bank & 0xFE) % 4][offset];
                else 
                    return prg_rom[(prg_bank & 0xFE | 1) % 4][offset];

            case 2: // Fixed first bank at $8000, switchable $C000
                if (addr < 0xC000) 
                    return prg_rom[0][offset];
                else 
                    return prg_rom[prg_bank % 4][offset];

            case 3: // Switchable $8000, fixed last bank at $C000
            default:
                if (addr < 0xC000) 
                    return prg_rom[prg_bank % 4][offset];
                else 
                    return prg_rom[3][offset]; // Last bank of Dragon Warrior (64KB total)
        }
    }
    return 0;
}

void MapperMMC1::write(uint16_t addr, uint8_t val) {
    // Write to PRG RAM
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        prg_ram[addr - 0x6000] = val;
        return;
    }

    if (addr < 0x8000) return;

    // MMC1 Shift Register Logic: Serial write to internal registers
    // 
    if (val & 0x80) {
        // Reset shift register
        shift_register = 0x10;
        control |= 0x0C;
    } else {
        bool complete = (shift_register & 0x01);
        shift_register >>= 1;
        shift_register |= ((val & 0x01) << 4);

        if (complete) {
            uint8_t target_reg = (addr >> 13) & 0x03;
            uint8_t data = shift_register;

            switch (target_reg) {
                case 0: // $8000-$9FFF: Control Register
                    control = data;
                    mirroring = data & 0x03;
                    break;
                case 1: // $A000-$BFFF: CHR Bank 0
                    chr_bank0 = data;
                    break;
                case 2: // $C000-$DFFF: CHR Bank 1
                    chr_bank1 = data;
                    break;
                case 3: // $E000-$FFFF: PRG Bank
                    prg_bank = data & 0x0F;
                    break;
            }
            shift_register = 0x10;
        }
    }
}

uint8_t MapperMMC1::read_chr(uint16_t addr) {
    uint16_t offset = addr & 0x0FFF;
    if (control & 0x10) { // 4KB CHR Bank switching mode
        if (addr < 0x1000) return chr_rom[chr_bank0 % 4][offset];
        else return chr_rom[chr_bank1 % 4][offset];
    } else { // 8KB CHR Bank switching mode
        uint8_t bank = (chr_bank0 & 0xFE) % 4;
        if (addr < 0x1000) return chr_rom[bank][offset];
        else return chr_rom[(bank | 1) % 4][offset];
    }
}

// FIX: Added the missing write_chr method to resolve PPU.cpp build error
void MapperMMC1::write_chr(uint16_t addr, uint8_t val) {
    uint16_t offset = addr & 0x0FFF;
    if (control & 0x10) { // 4KB CHR mode
        if (addr < 0x1000) chr_rom[chr_bank0 % 4][offset] = val;
        else chr_rom[chr_bank1 % 4][offset] = val;
    } else { // 8KB CHR mode
        uint8_t bank = (chr_bank0 & 0xFE) % 4;
        if (addr < 0x1000) chr_rom[bank][offset] = val;
        else chr_rom[(bank | 1) % 4][offset] = val;
    }
}
