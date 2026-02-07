#include "cpu_shared.h"
#include "../MapperMMC1.h"
#include <android/log.h>

#define LOG_TAG "DWA_DISPATCHER"

// Access the global mapper defined in native-lib.cpp
extern MapperMMC1 mapper;

// Forward declarations for the recompiled C++ bank namespaces.
// Each bank contains the logic for a 16KB chunk of the original NES ROM.
namespace Bank00 { void execute(); }
namespace Bank01 { void execute(); }
namespace Bank02 { void execute(); }
namespace Bank03 { void execute(); }

namespace Dispatcher {
    /**
     * Translates the 6502 Program Counter (reg_PC) into a call to 
     * the corresponding recompiled C++ bank logic.
     */
    void execute() {
        static int error_count = 0;

        // --- MMC1 PRG Memory Mapping ---
        // NES Address Range $8000 - $FFFF
        
        // 1. Fixed Bank Logic ($C000 - $FFFF)
        // In MMC1 Mode 3 (standard), the last 16KB of PRG is fixed at $C000.
        if (reg_PC >= 0xC000) {
            Bank03::execute();
            return;
        } 

        // 2. Swappable Bank Logic ($8000 - $BFFF)
        if (reg_PC >= 0x8000) {
            uint8_t mode = (mapper.control >> 2) & 0x03;
            uint8_t target_bank = 0;

            switch (mode) {
                case 0:
                case 1:
                    // 32KB Switching: $8000 switches a 32KB area.
                    // We map the low 16KB of the 32KB pair.
                    target_bank = (mapper.prg_bank & 0x0E);
                    break;
                case 2:
                    // Mode 2: Fix first bank at $8000, switch bank at $C000.
                    // Since $C000 is handled above, here we are always Bank 0.
                    target_bank = 0;
                    break;
                case 3:
                default:
                    // Mode 3: Switch 16KB at $8000, fix last bank at $C000.
                    // This is the most common mode for Dragon Warrior.
                    target_bank = mapper.prg_bank % 4;
                    break;
            }

            // Route to the correct C++ namespace
            switch (target_bank) {
                case 0: Bank00::execute(); break;
                case 1: Bank01::execute(); break;
                case 2: Bank02::execute(); break;
                case 3: Bank03::execute(); break; 
                default:
                    if (error_count < 10) {
                        __android_log_print(ANDROID_LOG_WARN, LOG_TAG, 
                            "Bank Overflow: Requested %d at PC=$%04X", target_bank, reg_PC);
                        error_count++;
                    }
                    Bank00::execute(); // Emergency fallback
                    break;
            }
            return;
        } 

        // --- 3. Illegal Execution Area ($0000 - $7FFF) ---
        // If reg_PC points here, it means the game tried to jump into RAM or I/O space.
        // Recompiled code does not exist for these addresses.
        if (error_count < 5) {
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, 
                "CRITICAL: Jump to Non-Code Area $%04X", reg_PC);
            error_count++;
        }

        // To prevent a hard CPU lock/hang, we force the PC forward.
        // This will likely cause the game to crash, but allows the UI to stay responsive.
        reg_PC++; 
        is_running = false; // Stop the engine to prevent log spamming
    }
}
