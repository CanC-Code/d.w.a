#!/usr/bin/env python3
import os
import re

# Configuration - Absolute Path Resolution
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
SOURCE_DIR = os.path.normpath(os.path.join(SCRIPT_DIR, "..", "source_files"))
OUTPUT_DIR = os.path.normpath(os.path.join(SCRIPT_DIR, "..", "app", "src", "main", "cpp", "recompiled"))

# 6502 Cycle Table
CYCLE_TABLE = {
    'adc': 2, 'and': 2, 'asl': 2, 'bcc': 2, 'bcs': 2, 'beq': 2, 'bit': 3, 'bmi': 2, 'bne': 2, 'bpl': 2,
    'brk': 7, 'bvc': 2, 'bvs': 2, 'clc': 2, 'cld': 2, 'cli': 2, 'clv': 2, 'cmp': 2, 'cpx': 2, 'cpy': 2,
    'dec': 5, 'dex': 2, 'dey': 2, 'eor': 2, 'inc': 5, 'inx': 2, 'iny': 2, 'jmp': 3, 'jsr': 6, 'lda': 2,
    'ldx': 2, 'ldy': 2, 'lsr': 2, 'nop': 2, 'ora': 2, 'pha': 3, 'php': 3, 'pla': 4, 'plp': 4, 'rol': 2,
    'ror': 2, 'rti': 6, 'rts': 6, 'sbc': 2, 'sec': 2, 'sed': 2, 'sei': 2, 'sta': 3, 'stx': 3, 'sty': 3,
    'tax': 2, 'tay': 2, 'tsx': 2, 'txa': 2, 'txs': 2, 'tya': 2
}

def get_instruction_size(opcode, operand):
    opcode = opcode.lower()
    if opcode in ['.byte', '.db']: return 1
    if opcode in ['.word', '.dw', '.addr']: return 2
    if not operand or operand.strip().upper() == 'A': return 1
    if operand.startswith('#'): return 2
    if '(' in operand: return 2 if opcode != 'jmp' else 3
    if opcode in ['bcc', 'bcs', 'beq', 'bmi', 'bne', 'bpl', 'bvc', 'bvs']: return 2
    clean = re.sub(r'[#$(),\sXY]', '', operand)
    try:
        val = int(clean, 16) if '$' in operand else int(clean)
        return 2 if val <= 0xFF else 3
    except: return 3

class Recompiler:
    def __init__(self):
        self.symbols = {}
        self.anonymous_labels = []

    def first_pass(self, lines, start_pc):
        current_pc = start_pc
        self.symbols = {}
        self.anonymous_labels = []
        for line in lines:
            line = line.split(';')[0].strip()
            if not line: continue
            org_match = re.match(r'^\.org\s+\$?([0-9A-Fa-f]+)', line)
            if org_match:
                current_pc = int(org_match.group(1), 16)
                continue
            label_match = re.match(r'^(\+|-|[\w\d_]+):', line)
            if label_match:
                lbl = label_match.group(1)
                if lbl in ['+', '-']: self.anonymous_labels.append((current_pc, lbl))
                else: self.symbols[lbl] = current_pc
            match = re.match(r'^(\.?\w+)\s*(.*)$', line)
            if match:
                opcode, operand = match.groups()
                if opcode.lower() in CYCLE_TABLE or opcode.startswith('.'):
                    current_pc += get_instruction_size(opcode, operand)

    def resolve(self, symbol, current_pc):
        symbol = symbol.strip()
        if symbol == '+':
            for pc, type in self.anonymous_labels:
                if pc > current_pc and type == '+': return f"0x{pc:04X}"
        if symbol == '-':
            for pc, type in reversed(self.anonymous_labels):
                if pc < current_pc and type == '-': return f"0x{pc:04X}"
        if symbol.startswith('$'): return f"0x{int(symbol[1:], 16):04X}"
        if symbol in self.symbols: return f"0x{self.symbols[symbol]:04X}"
        return "0x0000"

    def translate(self, opcode, operand, pc):
        opcode = opcode.lower()
        size = get_instruction_size(opcode, operand)
        cycles = CYCLE_TABLE.get(opcode, 2)
        raw_symbol = re.sub(r'[#$(),\sXY]', '', operand)
        target = self.resolve(raw_symbol, pc)
        prefix = f"cycles_to_run -= {cycles}; "

        is_imm = operand.startswith('#')
        is_ind_y = "),Y" in operand.upper()
        is_ind_x = ",X)" in operand.upper()
        is_abs_x = ",X" in operand.upper() and not is_ind_x
        is_abs_y = ",Y" in operand.upper() and not is_ind_y

        if is_ind_y: target = f"read_pointer_indexed_y({target}, nullptr)"
        elif is_ind_x: target = f"read_pointer_indexed_x({target})"
        elif is_abs_x: target = f"addr_abs_x({target}, nullptr)"
        elif is_abs_y: target = f"addr_abs_y({target}, nullptr)"

        # Branching (Fixed: Added BVC/BVS)
        if opcode in ['beq', 'bne', 'bcs', 'bcc', 'bmi', 'bpl', 'bvs', 'bvc']:
            conds = {
                'beq': 'reg_P & FLAG_Z',
                'bne': '!(reg_P & FLAG_Z)',
                'bcs': 'reg_P & FLAG_C',
                'bcc': '!(reg_P & FLAG_C)',
                'bmi': 'reg_P & FLAG_N',
                'bpl': '!(reg_P & FLAG_N)',
                'bvs': 'reg_P & FLAG_V',
                'bvc': '!(reg_P & FLAG_V)'
            }
            y = "if (cycles_to_run <= 0) return; " if int(target, 16) < pc else ""
            return f"{prefix}{y}if ({conds[opcode]}) {{ reg_PC = {target}; return; }}", size

        ops = {
            'lda': f"reg_A = {'(uint8_t)'+target if is_imm else 'bus_read('+target+')'}; update_nz(reg_A);",
            'ldx': f"reg_X = {'(uint8_t)'+target if is_imm else 'bus_read('+target+')'}; update_nz(reg_X);",
            'ldy': f"reg_Y = {'(uint8_t)'+target if is_imm else 'bus_read('+target+')'}; update_nz(reg_Y);",
            'sta': f"bus_write({target}, reg_A);",
            'stx': f"bus_write({target}, reg_X);",
            'sty': f"bus_write({target}, reg_Y);",
            'cmp': f"update_flags_cmp(reg_A, {'(uint8_t)'+target if is_imm else 'bus_read('+target+')'});",
            'cpx': f"update_flags_cmp(reg_X, {'(uint8_t)'+target if is_imm else 'bus_read('+target+')'});",
            'cpy': f"update_flags_cmp(reg_Y, {'(uint8_t)'+target if is_imm else 'bus_read('+target+')'});",
            'adc': f"cpu_adc({'(uint8_t)'+target if is_imm else 'bus_read('+target+')'});",
            'sbc': f"cpu_sbc({'(uint8_t)'+target if is_imm else 'bus_read('+target+')'});",
            'bit': f"cpu_bit(bus_read({target}));",
            'inc': f"{{ uint16_t a = {target}; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); }}",
            'dec': f"{{ uint16_t a = {target}; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); }}",
            'asl': f"reg_A = cpu_asl(reg_A);" if not operand or operand.upper()=='A' else f"{{ uint16_t a = {target}; bus_write(a, cpu_asl(bus_read(a))); }}",
            'lsr': f"reg_A = cpu_lsr(reg_A);" if not operand or operand.upper()=='A' else f"{{ uint16_t a = {target}; bus_write(a, cpu_lsr(bus_read(a))); }}",
            'rol': f"reg_A = cpu_rol(reg_A);" if not operand or operand.upper()=='A' else f"{{ uint16_t a = {target}; bus_write(a, cpu_rol(bus_read(a))); }}",
            'ror': f"reg_A = cpu_ror(reg_A);" if not operand or operand.upper()=='A' else f"{{ uint16_t a = {target}; bus_write(a, cpu_ror(bus_read(a))); }}",
            'jsr': f"{{ uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = {target}; return; }}",
            'jmp': f"reg_PC = {target}; return;",
            'rts': f"reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return;",
            'rti': f"reg_P = cpu_pop(); reg_PC = (cpu_pop() | (cpu_pop() << 8)); return;",
            'tax': "reg_X = reg_A; update_nz(reg_X);", 'txa': "reg_A = reg_X; update_nz(reg_A);",
            'tay': "reg_Y = reg_A; update_nz(reg_Y);", 'tya': "reg_A = reg_Y; update_nz(reg_A);",
            'pha': "cpu_push(reg_A);", 'pla': "reg_A = cpu_pop(); update_nz(reg_A);",
            'ora': f"reg_A |= {'(uint8_t)'+target if is_imm else 'bus_read('+target+')'}; update_nz(reg_A);",
            'and': f"reg_A &= {'(uint8_t)'+target if is_imm else 'bus_read('+target+')'}; update_nz(reg_A);",
            'eor': f"reg_A ^= {'(uint8_t)'+target if is_imm else 'bus_read('+target+')'}; update_nz(reg_A);",
            'sei': "reg_P |= FLAG_I;", 'cli': "reg_P &= ~FLAG_I;",
            'sec': "reg_P |= FLAG_C;", 'clc': "reg_P &= ~FLAG_C;",
            'nop': "// NOP"
        }

        res = ops.get(opcode, f"// Unsupported {opcode}")
        return f"{prefix}{res}", size

    def convert(self, filename):
        bank_name = os.path.splitext(filename)[0].replace("-", "_")
        start_pc = 0xC000 if "Bank03" in bank_name else 0x8000
        path = os.path.join(SOURCE_DIR, filename)
        
        with open(path, 'r', errors='ignore') as f: lines = f.readlines()
        self.first_pass(lines, start_pc)
        
        output_path = os.path.join(OUTPUT_DIR, f"{bank_name}.cpp")
        with open(output_path, 'w') as out:
            out.write(f'#include "cpu_shared.h"\nnamespace {bank_name} {{\nvoid execute_at(uint16_t pc) {{\n    switch (pc) {{\n')
            current_pc = start_pc
            for line in lines:
                clean = line.split(';')[0].strip()
                if not clean: continue
                org_match = re.match(r'^\.org\s+\$?([0-9A-Fa-f]+)', clean)
                if org_match: current_pc = int(org_match.group(1), 16); continue
                
                match = re.match(r'^(?:(\+|-|[\w\d_]+):)?\s*(\.?\w+)?\s*(.*)$', clean)
                label, opcode, operand = match.groups()
                
                if opcode and opcode.lower() in CYCLE_TABLE:
                    code, size = self.translate(opcode, operand, current_pc)
                    out.write(f'        case 0x{current_pc:04X}: {code}')
                    if 'return' not in code: out.write(f' reg_PC += {size}; return;')
                    out.write(f' // {clean}\n')
                    current_pc += size
            
            if "Bank03" in bank_name:
                out.write("        case 0xFFFA: reg_PC = cpu_read_pointer(0xFFFA); return;\n")
                out.write("        case 0xFFFC: reg_PC = cpu_read_pointer(0xFFFC); return;\n")
                out.write("        case 0xFFFE: reg_PC = cpu_read_pointer(0xFFFE); return;\n")
            out.write('        default: reg_PC++; return;\n    }\n}\n}\n')

if __name__ == "__main__":
    if not os.path.exists(SOURCE_DIR):
        print(f"Error: Source directory {SOURCE_DIR} not found.")
        exit(1)
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    rec = Recompiler()
    for f in os.listdir(SOURCE_DIR):
        if f.endswith(".asm") and "Defines" not in f:
            print(f"Recompiling {f}...")
            rec.convert(f)
