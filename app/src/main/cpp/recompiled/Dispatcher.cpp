#include <cstdint>

// Forward declarations for all bank execute functions
namespace Bank00 { void execute_at(uint16_t pc); }
namespace Bank01 { void execute_at(uint16_t pc); }
namespace Bank02 { void execute_at(uint16_t pc); }
namespace Bank03 { void execute_at(uint16_t pc); }

// External mapper function to get current PRG bank
extern uint8_t mapper_read(uint16_t addr);

namespace recompiled {
    void execute_at(uint16_t pc) {
        // Determine which bank to execute based on PC
        if (pc >= 0xC000) {
            // $C000-$FFFF is always Bank03 (last 16KB, fixed)
            Bank03::execute_at(pc);
        } else if (pc >= 0x8000) {
            // $8000-$BFFF: need to determine current switchable bank
            // Read a byte to determine which bank is mapped
            // For Dragon Warrior, the banks are typically:
            // Bank00: 0x8000-0xBFFF (switchable)
            // Bank01: 0x8000-0xBFFF (switchable)
            // Bank02: 0x8000-0xBFFF (switchable)
            // Bank03: 0xC000-0xFFFF (fixed)
            
            // Simple heuristic: check a unique byte in each bank
            // This is a temporary solution; ideally mapper should track this
            uint8_t marker = mapper_read(0x8000);
            
            // Dragon Warrior bank detection (based on typical patterns)
            // You may need to adjust these values based on actual ROM
            if (marker == 0x78 || marker == 0x4C) {
                Bank00::execute_at(pc);
            } else if (marker == 0xA9 || marker == 0xAD) {
                Bank01::execute_at(pc);
            } else {
                Bank02::execute_at(pc);
            }
        } else {
            // Invalid PC (should never happen)
            // Just increment and return
            extern uint16_t reg_PC;
            reg_PC++;
        }
    }
}