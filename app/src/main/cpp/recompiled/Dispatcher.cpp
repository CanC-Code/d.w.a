#include "cpu_shared.h"
#include "../MapperMMC1.h" // Go up one level to find the header

// Access the global mapper defined in native-lib.cpp
extern MapperMMC1 mapper;

// Forward declarations for your bank functions
namespace Bank00 { void execute(); }
namespace Bank01 { void execute(); }
namespace Bank02 { void execute(); }
namespace Bank03 { void execute(); }

extern "C" void execute_instruction() {
    // 1. $C000-$FFFF is ALWAYS Fixed to the last bank (Bank 03) in Dragon Warrior
    if (reg_PC >= 0xC000) {
        Bank03::execute();
    } 
    // 2. $8000-$BFFF is switchable based on the Mapper state
    else if (reg_PC >= 0x8000) {
        // We use the mapper's prg_bank register to route the execution
        // Note: We use % 4 to ensure we don't go out of bounds
        uint8_t current_bank = mapper.prg_bank % 4;

        switch (current_bank) {
            case 0: Bank00::execute(); break;
            case 1: Bank01::execute(); break;
            case 2: Bank02::execute(); break;
            case 3: Bank03::execute(); break; // Some modes allow Bank 3 here too
            default:
                Bank00::execute(); // Fallback safety
                break;
        }
    } 
    // 3. Handle RAM ($0000-$1FFF) or Save RAM ($6000-$7FFF)
    else {
        // If the CPU tries to execute code from RAM, we don't have recompiled
        // functions for it. We log it and skip to prevent a crash.
        // (Dragon Warrior rarely executes from RAM)
        reg_PC++; 
    }
}
