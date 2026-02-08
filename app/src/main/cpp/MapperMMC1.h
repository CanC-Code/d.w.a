#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>
#include <cstdio>

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
    // Memory layout: 4 Banks of 16KB PRG, 8KB CHR, 8KB Work/Save RAM
    uint8_t prg_rom[4][16384];
    uint8_t chr_rom[8192];
    uint8_t prg_ram[8192]; 

    // MMC1 Internal Registers
    uint8_t shift_register;
    uint8_t write_count;
    uint8_t control;
    uint8_t chr_bank_0;
    uint8_t chr_bank_1;
    uint8_t prg_bank;

    MapperMMC1();

    void reset();
    void write(uint16_t addr, uint8_t val);
    uint8_t read_prg(uint16_t addr);
    uint8_t read_chr(uint16_t addr);
    void write_chr(uint16_t addr, uint8_t val);

    Mirroring get_mirroring() const;
    const uint8_t* get_bank_ptr(uint16_t addr) const;

    // --- Persistence Implementation ---
    
    void saveToFile(const char* path) {
        if (FILE* f = fopen(path, "wb")) {
            fwrite(prg_ram, 1, 8192, f);
            fclose(f);
        }
    }

    void loadFromFile(const char* path) {
        if (FILE* f = fopen(path, "rb")) {
            fread(prg_ram, 1, 8192, f);
            fclose(f);
        }
    }
};

#endif // MAPPERMMC1_H
