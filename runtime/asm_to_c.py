import os
import re

ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"

# Table to ensure reg_PC increments correctly based on 6502 addressing modes
# This is a simplified map based on operand patterns
def get_ins_size(opcode, op):
    if not op or op.strip() == "": return 1
    if op.startswith('#'): return 2
    if '(' in op: return 2 # Indirect is usually ZP in NES
    if len(op) > 4: return 3 # $1234
    return 2 # $12

def parse_addr(opcode, op):
    op = op.strip()
    if not op: return "0"
    is_write = opcode in ['STA', 'STX', 'STY', 'INC', 'DEC', 'ASL', 'LSR', 'ROL', 'ROR']
    if op.startswith('#$'): return f"0x{op[2:]}"
    hex_addr = op.replace('$', '0x').split(',')[0].strip('()')
    
    if ',X' in op.upper(): addr_calc = f"({hex_addr} + reg_X)"
    elif ',Y' in op.upper(): addr_calc = f"({hex_addr} + reg_Y)"
    elif op.startswith('(') and ',Y' in op.upper(): addr_calc = f"(read_pointer({hex_addr}) + reg_Y)"
    else: addr_calc = hex_addr

    return addr_calc if is_write else f"bus_read({addr_calc})"

def translate_line(line):
    clean = line.split(';')[0].strip()
    match = re.match(r'^(?:\w+:)?\s*(\w{3})\s*(.*)$', clean)
    if not match: return None, 0
    opcode, op = match.groups()
    opcode = opcode.upper()
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
        'TAX': "reg_X = reg_A; update_nz(reg_X);",
        'TAY': "reg_Y = reg_A; update_nz(reg_Y);",
        'TXA': "reg_A = reg_X; update_nz(reg_A);",
        'TYA': "reg_A = reg_Y; update_nz(reg_A);",
        'PHA': "push_stack(reg_A);",
        'PLA': "reg_A = pop_stack(); update_nz(reg_A);",
        'RTS': "reg_PC = (pop_stack() | (pop_stack() << 8)) + 1; return;",
        'JSR': f"{{ uint16_t ret = reg_PC + 2; push_stack(ret >> 8); push_stack(ret & 0xFF); reg_PC = {op.replace('$','0x')}; return; }}",
        'JMP': f"reg_PC = {op.replace('$','0x')}; return;",
        'BNE': f"if (!(reg_P & 0x02)) {{ reg_PC = {op.replace('$','0x')}; return; }}",
        'BEQ': f"if (reg_P & 0x02) {{ reg_PC = {op.replace('$','0x')}; return; }}",
        'SEC': "reg_P |= 0x01;",
        'CLC': "reg_P &= ~0x01;",
        'CMP': f"update_flags_cmp(reg_A, {val});",
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
            label_match = re.search(r'^\s*L([0-9A-F]{4}):', line)
            if label_match:
                addr = label_match.group(1)
                out.write(f'            case 0x{addr}:\n')
                code, size = translate_line(line)
                if code:
                    # RTS, JMP, and Branches have 'return' inside them, others need to break
                    suffix = "" if ("return" in code) else f" reg_PC += {size}; return;"
                    out.write(f'                {code}{suffix}\n')

        out.write('            default: reg_PC++; return;\n        }\n    }\n}\n')

# Process
os.makedirs(OUT_DIR, exist_ok=True)
for f in os.listdir(ASM_DIR):
    if f.endswith('.asm'): convert_file(f)
