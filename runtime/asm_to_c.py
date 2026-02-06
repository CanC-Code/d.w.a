import os
import re

ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"

# Global dictionary to store all .alias and label definitions
global_symbols = {}

def pre_parse_symbols():
    """Scan all files first to build a global symbol table."""
    for filename in os.listdir(ASM_DIR):
        if not (filename.endswith('.asm') or filename.endswith('.txt')):
            continue
        try:
            with open(os.path.join(ASM_DIR, filename), 'r') as f:
                for line in f:
                    # Strip comments
                    clean = line.split(';')[0].strip()
                    if not clean: continue

                    # 1. Match .alias Name Value
                    alias_match = re.match(r'^\.alias\s+(\w+)\s+\$?([0-9A-Fa-f]+)', clean)
                    if alias_match:
                        name, val = alias_match.groups()
                        global_symbols[name] = f"0x{val}"

                    # 2. Match LXXXX: labels (Standard NES style)
                    label_match = re.match(r'^(L[0-9A-Fa-f]{4}):', clean)
                    if label_match:
                        name = label_match.group(1)
                        global_symbols[name] = f"0x{name[1:]}"

                    # 3. Match named labels
                    named_label = re.match(r'^(\w+):', clean)
                    if named_label:
                        name = named_label.group(1)
                        # If it's not an L-address, we keep the name for the C++ linker/header
                        if name not in global_symbols:
                            global_symbols[name] = name
        except Exception as e:
            print(f"Error parsing {filename}: {e}")

def get_ins_size(opcode, op):
    """Determines instruction size (1, 2, or 3 bytes)."""
    if not op or op.strip() == "": return 1
    if op.startswith('#'): return 2 # Immediate
    if '(' in op: return 2          # Indirect (usually Zero Page in NES)

    # If it contains a 3-4 digit hex, it's absolute (3 bytes)
    if re.search(r'\$[0-9A-Fa-f]{3,4}', op): return 3
    
    # Check resolved symbol value
    clean_op = op.strip().replace('$', '').split(',')[0].strip('()')
    if clean_op in global_symbols:
        val = str(global_symbols[clean_op])
        if '0x' in val and len(val) > 4: return 3 # e.g., 0xC006

    return 2 # Default to Zero Page/Relative

def parse_addr(opcode, op):
    """Converts ASM operand to C++ bus access."""
    op = op.strip()
    if not op: return "0"

    # Handle Immediate
    if op.startswith('#'):
        val = op.replace('#', '').replace('$', '0x').replace('%', '0b')
        return val

    # Resolve core symbol
    core = re.search(r'[a-zA-Z0-9_]+', op.replace('$', '')).group(0)
    resolved = global_symbols.get(core, f"0x{core}" if core.isalnum() else core)

    # Addressing Mode Translation
    if '(' in op and '),Y' in op.upper():
        return f"bus_read(read_pointer({resolved}) + reg_Y)"
    
    is_write = opcode in ['STA', 'STX', 'STY', 'INC', 'DEC', 'ASL', 'LSR', 'ROL', 'ROR']
    
    if ',X' in op.upper(): addr_calc = f"({resolved} + reg_X)"
    elif ',Y' in op.upper(): addr_calc = f"({resolved} + reg_Y)"
    else: addr_calc = resolved

    return addr_calc if is_write else f"bus_read({addr_calc})"

def translate_line(line, line_idx, all_lines):
    """Translates a single line of ASM to C++ code."""
    clean = line.split(';')[0].strip()
    match = re.match(r'^(?:\w+:)?\s*(\w{3})\s*(.*)$', clean)
    if not match: return None, 0

    opcode, op = match.groups()
    opcode = opcode.upper()
    op = op.strip()

    # 1. Resolve relative branch (+ / -)
    target_hex = "0"
    if op in ["+", "++", "-", "--"]:
        direction = 1 if "+" in op else -1
        count = len(op)
        found = 0
        search_idx = line_idx
        while 0 <= search_idx < len(all_lines):
            search_idx += direction
            if search_idx < 0 or search_idx >= len(all_lines): break
            if f":*" in all_lines[search_idx] or (direction == 1 and op in all_lines[search_idx]):
                found += 1
                if found == count:
                    # Look for a label on this line
                    lbl = re.search(r'(L[0-9A-F]{4}):', all_lines[search_idx])
                    target_hex = f"0x{lbl.group(1)[1:]}" if lbl else "/* Relative Error */"
                    break
    else:
        # Standard symbol resolution for JMP/Branch
        target_match = re.search(r'[a-zA-Z0-9_]+', op.replace('$', ''))
        if target_match:
            core = target_match.group(0)
            target_hex = global_symbols.get(core, core)

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
        'BIT': f"cpu_bit({val});",
        'CMP': f"update_flags_cmp(reg_A, {val});",
        'CPX': f"update_flags_cmp(reg_X, {val});",
        'CPY': f"update_flags_cmp(reg_Y, {val});",
        'INC': f"{{ uint16_t a = {val}; bus_write(a, bus_read(a) + 1); }}",
        'DEC': f"{{ uint16_t a = {val}; bus_write(a, bus_read(a) - 1); }}",
        'TAX': "reg_X = reg_A; update_nz(reg_X);",
        'TAY': "reg_Y = reg_A; update_nz(reg_Y);",
        'TXA': "reg_A = reg_X; update_nz(reg_A);",
        'TYA': "reg_A = reg_Y; update_nz(reg_A);",
        'PHA': "push_stack(reg_A);",
        'PLA': "reg_A = pop_stack(); update_nz(reg_A);",
        'RTS': "reg_PC = (pop_stack() | (pop_stack() << 8)) + 1; return;",
        'RTI': "reg_P = pop_stack(); reg_PC = (pop_stack() | (pop_stack() << 8)); return;",
        'JSR': f"{{ uint16_t ret = reg_PC + 2; push_stack(ret >> 8); push_stack(ret & 0xFF); reg_PC = {target_hex}; return; }}",
        'JMP': f"reg_PC = {target_hex}; return;",
        'BNE': f"if (!(reg_P & 0x02)) {{ reg_PC = {target_hex}; return; }}",
        'BEQ': f"if (reg_P & 0x02) {{ reg_PC = {target_hex}; return; }}",
        'BCC': f"if (!(reg_P & 0x01)) {{ reg_PC = {target_hex}; return; }}",
        'BCS': f"if (reg_P & 0x01) {{ reg_PC = {target_hex}; return; }}",
        'SEC': "reg_P |= 0x01;",
        'CLC': "reg_P &= ~0x01;",
        'SEI': "reg_P |= 0x04;",
        'CLI': "reg_P &= ~0x04;",
    }

    code = mapping.get(opcode, f"// UNHANDLED: {opcode} {op}")
    return code, size

def convert_file(filename):
    bank_name = os.path.splitext(filename)[0].replace("-", "_")
    with open(os.path.join(ASM_DIR, filename), 'r') as f:
        lines = f.readlines()

    with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
        out.write('#include "cpu_shared.h"\n\nnamespace ' + bank_name + ' {\n')
        out.write('    void execute() {\n        switch(reg_PC) {\n')

        for idx, line in enumerate(lines):
            # Check for label or code lines
            label_match = re.search(r'^\s*(L[0-9A-F]{4}):', line)
            if label_match:
                addr = f"0x{label_match.group(1)[1:]}"
                out.write(f'            case {addr}:\n')
                
                code, size = translate_line(line, idx, lines)
                if code:
                    # Logic: If it's a branch/jump, it has its own 'return'. 
                    # Otherwise, increment PC and return to the main loop.
                    suffix = "" if "return" in code else f" reg_PC += {size}; return;"
                    out.write(f'                {code}{suffix}\n')

        out.write('            default: reg_PC++; return;\n        }\n    }\n}\n')

if __name__ == "__main__":
    os.makedirs(OUT_DIR, exist_ok=True)
    pre_parse_symbols()
    for f in os.listdir(ASM_DIR):
        if f.endswith('.asm') or f.endswith('.txt'):
            convert_file(f)
