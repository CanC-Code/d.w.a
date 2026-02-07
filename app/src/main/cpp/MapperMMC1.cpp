#include "MapperMMC1.h"
#include <cstring>
#include "recompiled/cpu_shared.h"

MapperMMC1::MapperMMC1() {
    reset();
}

void MapperMMC1::reset() {
    shift_register = 0x10;
    control = 0x0C; // Initial: 16KB PRG switching, fix last bank at $C000
    prg_bank = 0;
    chr_bank0 = 0;
    chr_bank1 = 0;
    mirroring = 0;

    // Initialize PRG RAM (SRAM) for save data
    std::memset(prg_ram, 0, sizeof(prg_ram));
}

uint8_t MapperMMC1::read_prg(uint16_t addr) {
    // PRG RAM Range ($6000-$7FFF)
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        return prg_ram[addr - 0x6000];
    }

    // PRG ROM Range ($8000-$FFFF)
    if (addr >= 0x8000) {
        uint16_t offset = addr & 0x3FFF;
        uint8_t mode = (control >> 2) & 0x03;

        switch (mode) {
            case 0: 
            case 1: // 32KB switching mode
            {
                uint8_t bank = (prg_bank & 0x0E) % 4;
                if (addr < 0xC000) 
                    return prg_rom[bank][offset];
                else 
                    return prg_rom[bank | 1][offset];
            }
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
                    return prg_rom[3][offset]; // Dragon Warrior Last Bank
        }
    }
    return 0;
}



void MapperMMC1::write(uint16_t addr, uint8_t val) {
    // PRG RAM Write ($6000-$7FFF)
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        prg_ram[addr - 0x6000] = val;
        return;
    }

    if (addr < 0x8000) return;

    // MMC1 Serial Write Logic
    if (val & 0x80) {
        // Reset shift register to initial state
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
                case 0: // $8000-$9FFF: Control
                    control = data;
                    mirroring = data & 0x03;
                    break;
                case 1: // $A000-$BFFF: CHR Bank 0
                    chr_bank0 = data & 0x1F;
                    break;
                case 2: // $C000-$DFFF: CHR Bank 1
                    chr_bank1 = data & 0x1F;
                    break;
                case 3: // $E000-$FFFF: PRG Bank
                    // Bit 4 usually handles PRG-RAM enable; we mask to 0-15 for bank
                    prg_bank = data & 0x0F; 
                    break;
            }
            shift_register = 0x10;
        }
    }
}

uint8_t MapperMMC1::read_chr(uint16_t addr) {
    uint16_t offset = addr & 0x0FFF;
    if (control & 0x10) { // 4KB CHR Mode
        if (addr < 0x1000) return chr_rom[chr_bank0 % 4][offset];
        else return chr_rom[chr_bank1 % 4][offset];
    } else { // 8KB CHR Mode
        uint8_t bank = (chr_bank0 & 0xFE) % 4;
        if (addr < 0x1000) return chr_rom[bank][offset];
        else return chr_rom[(bank | 1) % 4][offset];
    }
}

void MapperMMC1::write_chr(uint16_t addr, uint8_t val) {
    uint16_t offset = addr & 0x0FFF;
    // Dragon Warrior uses CHR-ROM, but PPU may attempt to write
    // during clear-screen or specific logic. We update internal buffer
    // to prevent crashes, though it won't persist if it's true ROM.
    if (control & 0x10) { 
        if (addr < 0x1000) chr_rom[chr_bank0 % 4][offset] = val;
        else chr_rom[chr_bank1 % 4][offset] = val;
    } else {
        uint8_t bank = (chr_bank0 & 0xFE) % 4;
        if (addr < 0x1000) chr_rom[bank][offset] = val;
        else chr_rom[(bank | 1) % 4][offset] = val;
    }
}
