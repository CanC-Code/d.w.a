#include "cpu_shared.h"
#include "../MapperMMC1.h"
#include <android/log.h>

#define LOG_TAG "DWA_DISPATCHER"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern MapperMMC1 mapper;

namespace Bank00 { void execute(); }
namespace Bank01 { void execute(); }
namespace Bank02 { void execute(); }
namespace Bank03 { void execute(); }

namespace Dispatcher {
    void execute() {
        if (reg_PC >= 0xC000) {
            // FIXED BANK: Always Bank 03 in Mode 3
            Bank03::execute();
        }
        else if (reg_PC >= 0x8000) {
            // SWITCHABLE BANK: Determine current bank via mapper control register
            // We use the mapper's internal state directly to fix the "no member" error
            uint8_t mode = (mapper.control >> 2) & 0x03;
            uint8_t current_bank;
            
            if (mode == 2) current_bank = 0; // Fixed to first
            else current_bank = mapper.prg_bank & 0x03;

            switch (current_bank) {
                case 0: Bank00::execute(); break;
                case 1: Bank01::execute(); break;
                case 2: Bank02::execute(); break;
                case 3: Bank03::execute(); break;
                default:
                    LOGE("MMC1 Error: Invalid bank %d at PC=$%04X", current_bank, reg_PC);
                    reg_PC++; 
                    break;
            }
        }
        else {
            reg_PC++;
        }
    }
}
