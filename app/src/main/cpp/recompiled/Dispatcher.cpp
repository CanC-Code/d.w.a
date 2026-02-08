#include "recompiled/cpu_shared.h"
#include "MapperMMC1.h"
#include <android/log.h>

#define LOG_TAG "DWA_DISPATCHER"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern MapperMMC1 mapper;

// Recompiled bank namespaces
namespace Bank00 { void execute_at(uint16_t pc); }
namespace Bank01 { void execute_at(uint16_t pc); }
namespace Bank02 { void execute_at(uint16_t pc); }
namespace Bank03 { void execute_at(uint16_t pc); }

typedef void (*BankExecFn)(uint16_t);

// Dispatch table mapping bank index to recompiled function
static const BankExecFn bank_dispatch_table[] = {
    Bank00::execute_at,
    Bank01::execute_at,
    Bank02::execute_at,
    Bank03::execute_at
};

namespace Dispatcher {

    /**
     * MMC1 PRG Banking Logic:
     * Mode 0, 1: Switch 32 KB at $8000, ignoring low bit of bank number.
     * Mode 2: Fix first bank at $8000, switch 16 KB at $C000.
     * Mode 3: Switch 16 KB at $8000, fix last bank at $C000.
     */
    inline uint8_t get_active_bank(uint16_t pc) {
        uint8_t mode = (mapper.control >> 2) & 0x03;
        uint8_t bank_reg = mapper.prg_bank & 0x0F; // DW uses 4-8 banks, mask to 4 for safety

        if (pc >= 0xC000) {
            switch (mode) {
                case 0: 
                case 1: return (bank_reg & 0x0E) | 0x01; // High 16KB of 32KB chunk
                case 2: return bank_reg;                // Swappable
                case 3: default: return 0x03;           // Fixed to Bank 3
            }
        } else if (pc >= 0x8000) {
            switch (mode) {
                case 0: 
                case 1: return (bank_reg & 0x0E);       // Low 16KB of 32KB chunk
                case 2: return 0x00;                    // Fixed to Bank 0
                case 3: default: return bank_reg;       // Swappable
            }
        }
        return 0xFF; // Outside of PRG ROM
    }

    void execute() {
        /**
         * The loop continues as long as we have cycles and are in ROM.
         * The yield logic in the recompiled asm_to_c.py ensures that
         * tight loops (like waiting for Sprite 0 or VBlank) return here
         * so the PPU can update.
         */
        while (cycles_to_run > 0) {
            if (reg_PC < 0x8000) {
                // Fallback for code running in RAM ($0000-$07FF)
                // Dragon Warrior rarely does this, but it's vital for safety.
                // We increment PC and burn a cycle to prevent infinite lock.
                reg_PC++;
                add_cycles(1); 
                continue;
            }

            uint8_t target_bank = get_active_bank(reg_PC);
            
            // Dragon Warrior (iNES Mapper 1) is typically 64KB (4 banks)
            // Mask to prevent out-of-bounds array access.
            target_bank &= 0x03; 

            // Jump into the recompiled C++ logic for that specific bank.
            bank_dispatch_table[target_bank](reg_PC);
        }
    }
}
