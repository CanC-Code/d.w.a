#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>
#include <cstring>

class MapperMMC1 {
public:
    // Dragon Warrior: 64KB PRG-ROM (4 x 16KB banks) + 8KB CHR-ROM
    uint8_t prg_rom[4][16384];
    uint8_t chr_rom[8192];
    uint8_t prg_ram[8192];  // Battery-backed save RAM
    
    // MMC1 internal state
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
        control = 0x0C;  // Mode 3: $C000-$FFFF fixed, $8000-$BFFF switchable
        chr_bank_0 = 0;
        chr_bank_1 = 0;
        prg_bank = 0;
    }
    
    void write(uint16_t addr, uint8_t val) {
        // $6000-$7FFF: PRG-RAM (save RAM)
        if (addr >= 0x6000 && addr < 0x8000) {
            prg_ram[addr - 0x6000] = val;
            return;
        }
        
        // $8000-$FFFF: MMC1 control registers
        if (addr >= 0x8000) {
            // Bit 7 = reset shift register
            if (val & 0x80) {
                shift_register = 0x10;
                write_count = 0;
                control |= 0x0C;  // Ensure mode 3
                return;
            }
            
            // Shift in bit 0
            shift_register >>= 1;
            shift_register |= (val & 0x01) << 4;
            write_count++;
            
            // After 5 writes, update the appropriate register
            if (write_count == 5) {
                uint8_t data = shift_register;
                
                if (addr >= 0x8000 && addr <= 0x9FFF) {
                    // Control register
                    control = data & 0x1F;
                }
                else if (addr >= 0xA000 && addr <= 0xBFFF) {
                    // CHR bank 0
                    chr_bank_0 = data & 0x1F;
                }
                else if (addr >= 0xC000 && addr <= 0xDFFF) {
                    // CHR bank 1
                    chr_bank_1 = data & 0x1F;
                }
                else if (addr >= 0xE000 && addr <= 0xFFFF) {
                    // PRG bank
                    prg_bank = data & 0x0F;
                }
                
                shift_register = 0x10;
                write_count = 0;
            }
        }
    }
    
    uint8_t read_prg(uint16_t addr) {
        // $6000-$7FFF: PRG-RAM
        if (addr >= 0x6000 && addr < 0x8000) {
            return prg_ram[addr - 0x6000];
        }
        
        // $8000-$FFFF: PRG-ROM
        if (addr >= 0x8000) {
            uint16_t offset = addr & 0x3FFF;  // 16KB offset
            uint8_t mode = (control >> 2) & 0x03;
            
            switch (mode) {
                case 0:
                case 1:
                    // 32KB mode: use prg_bank for first 16KB, prg_bank+1 for second
                    {
                        uint8_t bank = (prg_bank & 0x0E);
                        if (addr >= 0xC000) bank++;
                        return prg_rom[bank & 0x03][offset];
                    }
                    
                case 2:
                    // Fix first bank to 0, switch second bank
                    if (addr < 0xC000) {
                        return prg_rom[0][offset];
                    } else {
                        return prg_rom[prg_bank & 0x03][offset];
                    }
                    
                case 3:
                default:
                    // Switch first bank, fix last bank to 3 (Dragon Warrior uses this)
                    if (addr < 0xC000) {
                        return prg_rom[prg_bank & 0x03][offset];
                    } else {
                        return prg_rom[3][offset];
                    }
            }
        }
        
        return 0;
    }
    
    uint8_t read_chr(uint16_t addr) {
        // Simple 8KB CHR-ROM (no banking for Dragon Warrior)
        return chr_rom[addr & 0x1FFF];
    }
};

#endif // MAPPERMMC1_H
