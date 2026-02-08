#include "recompiled/cpu_shared.h"
#include "MapperMMC1.h"
#include <android/log.h>

#define LOG_TAG "DWA_DISPATCHER"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern MapperMMC1 mapper;

// Recompiled bank namespaces from Bank00.cpp, Bank01.cpp, etc.
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
        uint8_t bank_reg = mapper.prg_bank & 0x0F; 

        if (pc >= 0xC000) {
            switch (mode) {
                case 0: 
                case 1: return (bank_reg & 0x0E) | 0x01; 
                case 2: return bank_reg;                
                case 3: default: return 0x03;           
            }
        } else if (pc >= 0x8000) {
            switch (mode) {
                case 0: 
                case 1: return (bank_reg & 0x0E);       
                case 2: return 0x00;                    
                case 3: default: return bank_reg;       
            }
        }
        return 0xFF; // Outside of PRG ROM ($0000-$7FFF)
    }

    void execute() {
        // The loop continues as long as we have cycles remaining for this frame.
        while (cycles_to_run > 0) {
            
            // 1. Handle RAM Execution Fallback
            if (reg_PC < 0x8000) {
                // If code is in RAM (usually $0000-$07FF or $6000-$7FFF),
                // we don't have recompiled functions for it.
                // We perform a dummy read/increment to prevent the emulator from hanging.
                // In a full emulator, this would call a standard 6502 interpreter.
                reg_PC++;
                add_cycles(1);
                continue;
            }

            // 2. Determine which 16KB bank is mapped to current PC
            uint8_t target_bank = get_active_bank(reg_PC);

            // 3. Safety Check: Dragon Warrior is 64KB (4 banks). 
            // If the bank is 0xFF or out of bounds, reset PC to a known safe state.
            if (target_bank >= 4) {
                LOGE("Dispatcher Error: Invalid bank %d at PC $%04X", target_bank, reg_PC);
                reg_PC = 0xC000; // Emergency jump to reset vector area
                return;
            }

            // 4. Execute recompiled code
            // The recompiled function will execute instructions until it hits
            // a branch, jump, or return, then it will return here.
            bank_dispatch_table[target_bank](reg_PC);
            
            // 5. Atomic check for engine stop (prevents hang on exit)
            if (!is_running) break;
        }
    }
}
