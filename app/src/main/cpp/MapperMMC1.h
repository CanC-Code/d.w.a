#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>

/**
 * Mirroring modes supported by MMC1.
 * Critical for Dragon Warrior's UI windows and overworld scrolling.
 */
enum class Mirroring {
    ONE_SCREEN_LOW,
    ONE_SCREEN_HIGH,
    VERTICAL,
    HORIZONTAL
};

class MapperMMC1 {
public:
    // Dragon Warrior: 64KB PRG-ROM (4 x 16KB banks)
    // CHR is 8KB (Standard for NES-DW)
    uint8_t prg_rom[4][16384];
    uint8_t chr_rom[8192];
    uint8_t prg_ram[8192];  // Battery-backed Save RAM ($6000-$7FFF)

    // MMC1 internal state registers
    uint8_t shift_register;
    uint8_t write_count;
    uint8_t control;
    uint8_t chr_bank_0;
    uint8_t chr_bank_1;
    uint8_t prg_bank;

    MapperMMC1();

    /**
     * Resets mapper to power-on state.
     * Mode 3, Horizontal Mirroring, Fixed Upper Bank.
     */
    void reset();

    /**
     * Serial write logic.
     * Includes the 2-cycle write protection required for recompiled speed.
     */
    void write(uint16_t addr, uint8_t val);

    /**
     * Standard bus reads for PRG and CHR.
     */
    uint8_t read_prg(uint16_t addr);
    uint8_t read_chr(uint16_t addr);

    /**
     * Special function for recompiled tile updates (CHR-RAM emulation).
     */
    void write_chr(uint16_t addr, uint8_t val);

    /**
     * Returns the current mirroring mode based on the Control register.
     * Used by PPU to calculate Nametable offsets.
     */
    Mirroring get_mirroring() const;

    /**
     * High-speed bank resolution for the C++ Dispatcher.
     * Returns a direct pointer to the 16KB physical bank mapped to the address.
     */
    const uint8_t* get_bank_ptr(uint16_t addr) const;

    /**
     * Persistence helpers for Dragon Warrior Save Files.
     */
    void saveToFile(const char* path);
    void loadFromFile(const char* path);
};

#endif // MAPPERMMC1_H
