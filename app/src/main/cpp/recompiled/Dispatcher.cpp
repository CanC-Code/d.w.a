#include "cpu_shared.h"
#include "../MapperMMC1.h"

// Access the global mapper defined in native-lib.cpp
extern MapperMMC1 mapper;

// Forward declarations for the namespaced bank functions
namespace Bank00 { void execute(); }
namespace Bank01 { void execute(); }
namespace Bank02 { void execute(); }
namespace Bank03 { void execute(); }

namespace Dispatcher {
    void execute() {
        // Add bounds checking and logging for debugging
        static bool first_run = true;
        static int error_count = 0;
        
        if (first_run) {
            LOG_CPU("First dispatch: PC=$%04X, PRG bank=%d, Mapper control=$%02X", 
                    reg_PC, mapper.prg_bank, mapper.control);
            first_run = false;
        }
        
        // --- NES Memory Map Dispatching ---
        // Dragon Warrior I uses MMC1 PRG mode 3: 
        // $8000-$BFFF is swappable, $C000-$FFFF is fixed to the last bank.

        // 1. $C000-$FFFF: Fixed PRG Bank (Always Bank 03)
        if (reg_PC >= 0xC000) {
            Bank03::execute();
        } 
        
        // 2. $8000-$BFFF: Swappable PRG Bank Slot
        else if (reg_PC >= 0x8000) {
            // MMC1 can have up to 256KB PRG, but Dragon Warrior is 64KB (4 banks).
            // We use % 4 to ensure we stay within the compiled bank range.
            uint8_t current_bank = mapper.prg_bank % 4;

            switch (current_bank) {
                case 0: Bank00::execute(); break;
                case 1: Bank01::execute(); break;
                case 2: Bank02::execute(); break;
                case 3: Bank03::execute(); break; 
                default:
                    // Fallback to Bank 0 if the mapper is in an unexpected state
                    if (error_count < 10) {
                        LOG_CPU("WARNING: Unexpected bank %d at PC=$%04X, using Bank 0", 
                                current_bank, reg_PC);
                        error_count++;
                    }
                    Bank00::execute(); 
                    break;
            }
        } 
        
        // 3. $0000-$7FFF: RAM, I/O, and Save RAM
        else {
            // Execution should never happen here in recompiled code.
            // If it does, we skip to avoid a freeze and log the event.
            if (error_count < 10) {
                LOG_CPU("ERROR: Execution in RAM/IO at PC=$%04X! Skipping.", reg_PC);
                LOG_CPU("       A=$%02X X=$%02X Y=$%02X P=$%02X S=$%02X", 
                        reg_A, reg_X, reg_Y, reg_P, reg_S);
                error_count++;
            }
            reg_PC++; 
        }
    }
}
