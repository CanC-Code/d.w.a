#include "MapperMMC1.h"
#include <cstring>
#include "recompiled/cpu_shared.h"

// Reference total_cycles to implement the MMC1 write-protection timer
extern int64_t total_cycles;
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
    control = 0x1F; 
    prg_bank = 0;
    chr_bank_0 = 0;
    chr_bank_1 = 0;
}

const uint8_t* MapperMMC1::get_bank_ptr(uint16_t addr) const {
    uint8_t mode = (control >> 2) & 0x03;
    uint8_t bank_select = prg_bank & 0x0F;

    if (addr >= 0x8000 && addr < 0xC000) {
        switch (mode) {
            case 0: case 1: return prg_rom[(bank_select & 0x0E) & 0x03]; 
            case 2: return prg_rom[0];                               
            case 3: return prg_rom[bank_select & 0x03];                 
            default: return prg_rom[0];
        }
    } else if (addr >= 0xC000) {
        switch (mode) {
            case 0: case 1: return prg_rom[(bank_select | 0x01) & 0x03]; 
            case 2: return prg_rom[bank_select & 0x03];                 
            case 3: return prg_rom[3];                               
            default: return prg_rom[3];
        }
    }
    return nullptr;
}

Mirroring MapperMMC1::get_mirroring() const {
    switch (control & 0x03) {
        case 0: return Mirroring::ONE_SCREEN_LOW;
        case 1: return Mirroring::ONE_SCREEN_HIGH;
        case 2: return Mirroring::VERTICAL;
        case 3: return Mirroring::HORIZONTAL;
        default: return Mirroring::HORIZONTAL;
    }
}

void MapperMMC1::write(uint16_t addr, uint8_t val) {
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        if (!(prg_bank & 0x10)) prg_ram[addr - 0x6000] = val;
        return;
    }

    if (addr < 0x8000) return;

    // MMC1 Ignore Write Logic: Consecutive writes on the same cycle or 
    // immediately following cycle are ignored by the serial hardware.
    if (total_cycles <= last_write_cycle + 1) return;
    last_write_cycle = total_cycles;

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
        else                     prg_bank = data;

        shift_register = 0x10;
        write_count = 0;
    }
}

uint8_t MapperMMC1::read_prg(uint16_t addr) {
    if (addr >= 0x6000 && addr < 0x8000) return prg_ram[addr - 0x6000];
    const uint8_t* bank = get_bank_ptr(addr);
    return bank ? bank[addr & 0x3FFF] : 0;
}

uint8_t MapperMMC1::read_chr(uint16_t addr) {
    // Corrected CHR Banking Logic
    if (!(control & 0x10)) {
        // 8KB Mode: Use chr_bank_0 (ignore LSB)
        uint8_t bank = (chr_bank_0 & 0x1E) % 2; // DW usually has 2 CHR banks
        return chr_rom[bank][addr & 0x1FFF];
    } else {
        // 4KB Mode
        if (addr < 0x1000) return chr_rom[chr_bank_0 % 2][addr & 0x0FFF];
        else               return chr_rom[chr_bank_1 % 2][addr & 0x0FFF];
    }
}
