#include <jni.h>
#include <string>
#include <vector>
#include <fstream>
#include <android/log.h>

// --- 6502 CPU Registers ---
uint16_t reg_PC;        // Program Counter
uint8_t  reg_A, reg_X, reg_Y; // Accumulator, X, Y
uint8_t  reg_S;         // Stack Pointer
uint8_t  reg_P;         // Status Register (Flags)

// Status Flag Bits
#define FLAG_C (1 << 0) // Carry
#define FLAG_Z (1 << 1) // Zero
#define FLAG_I (1 << 2) // Interrupt Disable
#define FLAG_D (1 << 3) // Decimal
#define FLAG_B (1 << 4) // Break
#define FLAG_U (1 << 5) // Unused
#define FLAG_V (1 << 6) // Overflow
#define FLAG_N (1 << 7) // Negative

// --- Helper Functions for Flags ---
void set_zn(uint8_t val) {
    if (val == 0) reg_P |= FLAG_Z; else reg_P &= ~FLAG_Z;
    if (val & 0x80) reg_P |= FLAG_N; else reg_P &= ~FLAG_N;
}

// --- The CPU Step Function ---
// This executes one instruction and returns the number of cycles used.
int step_cpu() {
    uint8_t opcode = read_byte(reg_PC++);
    
    switch (opcode) {
        // --- Examples of instructions found in Bank03.asm ---
        
        case 0x78: // SEI (Set Interrupt Disable)
            reg_P |= FLAG_I;
            return 2;

        case 0xD8: // CLD (Clear Decimal Mode)
            reg_P &= ~FLAG_D;
            return 2;

        case 0xA9: // LDA Immediate
        {
            reg_A = read_byte(reg_PC++);
            set_zn(reg_A);
            return 2;
        }

        case 0x8D: // STA Absolute
        {
            uint16_t low = read_byte(reg_PC++);
            uint16_t high = read_byte(reg_PC++);
            write_byte((high << 8) | low, reg_A);
            return 4;
        }

        case 0x20: // JSR (Jump to Subroutine)
        {
            uint16_t low = read_byte(reg_PC++);
            uint16_t high = read_byte(reg_PC++);
            uint16_t target = (high << 8) | low;
            uint16_t ret_addr = reg_PC - 1;
            write_byte(0x0100 + reg_S--, (ret_addr >> 8) & 0xFF);
            write_byte(0x0100 + reg_S--, ret_addr & 0xFF);
            reg_PC = target;
            return 6;
        }

        case 0x60: // RTS (Return from Subroutine)
        {
            uint16_t low = read_byte(0x0100 + ++reg_S);
            uint16_t high = read_byte(0x0100 + ++reg_S);
            reg_PC = ((high << 8) | low) + 1;
            return 6;
        }

        default:
            LOGE("Unknown Opcode: 0x%02X at PC: 0x%04X", opcode, reg_PC - 1);
            return 1;
    }
}

// --- Boot/Reset Logic ---
void reset_cpu() {
    // According to Header.asm and Bank03.asm, the reset vector is at $FFFC
    uint16_t low = read_byte(0xFFFC);
    uint16_t high = read_byte(0xFFFD);
    reg_PC = (high << 8) | low;
    
    reg_A = reg_X = reg_Y = 0;
    reg_S = 0xFD; // Initial stack pointer
    reg_P = FLAG_I | FLAG_U;
    
    LOGI("CPU Reset. Starting execution at: 0x%04X", reg_PC);
}
