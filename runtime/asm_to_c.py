#!/usr/bin/env python3
"""
Dragon Warrior ASM to C++ Converter
Updated to support the execute_at(uint16_t pc) signature for the Recompiled Dispatcher.
"""

import os
import re
import sys

# Paths
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
SOURCE_DIR = os.path.join(SCRIPT_DIR, "..", "source_files")
OUTPUT_DIR = os.path.join(SCRIPT_DIR, "..", "app", "src", "main", "cpp", "recompiled")

# Global symbol table (label -> address)
global_symbols = {}

# Valid 6502 opcodes
VALID_OPCODES = {
    'adc', 'and', 'asl', 'bcc', 'bcs', 'beq', 'bit', 'bmi', 'bne', 'bpl',
    'brk', 'bvc', 'bvs', 'clc', 'cld', 'cli', 'clv', 'cmp', 'cpx', 'cpy',
    'dec', 'dex', 'dey', 'eor', 'inc', 'inx', 'iny', 'jmp', 'jsr', 'lda',
    'ldx', 'ldy', 'lsr', 'nop', 'ora', 'pha', 'php', 'pla', 'plp', 'rol',
    'ror', 'rti', 'rts', 'sbc', 'sec', 'sed', 'sei', 'sta', 'stx', 'sty',
    'tax', 'tay', 'tsx', 'txa', 'txs', 'tya'
}

def get_instruction_size(opcode, operand):
    opcode = opcode.lower()
    if opcode in ['.byte', '.db']: return 1
    if opcode in ['.word', '.dw', '.addr']: return 2

    implied_opcodes = {
        'brk', 'clc', 'cld', 'cli', 'clv', 'dex', 'dey', 'inx', 'iny',
        'nop', 'pha', 'php', 'pla', 'plp', 'rti', 'rts', 'sec', 'sed',
        'sei', 'tax', 'tay', 'tsx', 'txa', 'txs', 'tya'
    }
    if opcode in implied_opcodes: return 1
    if opcode in ['asl', 'lsr', 'rol', 'ror'] and (not operand or operand.strip().upper() == 'A'):
        return 1
    if not operand or operand.strip() == "": return 1
    if operand.startswith('#'): return 2
    if '(' in operand: return 2
    if opcode in ['bcc', 'bcs', 'beq', 'bmi', 'bne', 'bpl', 'bvc', 'bvs']: return 2
    if ',' in operand:
        clean = re.sub(r'[#$(),\sXY%]', '', operand)
        return 2 if len(clean) <= 2 else 3
    clean = re.sub(r'[#$(),\sXY%]', '', operand)
    if clean in global_symbols:
        val = int(global_symbols[clean], 16)
        return 2 if val <= 0xFF else 3
    return 2 if len(clean) <= 2 else 3

def resolve_symbol(symbol):
    if not symbol: return "0x00"
    if '%' in symbol:
        match = re.search(r'%([01]+)', symbol)
        if match: return hex(int(match.group(1), 2))
    clean = re.sub(r'[#$(),\sXY]', '', symbol)
    if clean in global_symbols: return global_symbols[clean]
    hex_match = re.search(r'([0-9A-Fa-f]+)', clean)
    if hex_match: return hex(int(hex_match.group(1), 16))
    return "0x00"

def translate_instruction(opcode, operand, pc):
    opcode = opcode.lower()
    if opcode in ['.byte', '.db', '.word', '.dw', '.addr', '.asc', '.ds']:
        return "// Data", get_instruction_size(opcode, operand)
    if opcode not in VALID_OPCODES:
        return f"// Unknown: {opcode}", 0

    size = get_instruction_size(opcode, operand)
    val = resolve_symbol(operand)

    # Simplified immediate/absolute logic for brevity in this generator fix
    if operand and operand.startswith('#'):
        imm = int(val, 16) & 0xFF
        imm_hex = f"0x{imm:02X}"
        if opcode == 'lda': return f'reg_A = {imm_hex}; update_nz(reg_A);', size
        if opcode == 'ldx': return f'reg_X = {imm_hex}; update_nz(reg_X);', size
        if opcode == 'ldy': return f'reg_Y = {imm_hex}; update_nz(reg_Y);', size
        if opcode == 'cmp': return f'update_flags_cmp(reg_A, {imm_hex});', size
        if opcode == 'ora': return f'reg_A |= {imm_hex}; update_nz(reg_A);', size
        if opcode == 'and': return f'reg_A &= {imm_hex}; update_nz(reg_A);', size
        if opcode == 'eor': return f'reg_A ^= {imm_hex}; update_nz(reg_A);', size
        if opcode == 'adc': return f'cpu_adc({imm_hex});', size
        if opcode == 'sbc': return f'cpu_sbc({imm_hex});', size

    addr = int(val, 16) & 0xFFFF
    addr_str = f"0x{addr:04X}"

    # Branch instructions
    branch_ops = {'beq': 'reg_P & FLAG_Z', 'bne': '!(reg_P & FLAG_Z)', 'bcs': 'reg_P & FLAG_C', 'bcc': '!(reg_P & FLAG_C)'}
    if opcode in branch_ops:
        return f'if ({branch_ops[opcode]}) {{ reg_PC = {addr_str}; return; }}', size

    # Basic Jumps/Returns
    if opcode == 'jmp': return f'reg_PC = {addr_str}; return;', size
    if opcode == 'jsr': return f'{{ uint16_t ret = reg_PC + {size - 1}; push_stack(ret >> 8); push_stack(ret & 0xFF); reg_PC = {addr_str}; return; }}', size
    if opcode == 'rts': return 'reg_PC = (pop_stack() | (pop_stack() << 8)) + 1; return;', size
    
    # Defaults for other ops (LDA abs, etc.)
    return f'// instruction: {opcode} {operand}', size

def convert_asm_file(filename):
    filepath = os.path.join(SOURCE_DIR, filename)
    bank_name = os.path.splitext(filename)[0].replace("-", "_")
    current_pc = 0xC000 if "Bank03" in filename else 0x8000
    output_path = os.path.join(OUTPUT_DIR, f"{bank_name}.cpp")

    with open(output_path, 'w') as out:
        out.write('#include "cpu_shared.h"\n\n')
        out.write(f'namespace {bank_name} {{\n')
        # CRITICAL FIX: Match the signature Dispatcher is calling
        out.write('void execute_at(uint16_t pc) {\n')
        out.write('    switch (pc) {\n')

        with open(filepath, 'r') as f:
            for line in f:
                clean = line.split(';')[0].strip()
                if not clean: continue
                org_match = re.match(r'^\.org\s+\$?([0-9A-Fa-f]+)', clean)
                if org_match:
                    current_pc = int(org_match.group(1), 16)
                    continue
                match = re.match(r'^(?:(\w+):)?\s*(\.?\w+)?\s*(.*)$', clean)
                if not match: continue
                label, opcode, operand = match.groups()
                if not opcode: continue

                code, size = translate_instruction(opcode, operand, current_pc)
                if size > 0 and opcode.lower() in VALID_OPCODES:
                    out.write(f'        case 0x{current_pc & 0xFFFF:04X}: ')
                    out.write(code)
                    if 'return' not in code:
                        out.write(f' reg_PC += {size}; return;')
                    out.write(f' // {clean}\n')
                current_pc += size

        out.write('        default: reg_PC++; return;\n')
        out.write('    }\n')
        out.write('}\n')
        out.write('}\n')

def pre_parse_symbols(asm_files):
    for filename in sorted(asm_files):
        filepath = os.path.join(SOURCE_DIR, filename)
        current_pc = 0xC000 if "Bank03" in filename else 0x8000
        with open(filepath, 'r') as f:
            for line in f:
                line = line.split(';')[0].strip()
                if not line: continue
                org_match = re.match(r'^\.org\s+\$?([0-9A-Fa-f]+)', line)
                if org_match: current_pc = int(org_match.group(1), 16); continue
                label_match = re.match(r'^(\w+):', line)
                if label_match:
                    global_symbols[label_match.group(1)] = hex(current_pc & 0xFFFF)
                    line = re.sub(r'^\w+:\s*', '', line).strip()
                if not line: continue
                match = re.match(r'^(\.?\w+)\s*(.*)$', line)
                if match:
                    opcode, operand = match.groups()
                    current_pc += get_instruction_size(opcode, operand)

def main():
    if not os.path.exists(SOURCE_DIR): return 1
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    asm_files = [f for f in os.listdir(SOURCE_DIR) if f.endswith('.asm') and "Defines" not in f]
    pre_parse_symbols(asm_files)
    for filename in asm_files:
        convert_asm_file(filename)
    return 0

if __name__ == "__main__":
    sys.exit(main())
