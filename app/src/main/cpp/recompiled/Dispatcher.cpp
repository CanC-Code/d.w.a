#include "cpu_shared.h"
#include "../MapperMMC1.h"

// Access the global mapper defined in native-lib.cpp
extern MapperMMC1 mapper;

// Forward declarations for the namespaced bank functions.
// In a native port, each Bank namespace contains a switch-case 
// mapping the 6502 PC to a recompiled C function.
namespace Bank00 { void execute(); }
namespace Bank01 { void execute(); }
namespace Bank02 { void execute(); }
namespace Bank03 { void execute(); }

namespace Dispatcher {
    void execute() {
        // Cache the entry PC to check for execution progress
        uint16_t entry_pc = reg_PC;

        // --- MMC1 PRG Bank Mapping (Mode 3 logic) ---
        // $8000-$BFFF: Swappable Bank
        // $C000-$FFFF: Fixed to Last Bank (Bank 03)
        
        

        if (reg_PC >= 0xC000) {
            // Fixed Bank 03: Contains the Reset vector and NMI handler.
            // This is the "Kernel" of the game.
            Bank03::execute();
        } 
        else if (reg_PC >= 0x8000) {
            // Swappable Bank Slot: MMC1 Bank Register determines which code is visible.
            // Dragon Warrior 1 is 64KB (4 banks of 16KB).
            uint8_t current_bank = mapper.prg_bank & 0x03;

            switch (current_bank) {
                case 0: Bank00::execute(); break;
                case 1: Bank01::execute(); break;
                case 2: Bank02::execute(); break;
                case 3: Bank03::execute(); break; 
                default: Bank03::execute(); break; // Failsafe
            }
        } 
        else {
            // Execution in RAM ($0000-$1FFF) or I/O space is invalid for recompiled code.
            // This usually happens during a corrupted stack return (RTS).
            reg_PC++; 
        }

        // --- THE NATIVE "STEP" FAILSAFE ---
        // If reg_PC did not change, it means the specific address was not 
        // accounted for in the recompiled Bank::execute() switch statement.
        // We increment the PC to find the next valid entry point and avoid a thread hang.
        if (reg_PC == entry_pc) {
            reg_PC++;
        }
    }
}
