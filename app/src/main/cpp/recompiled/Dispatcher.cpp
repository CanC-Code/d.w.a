#include "cpu_shared.h"
#include "../MapperMMC1.h"
#include <android/log.h>

#define LOG_TAG "DWA_DISPATCHER"

extern MapperMMC1 mapper;
extern uint8_t cpu_ram[0x0800];

// Define recompiled bank entry points
namespace Bank00 { void execute(); }
namespace Bank01 { void execute(); }
namespace Bank02 { void execute(); }
namespace Bank03 { void execute(); }

namespace Dispatcher {

    volatile bool nmi_pending = false;

    void request_nmi() {
        nmi_pending = true;
    }

    void call_bank(uint8_t bank_index) {
        switch (bank_index & 0x03) {
            case 0: Bank00::execute(); break;
            case 1: Bank01::execute(); break;
            case 2: Bank02::execute(); break;
            case 3: Bank03::execute(); break;
        }
    }

    void execute() {
        // --- 1. PRIORITY NMI HANDLING ---
        // If an NMI is pending, we MUST intercept the PC before the next instruction.
        // Dragon Warrior loops at $FF74 waiting for this to fire.
        if (nmi_pending) {
            nmi_pending = false;

            // Save state to stack (Standard 6502 NMI sequence)
            push_stack((uint8_t)(reg_PC >> 8));
            push_stack((uint8_t)(reg_PC & 0xFF));
            push_stack(reg_P);

            // Set the I flag (Interrupt Inhibit)
            reg_P |= FLAG_I; 

            // Load NMI Vector ($FFFA/$FFFB)
            uint16_t lo = bus_read(0xFFFA);
            uint16_t hi = bus_read(0xFFFB);
            reg_PC = (hi << 8) | lo;

            // Log NMI event for debugging
            // __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "NMI Intercept: PC moved to 0x%04X", reg_PC);
            return; 
        }

        uint16_t entry_pc = reg_PC;

        // --- 2. PRG BANKING LOGIC ---
        uint8_t prg_mode = (mapper.control >> 2) & 0x03;
        uint8_t bank_reg = mapper.prg_bank & 0x0F;

        if (reg_PC >= 0x8000) {
            switch (prg_mode) {
                case 0:
                case 1:
                    if (reg_PC < 0xC000) call_bank(bank_reg & 0x0E);
                    else call_bank((bank_reg & 0x0E) | 0x01);
                    break;
                case 2:
                    if (reg_PC < 0xC000) Bank00::execute();
                    else call_bank(bank_reg);
                    break;
                case 3:
                default:
                    if (reg_PC < 0xC000) call_bank(bank_reg);
                    else Bank03::execute();
                    break;
            }
        } 
        else if (reg_PC < 0x2000) {
            // RAM/Internal Execution
            // Many DW routines jump to RAM at $00xx or $01xx for menu transitions.
            // We default to Bank 03 which typically contains the core kernel.
            Bank03::execute(); 
            if (reg_PC == entry_pc) Bank00::execute();
        }

        // --- 3. GAP DETECTION ---
        if (reg_PC == entry_pc) {
            __android_log_print(ANDROID_LOG_WARN, LOG_TAG, 
                "RECOMPILER GAP: 0x%04X (Bank Reg: %d, Mode: %d)", 
                entry_pc, bank_reg, prg_mode);
            
            // Advance PC to prevent infinite loop on unknown opcode
            reg_PC++;
        }
    }
}
