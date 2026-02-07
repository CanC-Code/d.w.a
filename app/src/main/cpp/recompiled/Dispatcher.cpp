#include "cpu_shared.h"
#include "../MapperMMC1.h"
#include <android/log.h>

#define LOG_TAG "DWA_DISPATCHER"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern MapperMMC1 mapper;

// --- RECOMPILED BANK ENTRY POINTS ---
namespace Bank00 { void execute_at(uint16_t pc); }
namespace Bank01 { void execute_at(uint16_t pc); }
namespace Bank02 { void execute_at(uint16_t pc); }
namespace Bank03 { void execute_at(uint16_t pc); }

typedef void (*BankExecFn)(uint16_t);
static const BankExecFn bank_dispatch_table[] = {
    Bank00::execute_at, 
    Bank01::execute_at, 
    Bank02::execute_at, 
    Bank03::execute_at
};

namespace Dispatcher {
    /**
     * Maps the 6502 PC to the correct recompiled bank and executes one instruction.
     * Fixed: Updated signature to execute_at(uint16_t pc) to match native-lib.cpp.
     */
    void execute_at(uint16_t pc) {
        // MMC1 Control Register:
        // Bits 2-3 determine PRG ROM banking mode
        // 0, 1: Switch 32KB at $8000
        // 2: Fix first bank at $8000, switch 16KB at $C000
        // 3: Switch 16KB at $8000, fix last bank at $C000
        uint8_t mode = (mapper.control >> 2) & 0x03;
        uint8_t bank_reg = mapper.prg_bank & 0x0F; 
        uint8_t target_index = 0;

        if (pc >= 0x8000) {
            if (pc >= 0xC000) {
                // Address range $C000 - $FFFF
                switch (mode) {
                    case 0: case 1: 
                        target_index = (bank_reg & 0x0E) | 0x01; break;
                    case 2: 
                        target_index = bank_reg; break;
                    case 3: 
                    default:
                        target_index = 3; break; // Bank 3 is fixed at $C000
                }
            } else {
                // Address range $8000 - $BFFF
                switch (mode) {
                    case 0: case 1: 
                        target_index = (bank_reg & 0x0E); break;
                    case 2: 
                        target_index = 0; break; // Bank 0 is fixed at $8000
                    case 3: 
                    default:
                        target_index = bank_reg; break;
                }
            }

            // Safety check for 64KB ROM (4 banks of 16KB)
            target_index %= 4; 

            // Dispatch to the recompiled bank code
            bank_dispatch_table[target_index](pc);
        } 
        else {
            // Execution in RAM ($0000-$1FFF) or Hardware Registers ($2000-$7FFF)
            // NES games rarely execute code here, but if they do, the recompiler can't reach it.
            reg_PC++; 
        }
    }
}
