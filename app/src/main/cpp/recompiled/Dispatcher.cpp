#include "cpu_shared.h"
#include "../MapperMMC1.h"
#include <android/log.h>

#define LOG_TAG "DWA_DISPATCHER"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern MapperMMC1 mapper;

// --- RECOMPILED BANK ENTRY POINTS ---
// These are the actual recompiled function bodies (generated from 6502 ASM).
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
     * Executes the recompiled logic based on the current Bank Mode and PC.
     * Fixed: Added safety bounds and PC-relative execution.
     */
    void execute() {
        uint8_t mode = (mapper.control >> 2) & 0x03;
        uint8_t bank_reg = mapper.prg_bank & 0x0F; // Use 4 bits for bank select
        uint8_t target_index = 0;

        // --- MMC1 PRG BANK SELECTION LOGIC ---
        if (reg_PC >= 0x8000) {
            if (reg_PC >= 0xC000) {
                // Window: $C000 - $FFFF
                switch (mode) {
                    case 0: case 1: // 32KB Mode
                        target_index = (bank_reg & 0x0E) | 0x01; break;
                    case 2: // $C000 is Switchable
                        target_index = bank_reg; break;
                    case 3: // $C000 is FIXED to Last Bank
                    default:
                        target_index = 3; break; 
                }
            } else {
                // Window: $8000 - $BFFF
                switch (mode) {
                    case 0: case 1: // 32KB Mode
                        target_index = (bank_reg & 0x0E); break;
                    case 2: // $8000 is FIXED to First Bank
                        target_index = 0; break;
                    case 3: // $8000 is Switchable
                    default:
                        target_index = bank_reg; break;
                }
            }

            // Ensure we don't index out of bounds if the ROM is small
            target_index %= 4; 

            // Execute recompiled code at the specific PC
            // We pass reg_PC so the bank knows where to start the jump table inside.
            bank_dispatch_table[target_index](reg_PC);
        } 
        
        // --- RAM EXECUTION SAFETY ---
        else {
            // If the game jumps to RAM (e.g. specialized music code), we must interpret.
            // For now, we increment to prevent a hard hang, but log the event.
            static uint32_t ram_exec_logs = 0;
            if (ram_exec_logs++ < 10) {
                LOGE("CRITICAL: Attempted execution in RAM at $%04X. Interpreter required.", reg_PC);
            }
            reg_PC++; 
        }
    }
}
