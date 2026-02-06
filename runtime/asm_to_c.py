import os
import re

# Configuration - Resolve paths relative to this script's location
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
# Moves up one level from 'runtime' then into the cpp folder
OUT_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "app", "src", "main", "cpp", "recompiled"))
ASM_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "source_files"))

global_symbols = {}

VALID_OPCODES = {
    'adc', 'and', 'asl', 'bcc', 'bcs', 'beq', 'bit', 'bmi', 'bne', 'bpl',
    'brk', 'bvc', 'bvs', 'clc', 'cld', 'cli', 'clv', 'cmp', 'cpx', 'cpy',
    'dec', 'dex', 'dey', 'eor', 'inc', 'inx', 'iny', 'jmp', 'jsr', 'lda',
    'ldx', 'ldy', 'lsr', 'nop', 'ora', 'pha', 'php', 'pla', 'plp', 'rol',
    'ror', 'rti', 'rts', 'sbc', 'sec', 'sed', 'sei', 'sta', 'stx', 'sty',
    'tax', 'tay', 'tsx', 'txa', 'txs', 'tya'
}

def get_ins_size(opcode, op):
    opcode = opcode.lower()
    if opcode in ['.byte', '.db']: return 1
    if opcode in ['.word', '.dw', '.addr']: return 2
    
    if opcode in ['clc', 'cld', 'cli', 'clv', 'dex', 'dey', 'inx', 'iny', 'nop', 
                  'pha', 'php', 'pla', 'plp', 'rti', 'rts', 'sec', 'sed', 'sei', 
                  'tax', 'tay', 'tsx', 'txa', 'txs', 'tya']:
        return 1
    if opcode in ['asl', 'lsr', 'rol', 'ror'] and (not op or op.strip().upper() == 'A'):
        return 1

    if not op or op.strip() == "": return 1
    if op.startswith('#'): return 2
    if '(' in op: return 2
    if ',' in op:
        clean_op = re.sub(r'[#$(),\sXY%]', '', op)
        return 2 if len(clean_op) <= 2 else 3
    if opcode in ['bcc', 'bcs', 'beq', 'bmi', 'bne', 'bpl', 'bvc', 'bvs']:
        return 2

    clean_op = re.sub(r'[#$(),\sXY%]', '', op)
    if clean_op in global_symbols:
        val = int(global_symbols[clean_op], 16)
        return 2 if val <= 0xFF else 3
    return 2 if len(clean_op) <= 2 else 3

def resolve_symbol(symbol):
    if not symbol: return "0x00"
    if '%' in symbol:
        bin_match = re.search(r'%([01]+)', symbol)
        if bin_match: return hex(int(bin_match.group(1), 2))
    clean_sym = re.sub(r'[#$(),\sXY]', '', symbol)
    if clean_sym in global_symbols: return global_symbols[clean_sym]
    hex_match = re.search(r'([0-9A-Fa-f]+)', clean_sym)
    if hex_match: return hex(int(hex_match.group(1), 16))
    return "0x00"

def translate_line(opcode, op, current_pc):
    opcode = opcode.lower()
    if opcode in ['.byte', '.db', '.word', '.dw', '.addr', '.asc', '.ds']:
        return "// Data", get_ins_size(opcode, op)
    if opcode not in VALID_OPCODES:
        return f"// Label/Unknown: {opcode}", 0

    val_raw = resolve_symbol(op)
    size = get_ins_size(opcode, op)

    # Immediate
    if op.startswith('#'):
        imm = int(val_raw, 16) & 0xFF
        imm_s = f"0x{imm:02x}"
        imm_map = {
            'lda': f'reg_A = {imm_s}; update_nz(reg_A);',
            'ldx': f'reg_X = {imm_s}; update_nz(reg_X);',
            'ldy': f'reg_Y = {imm_s}; update_nz(reg_Y);',
            'cmp': f'update_flags_cmp(reg_A, {imm_s});',
            'cpx': f'update_flags_cmp(reg_X, {imm_s});',
            'cpy': f'update_flags_cmp(reg_Y, {imm_s});',
            'ora': f'reg_A |= {imm_s}; update_nz(reg_A);',
            'and': f'reg_A &= {imm_s}; update_nz(reg_A);',
            'eor': f'reg_A ^= {imm_s}; update_nz(reg_A);',
            'adc': f'cpu_adc({imm_s});',
            'sbc': f'cpu_sbc({imm_s});',
        }
        if opcode in imm_map: return imm_map[opcode], size

    # Indirect
    if '(' in op:
        addr = int(val_raw, 16) & 0xFF
        if opcode == 'jmp': return f'reg_PC = read_pointer(0x{addr:04x}); return;', size
        if '),Y' in op.upper():
            code_map = {
                'lda': f'reg_A = bus_read(read_pointer_indexed_y({hex(addr)})); update_nz(reg_A);',
                'sta': f'bus_write(read_pointer_indexed_y({hex(addr)}), reg_A);',
                'cmp': f'update_flags_cmp(reg_A, bus_read(read_pointer_indexed_y({hex(addr)})));',
            }
            if opcode in code_map: return code_map[opcode], size
        if ',X)' in op.upper():
            return f'reg_A = bus_read(read_pointer_indexed_x({hex(addr)})); update_nz(reg_A);', size

    # Absolute / Indexed / Zero Page
    addr_val = int(val_raw, 16) & 0xFFFF
    m_str = f"0x{addr_val:04x}"
    is_acc = (not op or op.strip().upper() == 'A') and opcode in ['asl', 'lsr', 'rol', 'ror']

    if ',' in op:
        reg = 'reg_X' if ',X' in op.upper() else 'reg_Y'
        final_addr = f"(uint16_t)({m_str} + {reg})"
    else:
        final_addr = m_str

    mapping = {
        'lda': f'reg_A = bus_read({final_addr}); update_nz(reg_A);',
        'ldx': f'reg_X = bus_read({final_addr}); update_nz(reg_X);',
        'ldy': f'reg_Y = bus_read({final_addr}); update_nz(reg_Y);',
        'sta': f'bus_write({final_addr}, reg_A);',
        'stx': f'bus_write({final_addr}, reg_X);',
        'sty': f'bus_write({final_addr}, reg_Y);',
        'bit': f'cpu_bit(bus_read({final_addr}));',
        'cmp': f'update_flags_cmp(reg_A, bus_read({final_addr}));',
        'cpx': f'update_flags_cmp(reg_X, bus_read({final_addr}));',
        'cpy': f'update_flags_cmp(reg_Y, bus_read({final_addr}));',
        'jmp': f'reg_PC = {final_addr}; return;',
        'jsr': f'{{ uint16_t ret = (reg_PC + {size} - 1); push_stack(ret >> 8); push_stack(ret & 0xFF); reg_PC = {final_addr}; return; }}',
        'inc': f'{{ uint8_t v = bus_read({final_addr}) + 1; bus_write({final_addr}, v); update_nz(v); }}',
        'dec': f'{{ uint8_t v = bus_read({final_addr}) - 1; bus_write({final_addr}, v); update_nz(v); }}',
        'asl': f'reg_A = cpu_asl(reg_A);' if is_acc else f'{{ uint8_t v = cpu_asl(bus_read({final_addr})); bus_write({final_addr}, v); }}',
        'lsr': f'reg_A = cpu_lsr(reg_A);' if is_acc else f'{{ uint8_t v = cpu_lsr(bus_read({final_addr})); bus_write({final_addr}, v); }}',
        'rol': f'reg_A = cpu_rol(reg_A);' if is_acc else f'{{ uint8_t v = cpu_rol(bus_read({final_addr})); bus_write({final_addr}, v); }}',
        'ror': f'reg_A = cpu_ror(reg_A);' if is_acc else f'{{ uint8_t v = cpu_ror(bus_read({final_addr})); bus_write({final_addr}, v); }}',
    }

    branch_map = {
        'beq': 'reg_P & FLAG_Z', 'bne': '!(reg_P & FLAG_Z)',
        'bcs': 'reg_P & FLAG_C', 'bcc': '!(reg_P & FLAG_C)',
        'bmi': 'reg_P & FLAG_N', 'bpl': '!(reg_P & FLAG_N)',
    }
    if opcode in branch_map:
        return f'if ({branch_map[opcode]}) {{ reg_PC = {m_str}; return; }}', size

    standalone = {
        'rts': 'reg_PC = (pop_stack() | (pop_stack() << 8)) + 1; return;',
        'rti': 'reg_P = pop_stack(); reg_PC = (pop_stack() | (pop_stack() << 8)); return;',
        'tax': 'reg_X = reg_A; update_nz(reg_X);', 'txa': 'reg_A = reg_X; update_nz(reg_A);',
        'tay': 'reg_Y = reg_A; update_nz(reg_Y);', 'tya': 'reg_A = reg_Y; update_nz(reg_A);',
        'clc': 'reg_P &= ~FLAG_C;', 'sec': 'reg_P |= FLAG_C;',
        'dex': 'reg_X--; update_nz(reg_X);', 'inx': 'reg_X++; update_nz(reg_X);',
        'dey': 'reg_Y--; update_nz(reg_Y);', 'iny': 'reg_Y++; update_nz(reg_Y);',
        'nop': '// NOP',
    }

    if opcode in standalone: return standalone[opcode], size
    return mapping.get(opcode, f"// Unsupported: {opcode}"), size

def pre_parse_symbols():
    if not os.path.exists(ASM_DIR): return
    for filename in sorted(os.listdir(ASM_DIR)):
        if not filename.endswith('.asm'): continue
        current_pc = 0xC000 if "Bank03" in filename else 0x8000
        with open(os.path.join(ASM_DIR, filename), 'r') as f:
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
                    line = re.sub(r'^\w+:\s*', '', line).strip()
                if not line: continue
                match = re.match(r'^(\.?\w{2,5})\s*(.*)$', line)
                if match:
                    current_pc += get_ins_size(match.group(1), match.group(2))

def convert_all():
    print(f"Resolving ASM from: {ASM_DIR}")
    print(f"Writing CPP to: {OUT_DIR}")
    if not os.path.exists(OUT_DIR): os.makedirs(OUT_DIR)
    pre_parse_symbols()
    
    for filename in sorted(os.listdir(ASM_DIR)):
        if not filename.endswith('.asm') or "Defines" in filename: continue
        bank_name = os.path.splitext(filename)[0].replace("-", "_")
        current_pc = 0xC000 if "Bank03" in filename else 0x8000
        
        with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
            out.write('#include "cpu_shared.h"\nnamespace ' + bank_name + ' {\nvoid execute() {\nswitch(reg_PC) {\n')
            with open(os.path.join(ASM_DIR, filename), 'r') as f:
                for line in f:
                    clean = line.split(';')[0].strip()
                    if not clean: continue
                    org_m = re.match(r'^\.org\s+\$?([0-9A-Fa-f]+)', clean)
                    if org_m: current_pc = int(org_m.group(1), 16); continue
                    
                    match = re.match(r'^(?:(\w+):)?\s*(\.?\w{2,5})?\s*(.*)$', clean)
                    if not match: continue
                    _, opcode, operand = match.groups()
                    
                    if opcode:
                        code, size = translate_line(opcode, operand, current_pc)
                        if size > 0 and opcode.lower() in VALID_OPCODES:
                            out.write(f'            case 0x{current_pc & 0xFFFF:04x}: {code}')
                            if "return" not in code: out.write(f' reg_PC += {size}; return;')
                            out.write(f' // {clean}\n')
                        current_pc += size
            out.write('default: return; }}}')

if __name__ == "__main__":
    convert_all()
