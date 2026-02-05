import os
import re

ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"

if not os.path.exists(OUT_DIR):
    os.makedirs(OUT_DIR)

def parse_address(operand):
    operand = operand.strip()
    if operand.startswith('#$'): return f"0x{operand[2:]}"
    if ',X' in operand.upper():
        base = operand.replace('$', '0x').split(',')[0]
        return f"bus_read({base} + reg_X)"
    if ',Y' in operand.upper():
        base = operand.replace('$', '0x').split(',')[0]
        return f"bus_read({base} + reg_Y)"
    if operand.startswith('$'): return f"bus_read(0x{operand[1:]})"
    if operand.startswith('('):
        inner = operand.strip('()').replace('$', '0x')
        return f"bus_read(read_pointer({inner}))"
    return operand

def translate_line(line):
    clean_line = line.split(';')[0].strip()
    if not clean_line or clean_line.startswith('.'): return None
    match = re.match(r'^(\w{3})\s*(.*)$', clean_line)
    if not match: return f"// {clean_line}"
    opcode, operand = match.groups().upper()

    if opcode == 'LDA': return f"reg_A = {parse_address(operand)}; update_nz(reg_A);"
    if opcode == 'LDX': return f"reg_X = {parse_address(operand)}; update_nz(reg_X);"
    if opcode == 'LDY': return f"reg_Y = {parse_address(operand)}; update_nz(reg_Y);"
    if opcode in ['STA', 'STX', 'STY']:
        reg = opcode[-1]
        addr = operand.replace('$', '0x').split(',')[0]
        return f"bus_write({addr}, reg_{reg});"
    if opcode == 'JSR': return f"{operand}();"
    if opcode == 'JMP': return f"{operand}(); return;"
    if opcode == 'RTS': return "return;"
    if opcode == 'BNE': return f"if (!(reg_P & 0x02)) {{ {operand}(); return; }}"
    if opcode == 'BEQ': return f"if (reg_P & 0x02) {{ {operand}(); return; }}"
    return f"// {opcode} {operand}"

def convert_file(filename):
    bank_name = os.path.splitext(filename)[0]
    in_function = False
    
    with open(os.path.join(ASM_DIR, filename), 'r', encoding='utf-8', errors='ignore') as f:
        lines = f.readlines()

    with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
        out.write('#include <stdint.h>\n')
        out.write('extern uint8_t reg_A, reg_X, reg_Y, reg_P, reg_S;\n')
        out.write('extern uint8_t bus_read(uint16_t addr);\n')
        out.write('extern void bus_write(uint16_t addr, uint8_t val);\n')
        out.write('extern void update_nz(uint8_t val);\n')
        out.write('extern uint16_t read_pointer(uint16_t addr);\n\n')

        for line in lines:
            label_match = re.match(r'^(\w+):', line.strip())
            if label_match:
                if in_function: out.write("}\n")
                label = label_match.group(1)
                
                # FIX: Make functions 'static' to prevent duplicate symbol errors in linker
                # Exception: keep 'power_on_reset' and 'nmi_handler' global
                is_global = label in ['power_on_reset', 'nmi_handler']
                prefix = "" if is_global else "static "
                
                out.write(f"{prefix}void {label}() {{\n")
                in_function = True
            elif line.strip():
                code = translate_line(line)
                if code:
                    out.write(f"    {code}\n")
                    if "return;" in code and "if (" not in code:
                        out.write("}\n")
                        in_function = False

        if in_function: out.write("}\n")

for f in os.listdir(ASM_DIR):
    if f.endswith('.asm'):
        convert_file(f)
