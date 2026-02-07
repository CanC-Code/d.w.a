#include "cpu_shared.h"
#include "../MapperMMC1.h"
#include <android/log.h>

// Access the global mapper defined in native-lib.cpp
extern MapperMMC1 mapper;

// Forward declarations for the namespaced bank functions
namespace Bank00 { void execute(); }
namespace Bank01 { void execute(); }
namespace Bank02 { void execute(); }
namespace Bank03 { void execute(); }

namespace Dispatcher {
    /**
     * The Dispatcher translates the current Program Counter (reg_PC) 
     * into a call to the corresponding recompiled C++ bank.
     */
    void execute() {
        static int error_count = 0;

        // --- MMC1 PRG Mode 3 Logic ---
        // $C000 - $FFFF: Always Fixed to the Last Bank (Bank 3)
        if (reg_PC >= 0xC000) {
            Bank03::execute();
            return;
        } 

        // $8000 - $BFFF: Swappable Bank Slot
        if (reg_PC >= 0x8000) {
            // Use get_prg_mode() to ensure we respect the mapper's internal state.
            // Dragon Warrior stays in Mode 3 (0x03), but we handle others for stability.
            uint8_t mode = (mapper.control >> 2) & 0x03;
            uint8_t current_bank;

            if (mode <= 1) {
                // 32KB Mode: Switch 32KB at $8000. 
                // We treat this as Bank 0 and 1 or 2 and 3.
                current_bank = (mapper.prg_bank & 0x0E) % 4;
            } else if (mode == 2) {
                // Fix first bank at $8000, switch second at $C000 (handled above)
                current_bank = 0;
            } else {
                // Mode 3: Switch 16KB at $8000, fix last bank at $C000 (standard)
                current_bank = mapper.prg_bank % 4;
            }

            switch (current_bank) {
                case 0: Bank00::execute(); break;
                case 1: Bank01::execute(); break;
                case 2: Bank02::execute(); break;
                case 3: Bank03::execute(); break; 
                default:
                    if (error_count < 5) {
                        __android_log_print(ANDROID_LOG_WARN, "DWA_DISPATCH", 
                            "Invalid Bank %d at PC=$%04X", current_bank, reg_PC);
                        error_count++;
                    }
                    Bank00::execute(); 
                    break;
            }
            return;
        } 

        // --- Safety Fallback ---
        // If reg_PC is in RAM ($0000-$1FFF), IO ($2000-$5FFF), or Save RAM ($6000-$7FFF)
        // recompiled code cannot "execute" here. We force a return or increment.
        if (error_count < 1) {
            __android_log_print(ANDROID_LOG_ERROR, "DWA_DISPATCH", 
                "Illegal Execution at $%04X (RAM/IO Area)", reg_PC);
            error_count++;
        }
        
        // This is a "Stop" condition. If the CPU jumps to RAM, the recompiled
        // engine cannot continue because there is no C++ code for that memory.
        // We increment PC to prevent an infinite loop, but the game has likely crashed.
        reg_PC++; 
    }
}
