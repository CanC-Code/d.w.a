#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>
#include <vector>
#include <string>

/**
 * MMC1 Mirroring Modes
 * 0: One-Screen (Lower Bank)
 * 1: One-Screen (Upper Bank)
 * 2: Vertical (Side-by-side)
 * 3: Horizontal (Stacked) - Used by Dragon Warrior for the World Map.
 */
enum class Mirroring {
    ONE_SCREEN_LOW = 0,
    ONE_SCREEN_HIGH = 1,
    VERTICAL = 2,
    HORIZONTAL = 3
};

class MapperMMC1 {
public:
    // Memory Storage
    // Dragon Warrior 1: 64KB PRG-ROM (4 banks of 16KB)
    uint8_t prg_rom[4][16384];
    // 8KB CHR-ROM (2 banks of 4KB)
    uint8_t chr_rom[2][4096]; 
    // 8KB PRG-RAM (Work RAM / Save Data)
    uint8_t prg_ram[8192];    

    // --- MMC1 INTERNAL REGISTERS ---
    uint8_t shift_register; // 5-bit shift register
    uint8_t write_count;    // 0 to 4
    uint8_t control;        // $8000-$9FFF (Mirroring, Bank Modes)
    uint8_t chr_bank_0;     // $A000-$BFFF
    uint8_t chr_bank_1;     // $C000-$DFFF
    uint8_t prg_bank;       // $E000-$FFFF (Bank select + RAM protection)

    MapperMMC1();
    
    // Core Engine Interface
    void reset();
    void write(uint16_t addr, uint8_t val);
    uint8_t read_prg(uint16_t addr);
    uint8_t read_chr(uint16_t addr);
    void write_chr(uint16_t addr, uint8_t val);

    /**
     * Helper for the Recompiled Dispatcher.
     * Fast retrieval of the current 16KB window based on CPU address.
     */
    const uint8_t* get_bank_ptr(uint16_t addr) const;

    /**
     * Determines current PPU Mirroring based on Control Register bits 0-1.
     * The PPU uses this to decide which Nametable to render.
     */
    Mirroring get_mirroring() const {
        return static_cast<Mirroring>(control & 0x03);
    }

    /**
     * PRG-RAM Protection Status.
     * If bit 4 of prg_bank is set, RAM is disabled.
     */
    bool is_ram_enabled() const {
        return !(prg_bank & 0x10);
    }

    // --- QOL: PERSISTENCE (SAVE STATES) ---
    
    /**
     * Captures the entire internal state of the mapper into a buffer.
     * Essential for SaveStates (Snapshots).
     */
    struct State {
        uint8_t shift_reg;
        uint8_t count;
        uint8_t ctrl;
        uint8_t chr0;
        uint8_t chr1;
        uint8_t prg;
    };

    State save_state() const {
        return { shift_register, write_count, control, chr_bank_0, chr_bank_1, prg_bank };
    }

    void load_state(const State& s) {
        shift_register = s.shift_reg;
        write_count = s.count;
        control = s.ctrl;
        chr_bank_0 = s.chr0;
        chr_bank_1 = s.chr1;
        prg_bank = s.prg;
    }
};

#endif // MAPPERMMC1_H
