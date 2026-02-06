#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>
#include <cstring>

class MapperMMC1 {
public:
    // Dragon Warrior I: 64KB PRG (4x16KB) and up to 32KB CHR (8x4KB)
    uint8_t prg_rom[4][16384]; 
    uint8_t chr_rom[8][4096];
    uint8_t prg_ram[8192]; // Added for Save Game/SRAM support ($6000-$7FFF)

    uint8_t shift_register;
    uint8_t control;
    uint8_t chr_bank_0;
    uint8_t chr_bank_1;
    uint8_t prg_bank;

    MapperMMC1() {
        reset();
    }

    void reset() {
        shift_register = 0x10;
        // Mode 3: $C000 fixed to last bank, $8000 switchable
        control = 0x1C; 
        prg_bank = 0;
        chr_bank_0 = 0;
        chr_bank_1 = 0;
        std::memset(prg_ram, 0, sizeof(prg_ram));
    }

    void write(uint16_t addr, uint8_t val) {
        if (val & 0x80) {
            shift_register = 0x10;
            control |= 0x0C;
        } else {
            bool complete = (shift_register & 0x01);
            shift_register >>= 1;
            shift_register |= ((val & 0x01) << 4);

            if (complete) {
                uint8_t data = shift_register & 0x1F;
                if (addr >= 0x8000 && addr <= 0x9FFF) control = data;
                else if (addr >= 0xA000 && addr <= 0xBFFF) chr_bank_0 = data;
                else if (addr >= 0xC000 && addr <= 0xDFFF) chr_bank_1 = data;
                else if (addr >= 0xE000) prg_bank = data & 0x0F;
                
                shift_register = 0x10;
            }
        }
    }

    uint8_t read_prg(uint16_t addr) {
        // Handle PRG RAM (SRAM) access
        if (addr >= 0x6000 && addr <= 0x7FFF) return prg_ram[addr - 0x6000];

        uint16_t offset = addr & 0x3FFF;
        uint8_t mode = (control >> 2) & 0x03;

        switch (mode) {
            case 0: case 1: // 32KB switching
            {
                uint8_t bank = (prg_bank & 0x0E);
                return prg_rom[(bank + (addr >= 0xC000)) % 4][offset];
            }
            case 2: // Fix first bank, switch second
                if (addr < 0xC000) return prg_rom[0][offset];
                return prg_rom[prg_bank % 4][offset];
            case 3: // Switch first bank, fix last (DW Default)
                if (addr < 0xC000) return prg_rom[prg_bank % 4][offset];
                return prg_rom[3][offset];
        }
        return 0;
    }

    uint8_t read_chr(uint16_t addr) {
        uint16_t offset = addr & 0x0FFF;
        // Control bit 4: 0 = 8KB mode, 1 = 4KB mode
        if (control & 0x10) {
            if (addr < 0x1000) return chr_rom[chr_bank_0 % 8][offset];
            return chr_rom[chr_bank_1 % 8][offset];
        } else {
            return chr_rom[((chr_bank_0 & 0x1E) + (addr >= 0x1000)) % 8][offset];
        }
    }
    
    // Helper to handle writes to PRG RAM
    void write_prg_ram(uint16_t addr, uint8_t val) {
        if (addr >= 0x6000 && addr <= 0x7FFF) prg_ram[addr - 0x6000] = val;
    }
};

#endif
