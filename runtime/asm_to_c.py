import os
import re

# Configuration
ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"

# Global Hardware State
global_symbols = {}

def get_ins_size(opcode, op):
    opcode = opcode.lower()
    # Implied/Single byte instructions
    if opcode in ['clc', 'cld', 'cli', 'clv', 'dex', 'dey', 'inx', 'iny', 'nop', 'pha', 'php', 'pla', 'plp', 'rti', 'rts', 'sec', 'sed', 'sei', 'tax', 'tay', 'tsx', 'txa', 'txs', 'tya']:
        return 1
    if not op or op.strip() == "": return 1
    if op.startswith('#'): return 2 # Immediate
    if '(' in op: return 2 # Indirect (usually ZP)

    # Branching (always 2 bytes)
    if opcode in ['bcc', 'bcs', 'beq', 'bmi', 'bne', 'bpl', 'bvc', 'bvs']:
        return 2

    # Absolute vs Zero Page
    res = resolve_symbol(op)
    try:
        val = int(res, 0)
        return 2 if val <= 0xFF else 3
    except:
        return 3

def resolve_symbol(symbol):
    if not symbol: return "0x0"
    # Clean symbol from addressing mode syntax
    clean_sym = re.sub(r'[#$(),\sXY]', '', symbol)
    if clean_sym in global_symbols:
        return global_symbols[clean_sym]
    # Check for hex
    hex_match = re.match(r'^([0-9A-Fa-f]{2,4})$', clean_sym)
    if hex_match:
        return f"0x{hex_match.group(1).lower()}"
    return "0x0"

def translate_line(opcode, op, current_pc):
    opcode = opcode.lower()
    addr = resolve_symbol(op)
    size = get_ins_size(opcode, op)

    # --- 1. Immediate Addressing ---
    if op.startswith('#'):
        imm = addr
        imm_map = {
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
            'sbc': f'cpu_sbc({imm});',
        }
        if opcode in imm_map: return imm_map[opcode], size

    # --- 2. Branching Logic ---
    branch_map = {
        'beq': 'reg_P & FLAG_Z',
        'bne': '!(reg_P & FLAG_Z)',
        'bcs': 'reg_P & FLAG_C',
        'bcc': '!(reg_P & FLAG_C)',
        'bmi': 'reg_P & FLAG_N',
        'bpl': '!(reg_P & FLAG_N)',
    }
    if opcode in branch_map:
        return f'if ({branch_map[opcode]}) {{ reg_PC = {addr}; return; }}', size

    # --- 3. Absolute / Zero Page Addressing ---
    mapping = {
        'lda': f'reg_A = bus_read({addr}); update_nz(reg_A);',
        'ldx': f'reg_X = bus_read({addr}); update_nz(reg_X);',
        'ldy': f'reg_Y = bus_read({addr}); update_nz(reg_Y);',
        'sta': f'bus_write({addr}, reg_A);',
        'stx': f'bus_write({addr}, reg_X);',
        'sty': f'bus_write({addr}, reg_Y);',
        'cmp': f'update_flags_cmp(reg_A, bus_read({addr}));',
        'cpx': f'update_flags_cmp(reg_X, bus_read({addr}));',
        'cpy': f'update_flags_cmp(reg_Y, bus_read({addr}));',
        'jmp': f'reg_PC = {addr}; return;',
        'jsr': f'push_stack(((uint16_t)(reg_PC + 2)) >> 8); push_stack(((uint16_t)(reg_PC + 2)) & 0xFF); reg_PC = {addr}; return;',
        'rts': f'reg_PC = (pop_stack() | (pop_stack() << 8)) + 1; return;',
        'rti': f'reg_P = pop_stack(); reg_PC = (pop_stack() | (pop_stack() << 8)); return;',
        'inc': f'{{ uint8_t v = bus_read({addr}) + 1; bus_write({addr}, v); update_nz(v); }}',
        'dec': f'{{ uint8_t v = bus_read({addr}) - 1; bus_write({addr}, v); update_nz(v); }}',
        'tax': 'reg_X = reg_A; update_nz(reg_X);',
        'txa': 'reg_A = reg_X; update_nz(reg_A);',
        'tay': 'reg_Y = reg_A; update_nz(reg_Y);',
        'tya': 'reg_A = reg_Y; update_nz(reg_A);',
        'pha': 'push_stack(reg_A);',
        'pla': 'reg_A = pop_stack(); update_nz(reg_A);',
        'clc': 'reg_P &= ~FLAG_C;',
        'sec': 'reg_P |= FLAG_C;',
        'sei': 'reg_P |= FLAG_I;',
        'cli': 'reg_P &= ~FLAG_I;',
    }

    code = mapping.get(opcode, f"// Unsupported: {opcode} {op}")
    return code, size

def pre_parse_symbols():
    print("Parsing symbols...")
    for filename in sorted(os.listdir(ASM_DIR)):
        if not filename.endswith('.asm'): continue
        # Default starting PC: Bank03 is fixed at 0xC000
        current_pc = 0xC000 if "Bank03" in filename else 0x8000
        
        with open(os.path.join(ASM_DIR, filename), 'r') as f:
            for line in f:
                clean = line.split(';')[0].strip()
                if not clean: continue

                org_match = re.match(r'^\.org\s+\$?([0-9A-Fa-f]+)', clean)
                if org_match:
                    current_pc = int(org_match.group(1), 16)
                    continue

                label_match = re.match(r'^(\w+):', clean)
                if label_match:
                    global_symbols[label_match.group(1)] = hex(current_pc)
                    clean = clean[len(label_match.group(0)):].strip()
                    if not clean: continue

                ins_match = re.match(r'^(\w{3})\s*(.*)$', clean)
                if ins_match:
                    current_pc += get_ins_size(ins_match.group(1), ins_match.group(2))
                elif ".byte" in clean or ".db" in clean:
                    current_pc += len(clean.split(','))

def convert_all():
    if not os.path.exists(OUT_DIR): os.makedirs(OUT_DIR)
    
    pre_parse_symbols()

    for filename in sorted(os.listdir(ASM_DIR)):
        if not filename.endswith('.asm') or "Defines" in filename: continue
        bank_name = os.path.splitext(filename)[0].replace("-", "_")
        
        print(f"Converting {filename} -> {bank_name}.cpp")
        current_pc = 0xC000 if "Bank03" in filename else 0x8000

        with open(os.path.join(ASM_DIR, filename), 'r') as f:
            lines = f.readlines()

        with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
            out.write('#include "cpu_shared.h"\n\n')
            out.write(f'namespace {bank_name} {{\n    void execute() {{\n        switch(reg_PC) {{\n')

            for line in lines:
                clean = line.split(';')[0].strip()
                if not clean: continue

                org_match = re.match(r'^\.org\s+\$?([0-9A-Fa-f]+)', clean)
                if org_match:
                    current_pc = int(org_match.group(1), 16)
                    continue

                code_match = re.match(r'^(?:\w+:)?\s*(\w{3})\s*(.*)$', clean)
                if code_match:
                    opcode, op = code_match.groups()
                    code, size = translate_line(opcode, op, current_pc)
                    out.write(f'            case 0x{current_pc:04x}: {code}')
                    if "return" not in code: out.write(f' reg_PC += {size}; return;')
                    out.write('\n')
                    current_pc += size
                elif ".byte" in clean or ".db" in clean:
                    current_pc += len(clean.split(','))

            out.write('            default: return;\n        }\n    }\n}\n')

if __name__ == "__main__":
    convert_all()
