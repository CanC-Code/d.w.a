#include "MapperMMC1.h"
#include <cstring>
#include <android/log.h>

#define LOG_TAG "DWA_MAPPER"
#define LOG_M(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

MapperMMC1::MapperMMC1() {
    // Zero out memory to prevent reading garbage if ROM isn't fully loaded
    std::memset(prg_rom, 0, sizeof(prg_rom));
    std::memset(chr_rom, 0, sizeof(chr_rom));
    std::memset(prg_ram, 0, sizeof(prg_ram));
    reset();
}

void MapperMMC1::reset() {
    // MMC1 shift register is initialized with bit 4 set as a sentinel
    shift_register = 0x10;
    write_count = 0;
    
    // Control: $1C = PRG Mode 3 (fixed last bank), Vertical Mirroring
    control = 0x1C; 
    mirroring = 0x02; 
    
    prg_bank = 0;
    chr_bank_0 = 0;
    chr_bank_1 = 0;
}

void MapperMMC1::write(uint16_t addr, uint8_t val) {
    if (val & 0x80) {
        // Reset Signal (any write with bit 7 set resets the shift register)
        shift_register = 0x10;
        write_count = 0;
        control |= 0x0C; // Lock to PRG mode 3 for safety
    } else {
        // Shift bit 0 of 'val' into our 5-bit shift register
        bool bit = val & 0x01;
        
        // Logical MMC1 shift: data moves into bit 4, everything shifts right
        // We use the sentinel bit method: when bit 0 of the register becomes 1, we've had 5 writes.
        bool complete = (shift_register & 0x01);
        shift_register >>= 1;
        shift_register |= (bit << 4);
        
        if (complete) {
            uint8_t data = shift_register & 0x1F;
            
            // Register selection based on bits 13 and 14 of the address
            uint8_t reg_select = (addr >> 13) & 0x03;

            switch (reg_select) {
                case 0: // $8000 - $9FFF: Control
                    control = data;
                    mirroring = control & 0x03;
                    break;
                case 1: // $A000 - $BFFF: CHR Bank 0
                    chr_bank_0 = data;
                    break;
                case 2: // $C000 - $DFFF: CHR Bank 1
                    chr_bank_1 = data;
                    break;
                case 3: // $E000 - $FFFF: PRG Bank
                    prg_bank = data & 0x0F;
                    break;
            }

            // Reset register for next 5-bit command
            shift_register = 0x10;
        }
    }
}

uint8_t MapperMMC1::read_prg(uint16_t addr) {
    // Save RAM Access ($6000-$7FFF)
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        return prg_ram[addr - 0x6000];
    }

    uint16_t offset = addr & 0x3FFF;
    uint8_t mode = get_prg_mode();

    // Dragon Warrior I specific: 4 banks total. 
    // Modulo 4 (% 4) is critical here to prevent Segmentation Faults.
    switch (mode) {
        case 0: 
        case 1: // 32KB Mode
        {
            uint8_t bank = (prg_bank & 0x0E) % 4; 
            if (addr < 0xC000) return prg_rom[bank][offset];
            return prg_rom[(bank + 1) % 4][offset];
        }
        case 2: // Fix first bank at $8000, switch second at $C000
            if (addr < 0xC000) return prg_rom[0][offset];
            return prg_rom[prg_bank % 4][offset];
        case 3: // Switch first at $8000, fix last bank at $C000 (standard DW1)
        default:
            if (addr < 0xC000) return prg_rom[prg_bank % 4][offset];
            return prg_rom[3][offset]; 
    }
}

uint8_t MapperMMC1::read_chr(uint16_t addr) {
    uint16_t offset = addr & 0x0FFF;

    if (get_chr_mode()) { 
        // 4KB Mode
        if (addr < 0x1000) return chr_rom[chr_bank_0 % 4][offset];
        return chr_rom[chr_bank_1 % 4][offset];
    } else { 
        // 8KB Mode
        uint8_t bank = (chr_bank_0 & 0x1E) % 4;
        if (addr < 0x1000) return chr_rom[bank][offset];
        return chr_rom[(bank + 1) % 4][offset];
    }
}

void MapperMMC1::write_chr(uint16_t addr, uint8_t val) {
    uint16_t offset = addr & 0x0FFF;
    // Dragon Warrior uses CHR-ROM, writes are technically ignored, 
    // but implemented for safety and potential CHR-RAM hacks.
    if (get_chr_mode()) {
        if (addr < 0x1000) chr_rom[chr_bank_0 % 4][offset] = val;
        else chr_rom[chr_bank_1 % 4][offset] = val;
    } else {
        uint8_t bank = (chr_bank_0 & 0x1E) % 4;
        if (addr < 0x1000) chr_rom[bank][offset] = val;
        else chr_rom[(bank + 1) % 4][offset] = val;
    }
}
