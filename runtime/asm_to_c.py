import os
import re

ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"

global_symbols = {}

def pre_parse_symbols():
    if not os.path.exists(ASM_DIR):
        return

    # First pass: Get all defines from the defines file
    def_file = os.path.join(ASM_DIR, "Dragon_Warrior_Defines.asm")
    if os.path.exists(def_file):
        parse_file_symbols(def_file)

    # Second pass: Parse all bank files
    for filename in os.listdir(ASM_DIR):
        if filename.endswith('.asm') and filename != "Dragon_Warrior_Defines.asm":
            parse_file_symbols(os.path.join(ASM_DIR, filename))

def parse_file_symbols(path):
    current_pc = 0x8000
    with open(path, 'r') as f:
        for line in f:
            clean = line.split(';')[0].strip()
            if not clean: continue
            
            org_match = re.match(r'^\.org\s+\$([0-9A-Fa-f]+)', clean)
            if org_match:
                current_pc = int(org_match.group(1), 16)
                continue

            # Handle .alias and labels
            match = re.match(r'^\.alias\s+(\w+)\s+\$?([0-9A-Fa-f]+)', clean)
            if match:
                global_symbols[match.group(1)] = f"0x{match.group(2).lower()}"
                continue

            label_match = re.match(r'^(\w+):', clean)
            if label_match:
                global_symbols[label_match.group(1)] = hex(current_pc).lower()

            ins_match = re.match(r'^\s*(\w{3})\s*(.*)$', clean)
            if ins_match:
                opcode, op = ins_match.groups()
                current_pc += get_ins_size(opcode, op)

def resolve_symbol(symbol):
    if not symbol: return "0x0"
    clean_sym = symbol.strip('#$() ,XY')
    if clean_sym in global_symbols:
        return global_symbols[clean_sym]
    # Handle direct hex $FFFF or FFFF
    hex_match = re.match(r'^\$?([0-9A-Fa-f]{2,4})$', clean_sym)
    if hex_match:
        return f"0x{hex_match.group(1).lower()}"
    return "0xFFFF /* UNRESOLVED */"

def get_ins_size(opcode, op):
    if not op or op.strip() == "": return 1
    if op.startswith('#'): return 2
    if '(' in op: return 2
    res = resolve_symbol(op)
    if "0x" in res and "UNRESOLVED" not in res:
        try:
            val = int(res, 16)
            return 3 if val > 0xFF else 2
        except: return 2
    return 2

# ... (parse_addr and translate_line remain similar to your version) ...

def convert_file(filename):
    bank_name = os.path.splitext(filename)[0].replace("-", "_")
    if bank_name.lower().startswith("bank"):
        bank_name = "Bank" + bank_name[4:]
    else: return # Skip non-bank files

    with open(os.path.join(ASM_DIR, filename), 'r') as f:
        lines = f.readlines()

    used_addresses = set()
    with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
        out.write('#include "cpu_shared.h"\n\n')
        out.write(f'namespace {bank_name} {{\n    void execute() {{\n        switch(reg_PC) {{\n')

        current_pc = 0x8000
        for line in lines:
            clean = line.split(';')[0].strip()
            # Track PC even for lines without labels
            org_match = re.match(r'^\.org\s+\$([0-9A-Fa-f]+)', clean)
            if org_match:
                current_pc = int(org_match.group(1), 16)
                continue

            label_match = re.match(r'^(\w+):', clean)
            addr_str = hex(current_pc).lower()
            
            if addr_str not in used_addresses:
                out.write(f'            case {addr_str}:\n')
                used_addresses.add(addr_str)

            code_match = re.match(r'^(?:\w+:)?\s*(\w{3})\s*(.*)$', clean)
            if code_match:
                opcode, op = code_match.groups()
                code, size = translate_line(line) # Use your translation logic
                if code:
                    suffix = "" if "return" in code else f" reg_PC += {size}; return;"
                    out.write(f'                {code}{suffix}\n')
                current_pc += size

        out.write('            default: reg_PC++; return;\n        }\n    }\n}\n')
