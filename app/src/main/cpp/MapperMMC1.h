#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>
#include <cstring>

class MapperMMC1 {
public:
    // Dragon Warrior I: 64KB PRG (4x16KB) and 8KB-16KB CHR
    uint8_t prg_rom[4][16384]; 
    uint8_t chr_rom[8][4096];
    uint8_t prg_ram[8192]; 

    uint8_t shift_register;
    uint8_t write_count; // Tracks 5 serial writes
    uint8_t control;
    uint8_t chr_bank_0;
    uint8_t chr_bank_1;
    uint8_t prg_bank;
    bool ram_enabled;

    MapperMMC1() {
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
        ram_enabled = true;
        std::memset(prg_ram, 0, sizeof(prg_ram));
    }

    void write(uint16_t addr, uint8_t val) {
        // If Bit 7 is set, reset the shift register
        if (val & 0x80) {
            shift_register = 0x10;
            write_count = 0;
            control |= 0x0C; // Resetting usually sets PRG mode to 3
        } else {
            // Write LSB of val to shift register
            shift_register = ((val & 0x01) << 4) | (shift_register >> 1);
            write_count++;

            // MMC1 registers are updated after 5 bits are shifted in
            if (write_count == 5) {
                uint8_t data = shift_register & 0x1F;
                
                if (addr >= 0x8000 && addr <= 0x9FFF) {
                    control = data;
                } else if (addr >= 0xA000 && addr <= 0xBFFF) {
                    chr_bank_0 = data;
                } else if (addr >= 0xC000 && addr <= 0xDFFF) {
                    chr_bank_1 = data;
                } else if (addr >= 0xE000) {
                    prg_bank = data & 0x0F;
                    // Bit 4 of PRG register (on some boards) enables/disables RAM
                    ram_enabled = !(data & 0x10); 
                }

                shift_register = 0x10;
                write_count = 0;
            }
        }
    }

    uint8_t read_prg(uint16_t addr) {
        // Handle PRG RAM (SRAM) access ($6000-$7FFF)
        if (addr >= 0x6000 && addr <= 0x7FFF) {
            return ram_enabled ? prg_ram[addr - 0x6000] : 0x00;
        }

        uint16_t offset = addr & 0x3FFF;
        uint8_t mode = (control >> 2) & 0x03;

        switch (mode) {
            case 0: case 1: // 32KB switching mode
            {
                // Bit 0 of prg_bank is ignored in 32KB mode
                uint8_t bank = (prg_bank & 0x0E); 
                if (addr >= 0xC000) bank++;
                return prg_rom[bank % 4][offset];
            }
            case 2: // Fix first bank ($8000) to 0, switch second ($C000)
                if (addr < 0xC000) return prg_rom[0][offset];
                return prg_rom[prg_bank % 4][offset];
            case 3: // Switch first bank ($8000), fix last ($C000) to 3
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
            // 8KB Mode: bit 0 of chr_bank_0 is ignored
            uint8_t bank = (chr_bank_0 & 0x1E);
            if (addr >= 0x1000) bank++;
            return chr_rom[bank % 8][offset];
        }
    }

    void write_prg_ram(uint16_t addr, uint8_t val) {
        if (ram_enabled && addr >= 0x6000 && addr <= 0x7FFF) {
            prg_ram[addr - 0x6000] = val;
        }
    }
};

#endif
