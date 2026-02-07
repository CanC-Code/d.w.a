#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>
#include <cstring>

class MapperMMC1 {
public:
    // Dragon Warrior 1: 64KB PRG-ROM (4 banks of 16KB) + 8KB CHR-ROM
    uint8_t prg_rom[4][16384];
    uint8_t chr_rom[2][4096]; 
    uint8_t prg_ram[8192];    

    // Internal MMC1 Registers
    uint8_t shift_register;
    uint8_t write_count;
    uint8_t control;
    uint8_t chr_bank_0;
    uint8_t chr_bank_1;
    uint8_t prg_bank;

    MapperMMC1() {
        std::memset(prg_rom, 0, sizeof(prg_rom));
        std::memset(chr_rom, 0, sizeof(chr_rom));
        std::memset(prg_ram, 0, sizeof(prg_ram));
        reset();
    }

    void reset() {
        shift_register = 0x10;
        write_count = 0;
        // Default: Fix $C000 to last bank, Switch $8000
        control = 0x0C; 
        chr_bank_0 = 0;
        chr_bank_1 = 0;
        prg_bank = 0;
    }

    /**
     * CRITICAL FOR RECOMPILED CODE:
     * Returns the base pointer for the currently mapped bank at $8000 or $C000.
     * This allows the Dispatcher to jump into the correct C++ function.
     */
    const uint8_t* get_bank_ptr(uint16_t addr) const {
        uint8_t mode = (control >> 2) & 0x03;
        if (addr >= 0xC000) {
            // High Bank ($C000-$FFFF)
            if (mode == 0 || mode == 1) return prg_rom[(prg_bank & 0x0E) | 1];
            if (mode == 2) return prg_rom[prg_bank & 0x03];
            return prg_rom[3]; // Mode 3: Fixed to last bank
        } else {
            // Low Bank ($8000-$BFFF)
            if (mode == 0 || mode == 1) return prg_rom[prg_bank & 0x0E];
            if (mode == 2) return prg_rom[0]; // Mode 2: Fixed to first bank
            return prg_rom[prg_bank & 0x03];
        }
    }

    void write(uint16_t addr, uint8_t val) {
        if (addr >= 0x6000 && addr < 0x8000) {
            prg_ram[addr - 0x6000] = val;
            return;
        }

        if (addr >= 0x8000) {
            if (val & 0x80) {
                shift_register = 0x10;
                write_count = 0;
                control |= 0x0C;
                return;
            }

            shift_register = (shift_register >> 1) | ((val & 0x01) << 4);
            write_count++;

            if (write_count == 5) {
                uint8_t data = shift_register & 0x1F;
                if (addr <= 0x9FFF)      control = data;
                else if (addr <= 0xBFFF) chr_bank_0 = data;
                else if (addr <= 0xDFFF) chr_bank_1 = data;
                else                     prg_bank = data & 0x0F;

                shift_register = 0x10;
                write_count = 0;
            }
        }
    }

    uint8_t read_prg(uint16_t addr) {
        if (addr < 0x8000) return (addr >= 0x6000) ? prg_ram[addr - 0x6000] : 0;
        const uint8_t* bank = get_bank_ptr(addr);
        return bank[addr & 0x3FFF];
    }

    uint8_t read_chr(uint16_t addr) {
        // Dragon Warrior has 8KB CHR. Most versions don't actually swap CHR,
        // but we support the 4KB/8KB modes to be safe.
        bool mode_4kb = control & 0x10;
        if (mode_4kb) {
            if (addr < 0x1000) return chr_rom[0][addr & 0x0FFF];
            return chr_rom[1][addr & 0x0FFF];
        } else {
            return (addr < 0x1000) ? chr_rom[0][addr] : chr_rom[1][addr - 0x1000];
        }
    }
};

#endif
