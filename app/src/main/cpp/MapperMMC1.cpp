#include "MapperMMC1.h"
#include <cstring>
#include "recompiled/cpu_shared.h"

// Reference total_cycles to implement the MMC1 hardware write-rate limit
extern "C" int64_t total_cycles;
static int64_t last_write_cycle = -100;

MapperMMC1::MapperMMC1() {
    std::memset(prg_rom, 0, sizeof(prg_rom));
    std::memset(chr_rom, 0, sizeof(chr_rom));
    std::memset(prg_ram, 0, sizeof(prg_ram));
    reset();
}

void MapperMMC1::reset() {
    shift_register = 0x10; 
    write_count = 0;
    // Initial State: $C000-$FFFF fixed to last bank, $8000 switchable
    control = 0x1C; 
    prg_bank = 0;
    chr_bank_0 = 0;
    chr_bank_1 = 0;
}



/**
 * Resolves which 16KB PRG bank the CPU is currently looking at.
 * Dragon Warrior (64KB) uses 4 banks (0, 1, 2, 3).
 */
const uint8_t* MapperMMC1::get_bank_ptr(uint16_t addr) const {
    uint8_t mode = (control >> 2) & 0x03;
    uint8_t bank_select = prg_bank & 0x0F;

    if (addr >= 0x8000 && addr < 0xC000) {
        switch (mode) {
            case 0: 
            case 1: // 32KB Mode: Switch 32KB at $8000, ignoring low bit
                return prg_rom[(bank_select & 0x0E) % 4];
            case 2: // Fix first bank at $8000
                return prg_rom[0];
            case 3: // Switchable bank at $8000
            default:
                return prg_rom[bank_select % 4];
        }
    } else if (addr >= 0xC000) {
        switch (mode) {
            case 0: 
            case 1: // 32KB Mode: Upper 16KB of the 32KB chunk
                return prg_rom[(bank_select | 0x01) % 4];
            case 2: // Switchable bank at $C000
                return prg_rom[bank_select % 4];
            case 3: // Fix last bank at $C000 (Standard Dragon Warrior)
            default:
                return prg_rom[3]; 
        }
    }
    return nullptr;
}

Mirroring MapperMMC1::get_mirroring() const {
    switch (control & 0x03) {
        case 0: return Mirroring::ONE_SCREEN_LOW;
        case 1: return Mirroring::ONE_SCREEN_HIGH;
        case 2: return Mirroring::VERTICAL;
        case 3: 
        default: return Mirroring::HORIZONTAL;
    }
}

/**
 * Handles the serial-to-parallel write logic of the MMC1.
 */
void MapperMMC1::write(uint16_t addr, uint8_t val) {
    // PRG RAM ($6000-$7FFF) - Battery backed saves
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        // Bit 4 of prg_bank is the RAM Disable bit (0 = enabled)
        if (!(prg_bank & 0x10)) {
            prg_ram[addr - 0x6000] = val;
        }
        return;
    }

    if (addr < 0x8000) return;

    // MMC1 Write Protection: Ignore writes on consecutive cycles.
    // In recompiled code, this is vital as loops can hit the register too fast.
    if (total_cycles <= last_write_cycle + 1) return;
    last_write_cycle = total_cycles;

    if (val & 0x80) {
        shift_register = 0x10;
        write_count = 0;
        control |= 0x0C; // Reset sets PRG mode 3
        return;
    }

    // Shift in 1 bit of data
    bool complete = (shift_register & 0x01);
    shift_register >>= 1;
    shift_register |= ((val & 0x01) << 4);
    write_count++;

    if (write_count == 5) {
        uint8_t data = shift_register & 0x1F;
        if (addr <= 0x9FFF)      control = data;
        else if (addr <= 0xBFFF) chr_bank_0 = data;
        else if (addr <= 0xDFFF) chr_bank_1 = data;
        else                     prg_bank = data;

        shift_register = 0x10;
        write_count = 0;
    }
}

uint8_t MapperMMC1::read_prg(uint16_t addr) {
    if (addr >= 0x6000 && addr < 0x8000) {
        return prg_ram[addr - 0x6000];
    }
    const uint8_t* bank = get_bank_ptr(addr);
    return bank ? bank[addr & 0x3FFF] : 0xFF;
}

uint8_t MapperMMC1::read_chr(uint16_t addr) {
    addr &= 0x1FFF;
    if (!(control & 0x10)) {
        // 8KB mode: Use chr_bank_0, ignore low bit
        uint32_t bank = (chr_bank_0 & 0x1E);
        return chr_rom[bank][addr];
    } else {
        // 4KB mode
        if (addr < 0x1000) {
            return chr_rom[chr_bank_0 & 0x1F][addr];
        } else {
            return chr_rom[chr_bank_1 & 0x1F][addr - 0x1000];
        }
    }
}

void MapperMMC1::write_chr(uint16_t addr, uint8_t val) {
    // Write to CHR-RAM (Dragon Warrior uses this for dynamic tile updates)
    addr &= 0x1FFF;
    if (!(control & 0x10)) {
        uint32_t bank = (chr_bank_0 & 0x1E);
        chr_rom[bank][addr] = val;
    } else {
        if (addr < 0x1000) {
            chr_rom[chr_bank_0 & 0x1F][addr] = val;
        } else {
            chr_rom[chr_bank_1 & 0x1F][addr - 0x1000] = val;
        }
    }
}
