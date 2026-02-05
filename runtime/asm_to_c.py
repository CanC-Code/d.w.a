import os
import re

ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"

if not os.path.exists(OUT_DIR):
    os.makedirs(OUT_DIR)

def parse_address(operand):
    """Converts 6502 addressing modes to C++ bus/register logic."""
    operand = operand.strip()
    
    # Immediate: #$05
    if operand.startswith('#$'):
        return f"0x{operand[2:]}"
    
    # Absolute Indexed X: $0300,X
    if ',X' in operand.upper():
        base = operand.replace('$', '0x').split(',')[0]
        return f"bus_read({base} + reg_X)"
    
    # Absolute Indexed Y: $0300,Y
    if ',Y' in operand.upper():
        base = operand.replace('$', '0x').split(',')[0]
        return f"bus_read({base} + reg_Y)"

    # Zero Page / Absolute: $12 or $0300
    if operand.startswith('$'):
        return f"bus_read(0x{operand[1:]})"
    
    # Indirect: ($12)
    if operand.startswith('('):
        inner = operand.strip('()').replace('$', '0x')
        return f"bus_read(read_pointer({inner}))"

    return operand

def translate_line(line):
    """Translates a single line of 6502 ASM to C++."""
    # Strip comments and extra whitespace
    clean_line = line.split(';')[0].strip()
    if not clean_line or clean_line.startswith('.'):
        return None

    # Regex to catch: OPCODE OPERAND
    match = re.match(r'^(\w{3})\s*(.*)$', clean_line)
    if not match:
        return f"// {clean_line}"

    opcode, operand = match.groups()
    opcode = opcode.upper()

    # --- Core Instruction Set ---
    # Loads
    if opcode == 'LDA': return f"reg_A = {parse_address(operand)}; update_nz(reg_A);"
    if opcode == 'LDX': return f"reg_X = {parse_address(operand)}; update_nz(reg_X);"
    if opcode == 'LDY': return f"reg_Y = {parse_address(operand)}; update_nz(reg_Y);"
    
    # Stores (STA $addr -> bus_write($addr, reg_A))
    if opcode in ['STA', 'STX', 'STY']:
        reg = opcode[-1]
        addr = operand.replace('$', '0x').split(',')[0] # Basic addr extraction
        return f"bus_write({addr}, reg_{reg});"

    # Arithmetic & Logic
    if opcode == 'INC': return f"tmp_val = {parse_address(operand)} + 1; bus_write(target_addr, tmp_val); update_nz(tmp_val);"
    if opcode == 'INX': return "reg_X++; update_nz(reg_X);"
    if opcode == 'DEX': return "reg_X--; update_nz(reg_X);"
    if opcode == 'CMP': return f"update_nz(reg_A - {parse_address(operand)}); set_carry(reg_A >= {parse_address(operand)});"

    # Control Flow
    if opcode == 'JSR': return f"{operand}();"
    if opcode == 'JMP': return f"{operand}(); return;" # JMP ends current function flow
    if opcode == 'RTS': return "return;"

    # Branches (Simplified for C++ logic)
    if opcode == 'BNE': return f"if (!(reg_P & 0x02)) {{ {operand}(); return; }}"
    if opcode == 'BEQ': return f"if (reg_P & 0x02) {{ {operand}(); return; }}"

    return f"// {opcode} {operand} (Manual Bridge Required)"

def convert_file(filename):
    bank_name = os.path.splitext(filename)[0]
    in_function = False
    
    with open(os.path.join(ASM_DIR, filename), 'r', encoding='utf-8', errors='ignore') as f:
        lines = f.readlines()

    with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
        # Header Boilerplate
        out.write('#include <stdint.h>\n')
        out.write('// Global CPU context from native-lib\n')
        out.write('extern uint8_t reg_A, reg_X, reg_Y, reg_P, reg_S;\n')
        out.write('extern uint8_t bus_read(uint16_t addr);\n')
        out.write('extern void bus_write(uint16_t addr, uint8_t val);\n')
        out.write('extern void update_nz(uint8_t val);\n')
        out.write('extern void set_carry(bool c);\n\n')

        for line in lines:
            # Handle Labels (e.g., "Reset_Vector:")
            label_match = re.match(r'^(\w+):', line.strip())
            if label_match:
                if in_function: out.write("}\n") # Close previous
                label = label_match.group(1)
                out.write(f"\nvoid {label}() {{\n")
                in_function = True
            elif line.strip():
                code = translate_line(line)
                if code:
                    out.write(f"    {code}\n")
                    # If line is RTS or JMP, the function logic is effectively closed
                    if "return;" in code and not "if (" in code:
                        out.write("}\n")
                        in_function = False

        if in_function: out.write("}\n")

for f in os.listdir(ASM_DIR):
    if f.endswith('.asm'):
        convert_file(f)
