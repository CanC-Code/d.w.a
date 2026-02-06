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

    // Internal flag to track if an NMI is pending from the PPU
    bool nmi_pending = false;

    void request_nmi() {
        nmi_pending = true;
    }

    /**
     * Helper to route execution to the correct C++ bank.
     * Dragon Warrior 1 (NES) = 64KB PRG = 4 banks of 16KB.
     */
    void call_bank(uint8_t bank_index) {
        switch (bank_index & 0x03) { // Ensure index is within 0-3
            case 0: Bank00::execute(); break;
            case 1: Bank01::execute(); break;
            case 2: Bank02::execute(); break;
            case 3: Bank03::execute(); break;
        }
    }

    void execute() {
        // --- 1. NMI INTERRUPT HANDLING ---
        // NMIs occur during VBlank. DW1 uses this to update the frame counter
        // and sprite data. If we don't handle this, the screen stays black.
        if (nmi_pending) {
            nmi_pending = false;

            // 6502 Hardware: Push PC and Status to stack before jumping
            push_stack((uint8_t)(reg_PC >> 8));
            push_stack((uint8_t)(reg_PC & 0xFF));
            push_stack(reg_P);

            // NMIs are non-maskable, but the software still sets the I flag
            reg_P |= 0x04; // Set Interrupt Inhibit (bit 2)

            // Jump to the NMI Vector ($FFFA)
            uint16_t lo = bus_read(0xFFFA);
            uint16_t hi = bus_read(0xFFFB);
            reg_PC = (hi << 8) | lo;

            return; 
        }

        uint16_t entry_pc = reg_PC;

        // --- 2. MMC1 PRG BANKING LOGIC ---
        // MMC1 has different banking modes (32KB or 16KB switchable)
        uint8_t prg_mode = (mapper.control >> 2) & 0x03;
        uint8_t bank_reg = mapper.prg_bank & 0x0F;

        if (reg_PC >= 0x8000) {
            switch (prg_mode) {
                case 0:
                case 1:
                    // 32KB Switching Mode
                    // Banks are switched in pairs (0&1 or 2&3)
                    if (reg_PC < 0xC000) call_bank(bank_reg & 0x0E);
                    else call_bank((bank_reg & 0x0E) | 0x01);
                    break;

                case 2:
                    // Fix first bank at $8000 (Bank 0), switch bank at $C000
                    if (reg_PC < 0xC000) Bank00::execute();
                    else call_bank(bank_reg);
                    break;

                case 3:
                default:
                    // Switch bank at $8000, fix last bank at $C000 (Bank 3)
                    // This is the standard mode for Dragon Warrior.
                    if (reg_PC < 0xC000) call_bank(bank_reg);
                    else Bank03::execute();
                    break;
            }
        } 
        else if (reg_PC >= 0x0000 && reg_PC < 0x0800) {
            // RAM EXECUTION: Dragon Warrior copies small routines to RAM.
            // We search across all recompiled banks to find the RAM-target code.
            Bank03::execute(); 
            if (reg_PC == entry_pc) Bank00::execute(); 
        }

        // --- 3. MISSING ADDRESS FALLBACK ---
        // If the recompiled functions didn't recognize reg_PC, it won't change.
        if (reg_PC == entry_pc) {
            __android_log_print(ANDROID_LOG_WARN, LOG_TAG, 
                "RECOMPILER GAP: 0x%04X not found in Bank %d", 
                entry_pc, (entry_pc < 0xC000 ? (bank_reg % 4) : 3));
            
            // To prevent an infinite freeze, we skip the unknown byte.
            // Note: This usually means a jump table or dynamic jump was missed.
            reg_PC++;
        }
    }
}
