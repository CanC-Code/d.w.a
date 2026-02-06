#include "cpu_shared.h"
#include "../MapperMMC1.h"

// Access the global mapper defined in native-lib.cpp
extern MapperMMC1 mapper;

// Forward declarations for your bank functions
namespace Bank00 { void execute(); }
namespace Bank01 { void execute(); }
namespace Bank02 { void execute(); }
namespace Bank03 { void execute(); }

namespace Dispatcher {
    void execute() {
        // --- NES Memory Map Dispatching ---

        // 1. $C000-$FFFF: Fixed PRG Bank (Always Bank 03 in Dragon Warrior)
        if (reg_PC >= 0xC000) {
            Bank03::execute();
        } 
        
        // 2. $8000-$BFFF: Swappable PRG Bank Slot
        else if (reg_PC >= 0x8000) {
            // Get the current bank index from the mapper.
            // MMC1 has different modes, but Dragon Warrior typically switches 16KB slots.
            uint8_t current_bank = mapper.prg_bank % 4;

            switch (current_bank) {
                case 0: Bank00::execute(); break;
                case 1: Bank01::execute(); break;
                case 2: Bank02::execute(); break;
                case 3: Bank03::execute(); break; 
                default: Bank00::execute(); break;
            }
        } 
        
        // 3. $0000-$7FFF: RAM, Registers, and Save RAM
        else {
            // Recompiled code only exists for ROM. If the game jumps to RAM ($0000-$07FF)
            // or Save RAM ($6000-$7FFF) to execute code, we must increment PC 
            // manually or implement an interpreter fallback to avoid infinite loops.
            LOG_CPU("Attempted execution at RAM address: 0x%04X", reg_PC);
            reg_PC++; 
        }
    }
}
