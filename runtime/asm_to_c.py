import os
import re

ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"
exported_globals = {"power_on_reset": False, "nmi_handler": False}

def parse_addr(op):
    op = op.strip()
    if not op: return "0"
    if op.startswith('#$'): return f"0x{op[2:]}" # Immediate
    if op.startswith('$'):
        addr = op[1:].split(',')[0]
        if ',X' in op.upper(): return f"bus_read(0x{addr} + reg_X)"
        if ',Y' in op.upper(): return f"bus_read(0x{addr} + reg_Y)"
        return f"bus_read(0x{addr})"
    return "0"

def translate_line(line):
    clean = line.split(';')[0].strip()
    if not clean or clean.startswith('.'): return None
    match = re.match(r'^(\w{3})\s*(.*)$', clean)
    if not match: return None
    opcode, op = match.groups()
    opcode = opcode.upper()

    # Core logic mapping with Flag Updates
    mapping = {
        'LDA': f"reg_A = {parse_addr(op)}; update_nz(reg_A);",
        'LDX': f"reg_X = {parse_addr(op)}; update_nz(reg_X);",
        'LDY': f"reg_Y = {parse_addr(op)}; update_nz(reg_Y);",
        'STA': f"bus_write({op.replace('$','0x').split(',')[0]}, reg_A);",
        'STX': f"bus_write({op.replace('$','0x').split(',')[0]}, reg_X);",
        'STY': f"bus_write({op.replace('$','0x').split(',')[0]}, reg_Y);",
        'TAX': "reg_X = reg_A; update_nz(reg_X);",
        'TAY': "reg_Y = reg_A; update_nz(reg_Y);",
        'INX': "reg_X++; update_nz(reg_X);",
        'INY': "reg_Y++; update_nz(reg_Y);",
        'DEX': "reg_X--; update_nz(reg_X);",
        'DEY': "reg_Y--; update_nz(reg_Y);",
        'PHA': "push_stack(reg_A);",
        'PLA': "reg_A = pop_stack(); update_nz(reg_A);",
        'PHP': "push_stack(reg_P);",
        'PLP': "reg_P = pop_stack();",
        'CMP': f"update_flags_cmp(reg_A, {parse_addr(op)});",
        'CPX': f"update_flags_cmp(reg_X, {parse_addr(op)});",
        'CPY': f"update_flags_cmp(reg_Y, {parse_addr(op)});",
        'JSR': f"{op}();",
        'RTS': "return;",
        'BNE': f"if (!(reg_P & 0x02)) {{ {op}(); return; }}",
        'BEQ': f"if (reg_P & 0x02) {{ {op}(); return; }}",
        'BCS': f"if (reg_P & 0x01) {{ {op}(); return; }}",
        'BCC': f"if (!(reg_P & 0x01)) {{ {op}(); return; }}",
        'SEC': "reg_P |= 0x01;",
        'CLC': "reg_P &= ~0x01;",
        'SEI': "reg_P |= 0x04;",
        'CLI': "reg_P &= ~0x04;"
    }
    return mapping.get(opcode, f"// Unsupported: {opcode} {op}")

def convert_file(filename):
    global exported_globals
    bank_name = os.path.splitext(filename)[0].replace("-", "_")
    with open(os.path.join(ASM_DIR, filename), 'r', encoding='utf-8', errors='ignore') as f:
        lines = f.readlines()

    found_labels = [m.group(1) for l in lines if (m := re.search(r'^\s*(\w+):', l))]
    
    with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
        out.write('#include "cpu_shared.h"\n\n') # Centralized headers
        out.write(f'namespace {bank_name} {{\n')
        
        # Forward declarations
        for label in found_labels: out.write(f'    void {label}();\n')

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
        
        # Bridge logic
        resets = ['RESET', 'START', 'power_on_reset']
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
for f in sorted(os.listdir(ASM_DIR), reverse=True): # Process Bank03 first
    if f.endswith('.asm'): convert_file(f)
