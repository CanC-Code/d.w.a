#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>
#include <cstring>

class MapperMMC1 {
public:
    // Dragon Warrior 1: 64KB PRG-ROM (4 banks of 16KB) + 8KB CHR-ROM
    uint8_t prg_rom[4][16384];
    uint8_t chr_rom[2][4096]; // Split into 4KB chunks for flexible banking
    uint8_t prg_ram[8192];    // Battery-backed Save RAM

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
        // Bit 2,3 = 3: PRG-ROM Mode 3 (Fix $C000, Switch $8000)
        control = 0x0C; 
        chr_bank_0 = 0;
        chr_bank_1 = 0;
        prg_bank = 0;
    }

    /**
     * Helper for Dispatcher.cpp to determine which recompiled
     * bank to execute when reg_PC is in the $8000-$BFFF range.
     */
    uint8_t get_current_prg_bank() const {
        uint8_t mode = (control >> 2) & 0x03;
        if (mode == 2) return 0; // Fixed to first bank
        return prg_bank & 0x03;  // Current switchable bank
    }

    /**
     * Handles CPU writes to the Mapper area ($6000-$FFFF).
     * MMC1 uses a serial protocol: 5 writes to any address >= $8000
     * shifts 1 bit into the internal registers.
     */
    void write(uint16_t addr, uint8_t val) {
        // PRG-RAM (Save Data)
        if (addr >= 0x6000 && addr < 0x8000) {
            prg_ram[addr - 0x6000] = val;
            return;
        }

        if (addr >= 0x8000) {
            // Bit 7 set: Reset shift register
            if (val & 0x80) {
                shift_register = 0x10;
                write_count = 0;
                control |= 0x0C;
                return;
            }

            // Shift bit 0 of 'val' into our shift register
            bool bit0 = (val & 0x01);
            shift_register >>= 1;
            shift_register |= (bit0 ? 0x10 : 0x00);
            write_count++;

            // On the 5th write, copy shift register to internal control regs
            if (write_count == 5) {
                uint8_t data = shift_register & 0x1F;

                if (addr <= 0x9FFF) {
                    control = data;
                } else if (addr <= 0xBFFF) {
                    chr_bank_0 = data;
                } else if (addr <= 0xDFFF) {
                    chr_bank_1 = data;
                } else {
                    prg_bank = data & 0x0F;
                }

                shift_register = 0x10;
                write_count = 0;
            }
        }
    }

    /**
     * Handles CPU reads for the PRG-ROM space ($8000-$FFFF).
     */
    uint8_t read_prg(uint16_t addr) {
        if (addr < 0x8000) {
            return (addr >= 0x6000) ? prg_ram[addr - 0x6000] : 0;
        }

        uint16_t offset = addr & 0x3FFF;
        uint8_t mode = (control >> 2) & 0x03;

        switch (mode) {
            case 0:
            case 1: // 32KB switching mode
            {
                uint8_t bank = (prg_bank & 0x0E);
                if (addr >= 0xC000) bank++;
                return prg_rom[bank & 0x03][offset];
            }
            case 2: // Fix first bank ($8000) to 0, switch second ($C000)
                if (addr < 0xC000) return prg_rom[0][offset];
                return prg_rom[prg_bank & 0x03][offset];
            case 3: // Switch first bank ($8000), fix second ($C000) to 3
            default:
                if (addr < 0xC000) return prg_rom[prg_bank & 0x03][offset];
                return prg_rom[3][offset];
        }
    }

    /**
     * Handles PPU reads for the CHR-ROM space ($0000-$1FFF).
     */
    uint8_t read_chr(uint16_t addr) {
        bool mode = control & 0x10; // 0: 8KB mode, 1: 4KB mode
        if (mode) {
            // 4KB mode
            if (addr < 0x1000) return chr_rom[0][addr & 0x0FFF]; // Usually bank 0
            return chr_rom[1][addr & 0x0FFF]; // Usually bank 1
        } else {
            // 8KB mode
            uint8_t bank = (chr_bank_0 & 0xFE);
            if (addr >= 0x1000) bank++;
            // Note: Since DW1 only has 8KB CHR total, this usually defaults to bank 0
            return (addr < 0x1000) ? chr_rom[0][addr] : chr_rom[1][addr - 0x1000];
        }
    }
};

#endif // MAPPERMMC1_H
