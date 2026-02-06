#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>
#include <cstring>

class MapperMMC1 {
public:
    // Dragon Warrior I: 64KB PRG (4x16KB) and 16KB CHR
    uint8_t prg_rom[4][16384]; 
    uint8_t chr_rom[8][4096];
    uint8_t prg_ram[8192]; 

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
        // Mode 3: $C000 fixed to last bank, $8000 switchable
        control = 0x1C; 
        prg_bank = 0;
        chr_bank_0 = 0;
        chr_bank_1 = 0;
    }

    // --- PRG Reading ---
    // Handles the native port's request for code or data
    uint8_t read_prg(uint16_t addr) {
        if (addr >= 0x6000 && addr <= 0x7FFF) return prg_ram[addr - 0x6000];
        if (addr >= 0x8000 && addr <= 0xBFFF) {
            // Switchable bank slot
            return prg_rom[prg_bank & 0x03][addr - 0x8000];
        }
        if (addr >= 0xC000) {
            // Fixed to Last Bank (Bank 03)
            return prg_rom[3][addr - 0xC000];
        }
        return 0;
    }

    // --- CHR Reading ---
    // Used by the PPU rendering loop in native-lib.cpp
    uint8_t read_chr(uint16_t addr) {
        if (control & 0x10) { // 4KB CHR Mode
            if (addr <= 0x0FFF) return chr_rom[chr_bank_0 & 0x07][addr];
            return chr_rom[chr_bank_1 & 0x07][addr - 0x1000];
        } else { // 8KB CHR Mode
            uint8_t bank = (chr_bank_0 & 0x06);
            if (addr <= 0x0FFF) return chr_rom[bank][addr];
            return chr_rom[bank + 1][addr - 0x1000];
        }
    }

    // --- MMC1 Shift Register Logic ---
    
    void write(uint16_t addr, uint8_t val) {
        if (val & 0x80) { // Reset shift register
            reset();
            return;
        }

        // Add bit to shift register
        shift_register = (shift_register >> 1) | ((val & 0x01) << 4);
        write_count++;

        if (write_count == 5) {
            uint8_t target_reg = (addr >> 13) & 0x03;
            uint8_t data = shift_register & 0x1F;

            switch (target_reg) {
                case 0: control = data; break;
                case 1: chr_bank_0 = data; break;
                case 2: chr_bank_1 = data; break;
                case 3: prg_bank = data & 0x0F; break;
            }
            shift_register = 0x10;
            write_count = 0;
        }
    }
};

#endif
