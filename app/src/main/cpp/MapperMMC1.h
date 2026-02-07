#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>
#include <vector>
#include <string>

/**
 * MMC1 Mirroring Modes
 * MMC1 controls how the PPU maps Nametables ($2000-$2FFF).
 */
enum class Mirroring {
    ONE_SCREEN_LOW = 0,
    ONE_SCREEN_HIGH = 1,
    VERTICAL = 2,
    HORIZONTAL = 3
};

class MapperMMC1 {
public:
    // --- PHYSICAL MEMORY ---
    // Dragon Warrior 1: 64KB PRG-ROM = 4 banks of 16KB
    uint8_t prg_rom[4][16384];
    
    // Dragon Warrior 1: 8KB CHR-ROM (some versions use CHR-RAM)
    // We define 2 banks of 4KB to support 4KB switching mode.
    uint8_t chr_rom[2][4096]; 
    
    // 8KB PRG-RAM (Work RAM / Battery Save Data)
    uint8_t prg_ram[8192];    

    // --- MMC1 INTERNAL REGISTERS ---
    uint8_t shift_register; // 5-bit internal shift register
    uint8_t write_count;    // Track 0-4 writes per serial sequence
    uint8_t control;        // $8000: Mirroring, PRG Mode, CHR Mode
    uint8_t chr_bank_0;     // $A000: CHR Bank 0 (4KB or 8KB)
    uint8_t chr_bank_1;     // $C000: CHR Bank 1 (4KB only)
    uint8_t prg_bank;       // $E000: PRG Bank Select + RAM Disable bit

    MapperMMC1();

    // --- ENGINE INTERFACE ---
    void reset();
    void write(uint16_t addr, uint8_t val);
    uint8_t read_prg(uint16_t addr);
    uint8_t read_chr(uint16_t addr);
    void write_chr(uint16_t addr, uint8_t val);

    /**
     * Fast Pointer Retrieval:
     * Used by the Recompiled Dispatcher to avoid calling read_prg() 
     * on every single instruction fetch.
     */
    const uint8_t* get_bank_ptr(uint16_t addr) const;

    /**
     * PPU Mirroring:
     * Logic used by the PPU to resolve addresses $2000-$2FFF.
     */
    Mirroring get_mirroring() const {
        return static_cast<Mirroring>(control & 0x03);
    }

    

    /**
     * RAM Protection:
     * Dragon Warrior uses Bit 4 to "lock" the save data.
     */
    bool is_ram_enabled() const {
        return !(prg_bank & 0x10);
    }

    // --- SNAPSHOTS / SAVE STATES ---
    struct State {
        uint8_t shift_reg;
        uint8_t count;
        uint8_t ctrl;
        uint8_t chr0;
        uint8_t chr1;
        uint8_t prg;
        uint8_t sram[8192]; // Include SRAM in save states for consistency
    };

    State save_state() const {
        State s;
        s.shift_reg = shift_register;
        s.count = write_count;
        s.ctrl = control;
        s.chr0 = chr_bank_0;
        s.chr1 = chr_bank_1;
        s.prg = prg_bank;
        for(int i=0; i<8192; i++) s.sram[i] = prg_ram[i];
        return s;
    }

    void load_state(const State& s) {
        shift_register = s.shift_reg;
        write_count = s.count;
        control = s.ctrl;
        chr_bank_0 = s.chr0;
        chr_bank_1 = s.chr1;
        prg_bank = s.prg;
        for(int i=0; i<8192; i++) prg_ram[i] = s.sram[i];
    }
};

#endif // MAPPERMMC1_H
