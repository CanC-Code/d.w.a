import os
import re

ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"

# Global dictionary to store all .alias and label definitions
# Key: Symbol Name, Value: (Hex Value, Original Line Index in file)
global_symbols = {}

def pre_parse_symbols():
    """Scan all files first to build a global symbol table."""
    if not os.path.exists(ASM_DIR):
        print(f"Directory {ASM_DIR} not found.")
        return

    for filename in os.listdir(ASM_DIR):
        if not (filename.endswith('.asm') or filename.endswith('.txt')):
            continue
        try:
            with open(os.path.join(ASM_DIR, filename), 'r') as f:
                for line_idx, line in enumerate(f):
                    clean = line.split(';')[0].strip()
                    if not clean: continue

                    # 1. Match .alias Name Value
                    # [span_3](start_span)[span_4](start_span)Example: .alias ModAttribBits $C006[span_3](end_span)[span_4](end_span)
                    alias_match = re.match(r'^\.alias\s+(\w+)\s+\$?([0-9A-Fa-f]+)', clean)
                    if alias_match:
                        name, val = alias_match.groups()
                        # [span_5](start_span)[span_6](start_span)Use a list for aliases since they can be redefined[span_5](end_span)[span_6](end_span)
                        if name not in global_symbols:
                            global_symbols[name] = f"0x{val}"

                    # 2. [span_7](start_span)[span_8](start_span)Match LXXXX: labels (Standard NES style)[span_7](end_span)[span_8](end_span)
                    label_match = re.match(r'^(L[0-9A-Fa-f]{4}):', clean)
                    if label_match:
                        name = label_match.group(1)
                        global_symbols[name] = f"0x{name[1:]}"

                    # 3. [span_9](start_span)Match named labels (e.g., BankPointers:)[span_9](end_span)
                    named_label = re.match(r'^(\w+):', clean)
                    if named_label:
                        name = named_label.group(1)
                        if name not in global_symbols:
                            global_symbols[name] = name
        except Exception as e:
            print(f"Error parsing {filename}: {e}")

def resolve_relative_label(current_idx, direction, all_lines):
    """
    Resolves anonymous labels like '+' or '-'.
    '+' looks forward for the next label ending in ':' or '*'.
    '-' looks backward.
    """
    step = 1 if direction == '+' else -1
    search_idx = current_idx + step
    
    while 0 <= search_idx < len(all_lines):
        line = all_lines[search_idx].split(';')[0].strip()
        # Look for the specific anonymous marker or a standard label
        if direction in line or (':' in line and not line.startswith('.')):
            label_match = re.search(r'^(L[0-9A-Fa-f]{4}):|(\w+):', line)
            if label_match:
                found = label_match.group(1) or label_match.group(2)
                return global_symbols.get(found, found)
        search_idx += step
    return "0x0000"

def get_ins_size(opcode, op):
    """Determines instruction size (1, 2, or 3 bytes)."""
    if not op or op.strip() == "": return 1
    if op.startswith('#'): return 2
    if '(' in op: return 2 # Zero-page indirect

    # [span_10](start_span)Explicit 16-bit hex[span_10](end_span)
    if re.search(r'\$[0-9A-Fa-f]{3,4}', op): return 3

    clean_op = op.strip().replace('$', '').split(',')[0].strip('()')
    if clean_op in global_symbols:
        val = str(global_symbols[clean_op])
        # If the resolved address is > 0xFF, it's absolute (3 bytes)
        if '0x' in val and len(val) > 4: return 3

    return 2 # Default to Zero-page or Relative

def parse_addr(opcode, op, current_idx, all_lines):
    """Converts ASM operand to C++ bus access."""
    op = op.strip()
    if not op: return "0"

    # [span_11](start_span)[span_12](start_span)Handle Relative Anonymous Labels[span_11](end_span)[span_12](end_span)
    if op in ["+", "-", "*"]:
        return resolve_relative_label(current_idx, op, all_lines)

    # Handle Immediate
    if op.startswith('#'):
        val = op.replace('#', '').replace('$', '0x').replace('%', '0b')
        return val

    core_match = re.search(r'[a-zA-Z0-9_]+', op.replace('$', ''))
    if not core_match: return "0"

    core = core_match.group(0)
    resolved = global_symbols.get(core, f"0x{core}" if core.isalnum() else core)

    # Addressing Mode Translation
    if '(' in op and '),Y' in op.upper():
        return f"bus_read(read_pointer_indexed_y({resolved}))"

    is_write = opcode in ['STA', 'STX', 'STY', 'INC', 'DEC', 'ASL', 'LSR', 'ROL', 'ROR']

    if ',X' in op.upper(): addr_calc = f"({resolved} + reg_X)"
    elif ',Y' in op.upper(): addr_calc = f"({resolved} + reg_Y)"
    else: addr_calc = resolved

    return addr_calc if is_write else f"bus_read({addr_calc})"

def translate_line(line, line_idx, all_lines):
    """Translates a single line of ASM to C++ code."""
    clean = line.split(';')[0].strip()
    # [span_13](start_span)[span_14](start_span)Support both "LABEL: OP OP" and just "OP OP"[span_13](end_span)[span_14](end_span)
    match = re.match(r'^(?:\w+:)?\s*(\w{3})\s*(.*)$', clean)
    if not match: return None, 0

    opcode, op = match.groups()
    opcode = opcode.upper()
    op = op.strip()

    # Determine Branch Target
    target_hex = "0"
    if op in ["+", "-", "*"]:
        target_hex = resolve_relative_label(line_idx, op, all_lines)
    else:
        target_match = re.search(r'[a-zA-Z0-9_]+', op.replace('$', ''))
        if target_match:
            core = target_match.group(0)
            target_hex = global_symbols.get(core, core)

    val = parse_addr(opcode, op, line_idx, all_lines)
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
        'TXS': "reg_S = reg_X;",
        'TSX': "reg_X = reg_S; update_nz(reg_X);",
        'PHA': "push_stack(reg_A);",
        'PLA': "reg_A = pop_stack(); update_nz(reg_A);",
        'PHP': "push_stack(reg_P);",
        'PLP': "reg_P = pop_stack();",
        'RTS': "reg_PC = (pop_stack() | (pop_stack() << 8)) + 1; return;",
        'RTI': "reg_P = pop_stack(); reg_PC = (pop_stack() | (pop_stack() << 8)); return;",
        'JSR': f"{{ uint16_t ret = reg_PC + 2; push_stack(ret >> 8); push_stack(ret & 0xFF); reg_PC = {target_hex}; return; }}",
        'JMP': f"reg_PC = {target_hex}; return;",
        'BNE': f"if (!(reg_P & 0x02)) {{ reg_PC = {target_hex}; return; }}",
        'BEQ': f"if (reg_P & 0x02) {{ reg_PC = {target_hex}; return; }}",
        'BCC': f"if (!(reg_P & 0x01)) {{ reg_PC = {target_hex}; return; }}",
        'BCS': f"if (reg_P & 0x01) {{ reg_PC = {target_hex}; return; }}",
        'BPL': f"if (!(reg_P & 0x80)) {{ reg_PC = {target_hex}; return; }}",
        'BMI': f"if (reg_P & 0x80) {{ reg_PC = {target_hex}; return; }}",
        'BVC': f"if (!(reg_P & 0x40)) {{ reg_PC = {target_hex}; return; }}",
        'BVS': f"if (reg_P & 0x40) {{ reg_PC = {target_hex}; return; }}",
        'SEC': "reg_P |= 0x01;",
        'CLC': "reg_P &= ~0x01;",
        'SEI': "reg_P |= 0x04;",
        'CLI': "reg_P &= ~0x04;",
        'CLV': "reg_P &= ~0x40;",
        'INY': "reg_Y++; update_nz(reg_Y);",
        'DEY': "reg_Y--; update_nz(reg_Y);",
        'INX': "reg_X++; update_nz(reg_X);",
        'DEX': "reg_X--; update_nz(reg_X);",
        'ASL': "reg_A = cpu_asl(reg_A);", # Special case for accumulator, parser needs logic
        'NOP': "/* NOP */;",
    }

    code = mapping.get(opcode, f"// UNHANDLED: {opcode} {op}")
    return code, size

def convert_file(filename):
    bank_name = os.path.splitext(filename)[0].replace("-", "_")
    input_path = os.path.join(ASM_DIR, filename)
    if not os.path.exists(input_path): return

    with open(input_path, 'r') as f:
        lines = f.readlines()

    with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
        out.write('#include "cpu_shared.h"\n\nnamespace ' + bank_name + ' {\n')
        out.write('    void execute() {\n        switch(reg_PC) {\n')

        for idx, line in enumerate(lines):
            # [span_15](start_span)[span_16](start_span)Capture standard LXXXX labels and named labels[span_15](end_span)[span_16](end_span)
            label_match = re.search(r'^\s*(L[0-9A-F]{4}):|^\s*(\w+):', line)
            if label_match:
                label_name = label_match.group(1) or label_match.group(2)
                addr = global_symbols.get(label_name, "0x0000")
                
                # [span_17](start_span)Check if this line also contains an instruction[span_17](end_span)
                code, size = translate_line(line, idx, lines)
                if code:
                    out.write(f'            case {addr}:\n')
                    suffix = "" if "return" in code else f" reg_PC += {size}; return;"
                    out.write(f'                {code}{suffix}\n')
                else:
                    # Just a label for the switch case
                    out.write(f'            case {addr}: // Fallthrough\n')

        out.write('            default: reg_PC++; return;\n        }\n    }\n}\n')

if __name__ == "__main__":
    os.makedirs(OUT_DIR, exist_ok=True)
    pre_parse_symbols()
    for f in os.listdir(ASM_DIR):
        if f.endswith('.asm') or f.endswith('.txt'):
            convert_file(f)
