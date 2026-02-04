// Helper macros for flag management
#define SET_ZN(val) reg_P = (reg_P & ~(FLAG_Z | FLAG_N)) | ((val == 0) ? FLAG_Z : 0) | (val & 0x80)

int step_cpu() {
    uint8_t op = read_byte(reg_PC++);
    uint16_t addr = 0;
    uint8_t temp = 0;

    switch (op) {
        // --- DATA TRANSFER ---
        case 0xA9: reg_A = read_byte(reg_PC++); SET_ZN(reg_A); return 2; // LDA Imm
        case 0xA5: reg_A = read_byte(read_byte(reg_PC++)); SET_ZN(reg_A); return 3; // LDA ZP
        case 0xAD: { uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++); reg_A = read_byte((h<<8)|l); SET_ZN(reg_A); return 4; } // LDA Abs
        case 0xA2: reg_X = read_byte(reg_PC++); SET_ZN(reg_X); return 2; // LDX Imm
        case 0xA6: reg_X = read_byte(read_byte(reg_PC++)); SET_ZN(reg_X); return 3; // LDX ZP
        case 0xA0: reg_Y = read_byte(reg_PC++); SET_ZN(reg_Y); return 2; // LDY Imm
        case 0x85: write_byte(read_byte(reg_PC++), reg_A); return 3; // STA ZP
        case 0x8D: { uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++); write_byte((h<<8)|l, reg_A); return 4; } // STA Abs
        case 0x86: write_byte(read_byte(reg_PC++), reg_X); return 3; // STX ZP
        case 0x84: write_byte(read_byte(reg_PC++), reg_Y); return 3; // STY ZP

        // --- STACK OPERATIONS ---
        case 0x9A: reg_S = reg_X; return 2; // TXS
        case 0xBA: reg_X = reg_S; SET_ZN(reg_X); return 2; // TSX
        case 0x48: write_byte(0x0100 + reg_S--, reg_A); return 3; // PHA
        case 0x68: reg_A = read_byte(0x0100 + ++reg_S); SET_ZN(reg_A); return 4; // PLA
        case 0x08: write_byte(0x0100 + reg_S--, reg_P | 0x30); return 3; // PHP
        case 0x28: reg_P = read_byte(0x0100 + ++reg_S) | FLAG_U; return 4; // PLP

        // --- ARITHMETIC & LOGIC ---
        case 0xE9: { // SBC Imm
            uint8_t val = read_byte(reg_PC++);
            uint16_t res = (uint16_t)reg_A - (uint16_t)val - ((reg_P & 0x01) ? 0 : 1);
            if (!(res & 0xFF00)) reg_P |= 0x01; else reg_P &= ~0x01;
            if (((reg_A ^ val) & 0x80) && ((reg_A ^ res) & 0x80)) reg_P |= 0x40; else reg_P &= ~0x40;
            reg_A = (uint8_t)res; SET_ZN(reg_A); return 2;
        }
        case 0x69: { // ADC Imm
            uint8_t val = read_byte(reg_PC++);
            uint16_t res = (uint16_t)reg_A + (uint16_t)val + (reg_P & 0x01);
            if (res > 0xFF) reg_P |= 0x01; else reg_P &= ~0x01;
            if (!((reg_A ^ val) & 0x80) && ((reg_A ^ res) & 0x80)) reg_P |= 0x40; else reg_P &= ~0x40;
            reg_A = (uint8_t)res; SET_ZN(reg_A); return 2;
        }
        case 0xC9: temp = read_byte(reg_PC++); if(reg_A >= temp) reg_P |= 0x01; else reg_P &= ~0x01; SET_ZN(reg_A - temp); return 2; // CMP Imm
        case 0xE0: temp = read_byte(reg_PC++); if(reg_X >= temp) reg_P |= 0x01; else reg_P &= ~0x01; SET_ZN(reg_X - temp); return 2; // CPX Imm
        case 0xC0: temp = read_byte(reg_PC++); if(reg_Y >= temp) reg_P |= 0x01; else reg_P &= ~0x01; SET_ZN(reg_Y - temp); return 2; // CPY Imm
        case 0x29: reg_A &= read_byte(reg_PC++); SET_ZN(reg_A); return 2; // AND Imm
        case 0x09: reg_A |= read_byte(reg_PC++); SET_ZN(reg_A); return 2; // ORA Imm
        case 0x49: reg_A ^= read_byte(reg_PC++); SET_ZN(reg_A); return 2; // EOR Imm

        // --- INCREMENT / DECREMENT ---
        case 0xE8: reg_X++; SET_ZN(reg_X); return 2; // INX
        case 0xC8: reg_Y++; SET_ZN(reg_Y); return 2; // INY
        case 0xCA: reg_X--; SET_ZN(reg_X); return 2; // DEX
        case 0x88: reg_Y--; SET_ZN(reg_Y); return 2; // DEY
        case 0xE6: { uint8_t a = read_byte(reg_PC++); temp = read_byte(a) + 1; write_byte(a, temp); SET_ZN(temp); return 5; } // INC ZP

        // --- CONTROL FLOW ---
        case 0x4C: { uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++); reg_PC = (h<<8)|l; return 3; } // JMP Abs
        case 0x20: { // JSR
            uint16_t l = read_byte(reg_PC++), h = read_byte(reg_PC++);
            uint16_t ret = reg_PC - 1;
            write_byte(0x0100 + reg_S--, (ret >> 8) & 0xFF);
            write_byte(0x0100 + reg_S--, ret & 0xFF);
            reg_PC = (h << 8) | l; return 6;
        }
        case 0x60: { // RTS
            uint16_t l = read_byte(0x0100 + ++reg_S);
            uint16_t h = read_byte(0x0100 + ++reg_S);
            reg_PC = ((h << 8) | l) + 1; return 6;
        }
        case 0x40: { // RTI
            reg_P = read_byte(0x0100 + ++reg_S) | FLAG_U;
            uint16_t l = read_byte(0x0100 + ++reg_S);
            uint16_t h = read_byte(0x0100 + ++reg_S);
            reg_PC = (h << 8) | l; return 6;
        }

        // --- BRANCHES ---
        case 0x90: temp = read_byte(reg_PC++); if(!(reg_P & 0x01)) reg_PC += (int8_t)temp; return 2; // BCC
        case 0xB0: temp = read_byte(reg_PC++); if(reg_P & 0x01) reg_PC += (int8_t)temp; return 2; // BCS
        case 0xF0: temp = read_byte(reg_PC++); if(reg_P & FLAG_Z) reg_PC += (int8_t)temp; return 2; // BEQ
        case 0xD0: temp = read_byte(reg_PC++); if(!(reg_P & FLAG_Z)) reg_PC += (int8_t)temp; return 2; // BNE
        case 0x10: temp = read_byte(reg_PC++); if(!(reg_P & FLAG_N)) reg_PC += (int8_t)temp; return 2; // BPL
        case 0x30: temp = read_byte(reg_PC++); if(reg_P & FLAG_N) reg_PC += (int8_t)temp; return 2; // BMI

        // --- FLAG MODIFICATION ---
        case 0x18: reg_P &= ~0x01; return 2; // CLC
        case 0x38: reg_P |= 0x01; return 2; // SEC
        case 0x58: reg_P &= ~FLAG_I; return 2; // CLI
        case 0x78: reg_P |= FLAG_I; return 2; // SEI
        case 0xD8: reg_P &= ~FLAG_D; return 2; // CLD
        case 0xF8: reg_P |= FLAG_D; return 2; // SED

        // --- MISC ---
        case 0xEA: return 2; // NOP
        case 0x00: return 7; // BRK (Stub)

        default: 
            __android_log_print(ANDROID_LOG_ERROR, "DWA_NATIVE", "Unknown Opcode: %02X at %04X", op, reg_PC-1);
            return 1;
    }
}
