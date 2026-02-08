#include "MapperMMC1.h"
#include <cstring>
#include "recompiled/cpu_shared.h"

// Reference total_cycles to implement the MMC1 hardware write-rate limit
// The real MMC1 ignores writes that happen too quickly (consecutive cycles)
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
    // Mode 3 is standard for Dragon Warrior: 
    // $8000 is switchable, $C000 is fixed to the last 16KB bank.
    control = 0x1C; 
    prg_bank = 0;
    chr_bank_0 = 0;
    chr_bank_1 = 0;
}

const uint8_t* MapperMMC1::get_bank_ptr(uint16_t addr) const {
    uint8_t mode = (control >> 2) & 0x03;
    uint8_t bank_select = prg_bank & 0x0F;

    // Dragon Warrior (64KB) has 4 banks (0, 1, 2, 3). 
    // We use & 0x03 to stay within bounds of the prg_rom[4][16384] array.
    if (addr >= 0x8000 && addr < 0xC000) {
        switch (mode) {
            case 0: 
            case 1: return prg_rom[(bank_select & 0x0E) & 0x03]; // 32KB mode
            case 2: return prg_rom[0];                          // Fix first bank
            case 3: 
            default: return prg_rom[bank_select & 0x03];        // Switchable
        }
    } else if (addr >= 0xC000) {
        switch (mode) {
            case 0: 
            case 1: return prg_rom[(bank_select | 0x01) & 0x03]; // 32KB mode
            case 2: return prg_rom[bank_select & 0x03];          // Switchable
            case 3: 
            default: return prg_rom[3];                         // Fix last bank                              
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

void MapperMMC1::write(uint16_t addr, uint8_t val) {
    // PRG RAM ($6000-$7FFF) - Used for Save Games
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        // Bit 4 of prg_bank is the RAM Disable bit (0 = enabled)
        if (!(prg_bank & 0x10)) {
            prg_ram[addr - 0x6000] = val;
        }
        return;
    }

    // MMC1 registers are mapped from $8000-$FFFF
    if (addr < 0x8000) return;

    // Hardware Limit: MMC1 ignores writes occurring on consecutive CPU cycles.
    // This is a common pitfall in high-speed emulation.
    if (total_cycles <= last_write_cycle + 1) return;
    last_write_cycle = total_cycles;

    // Reset logic: Any write with bit 7 set resets the shift register.
    if (val & 0x80) {
        shift_register = 0x10;
        write_count = 0;
        control |= 0x0C; 
        return;
    }

    // Serial shift: MMC1 takes 5 writes to update one internal register.
    // Data is shifted in at bit 4.
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
    return bank ? bank[addr & 0x3FFF] : 0xFF;
}



uint8_t MapperMMC1::read_chr(uint16_t addr) {
    // addr is 0x0000 - 0x1FFF (8KB total space)
    if (!(control & 0x10)) {
        // 8KB switching mode
        uint32_t bank_offset = (chr_bank_0 & 0x1E) * 4096;
        return chr_rom[(bank_offset + addr) % 8192];
    } else {
        // 4KB switching mode
        if (addr < 0x1000) {
            uint32_t bank_offset = (chr_bank_0 & 0x1F) * 4096;
            return chr_rom[(bank_offset + addr) % 8192];
        } else {
            uint32_t bank_offset = (chr_bank_1 & 0x1F) * 4096;
            return chr_rom[(bank_offset + (addr - 0x1000)) % 8192];
        }
    }
}

void MapperMMC1::write_chr(uint16_t addr, uint8_t val) {
    // CHR-RAM support: essential for games using RAM instead of ROM for tiles.
    chr_rom[addr % 8192] = val;
}
