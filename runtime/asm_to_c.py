import os
import re

ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"

if not os.path.exists(OUT_DIR):
    os.makedirs(OUT_DIR)

def parse_operand(operand):
    """Converts 6502 operands into C++ bus calls."""
    operand = operand.strip()
    
    # Immediate: LDA #$05 -> 0x05
    if operand.startswith('#$'):
        return f"0x{operand[2:]}"
    if operand.startswith('#'):
        return operand[1:]

    # Zero Page / Absolute: LDA $12 -> bus_read(0x12)
    if operand.startswith('$'):
        addr = f"0x{operand[1:]}"
        return f"bus_read({addr})"
    
    # Labels (Variables): LDA PlayerHealth -> bus_read(PlayerHealth)
    return f"bus_read({operand})"

def translate_line(line):
    # Remove comments for processing
    clean_line = line.split(';')[0].strip()
    if not clean_line or clean_line.startswith('.'):
        return None

    # Match Opcode and Operand
    match = re.match(r'^(\w{3})\s*(.*)$', clean_line)
    if not match:
        return f"// {line.strip()}"

    opcode, operand = match.groups()

    # --- Opcode Translation Logic ---
    if opcode == 'LDA':
        return f"reg_A = {parse_operand(operand)}; update_nz(reg_A);"
    
    if opcode == 'LDX':
        return f"reg_X = {parse_operand(operand)}; update_nz(reg_X);"
    
    if opcode == 'LDY':
        return f"reg_Y = {parse_operand(operand)}; update_nz(reg_Y);"

    if opcode == 'STA':
        # STA is always a write, remove the bus_read if operand was a memory address
        addr = operand.replace('$', '0x') if operand.startswith('$') else operand
        return f"bus_write({addr}, reg_A);"

    if opcode == 'STX':
        addr = operand.replace('$', '0x') if operand.startswith('$') else operand
        return f"bus_write({addr}, reg_X);"

    if opcode == 'JSR':
        return f"{operand}();"

    if opcode == 'RTS':
        return "return;"

    if opcode == 'INX':
        return "reg_X++; update_nz(reg_X);"

    if opcode == 'DEX':
        return "reg_X--; update_nz(reg_X);"

    return f"// TODO: Implement {opcode} {operand}"

def convert_file(filename):
    bank_name = os.path.splitext(filename)[0]
    with open(os.path.join(ASM_DIR, filename), 'r', encoding='utf-8', errors='ignore') as f:
        lines = f.readlines()

    with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
        # Include the shared hardware state
        out.write('#include <stdint.h>\n')
        out.write('// Forward declarations of hardware bus\n')
        out.write('extern uint8_t reg_A, reg_X, reg_Y, reg_P, reg_S;\n')
        out.write('extern uint8_t bus_read(uint16_t addr);\n')
        out.write('extern void bus_write(uint16_t addr, uint8_t val);\n')
        out.write('extern void update_nz(uint8_t val);\n\n')

        for line in lines:
            # Detect Labels (ends with : or starts at column 0)
            label_match = re.match(r'^(\w+):', line)
            if label_match:
                label = label_match.group(1)
                out.write(f"\nvoid {label}() {{\n")
            elif line.strip():
                code = translate_line(line)
                if code:
                    out.write(f"    {code}\n")
            
            # Close function on RTS
            if 'RTS' in line:
                out.write("}\n")

# Process all ASM files
for f in os.listdir(ASM_DIR):
    if f.endswith('.asm'):
        print(f"Recompiling {f}...")
        convert_file(f)
