import os
import re

ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"

global_symbols = {}

def pre_parse_symbols():
    if not os.path.exists(ASM_DIR):
        print(f"Directory {ASM_DIR} not found.")
        return

    for filename in os.listdir(ASM_DIR):
        if not (filename.endswith('.asm') or filename.endswith('.txt')):
            continue

        current_pc = 0x8000 
        try:
            with open(os.path.join(ASM_DIR, filename), 'r') as f:
                for line in f:
                    clean = line.split(';')[0].strip()
                    if not clean: continue

                    org_match = re.match(r'^\.org\s+\$([0-9A-Fa-f]+)', clean)
                    if org_match:
                        current_pc = int(org_match.group(1), 16)
                        continue

                    alias_match = re.match(r'^\.alias\s+(\w+)\s+\$?([0-9A-Fa-f]+)', clean)
                    if alias_match:
                        name, val = alias_match.groups()
                        global_symbols[name] = f"0x{val.upper()}"
                        continue

                    label_match = re.match(r'^(\w+):', clean)
                    if label_match:
                        name = label_match.group(1)
                        if re.match(r'^L[0-9A-Fa-f]{4}$', name):
                            global_symbols[name] = f"0x{name[1:].upper()}"
                        else:
                            global_symbols[name] = hex(current_pc).upper().replace('X', 'x')

                    ins_match = re.match(r'^\s*(\w{3})\s*(.*)$', clean)
                    if ins_match:
                        opcode, op = ins_match.groups()
                        current_pc += get_ins_size(opcode, op)
        except Exception as e:
            print(f"Error parsing {filename}: {e}")

def resolve_symbol(symbol):
    if not symbol: return "0x0"
    clean_sym = symbol.strip('#$() ,XY')
    if clean_sym in global_symbols:
        return global_symbols[clean_sym]
    if all(c in "0123456789ABCDEFabcdef" for c in clean_sym) and len(clean_sym) >= 2:
        return f"0x{clean_sym.upper()}"
    return f"0x0 /* UNRESOLVED: {clean_sym} */"

def get_ins_size(opcode, op):
    if not op or op.strip() == "": return 1
    if op.startswith('#'): return 2
    if '(' in op: return 2
    # If the operand is a 4-digit hex or a known label > 0xFF, it's 3 bytes
    res = resolve_symbol(op)
    if "0x" in res and not "UNRESOLVED" in res:
        val = int(res.split(' ')[0], 16)
        if val > 0xFF: return 3
    return 2

def parse_addr(opcode, op):
    op = op.strip()
    if not op: return "0"
    if op.startswith('#'): return resolve_symbol(op)

    is_write = opcode in ['STA', 'STX', 'STY', 'INC', 'DEC', 'ASL', 'LSR', 'ROL', 'ROR']
    
    if '(' in op and '),Y' in op.upper():
        return f"read_pointer_indexed_y({resolve_symbol(op)})"

    resolved = resolve_symbol(op)
    if ',X' in op.upper(): addr_calc = f"({resolved} + reg_X)"
    elif ',Y' in op.upper(): addr_calc = f"({resolved} + reg_Y)"
    else: addr_calc = resolved

    return addr_calc if is_write else f"bus_read({addr_calc})"

def translate_line(line):
    clean = line.split(';')[0].strip()
    match = re.match(r'^(?:\w+:)?\s*(\w{3})\s*(.*)$', clean)
    if not match: return None, 0

    opcode, op = match.groups()
    opcode = opcode.upper()
    target_hex = resolve_symbol(op)
    val = parse_addr(opcode, op)
    size = get_ins_size(opcode, op)

    mapping = {
        'LDA': f"reg_A = {val}; update_nz(reg_A);",
        'LDX': f"reg_X = {val}; update_nz(reg_X);",
        'LDY': f"reg_Y = {val}; update_nz(reg_Y);",
        'STA': f"bus_write({val}, reg_A);",
        'STX': f"bus_write({val}, reg_X);",
        'STY': f"bus_write({val}, reg_Y);",
        'ADC': f"cpu_adc({val});",
        'SBC': f"cpu_sbc({val});",
        'AND': f"reg_A &= {val}; update_nz(reg_A);",
        'ORA': f"reg_A |= {val}; update_nz(reg_A);",
        'EOR': f"reg_A ^= {val}; update_nz(reg_A);",
        'CMP': f"update_flags_cmp(reg_A, {val});",
        'CPX': f"update_flags_cmp(reg_X, {val});",
        'CPY': f"update_flags_cmp(reg_Y, {val});",
        'BIT': f"cpu_bit({val});",
        'INC': f"{{ uint8_t tmp = bus_read({val}) + 1; bus_write({val}, tmp); update_nz(tmp); }}",
        'DEC': f"{{ uint8_t tmp = bus_read({val}) - 1; bus_write({val}, tmp); update_nz(tmp); }}",
        'ASL': f"reg_A = cpu_asl(reg_A);" if not op else f"bus_write({val}, cpu_asl(bus_read({val})));",
        'LSR': f"reg_A = cpu_lsr(reg_A);" if not op else f"bus_write({val}, cpu_lsr(bus_read({val})));",
        'ROL': f"reg_A = cpu_rol(reg_A);" if not op else f"bus_write({val}, cpu_rol(bus_read({val})));",
        'ROR': f"reg_A = cpu_ror(reg_A);" if not op else f"bus_write({val}, cpu_ror(bus_read({val})));",
        'BNE': f"if (!(reg_P & 0x02)) {{ reg_PC = {target_hex}; return; }}",
        'BEQ': f"if (reg_P & 0x02) {{ reg_PC = {target_hex}; return; }}",
        'BCC': f"if (!(reg_P & 0x01)) {{ reg_PC = {target_hex}; return; }}",
        'BCS': f"if (reg_P & 0x01) {{ reg_PC = {target_hex}; return; }}",
        'BMI': f"if (reg_P & 0x80) {{ reg_PC = {target_hex}; return; }}",
        'BPL': f"if (!(reg_P & 0x80)) {{ reg_PC = {target_hex}; return; }}",
        'JSR': f"{{ uint16_t ret = reg_PC + 2; push_stack(ret >> 8); push_stack(ret & 0xFF); reg_PC = {target_hex}; return; }}",
        'JMP': f"reg_PC = {target_hex}; return;",
        'RTS': "reg_PC = (pop_stack() | (pop_stack() << 8)) + 1; return;",
        'INY': "reg_Y++; update_nz(reg_Y);",
        'DEY': "reg_Y--; update_nz(reg_Y);",
        'INX': "reg_X++; update_nz(reg_X);",
        'DEX': "reg_X--; update_nz(reg_X);",
        'TAX': "reg_X = reg_A; update_nz(reg_X);",
        'TAY': "reg_Y = reg_A; update_nz(reg_Y);",
        'TXA': "reg_A = reg_X; update_nz(reg_A);",
        'TYA': "reg_A = reg_Y; update_nz(reg_A);",
        'PHA': "push_stack(reg_A);",
        'PLA': "reg_A = pop_stack(); update_nz(reg_A);",
        'PHP': "push_stack(reg_P);",
        'PLP': "reg_P = pop_stack();",
        'CLC': "reg_P &= ~0x01;",
        'SEC': "reg_P |= 0x01;",
        'CLD': "reg_P &= ~0x08;",
        'SED': "reg_P |= 0x08;",
        'TXS': "reg_S = reg_X;",
        'TSX': "reg_X = reg_S; update_nz(reg_X);",
    }

    code = mapping.get(opcode, f"/* {opcode} {op} */")
    return code, size

def convert_file(filename):
    bank_name = os.path.splitext(filename)[0].replace("-", "_")
    with open(os.path.join(ASM_DIR, filename), 'r') as f:
        lines = f.readlines()

    used_addresses = set()

    with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
        out.write('#include "cpu_shared.h"\n\n')
        # Use extern "C" to match the linkage of your core helper functions
        out.write('extern "C" {\n')
        out.write('    namespace ' + bank_name + ' {\n')
        out.write('        void execute() {\n            switch(reg_PC) {\n')

        for line in lines:
            label_match = re.search(r'^\s*(\w+):', line)
            if label_match:
                label_name = label_match.group(1)
                addr = resolve_symbol(label_name)

                if addr not in used_addresses and "UNRESOLVED" not in addr:
                    out.write(f'                case {addr}:\n')
                    used_addresses.add(addr)

                code, size = translate_line(line)
                if code:
                    prefix = "                    " if addr in used_addresses else f"                    /* Alias: {label_name} */ "
                    suffix = "" if "return" in code else f" reg_PC += {size}; return;"
                    out.write(f'{prefix}{code}{suffix}\n')

        out.write('                default: reg_PC++; return;\n            }\n        }\n    }\n}\n')

if __name__ == "__main__":
    os.makedirs(OUT_DIR, exist_ok=True)
    pre_parse_symbols()
    for f in os.listdir(ASM_DIR):
        if f.endswith('.asm') or f.endswith('.txt'):
            convert_file(f)
