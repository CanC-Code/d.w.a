#!/usr/bin/env python3
"""
Dragon Warrior ASM to C++ Converter - FINAL PRODUCTION VERSION
Features: 
- Hardware-accurate JSR return addresses (PC+2)
- Indirect JMP page-wrap bug simulation
- Branch Yielding to prevent Android ANRs
- Corrected C++ signatures for all memory helpers
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
    if opcode in ['.byte', '.db', '.asc']: return 1
    if opcode in ['.word', '.dw', '.addr']: return 2
    if opcode in ['.ds']: return 0 

    implied_opcodes = {
        'brk', 'clc', 'cld', 'cli', 'clv', 'dex', 'dey', 'inx', 'iny',
        'nop', 'pha', 'php', 'pla', 'plp', 'rti', 'rts', 'sec', 'sed',
        'sei', 'tax', 'tay', 'tsx', 'txa', 'txs', 'tya'
    }
    if opcode in implied_opcodes: return 1
    if opcode in ['asl', 'lsr', 'rol', 'ror'] and (not operand or operand.strip().upper() == 'A'):
        return 1

    if not operand or operand.strip() == "": return 1

    operand = operand.strip()
    if operand.startswith('#'): return 2
    if operand.startswith('('): return 2
    if opcode in ['bcc', 'bcs', 'beq', 'bmi', 'bne', 'bpl', 'bvc', 'bvs']: return 2

    clean = re.sub(r'[#$(),\sXY%]', '', operand)
    if clean in global_symbols:
        val = int(global_symbols[clean], 16)
        return 2 if val <= 0xFF else 3

    if '$' in operand:
        hex_val = re.search(r'\$([0-9A-Fa-f]+)', operand)
        if hex_val:
            return 2 if len(hex_val.group(1)) <= 2 else 3

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
        return f"// Data: {opcode} {operand}", get_instruction_size(opcode, operand)

    if opcode not in VALID_OPCODES:
        return f"// Unsupported Opcode: {opcode}", 0

    size = get_instruction_size(opcode, operand)
    val = resolve_symbol(operand)
    addr_val = int(val, 16) & 0xFFFF
    addr_str = f"0x{addr_val:04X}"

    # Yield logic for loops (prevents Android ANR)
    yield_logic = ""
    if opcode in ['beq', 'bne', 'bcs', 'bcc', 'bmi', 'bpl', 'bvs', 'bvc']:
        if addr_val < pc: 
            yield_logic = "if (cycles_to_run <= 0) return; "

    # 1. IMMEDIATE
    if operand.startswith('#'):
        imm = f"0x{addr_val & 0xFF:02X}"
        ops = {
            'lda': f'reg_A = {imm}; update_nz(reg_A);',
            'ldx': f'reg_X = {imm}; update_nz(reg_X);',
            'ldy': f'reg_Y = {imm}; update_nz(reg_Y);',
            'cmp': f'update_flags_cmp(reg_A, {imm});',
            'cpx': f'update_flags_cmp(reg_X, {imm});',
            'cpy': f'update_flags_cmp(reg_Y, {imm});',
            'ora': f'reg_A |= {imm}; update_nz(reg_A);',
            'and': f'reg_A &= {imm}; update_nz(reg_A);',
            'eor': f'reg_A ^= {imm}; update_nz(reg_A);',
            'adc': f'cpu_adc({imm});',
            'sbc': f'cpu_sbc({imm});'
        }
        return ops.get(opcode, f"// Error {opcode} imm"), size

    # 2. INDIRECT
    if '(' in operand:
        zp = f"0x{addr_val & 0xFF:02X}"
        if '),Y' in operand.upper():
            ptr = f"read_pointer_indexed_y({zp}, nullptr)"
            ops = {
                'lda': f'reg_A = bus_read({ptr}); update_nz(reg_A);',
                'sta': f'bus_write({ptr}, reg_A);',
                'ora': f'reg_A |= bus_read({ptr}); update_nz(reg_A);',
                'and': f'reg_A &= bus_read({ptr}); update_nz(reg_A);',
                'eor': f'reg_A ^= bus_read({ptr}); update_nz(reg_A);',
                'adc': f'cpu_adc(bus_read({ptr}));',
                'sbc': f'cpu_sbc(bus_read({ptr}));',
                'cmp': f'update_flags_cmp(reg_A, bus_read({ptr}));'
            }
            return ops.get(opcode, f"// Error {opcode} ind_y"), size
        if ',X)' in operand.upper():
            ptr = f"read_pointer_indexed_x({zp})"
            return f'reg_A = bus_read({ptr}); update_nz(reg_A);', size
        if opcode == 'jmp':
            return f'reg_PC = cpu_read_jmp_indirect({addr_str}); return;', size

    # 3. ACCUMULATOR
    if not operand or operand.strip().upper() == 'A':
        if opcode == 'asl': return 'reg_A = cpu_asl(reg_A);', size
        if opcode == 'lsr': return 'reg_A = cpu_lsr(reg_A);', size
        if opcode == 'rol': return 'reg_A = cpu_rol(reg_A);', size
        if opcode == 'ror': return 'reg_A = cpu_ror(reg_A);', size

    # 4. RELATIVE
    branch_conds = {
        'beq': 'reg_P & FLAG_Z', 'bne': '!(reg_P & FLAG_Z)',
        'bcs': 'reg_P & FLAG_C', 'bcc': '!(reg_P & FLAG_C)',
        'bmi': 'reg_P & FLAG_N', 'bpl': '!(reg_P & FLAG_N)',
        'bvs': 'reg_P & FLAG_V', 'bvc': '!(reg_P & FLAG_V)'
    }
    if opcode in branch_conds:
        return f'{yield_logic}if ({branch_conds[opcode]}) {{ reg_PC = {addr_str}; return; }}', size

    # 5. ABSOLUTE / ZERO PAGE / INDEXED
    final_addr = addr_str
    if ',X' in operand.upper(): final_addr = f"addr_abs_x({addr_str}, nullptr)"
    if ',Y' in operand.upper(): final_addr = f"addr_abs_y({addr_str}, nullptr)"

    mem_ops = {
        'lda': f'reg_A = bus_read({final_addr}); update_nz(reg_A);',
        'ldx': f'reg_X = bus_read({final_addr}); update_nz(reg_X);',
        'ldy': f'reg_Y = bus_read({final_addr}); update_nz(reg_Y);',
        'sta': f'bus_write({final_addr}, reg_A);',
        'stx': f'bus_write({final_addr}, reg_X);',
        'sty': f'bus_write({final_addr}, reg_Y);',
        'cmp': f'update_flags_cmp(reg_A, bus_read({final_addr}));',
        'cpx': f'update_flags_cmp(reg_X, bus_read({final_addr}));',
        'cpy': f'update_flags_cmp(reg_Y, bus_read({final_addr}));',
        'bit': f'cpu_bit(bus_read({final_addr}));',
        'inc': f'{{ uint16_t a = {final_addr}; uint8_t v = bus_read(a) + 1; bus_write(a, v); update_nz(v); }}',
        'dec': f'{{ uint16_t a = {final_addr}; uint8_t v = bus_read(a) - 1; bus_write(a, v); update_nz(v); }}',
        'asl': f'{{ uint16_t a = {final_addr}; uint8_t v = cpu_asl(bus_read(a)); bus_write(a, v); }}',
        'lsr': f'{{ uint16_t a = {final_addr}; uint8_t v = cpu_lsr(bus_read(a)); bus_write(a, v); }}',
        'rol': f'{{ uint16_t a = {final_addr}; uint8_t v = cpu_rol(bus_read(a)); bus_write(a, v); }}',
        'ror': f'{{ uint16_t a = {final_addr}; uint8_t v = cpu_ror(bus_read(a)); bus_write(a, v); }}',
        'jmp': f'reg_PC = {final_addr}; return;',
        'jsr': f'{{ uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = {final_addr}; return; }}',
        'adc': f'cpu_adc(bus_read({final_addr}));',
        'sbc': f'cpu_sbc(bus_read({final_addr}));',
        'ora': f'reg_A |= bus_read({final_addr}); update_nz(reg_A);',
        'and': f'reg_A &= bus_read({final_addr}); update_nz(reg_A);',
        'eor': f'reg_A ^= bus_read({final_addr}); update_nz(reg_A);'
    }
    if opcode in mem_ops: return mem_ops[opcode], size

    # 6. IMPLIED
    implied_ops = {
        'clc': 'reg_P &= ~FLAG_C;', 'sec': 'reg_P |= FLAG_C;',
        'cli': 'reg_P &= ~FLAG_I;', 'sei': 'reg_P |= FLAG_I;',
        'clv': 'reg_P &= ~FLAG_V;', 'cld': 'reg_P &= ~FLAG_D;', 'sed': 'reg_P |= FLAG_D;',
        'tax': 'reg_X = reg_A; update_nz(reg_X);', 'txa': 'reg_A = reg_X; update_nz(reg_A);',
        'tay': 'reg_Y = reg_A; update_nz(reg_Y);', 'tya': 'reg_A = reg_Y; update_nz(reg_A);',
        'tsx': 'reg_X = reg_S; update_nz(reg_X);', 'txs': 'reg_S = reg_X;',
        'dex': 'reg_X--; update_nz(reg_X);', 'inx': 'reg_X++; update_nz(reg_X);',
        'dey': 'reg_Y--; update_nz(reg_Y);', 'iny': 'reg_Y++; update_nz(reg_Y);',
        'pha': 'cpu_push(reg_A);', 'pla': 'reg_A = cpu_pop(); update_nz(reg_A);',
        'php': 'cpu_push(reg_P | 0x10);', 'plp': 'reg_P = cpu_pop();',
        'rts': 'reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return;',
        'rti': 'reg_P = cpu_pop(); reg_PC = (cpu_pop() | (cpu_pop() << 8)); return;',
        'nop': '// NOP',
        'brk': 'cpu_push((reg_PC+2) >> 8); cpu_push((reg_PC+2) & 0xFF); cpu_push(reg_P | 0x10); reg_P |= FLAG_I; reg_PC = cpu_read_pointer(0xFFFE); return;'
    }
    if opcode in implied_ops: return implied_ops[opcode], size

    return f"// Unsupported: {opcode} {operand}", size

def convert_asm_file(filename):
    filepath = os.path.join(SOURCE_DIR, filename)
    bank_name = os.path.splitext(filename)[0].replace("-", "_")
    current_pc = 0xC000 if "Bank03" in filename else 0x8000
    output_path = os.path.join(OUTPUT_DIR, f"{bank_name}.cpp")

    with open(output_path, 'w') as out:
        out.write(f'// Generated for Dragon Warrior Android\n')
        out.write('#include "cpu_shared.h"\n\n')
        out.write(f'namespace {bank_name} {{\n')
        out.write('void execute_at(uint16_t pc) {\n')
        out.write('    switch (pc) {\n')

        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
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
                    addr_hex = f"0x{current_pc & 0xFFFF:04X}"
                    out.write(f'        case {addr_hex}: {code}')
                    if 'return' not in code: out.write(f' reg_PC += {size}; return;')
                    out.write(f' // {clean}\n')
                current_pc += size

        out.write('        default: reg_PC++; return;\n    }\n}\n}\n')

def pre_parse_symbols(asm_files):
    for filename in sorted(asm_files):
        filepath = os.path.join(SOURCE_DIR, filename)
        current_pc = 0xC000 if "Bank03" in filename else 0x8000
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            for line in f:
                line = line.split(';')[0].strip()
                if not line: continue
                org_match = re.match(r'^\.org\s+\$?([0-9A-Fa-f]+)', line)
                if org_match:
                    current_pc = int(org_match.group(1), 16)
                    continue
                label_match = re.match(r'^(\w+):', line)
                if label_match:
                    global_symbols[label_match.group(1)] = hex(current_pc & 0xFFFF)
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
    main()
