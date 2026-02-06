#include "cpu_shared.h"
#include "../MapperMMC1.h"
#include <android/log.h>

#define LOG_TAG "DWA_DISPATCHER"

extern MapperMMC1 mapper;
extern uint8_t cpu_ram[0x0800];

namespace Bank00 { void execute(); }
namespace Bank01 { void execute(); }
namespace Bank02 { void execute(); }
namespace Bank03 { void execute(); }

namespace Dispatcher {

    // Internal flag to track if an NMI is pending
    bool nmi_pending = false;

    void request_nmi() {
        nmi_pending = true;
    }

    void call_bank(uint8_t bank_index) {
        // Dragon Warrior 1 is 64KB (4 banks of 16KB)
        switch (bank_index & 0x03) {
            case 0: Bank00::execute(); break;
            case 1: Bank01::execute(); break;
            case 2: Bank02::execute(); break;
            case 3: Bank03::execute(); break;
        }
    }

    void execute() {
        // --- NMI TRIGGER LOGIC ---
        // If an NMI was requested by the PPU, we intercept the PC here.
        if (nmi_pending) {
            nmi_pending = false;
            
            // Standard 6502 NMI: Push PC and Status, then jump to vector at $FFFA
            push_stack((uint8_t)(reg_PC >> 8));
            push_stack((uint8_t)(reg_PC & 0xFF));
            push_stack(reg_P);
            
            // Disable IRQ mask (though NMI is non-maskable)
            reg_P |= FLAG_I; 
            
            // Load NMI Vector (Usually points to Bank 03)
            uint16_t lo = bus_read(0xFFFA);
            uint16_t hi = bus_read(0xFFFB);
            reg_PC = (hi << 8) | lo;
            
            __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "NMI Triggered! Jumping to: 0x%04X", reg_PC);
            return; 
        }

        uint16_t entry_pc = reg_PC;

        // --- MMC1 PRG Banking Logic ---
        uint8_t prg_mode = (mapper.control >> 2) & 0x03;
        uint8_t bank_reg = mapper.prg_bank & 0x0F;

        // Address Space $8000-$FFFF
        if (reg_PC >= 0x8000) {
            switch (prg_mode) {
                case 0:
                case 1:
                    // 32KB Mode
                    if (reg_PC < 0xC000) call_bank(bank_reg & 0x0E);
                    else call_bank(bank_reg | 0x01);
                    break;
                case 2:
                    // Fix $8000 to Bank 0, Switch $C000
                    if (reg_PC < 0xC000) Bank00::execute();
                    else call_bank(bank_reg);
                    break;
                case 3:
                default:
                    // Switch $8000, Fix $C000 to Bank 3 (DW1 Default)
                    if (reg_PC < 0xC000) call_bank(bank_reg);
                    else Bank03::execute();
                    break;
            }
        } 
        else if (reg_PC >= 0x0000 && reg_PC < 0x2000) {
            // RAM execution (rare in DW1, but happens in some menu routines)
            // If no case is found in the bank handlers, it will trigger the warning below.
            // We assume Bank03 might contain shared RAM-running logic.
            Bank03::execute();
        }

        // --- MISSING CODE DETECTION ---
        if (reg_PC == entry_pc) {
            // This is the "Gray Screen" killer. If you see this in logs,
            // your recompiler missed a jump target.
            __android_log_print(ANDROID_LOG_WARN, LOG_TAG, "MISSING ADDR: 0x%04X (Bank: %d)", 
                                entry_pc, (reg_PC < 0xC000 ? bank_reg : 3));
            reg_PC++;
        }
    }
}
