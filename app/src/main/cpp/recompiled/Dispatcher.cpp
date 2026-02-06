#include "cpu_shared.h"
#include "../MapperMMC1.h"

// Access the global mapper defined in native-lib.cpp
extern MapperMMC1 mapper;

// Forward declarations for the namespaced bank functions.
namespace Bank00 { void execute(); }
namespace Bank01 { void execute(); }
namespace Bank02 { void execute(); }
namespace Bank03 { void execute(); }

namespace Dispatcher {
    void execute() {
        uint16_t entry_pc = reg_PC;

        // --- MMC1 PRG Bank Mapping (Mode 3) ---
        // Dragon Warrior I (NES) Memory Map:
        // $8000-$BFFF: Switchable Bank (mapper.prg_bank)
        // $C000-$FFFF: Fixed Bank (Bank 03)

        if (reg_PC >= 0xC000) {
            // Fixed Bank 3 (Most of the game's core engine and NMI reside here)
            Bank03::execute();
        } 
        else if (reg_PC >= 0x8000) {
            // Swappable Bank Slot
            // We use & 0x03 to safely wrap 4 banks (64KB total)
            uint8_t current_bank = mapper.prg_bank & 0x03;

            switch (current_bank) {
                case 0: Bank00::execute(); break;
                case 1: Bank01::execute(); break;
                case 2: Bank02::execute(); break;
                case 3: Bank03::execute(); break; 
            }
        } 
        else {
            // If the PC falls into RAM ($0000-$1FFF) or I/O ($2000-$401F),
            // it's likely a stack return error or uninitialized pointer.
            // We force a increment to prevent the emulator thread from hanging.
            reg_PC++;
        }

        // --- THE "STUCK" CHECK ---
        // If the Bank::execute() function didn't change the PC, we have a 
        // hole in the recompilation. We must increment to find the next valid instruction.
        if (reg_PC == entry_pc) {
            reg_PC++;
        }
    }
}
