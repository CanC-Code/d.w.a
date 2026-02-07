#include "MapperMMC1.h"
#include <cstring>
#include "recompiled/cpu_shared.h"

MapperMMC1::MapperMMC1() {
    reset();
}

void MapperMMC1::reset() {
    shift_register = 0x10;
    control = 0x0C; // Mode 3: Fix last bank at $C000, switchable $8000
    prg_bank = 0;
    chr_bank0 = 0;
    chr_bank1 = 0;
    mirroring = 0;
    std::memset(prg_ram, 0, sizeof(prg_ram));
}

void MapperMMC1::write(uint16_t addr, uint8_t val) {
    // PRG RAM Write ($6000-$7FFF) - Used for Save Games
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        prg_ram[addr - 0x6000] = val;
        return;
    }

    if (addr < 0x8000) return;

    // MMC1 Serial Write Logic (Bit 7 is the reset flag)
    if (val & 0x80) {
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
                case 0: // $8000-$9FFF: Control (Mirroring and PRG/CHR Modes)
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
                    prg_bank = data & 0x0F; 
                    break;
            }
            shift_register = 0x10;
        }
    }
}

uint8_t MapperMMC1::read_prg(uint16_t addr) {
    if (addr >= 0x6000 && addr <= 0x7FFF) return prg_ram[addr - 0x6000];
    
    if (addr >= 0x8000) {
        uint16_t offset = addr & 0x3FFF;
        uint8_t mode = (control >> 2) & 0x03;

        switch (mode) {
            case 0: case 1: // 32KB switching mode
                return prg_rom[((prg_bank & 0x0E) | ((addr >> 14) & 1)) % 4][offset];
            case 2: // Fixed first bank at $8000, switchable $C000
                return (addr < 0xC000) ? prg_rom[0][offset] : prg_rom[prg_bank % 4][offset];
            case 3: // Switchable $8000, fixed last bank at $C000
            default:
                return (addr < 0xC000) ? prg_rom[prg_bank % 4][offset] : prg_rom[3][offset];
        }
    }
    return 0;
}

uint8_t MapperMMC1::read_chr(uint16_t addr) {
    uint16_t offset = addr & 0x0FFF;
    if (control & 0x10) { // 4KB CHR Mode
        return (addr < 0x1000) ? chr_rom[chr_bank0 % 4][offset] : chr_rom[chr_bank1 % 4][offset];
    } else { // 8KB CHR Mode
        return chr_rom[((chr_bank0 & 0xFE) | ((addr >> 12) & 1)) % 4][offset];
    }
}

void MapperMMC1::write_chr(uint16_t addr, uint8_t val) {
    uint16_t offset = addr & 0x0FFF;
    if (control & 0x10) { 
        if (addr < 0x1000) chr_rom[chr_bank0 % 4][offset] = val;
        else chr_rom[chr_bank1 % 4][offset] = val;
    } else {
        uint8_t bank = (chr_bank0 & 0xFE) % 4;
        if (addr < 0x1000) chr_rom[bank][offset] = val;
        else chr_rom[(bank | 1) % 4][offset] = val;
    }
}
