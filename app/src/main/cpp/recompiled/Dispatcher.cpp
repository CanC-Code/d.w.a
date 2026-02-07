#include "cpu_shared.h"
#include "../MapperMMC1.h"
#include <android/log.h>

#define LOG_TAG "DWA_DISPATCHER"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// The global mapper instance defined in native-lib.cpp
extern MapperMMC1 mapper;

// Namespaces containing the recompiled functions for each 16KB PRG bank
namespace Bank00 { void execute(); }
namespace Bank01 { void execute(); }
namespace Bank02 { void execute(); }
namespace Bank03 { void execute(); }

namespace Dispatcher {
    /**
     * The Dispatcher is the heart of the recompiled execution.
     * It looks at the current reg_PC and determines which switch-case
     * block (Bank) needs to handle the instruction.
     */
    void execute() {
        // NES PRG-ROM exists in the $8000-$FFFF range.
        
        if (reg_PC >= 0xC000) {
            // FIXED BANK: In MMC1 Mode 3 (standard for Dragon Warrior),
            // the upper 16KB is always fixed to the last bank.
            Bank03::execute();
        }
        else if (reg_PC >= 0x8000) {
            // SWITCHABLE BANK: This 16KB window ($8000-$BFFF) changes
            // based on the mapper's internal PRG bank register.
            uint8_t current_bank = mapper.get_current_prg_bank() & 0x03;

            switch (current_bank) {
                case 0: Bank00::execute(); break;
                case 1: Bank01::execute(); break;
                case 2: Bank02::execute(); break;
                case 3: Bank03::execute(); break;
                default:
                    LOGE("MMC1 Critical Error: Invalid bank %d requested at PC=$%04X", current_bank, reg_PC);
                    // Fallback to prevent infinite stall
                    reg_PC++; 
                    break;
            }
        }
        else {
            // RAM/IO SPACE EXECUTION ($0000-$7FFF)
            // Recompiled code should rarely execute here unless the game
            // has copied code to RAM (e.g., some music drivers or save routines).
            static int ram_exec_logs = 0;
            if (ram_exec_logs < 5) {
                LOGE("Dispatcher Warning: Attempted recompiled execution in RAM space at PC=$%04X", reg_PC);
                ram_exec_logs++;
            }
            
            // In a static recompiler, we cannot easily 'execute' RAM. 
            // We increment PC to attempt to return to ROM space, or this
            // would trigger a software fallback in a hybrid emulator.
            reg_PC++;
        }
    }
}
