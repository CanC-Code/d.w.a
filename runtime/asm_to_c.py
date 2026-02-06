import os
import re

ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"

# Global dictionary to store all .alias and label definitions across all files
global_symbols = {}

def pre_parse_symbols():
    """Scan all files first to build a global symbol table."""
    for filename in os.listdir(ASM_DIR):
        if not (filename.endswith('.asm') or filename.endswith('.txt')):
            continue
        try:
            with open(os.path.join(ASM_DIR, filename), 'r') as f:
                for line in f:
                    clean = line.split(';')[0].strip()
                    # Match .alias Name Value
                    alias_match = re.match(r'^\.alias\s+(\w+)\s+\$?([0-9A-F]+)', clean)
                    if alias_match:
                        name, val = alias_match.groups()
                        global_symbols[name] = f"0x{val}"

                    # Match LXXXX: labels
                    label_match = re.match(r'^(L[0-9A-F]{4}):', clean)
                    if label_match:
                        name = label_match.group(1)
                        global_symbols[name] = name.replace('L', '0x')
        except Exception as e:
            print(f"Error parsing {filename}: {e}")

def get_ins_size(opcode, op):
    if not op or op.strip() == "": return 1
    if op.startswith('#'): return 2
    if '(' in op: return 2 
    
    # Extract numeric part to judge size
    val_match = re.search(r'\$?([0-9A-Fa-f]{3,4})', op)
    if val_match: return 3
    
    # Check if symbol is known to be 16-bit
    clean_op = op.strip().replace('$', '').split(',')[0].strip('()')
    if clean_op in global_symbols:
        val = global_symbols[clean_op].replace('0x', '')
        if len(val) > 2: return 3
        
    return 2 

def parse_addr(opcode, op):
    op = op.strip()
    if not op: return "0"

    # 1. Handle Binary Literals
    if '% ' in op or op.startswith('%') or op.startswith('#%'):
        bin_val = op.replace('#', '').replace('%', '').strip()
        return str(int(bin_val, 2))

    # 2. Extract Core Symbol/Value
    # core_match pulls the alphanumeric part or hex part
    core_match = re.search(r'[\$]?([0-9A-Fa-f\w]+)', op)
    if not core_match: return "0"
    core = core_match.group(1)
    
    # 3. Resolve Symbol
    if core in global_symbols:
        resolved = global_symbols[core]
    elif '$' in op:
        # It's a raw hex literal from ASM, convert to C++ hex
        resolved = f"0x{core}"
    else:
        # It's a label not yet found; keep as name for Defines.h to handle
        resolved = core

    # 4. Handle Addressing Modes
    if '(' in op and '),Y' in op.upper():
        return f"bus_read(read_pointer({resolved}) + reg_Y)"
    
    is_write = opcode in ['STA', 'STX', 'STY', 'INC', 'DEC', 'ASL', 'LSR', 'ROL', 'ROR']
    
    if ',X' in op.upper(): addr_calc = f"({resolved} + reg_X)"
    elif ',Y' in op.upper(): addr_calc = f"({resolved} + reg_Y)"
    else: addr_calc = resolved

    return addr_calc if is_write else f"bus_read({addr_calc})"

def translate_line(line):
    clean = line.split(';')[0].strip()
    # Match opcode and operand
    match = re.match(r'^(?:\w+:)?\s*(\w{3})\s*(.*)$', clean)
    if not match: return None, 0

    opcode, op = match.groups()
    opcode = opcode.upper()
    op = op.strip()

    # 5. Fix relative branch syntax errors (+ / -)
    if op in ["+", "++", "+++", "-", "--", "---"]:
        return f"// TODO: Resolve relative branch {opcode} {op}", 0

    val = parse_addr(opcode, op)
    size = get_ins_size(opcode, op)

    # 6. Resolve jump/branch targets (similar logic to parse_addr but no bus_read)
    target_match = re.search(r'[\$]?([0-9A-Fa-f\w]+)', op)
    if target_match:
        target_core = target_match.group(1)
        if target_core in global_symbols:
            target_hex = global_symbols[target_core]
        elif '$' in op:
            target_hex = f"0x{target_core}"
        else:
            target_hex = target_core
    else:
        target_hex = "0"

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
        'JSR': f"{{ uint16_t ret = reg_PC + 2; push_stack(ret >> 8); push_stack(ret & 0xFF); reg_PC = {target_hex}; return; }}",
        'JMP': f"reg_PC = {target_hex}; return;",
        'BNE': f"if (!(reg_P & 0x02)) {{ reg_PC = {target_hex}; return; }}",
        'BEQ': f"if (reg_P & 0x02) {{ reg_PC = {target_hex}; return; }}",
        'BCC': f"if (!(reg_P & 0x01)) {{ reg_PC = {target_hex}; return; }}",
        'BCS': f"if (reg_P & 0x01) {{ reg_PC = {target_hex}; return; }}",
        'SEC': "reg_P |= 0x01;",
        'CLC': "reg_P &= ~0x01;",
    }

    code = mapping.get(opcode, f"// TODO: {opcode} {op}")
    return code, size

def convert_file(filename):
    bank_name = os.path.splitext(filename)[0].replace("-", "_")
    with open(os.path.join(ASM_DIR, filename), 'r') as f:
        lines = f.readlines()

    with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
        out.write('#include "cpu_shared.h"\n\nnamespace ' + bank_name + ' {\n')
        out.write('    void execute() {\n        switch(reg_PC) {\n')

        for line in lines:
            # Check for label cases
            label_match = re.search(r'^\s*(L[0-9A-F]{4}):', line)
            if label_match:
                addr = label_match.group(1).replace('L', '0x')
                out.write(f'            case {addr}:\n')
                code, size = translate_line(line)
                if code:
                    # Don't increment PC if the instruction forces a return (JMP/JSR/RTS)
                    suffix = "" if "return" in code else f" reg_PC += {size}; return;"
                    out.write(f'                {code}{suffix}\n')

        out.write('            default: reg_PC++; return;\n        }\n    }\n}\n')

# Main Process
if __name__ == "__main__":
    os.makedirs(OUT_DIR, exist_ok=True)
    print("Pre-parsing symbols...")
    pre_parse_symbols()
    
    print("Converting files...")
    for f in os.listdir(ASM_DIR):
        if f.endswith('.asm') or f.endswith('.txt'):
            convert_file(f)
    print("Done.")
