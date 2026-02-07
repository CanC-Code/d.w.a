#include "cpu_shared.h"
#include "../MapperMMC1.h"
#include <android/log.h>

#define LOG_TAG "DWA_DISPATCHER"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// External mapper instance
extern MapperMMC1 mapper;

// Forward declarations for bank namespaces
namespace Bank00 { void execute(); }
namespace Bank01 { void execute(); }
namespace Bank02 { void execute(); }
namespace Bank03 { void execute(); }

namespace Dispatcher {
    void execute() {
        // Dragon Warrior uses MMC1 Mode 3:
        // $8000-$BFFF: Switchable bank (controlled by mapper.prg_bank)
        // $C000-$FFFF: Fixed to last bank (Bank 3)
        
        if (reg_PC >= 0xC000) {
            // Fixed bank region
            Bank03::execute();
        }
        else if (reg_PC >= 0x8000) {
            // Switchable bank region
            uint8_t bank = mapper.prg_bank & 0x03;
            
            switch (bank) {
                case 0: Bank00::execute(); break;
                case 1: Bank01::execute(); break;
                case 2: Bank02::execute(); break;
                case 3: Bank03::execute(); break;
                default:
                    LOGE("Invalid bank %d at PC=$%04X", bank, reg_PC);
                    reg_PC++;
                    break;
            }
        }
        else {
            // Execution in RAM/IO space (should never happen with recompiled code)
            static int error_count = 0;
            if (error_count++ < 10) {
                LOGE("Execution in RAM/IO at PC=$%04X", reg_PC);
            }
            reg_PC++;
        }
    }
}
