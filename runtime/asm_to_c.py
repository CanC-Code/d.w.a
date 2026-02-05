import os
import re

ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"

if not os.path.exists(OUT_DIR):
    os.makedirs(OUT_DIR)

def translate_line(line):
    line = line.strip()
    if not line or line.startswith(';') or line.startswith('.'):
        return None
    
    # Simple Opcode Mapping
    if line.startswith('LDA'):
        val = line.split()[-1]
        return f"reg_A = read_param({val}); update_nz(reg_A);"
    if line.startswith('STA'):
        addr = line.split()[-1]
        return f"bus_write(parse_addr({addr}), reg_A);"
    if line.startswith('JSR'):
        label = line.split()[-1]
        return f"{label}();"
    if line.startswith('RTS'):
        return "return;"
    
    return f"// {line} (Manual port required)"

def convert_file(filename):
    bank_name = os.path.splitext(filename)[0]
    with open(os.path.join(ASM_DIR, filename), 'r') as f:
        lines = f.readlines()

    with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
        out.write('#include "../native-lib.cpp"\n\n')
        for line in lines:
            # Handle Labels
            if ':' in line:
                label = line.split(':')[0].strip()
                out.write(f"\nvoid {label}() {{\n")
            else:
                code = translate_line(line)
                if code:
                    out.write(f"    {code}\n")
            
            if 'RTS' in line:
                out.write("}\n")

for f in os.listdir(ASM_DIR):
    if f.endswith('.asm'):
        convert_file(f)
