#include "cpu_shared.h"
#include "../MapperMMC1.h"
#include <android/log.h>

#define LOG_TAG "DWA_DISPATCHER"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

extern MapperMMC1 mapper;

// --- RECOMPILED BANK ENTRY POINTS ---
// These namespaces contain the actual translated 6502 code for each 16KB bank.
namespace Bank00 { void execute(); }
namespace Bank01 { void execute(); }
namespace Bank02 { void execute(); }
namespace Bank03 { void execute(); }

// --- PERFORMANCE OPTIMIZATION: JUMP TABLE ---
// Using a function pointer table avoids 'switch' overhead and makes the dispatcher 
// significantly faster during intense combat or scrolling.
typedef void (*BankExecFn)();
static const BankExecFn bank_dispatch_table[] = {
    Bank00::execute, 
    Bank01::execute, 
    Bank02::execute, 
    Bank03::execute
};

namespace Dispatcher {
    /**
     * Executes the recompiled instruction at the current reg_PC.
     * This dispatcher implements full MMC1 PRG Banking logic.
     */
    void execute() {
        // Cache mapper state for this cycle to ensure atomicity
        const uint8_t ctrl = mapper.control;
        const uint8_t mode = (ctrl >> 2) & 0x03;
        const uint8_t bank_reg = mapper.prg_bank & 0x03;
        
        uint8_t target_index = 0;

        // --- REGION: $C000 - $FFFF (Fixed or Switchable) ---
        if (reg_PC >= 0xC000) {
            if (mode <= 1) {
                // Modes 0 & 1: 32KB Switching. $C000 is the upper 16KB of the 32KB bank.
                // In 32KB mode, the low bit of the bank register is ignored.
                target_index = (mapper.prg_bank & 0x02) | 0x01;
            } else if (mode == 2) {
                // Mode 2: $8000 is fixed to first, $C000 is switchable.
                target_index = bank_reg;
            } else {
                // Mode 3: $8000 is switchable, $C000 is fixed to last bank (Bank 3).
                target_index = 3; 
            }
            bank_dispatch_table[target_index]();
        } 
        
        // --- REGION: $8000 - $BFFF (Switchable or Fixed) ---
        else if (reg_PC >= 0x8000) {
            if (mode <= 1) {
                // Modes 0 & 1: 32KB Switching. $8000 is the lower 16KB.
                target_index = (mapper.prg_bank & 0x02);
            } else if (mode == 2) {
                // Mode 2: $8000 is fixed to the first bank (Bank 0).
                target_index = 0;
            } else {
                // Mode 3: $8000 is switchable.
                target_index = bank_reg;
            }
            bank_dispatch_table[target_index]();
        }

        // --- REGION: RAM/SRAM EXECUTION ($0000 - $7FFF) ---
        else {
            // Recompiled engines typically don't execute from RAM because it changes.
            // If the PC lands here, we increment to avoid hard-locking the engine.
            // In a production build, you might hook an interpreter here.
            reg_PC++;
            
            // Periodically log if the CPU "escapes" into RAM unexpectedly
            static int ram_warn_count = 0;
            if (++ram_warn_count % 1000 == 0) {
                LOGE("Dispatcher Warning: PC=$%04X is in RAM/SRAM. No recompiled code available.", reg_PC);
            }
        }
    }
}
