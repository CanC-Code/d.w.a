import os
import re

ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"

# Global dictionary to store all .alias and label definitions
global_symbols = {}

def pre_parse_symbols():
    """Scan all files first to build a global symbol table."""
    if not os.path.exists(ASM_DIR):
        print(f"Directory {ASM_DIR} not found.")
        return

    # Track current PC per file based on .org
    for filename in os.listdir(ASM_DIR):
        if not (filename.endswith('.asm') or filename.endswith('.txt')):
            continue
        
        current_pc = 0x8000 # Default for NES PRG
        try:
            with open(os.path.join(ASM_DIR, filename), 'r') as f:
                for line in f:
                    clean = line.split(';')[0].strip()
                    if not clean: continue

                    # Handle .org to track address-less labels
                    org_match = re.match(r'^\.org\s+\$([0-9A-Fa-f]+)', clean)
                    if org_match:
                        current_pc = int(org_match.group(1), 16)
                        continue

                    # 1. Match .alias
                    alias_match = re.match(r'^\.alias\s+(\w+)\s+\$?([0-9A-Fa-f]+)', clean)
                    if alias_match:
                        name, val = alias_match.groups()
                        global_symbols[name] = f"0x{val}"
                        continue

                    # 2. Match Labels (LXXXX: or Name:)
                    label_match = re.match(r'^(\w+):', clean)
                    if label_match:
                        name = label_match.group(1)
                        # If label is L8000 style, use the hex. Otherwise use current_pc
                        if re.match(r'^L[0-9A-Fa-f]{4}$', name):
                            global_symbols[name] = f"0x{name[1:]}"
                        else:
                            global_symbols[name] = hex(current_pc)

                    # Increment PC based on instruction size (approximation for label tracking)
                    ins_match = re.match(r'^\s*(\w{3})\s*(.*)$', clean)
                    if ins_match:
                        opcode, op = ins_match.groups()
                        current_pc += get_ins_size(opcode, op)
                        
        except Exception as e:
            print(f"Error parsing {filename}: {e}")

def resolve_symbol(symbol):
    """Ensures a symbol is returned as a hex string for the C++ compiler."""
    if not symbol: return "0"
    if symbol.startswith("0x") or symbol.isdigit():
        return symbol
    
    # Clean symbol from addressing mode characters
    clean_sym = symbol.strip('#$() ,XY')
    
    resolved = global_symbols.get(clean_sym)
    if resolved:
        return resolved
    
    # Fallback for raw hex constants like $C5E0
    if all(c in "0123456789ABCDEFabcdef" for c in clean_sym) and len(clean_sym) >= 2:
        return f"0x{clean_sym}"
    
    return f"/* UNRESOLVED: {symbol} */ 0"

def get_ins_size(opcode, op):
    """Determines instruction size."""
    if not op or op.strip() == "": return 1
    if op.startswith('#'): return 2
    if '(' in op: return 2
    if re.search(r'\$[0-9A-Fa-f]{3,4}', op): return 3
    return 2

def parse_addr(opcode, op):
    """Converts ASM operand to C++ bus access."""
    op = op.strip()
    if not op: return "0"

    # Handle Immediate
    if op.startswith('#'):
        return resolve_symbol(op)

    # Addressing Mode Translation
    is_write = opcode in ['STA', 'STX', 'STY', 'INC', 'DEC', 'ASL', 'LSR', 'ROL', 'ROR']
    
    if '(' in op and '),Y' in op.upper():
        resolved = resolve_symbol(op)
        return f"read_pointer_indexed_y({resolved})" # Custom helper in C++

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
    
    # Branch targets must be hex
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
        'ASL': f"reg_A = cpu_asl(reg_A);" if not op else f"bus_write({val}, cpu_asl(bus_read({val})));",
        'BNE': f"if (!(reg_P & 0x02)) {{ reg_PC = {target_hex}; return; }}",
        'BEQ': f"if (reg_P & 0x02) {{ reg_PC = {target_hex}; return; }}",
        'BCC': f"if (!(reg_P & 0x01)) {{ reg_PC = {target_hex}; return; }}",
        'BCS': f"if (reg_P & 0x01) {{ reg_PC = {target_hex}; return; }}",
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
    }

    code = mapping.get(opcode, f"/* {opcode} {op} */")
    return code, size

def convert_file(filename):
    bank_name = os.path.splitext(filename)[0].replace("-", "_")
    with open(os.path.join(ASM_DIR, filename), 'r') as f:
        lines = f.readlines()

    with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
        out.write('#include "cpu_shared.h"\n\nnamespace ' + bank_name + ' {\n')
        out.write('    void execute() {\n        switch(reg_PC) {\n')

        for line in lines:
            label_match = re.search(r'^\s*(\w+):', line)
            if label_match:
                label_name = label_match.group(1)
                addr = resolve_symbol(label_name)
                out.write(f'            case {addr}:\n')
                
                code, size = translate_line(line)
                if code:
                    suffix = "" if "return" in code else f" reg_PC += {size}; return;"
                    out.write(f'                {code}{suffix}\n')

        out.write('            default: reg_PC++; return;\n        }\n    }\n}\n')

if __name__ == "__main__":
    os.makedirs(OUT_DIR, exist_ok=True)
    pre_parse_symbols()
    for f in os.listdir(ASM_DIR):
        if f.endswith('.asm') or f.endswith('.txt'):
            convert_file(f)
