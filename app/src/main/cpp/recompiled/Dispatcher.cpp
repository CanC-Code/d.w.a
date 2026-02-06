#include "cpu_shared.h"
#include "../MapperMMC1.h"
#include <android/log.h>

#define LOG_TAG "DWA_DISPATCHER"

// Access the global mapper defined in native-lib.cpp
extern MapperMMC1 mapper;

// Forward declarations for the namespaced bank functions.
namespace Bank00 { void execute(); }
namespace Bank01 { void execute(); }
namespace Bank02 { void execute(); }
namespace Bank03 { void execute(); }

namespace Dispatcher {

    // Helper to execute based on a specific bank index
    void call_bank(uint8_t bank_index) {
        switch (bank_index & 0x03) {
            case 0: Bank00::execute(); break;
            case 1: Bank01::execute(); break;
            case 2: Bank02::execute(); break;
            case 3: Bank03::execute(); break;
        }
    }

    void execute() {
        uint16_t entry_pc = reg_PC;

        // --- MMC1 PRG Banking Logic ---
        // We must respect the 'control' register bits 2 and 3.
        uint8_t prg_mode = (mapper.control >> 2) & 0x03;
        uint8_t bank_reg = mapper.prg_bank & 0x0F;

        if (reg_PC >= 0x8000) {
            switch (prg_mode) {
                case 0:
                case 1:
                    // 32KB Mode: $8000 switches 32KB at a time (ignores low bit of bank_reg)
                    if (reg_PC < 0xC000) call_bank(bank_reg & 0x0E);
                    else call_bank(bank_reg | 0x01);
                    break;

                case 2:
                    // Fixed $8000 (Bank 00), Switchable $C000
                    if (reg_PC < 0xC000) Bank00::execute();
                    else call_bank(bank_reg);
                    break;

                case 3:
                default:
                    // Switchable $8000, Fixed $C000 (Bank 03) - Standard for DW1
                    if (reg_PC < 0xC000) call_bank(bank_reg);
                    else Bank03::execute();
                    break;
            }
        } 
        else if (reg_PC < 0x2000) {
            // Some games execute small routines out of RAM ($0000-$07FF)
            // If the recompiler didn't catch these, the game will hang here.
            // For now, we increment to avoid lockup, but this is a warning sign.
            reg_PC++;
        }

        // --- THE "SKIP" DETECTION ---
        // If reg_PC did not change, the switch-case in the Bank namespace 
        // does not have a 'case' for this address.
        if (reg_PC == entry_pc) {
            // Log the missing address so you can add it to your recompiled source
            __android_log_print(ANDROID_LOG_WARN, LOG_TAG, "MISSING ADDR: 0x%04X (Bank: %d, Mode: %d)", 
                                entry_pc, (mapper.prg_bank & 0x03), prg_mode);
            
            // Failsafe: step one byte. 
            // Note: If this is the middle of an instruction, it will likely crash later.
            reg_PC++;
        }
    }
}
