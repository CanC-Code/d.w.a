#include "cpu_shared.h"
#include "../MapperMMC1.h"
#include <android/log.h>

#define LOG_TAG "DWA_DISPATCHER"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern MapperMMC1 mapper;
extern int32_t cycles_to_run; // From native-lib.cpp

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

    inline uint8_t get_active_bank(uint16_t pc) {
        uint8_t mode = (mapper.control >> 2) & 0x03;
        uint8_t bank_reg = mapper.prg_bank & 0x0F;

        if (pc >= 0xC000) {
            switch (mode) {
                case 0: case 1: return (bank_reg & 0x0E) | 0x01;
                case 2:         return bank_reg; 
                case 3: default: return 3; // Fixed to last bank
            }
        } else { // $8000 - $BFFF
            switch (mode) {
                case 0: case 1: return (bank_reg & 0x0E);
                case 2:         return 0; // Fixed to first bank
                case 3: default: return bank_reg;
            }
        }
    }

    void execute() {
        /**
         * CRITICAL LOGIC: Dragon Warrior uses "Wait for VBlank" loops.
         * We MUST return to the main engine loop frequently so PPU flags 
         * can be updated. If we don't, the recompiled C++ will hang 
         * in a high-speed loop checking a flag that never changes.
         */
        
        while (reg_PC >= 0x8000 && cycles_to_run > 0) {
            uint8_t target_bank = get_active_bank(reg_PC);
            
            // Mask for safety (DW is 64KB = 4 banks)
            target_bank &= 0x03; 

            // Call recompiled logic
            bank_dispatch_table[target_bank](reg_PC);

            // Note: The recompiled bank code MUST be generated to check 
            // for cycle exhaustion or return frequently on branches.
        }

        // Handle RAM execution fallback
        if (reg_PC < 0x8000) {
            // Very rare in DW, but prevents a hang if the game jumps to RAM
            reg_PC++; 
            cycles_to_run--;
        }
    }
}
