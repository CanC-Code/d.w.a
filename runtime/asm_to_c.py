import os
import re

ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"

# Track if we have already exported the global symbols to avoid duplicates
exported_globals = {"power_on_reset": False, "nmi_handler": False}

def parse_address(operand):
    operand = operand.strip()
    if not operand: return "0"
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
    if not match: return None
    opcode, operand = match.groups()
    opcode = opcode.upper()

    if opcode == 'ADC': return f"cpu_adc({parse_address(operand)});"
    if opcode == 'SBC': return f"cpu_sbc({parse_address(operand)});"
    if opcode == 'LDA': return f"reg_A = {parse_address(operand)}; update_nz(reg_A);"
    if opcode == 'LDX': return f"reg_X = {parse_address(operand)}; update_nz(reg_X);"
    if opcode == 'LDY': return f"reg_Y = {parse_address(operand)}; update_nz(reg_Y);"
    if opcode in ['STA', 'STX', 'STY']:
        reg = opcode[-1]
        addr = operand.replace('$', '0x').split(',')[0].strip()
        return f"bus_write({addr or '0x0000'}, reg_{reg});"
    if opcode == 'JSR': return f"{operand}();"
    if opcode == 'JMP': return f"{operand}(); return;"
    if opcode == 'RTS': return "return;"
    if opcode == 'BNE': return f"if (!(reg_P & 0x02)) {{ {operand}(); return; }}"
    if opcode == 'BEQ': return f"if (reg_P & 0x02) {{ {operand}(); return; }}"
    if opcode == 'SEC': return "reg_P |= 0x01;"
    if opcode == 'CLC': return "reg_P &= ~0x01;"
    return f"// {opcode} {operand}"

def convert_file(filename):
    global exported_globals
    bank_name = os.path.splitext(filename)[0].replace("-", "_")
    os.makedirs(OUT_DIR, exist_ok=True)
    with open(os.path.join(ASM_DIR, filename), 'r', encoding='utf-8', errors='ignore') as f:
        lines = f.readlines()

    found_labels = set()
    
    with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
        out.write('#include <stdint.h>\n\nextern "C" {\n')
        out.write('    extern uint8_t reg_A, reg_X, reg_Y, reg_P, reg_S;\n')
        out.write('    extern void update_nz(uint8_t val);\n')
        out.write('    extern void cpu_adc(uint8_t val);\n')
        out.write('    extern void cpu_sbc(uint8_t val);\n')
        out.write('    extern uint8_t bus_read(uint16_t addr);\n')
        out.write('    extern void bus_write(uint16_t addr, uint8_t val);\n')
        out.write('    extern uint16_t read_pointer(uint16_t addr);\n}\n\n')

        out.write(f'namespace {bank_name} {{\n')
        
        for line in lines:
            l_match = re.search(r'^\s*(\w+):', line)
            if l_match:
                label = l_match.group(1)
                out.write(f'    void {label}();\n')
                found_labels.add(label)

        out.write('\n')
        in_func = False
        for line in lines:
            label_match = re.search(r'^\s*(\w+):', line)
            if label_match:
                if in_func: out.write("    }\n\n")
                out.write(f"    void {label_match.group(1)}() {{\n")
                in_func = True
            elif line.strip():
                code = translate_line(line)
                if code: out.write(f"        {code}\n")
                if code and "return;" in code and "if (" not in code:
                    out.write("    }\n\n")
                    in_func = False
        if in_func: out.write("    }\n")
        out.write('}\n\n')

        # GLOBAL BRIDGES - Only one bank gets to define the global C symbol
        out.write('extern "C" {\n')
        reset_labels = ['RESET', 'START', 'Reset', 'power_on_reset']
        nmi_labels = ['VBlank', 'VBLANK', 'NMI', 'nmi_handler']
        
        # We prioritize the "Fixed Bank" (usually Bank03 or higher)
        # by processing files in order, but checking if we already exported.
        if not exported_globals["power_on_reset"]:
            for r in reset_labels:
                if r in found_labels:
                    out.write(f'    void power_on_reset() {{ {bank_name}::{r}(); }}\n')
                    exported_globals["power_on_reset"] = True
                    break
        
        if not exported_globals["nmi_handler"]:
            for n in nmi_labels:
                if n in found_labels:
                    out.write(f'    void nmi_handler() {{ {bank_name}::{n}(); }}\n')
                    exported_globals["nmi_handler"] = True
                    break
        out.write('}\n')

# Sort files so Bank03 (usually fixed bank) is checked early or Bank00
files = sorted(os.listdir(ASM_DIR), reverse=True)
for f in files:
    if f.endswith('.asm'): convert_file(f)
