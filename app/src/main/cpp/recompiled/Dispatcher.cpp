#include "cpu_shared.h"
#include "../MapperMMC1.h"
#include <android/log.h>
#include <sched.h>

#define LOG_TAG "DWA_DISPATCHER"

extern MapperMMC1 mapper;
extern uint8_t cpu_ram[0x0800];

// Define recompiled bank entry points
namespace Bank00 { void execute(); }
namespace Bank01 { void execute(); }
namespace Bank02 { void execute(); }
namespace Bank03 { void execute(); }

namespace Dispatcher {

    // volatile to ensure the UI/PPU thread and Engine thread see the same value
    volatile bool nmi_pending = false;

    void request_nmi() {
        nmi_pending = true;
    }

    /**
     * Executes the recompiled C++ function corresponding to the 
     * current PRG-ROM bank mapped by the MMC1.
     */
    inline void call_bank(uint8_t bank_index) {
        switch (bank_index & 0x03) {
            case 0: Bank00::execute(); break;
            case 1: Bank01::execute(); break;
            case 2: Bank02::execute(); break;
            case 3: Bank03::execute(); break;
        }
    }

    void execute() {
        // --- 1. HARDWARE INTERRUPT HANDLING ---
        if (nmi_pending) {
            nmi_pending = false;

            // 6502 NMI hardware sequence
            push_stack((uint8_t)(reg_PC >> 8));
            push_stack((uint8_t)(reg_PC & 0xFF));
            
            /* * CRITICAL BITMASK FIX:
             * When pushing P to stack during NMI:
             * - Bit 4 (Break) must be 0
             * - Bit 5 (Unused) must be 1
             */
            push_stack((reg_P & ~0x10) | 0x20); 

            reg_P |= FLAG_I; // Set Interrupt Inhibit

            // Load NMI Vector from the end of Bank 3 ($FFFA)
            uint16_t lo = bus_read(0xFFFA);
            uint16_t hi = bus_read(0xFFFB);
            reg_PC = (hi << 8) | lo;
            return; 
        }

        uint16_t entry_pc = reg_PC;

        // --- 2. SPECIAL CASE: ENGINE WAIT LOOP ---
        // Dragon Warrior loops at $FF74 waiting for the NMI to set $2D to 1.
        // We yield to the Android OS to prevent 100% CPU usage during this idle time.
        if (reg_PC == 0xFF74 && !nmi_pending) {
            sched_yield(); 
        }

        

        // --- 3. PRG BANKING LOGIC ---
        uint8_t prg_mode = mapper.get_prg_mode();
        uint8_t bank_reg = mapper.prg_bank & 0x0F;

        if (reg_PC >= 0x8000) {
            switch (prg_mode) {
                case 0:
                case 1:
                    // 32KB Mode
                    if (reg_PC < 0xC000) call_bank(bank_reg & 0x0E);
                    else call_bank((bank_reg & 0x0E) | 0x01);
                    break;
                case 2:
                    // Fix first bank at $8000
                    if (reg_PC < 0xC000) Bank00::execute();
                    else call_bank(bank_reg);
                    break;
                case 3:
                default:
                    // Fix last bank at $C000 (Standard for most MMC1 games)
                    if (reg_PC < 0xC000) call_bank(bank_reg);
                    else Bank03::execute();
                    break;
            }
        } 
        else if (reg_PC < 0x2000) {
            // Internal RAM Execution (Trampolines)
            Bank03::execute(); 
        }
        else {
            // CPU TRAP: PC is in I/O space or unmapped memory ($2000-$5FFF)
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "CPU TRAP at 0x%04X. Forcing Reset.", reg_PC);
            uint16_t reset_lo = bus_read(0xFFFC);
            uint16_t reset_hi = bus_read(0xFFFD);
            reg_PC = (reset_hi << 8) | reset_lo;
        }

        // --- 4. SAFETY GAP DETECTION ---
        if (reg_PC == entry_pc) {
            __android_log_print(ANDROID_LOG_WARN, LOG_TAG, "RECOMPILER GAP: 0x%04X", entry_pc);
            reg_PC++; // Force advance to prevent thread lockup
        }
    }
}
