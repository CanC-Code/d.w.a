#include "cpu_shared.h"
#include "../MapperMMC1.h"
#include <android/log.h>

#define LOG_TAG "DWA_DISPATCHER"

extern MapperMMC1 mapper;
extern uint8_t cpu_ram[0x0800];

// Define recompiled bank entry points
// These correspond to the four 16KB PRG-ROM banks in Dragon Warrior
namespace Bank00 { void execute(); }
namespace Bank01 { void execute(); }
namespace Bank02 { void execute(); }
namespace Bank03 { void execute(); }

namespace Dispatcher {

    // Atomic boolean to ensure thread safety between PPU and Engine threads
    volatile bool nmi_pending = false;

    void request_nmi() {
        nmi_pending = true;
    }

    /**
     * Redirects execution to the appropriate recompiled C++ function 
     * based on the mapper's current bank index.
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
        // NMI has priority over instruction execution.
        if (nmi_pending) {
            nmi_pending = false;

            // 6502 NMI Sequence: 
            // 1. Push PC_high, PC_low, and Status (P) to stack.
            // 2. Set Interrupt Inhibit (I).
            // 3. Set PC to address stored at $FFFA.
            push_stack((uint8_t)(reg_PC >> 8));
            push_stack((uint8_t)(reg_PC & 0xFF));
            
            // Note: Bit 4 (B flag) is always pushed as 0 during hardware NMIs
            push_stack(reg_P & ~0x10); 

            reg_P |= 0x04; // Set I flag (Interrupt Inhibit)

            uint16_t lo = bus_read(0xFFFA);
            uint16_t hi = bus_read(0xFFFB);
            reg_PC = (hi << 8) | lo;
            return; 
        }

        uint16_t entry_pc = reg_PC;

        // --- 2. PRG BANKING LOGIC ---
        // MMC1 provides flexible mapping of the $8000-$FFFF address space.
        uint8_t prg_mode = mapper.get_prg_mode();
        uint8_t bank_reg = mapper.prg_bank & 0x0F;

        if (reg_PC >= 0x8000) {
            switch (prg_mode) {
                case 0:
                case 1:
                    // 32KB Mode: $8000 maps to bank_reg (even), $C000 to bank_reg + 1
                    if (reg_PC < 0xC000) call_bank(bank_reg & 0x0E);
                    else call_bank((bank_reg & 0x0E) | 0x01);
                    break;
                case 2:
                    // Fix first bank at $8000, switch second at $C000
                    if (reg_PC < 0xC000) Bank00::execute();
                    else call_bank(bank_reg);
                    break;
                case 3:
                default:
                    // Switch first bank at $8000, fix last bank at $C000 (Dragon Warrior Standard)
                    if (reg_PC < 0xC000) call_bank(bank_reg);
                    else Bank03::execute();
                    break;
            }
        } 
        else if (reg_PC < 0x2000) {
            // RAM Execution: Used for transient code or data-copying routines.
            // We usually default to Bank 03 as it contains the kernel logic 
            // capable of interpreting RAM-based jumps.
            Bank03::execute(); 
        }
        else if (reg_PC >= 0x2000 && reg_PC < 0x6000) {
            // Execution in I/O space or unmapped memory is illegal.
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "CPU TRAP: Execution attempted at I/O address 0x%04X", reg_PC);
            reg_PC = bus_read(0xFFFC) | (bus_read(0xFFFD) << 8); // Force Reset
        }

        // --- 3. SAFETY GAP DETECTION ---
        // If the PC hasn't moved, the recompiled code has "lost" the CPU.
        if (reg_PC == entry_pc) {
            __android_log_print(ANDROID_LOG_WARN, LOG_TAG, 
                "RECOMPILER GAP: 0x%04X (Bank: %d, Mode: %d)", 
                entry_pc, bank_reg, prg_mode);

            // Manual fallback: Read opcode and advance (Very slow, but prevents freezing)
            reg_PC++; 
        }
    }
}
