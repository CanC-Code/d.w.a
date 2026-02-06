import os
import re

ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"
exported_globals = {"power_on_reset": False, "nmi_handler": False}

def parse_addr(op):
    op = op.strip()
    if not op: return "0"
    # Immediate: #$00
    if op.startswith('#$'): return f"0x{op[2:]}"
    # Absolute/ZeroPage Indexed: $1234,X or $12,X
    if ',X' in op.upper():
        base = op.split(',')[0].replace('$', '0x')
        return f"bus_read({base} + reg_X)"
    # Absolute/ZeroPage Indexed: $1234,Y or $12,Y
    if ',Y' in op.upper():
        base = op.split(',')[0].replace('$', '0x')
        return f"bus_read({base} + reg_Y)"
    # Indirect Y: ($12),Y
    if op.startswith('(') and ',Y' in op.upper():
        ptr = op.strip('()').split(',')[0].replace('$', '0x')
        return f"bus_read(read_pointer({ptr}) + reg_Y)"
    # Simple Absolute/ZeroPage: $1234
    if op.startswith('$'): return f"bus_read(0x{op[1:]})"
    return "0"

def translate_line(line):
    clean = line.split(';')[0].strip()
    if not clean or clean.startswith('.'): return None
    match = re.match(r'^(\w{3})\s*(.*)$', clean)
    if not match: return None
    opcode, op = match.groups()
    opcode = opcode.upper()

    # Enhanced logic mapping
    mapping = {
        'LDA': f"reg_A = {parse_addr(op)}; update_nz(reg_A);",
        'LDX': f"reg_X = {parse_addr(op)}; update_nz(reg_X);",
        'LDY': f"reg_Y = {parse_addr(op)}; update_nz(reg_Y);",
        'STA': f"bus_write({op.replace('$','0x').split(',')[0].strip('()')}, reg_A);",
        'STX': f"bus_write({op.replace('$','0x').split(',')[0].strip('()')}, reg_X);",
        'STY': f"bus_write({op.replace('$','0x').split(',')[0].strip('()')}, reg_Y);",
        'ADC': f"cpu_adc({parse_addr(op)});",
        'SBC': f"cpu_sbc({parse_addr(op)});",
        'TAX': "reg_X = reg_A; update_nz(reg_X);",
        'TAY': "reg_Y = reg_A; update_nz(reg_Y);",
        'TXA': "reg_A = reg_X; update_nz(reg_A);",
        'TYA': "reg_A = reg_Y; update_nz(reg_A);",
        'INX': "reg_X++; update_nz(reg_X);",
        'INY': "reg_Y++; update_nz(reg_Y);",
        'DEX': "reg_X--; update_nz(reg_X);",
        'DEY': "reg_Y--; update_nz(reg_Y);",
        'INC': f"{{ uint16_t a = {op.replace('$','0x')}; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); }}",
        'DEC': f"{{ uint16_t a = {op.replace('$','0x')}; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); }}",
        'PHA': "push_stack(reg_A);",
        'PLA': "reg_A = pop_stack(); update_nz(reg_A);",
        'JSR': f"{op}();",
        'RTS': "return;",
        'BNE': f"if (!(reg_P & 0x02)) {{ {op}(); return; }}",
        'BEQ': f"if (reg_P & 0x02) {{ {op}(); return; }}",
        'BCS': f"if (reg_P & 0x01) {{ {op}(); return; }}",
        'BCC': f"if (!(reg_P & 0x01)) {{ {op}(); return; }}",
        'BMI': f"if (reg_P & 0x80) {{ {op}(); return; }}",
        'BPL': f"if (!(reg_P & 0x80)) {{ {op}(); return; }}",
        'CMP': f"update_flags_cmp(reg_A, {parse_addr(op)});",
        'CPX': f"update_flags_cmp(reg_X, {parse_addr(op)});",
        'CPY': f"update_flags_cmp(reg_Y, {parse_addr(op)});",
        'SEC': "reg_P |= 0x01;",
        'CLC': "reg_P &= ~0x01;",
        'SEI': "reg_P |= 0x04;",
        'CLI': "reg_P &= ~0x04;"
    }
    
    if opcode in mapping:
        return mapping[opcode]
    return f"// Unsupported: {opcode} {op}"

def convert_file(filename):
    global exported_globals
    bank_name = os.path.splitext(filename)[0].replace("-", "_")
    with open(os.path.join(ASM_DIR, filename), 'r', encoding='utf-8', errors='ignore') as f:
        lines = f.readlines()

    # Map labels to their banks for cross-bank calling
    found_labels = [m.group(1) for l in lines if (m := re.search(r'^\s*(\w+):', l))]

    with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
        out.write('#include "cpu_shared.h"\n')
        # Include other banks so JSR/JMP works
        for i in range(4):
            out.write(f'namespace Bank{i:02d} {{ void RESET(); void VBlank(); }}\n')
        
        out.write(f'\nnamespace {bank_name} {{\n')

        # Forward declarations for local labels
        for label in found_labels:
            out.write(f'    void {label}();\n')

        in_func = False
        for line in lines:
            label_match = re.search(r'^\s*(\w+):', line)
            if label_match:
                if in_func: out.write("    }\n\n")
                out.write(f"    void {label_match.group(1)}() {{\n")
                in_func = True
            elif in_func and line.strip():
                code = translate_line(line)
                if code: out.write(f"        {code}\n")

        if in_func: out.write("    }\n")
        out.write('}\n\nextern "C" {\n')

        # Critical: Bridge the hardware vectors correctly
        # Based on Dragon Warrior ASM, Bank03 usually holds the fixed vectors
        resets = ['RESET', 'START', 'Reset']
        nmis = ['VBlank', 'NMI', 'nmi_handler']

        if not exported_globals["power_on_reset"]:
            for r in resets:
                if r in found_labels:
                    out.write(f'    void power_on_reset() {{ {bank_name}::{r}(); }}\n')
                    exported_globals["power_on_reset"] = True; break

        if not exported_globals["nmi_handler"]:
            for n in nmis:
                if n in found_labels:
                    out.write(f'    void nmi_handler() {{ {bank_name}::{n}(); }}\n')
                    exported_globals["nmi_handler"] = True; break
        out.write('}\n')

os.makedirs(OUT_DIR, exist_ok=True)
for f in sorted(os.listdir(ASM_DIR), reverse=True):
    if f.endswith('.asm'): convert_file(f)
