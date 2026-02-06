import os
import re

ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"

# Global dictionary to store .alias definitions
aliases = {}

def get_ins_size(opcode, op):
    if not op or op.strip() == "": return 1
    if op.startswith('#'): return 2
    if '(' in op: return 2 
    # Check if operand is a known 8-bit alias
    if op in aliases and int(aliases[op], 16) <= 0xFF: return 2
    if len(op) > 4 or (op in aliases and int(aliases[op], 16) > 0xFF): return 3 
    return 2 

def parse_addr(opcode, op):
    op = op.strip()
    if not op: return "0"
    
    # Resolve aliases first
    for alias, val in aliases.items():
        # Match whole word only to avoid partial replacement
        op = re.sub(r'\b' + re.escape(alias) + r'\b', val, op)

    is_write = opcode in ['STA', 'STX', 'STY', 'INC', 'DEC', 'ASL', 'LSR', 'ROL', 'ROR']
    if op.startswith('#$'): return f"0x{op[2:]}"
    if op.startswith('#'): return op[1:] # Decimal or literal
    
    # Handle Indirect Indexed (ptr),Y
    if op.startswith('(') and '),Y' in op.upper():
        hex_addr = op.replace('(', '').replace('),Y', '').replace('$', '0x').strip()
        return f"bus_read(read_pointer({hex_addr}) + reg_Y)"
    
    # Handle Absolute/ZeroPage
    hex_addr = op.replace('$', '0x').split(',')[0].strip('()')
    
    if ',X' in op.upper(): addr_calc = f"({hex_addr} + reg_X)"
    elif ',Y' in op.upper(): addr_calc = f"({hex_addr} + reg_Y)"
    else: addr_calc = hex_addr

    return addr_calc if is_write else f"bus_read({addr_calc})"

def translate_line(line):
    # Strip comments and handle .alias
    clean = line.split(';')[0].strip()
    
    alias_match = re.match(r'^\.alias\s+(\w+)\s+\$?([0-9A-F]+)', clean)
    if alias_match:
        name, val = alias_match.groups()
        aliases[name] = f"0x{val}"
        return f"// Alias: {name} = {val}", 0

    match = re.match(r'^(?:\w+:)?\s*(\w{3})\s*(.*)$', clean)
    if not match: return None, 0
    
    opcode, op = match.groups()
    opcode = opcode.upper()
    val = parse_addr(opcode, op)
    size = get_ins_size(opcode, op)

    # Convert branch labels like + or - to hex if they match case patterns
    clean_op = op.replace('$', '0x')

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
        'INC': f"bus_write({val}, (bus_read({val}) + 1)); // Simplified INC",
        'DEC': f"bus_write({val}, (bus_read({val}) - 1)); // Simplified DEC",
        'ASL': f"reg_A = cpu_asl(reg_A);" if op == "" else f"bus_write({val}, cpu_asl(bus_read({val})));",
        'LSR': f"reg_A = cpu_lsr(reg_A);" if op == "" else f"bus_write({val}, cpu_lsr(bus_read({val})));",
        'TAX': "reg_X = reg_A; update_nz(reg_X);",
        'TAY': "reg_Y = reg_A; update_nz(reg_Y);",
        'TXA': "reg_A = reg_X; update_nz(reg_A);",
        'TYA': "reg_A = reg_Y; update_nz(reg_A);",
        'PHA': "push_stack(reg_A);",
        'PLA': "reg_A = pop_stack(); update_nz(reg_A);",
        'PHP': "push_stack(reg_P);",
        'PLP': "reg_P = pop_stack();",
        'RTS': "reg_PC = (pop_stack() | (pop_stack() << 8)) + 1; return;",
        'JSR': f"{{ uint16_t ret = reg_PC + 2; push_stack(ret >> 8); push_stack(ret & 0xFF); reg_PC = {clean_op}; return; }}",
        'JMP': f"reg_PC = {clean_op}; return;",
        'BNE': f"if (!(reg_P & 0x02)) {{ reg_PC = {clean_op}; return; }}",
        'BEQ': f"if (reg_P & 0x02) {{ reg_PC = {clean_op}; return; }}",
        'BCC': f"if (!(reg_P & 0x01)) {{ reg_PC = {clean_op}; return; }}",
        'BCS': f"if (reg_P & 0x01) {{ reg_PC = {clean_op}; return; }}",
        'BVC': f"if (!(reg_P & 0x40)) {{ reg_PC = {clean_op}; return; }}",
        'BVS': f"if (reg_P & 0x40) {{ reg_PC = {clean_op}; return; }}",
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
            # Handle standard labels LXXXX
            label_match = re.search(r'^\s*(L[0-9A-F]{4}):', line)
            if label_match:
                addr = label_match.group(1).replace('L', '0x')
                out.write(f'            case {addr}:\n')
                
                code, size = translate_line(line)
                if code:
                    suffix = "" if ("return" in code or "// Alias" in code) else f" reg_PC += {size}; return;"
                    out.write(f'                {code}{suffix}\n')
            
            # Briefly log data bytes for manual implementation/reference
            elif ".byte" in line or ".word" in line:
                out.write(f'                // Data: {line.strip()}\n')

        out.write('            default: reg_PC++; return;\n        }\n    }\n}\n')

# Process
os.makedirs(OUT_DIR, exist_ok=True)
for f in os.listdir(ASM_DIR):
    if f.endswith('.asm') or f.endswith('.txt'): # Added .txt support for your upload
        convert_file(f)
