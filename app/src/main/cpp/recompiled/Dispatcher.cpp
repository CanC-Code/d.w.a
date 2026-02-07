#include "cpu_shared.h"
#include "../MapperMMC1.h"
#include <android/log.h>

#define LOG_TAG "DWA_DISPATCHER"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// External reference to the global mapper defined in native-lib.cpp
extern MapperMMC1 mapper;

// --- RECOMPILED BANK ENTRY POINTS ---
namespace Bank00 { void execute_at(uint16_t pc); }
namespace Bank01 { void execute_at(uint16_t pc); }
namespace Bank02 { void execute_at(uint16_t pc); }
namespace Bank03 { void execute_at(uint16_t pc); }

typedef void (*BankExecFn)(uint16_t);

// Table of function pointers for the 4 available 16KB banks
static const BankExecFn bank_dispatch_table[] = {
    Bank00::execute_at,
    Bank01::execute_at,
    Bank02::execute_at,
    Bank03::execute_at
};

namespace Dispatcher {

    void execute_at(uint16_t pc) {
        if (pc >= 0x8000) {
            uint8_t mode = (mapper.control >> 2) & 0x03; // MMC1 PRG Banking Mode
            uint8_t bank_reg = mapper.prg_bank & 0x0F;
            uint8_t target_index = 0;

            if (pc >= 0xC000) {
                // --- Upper Window: $C000 - $FFFF ---
                switch (mode) {
                    case 0:
                    case 1: // 32KB Mode
                        target_index = (bank_reg & 0x0E) | 0x01;
                        break;
                    case 2: // $C000 is Switchable
                        target_index = bank_reg;
                        break;
                    case 3:
                    default: // $C000 is Fixed to the Last Bank
                        target_index = 3;
                        break;
                }
            } else {
                // --- Lower Window: $8000 - $BFFF ---
                switch (mode) {
                    case 0:
                    case 1: // 32KB Mode
                        target_index = (bank_reg & 0x0E);
                        break;
                    case 2: // $8000 is Fixed to the First Bank
                        target_index = 0;
                        break;
                    case 3:
                    default: // $8000 is Switchable
                        target_index = bank_reg;
                        break;
                }
            }

            target_index &= 0x03; // Safety mask for 64KB ROM
            bank_dispatch_table[target_index](pc);
        } else {
            reg_PC++;
        }
    }

    /**
     * Entry point called by engine_loop() in native-lib.cpp
     */
    void execute() {
        execute_at(reg_PC);
    }
}
