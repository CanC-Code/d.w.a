import os
import re

# Configuration
ASM_DIR = "source_files"  # Folder containing your .asm files
OUT_DIR = "app/src/main/cpp/recompiled"

# Global Hardware State
global_symbols = {}

def get_ins_size(opcode, op):
    opcode = opcode.lower()
    if opcode in ['clc', 'cld', 'cli', 'clv', 'dex', 'dey', 'inx', 'iny', 'nop', 'pha', 'php', 'pla', 'plp', 'rti', 'rts', 'sec', 'sed', 'sei', 'tax', 'tay', 'tsx', 'txa', 'txs', 'tya']:
        return 1
    if not op or op.strip() == "": return 1
    if op.startswith('#'): return 2
    if '(' in op: return 2 # Indirect/Indexed
    
    # Check for relative branching (2 bytes)
    if opcode in ['bcc', 'bcs', 'beq', 'bmi', 'bne', 'bpl', 'bvc', 'bvs']:
        return 2

    # Resolve address to determine Absolute (3) vs Zero Page (2)
    res = resolve_symbol(op)
    try:
        val = int(res, 16)
        return 3 if val > 0xFF else 2
    except:
        return 3 # Default to absolute if unknown

def resolve_symbol(symbol):
    if not symbol: return "0x0"
    # Clean symbol from addressing mode syntax
    clean_sym = re.sub(r'[#$(),\sXY]', '', symbol)
    if clean_sym in global_symbols:
        return global_symbols[clean_sym]
    # Check for raw hex
    hex_match = re.match(r'^([0-9A-Fa-f]{2,4})$', clean_sym)
    if hex_match:
        return f"0x{hex_match.group(1).lower()}"
    return "0x0"

def translate_line(opcode, op):
    opcode = opcode.lower()
    addr = resolve_symbol(op)
    size = get_ins_size(opcode, op)
    
    # Mapping table for basic translation
    mapping = {
        'lda': f'reg_A = bus_read({addr}); update_nz(reg_A);',
        'ldx': f'reg_X = bus_read({addr}); update_nz(reg_X);',
        'ldy': f'reg_Y = bus_read({addr}); update_nz(reg_Y);',
        'sta': f'bus_write({addr}, reg_A);',
        'stx': f'bus_write({addr}, reg_X);',
        'sty': f'bus_write({addr}, reg_Y);',
        'cmp': f'update_flags_cmp(reg_A, bus_read({addr}));',
        'cpx': f'update_flags_cmp(reg_X, bus_read({addr}));',
        'cpy': f'update_flags_cmp(reg_Y, bus_read({addr}));',
        'jmp': f'reg_PC = {addr}; return;',
        'jsr': f'push_stack((reg_PC + 2) >> 8); push_stack((reg_PC + 2) & 0xFF); reg_PC = {addr}; return;',
        'rts': f'reg_PC = (pop_stack() | (pop_stack() << 8)) + 1; return;',
        'beq': f'if (reg_P & FLAG_Z) {{ reg_PC = {addr}; return; }}',
        'bne': f'if (!(reg_P & FLAG_Z)) {{ reg_PC = {addr}; return; }}',
        'inc': f'{{ uint8_t v = bus_read({addr}) + 1; bus_write({addr}, v); update_nz(v); }}',
        'dec': f'{{ uint8_t v = bus_read({addr}) - 1; bus_write({addr}, v); update_nz(v); }}',
        'sei': 'reg_P |= FLAG_I;',
        'cli': 'reg_P &= ~FLAG_I;',
        'clc': 'reg_P &= ~FLAG_C;',
        'sec': 'reg_P |= FLAG_C;',
    }
    
    code = mapping.get(opcode, f"// Unsupported: {opcode} {op}")
    return code, size

def pre_parse_symbols():
    print("Parsing symbols...")
    for filename in sorted(os.listdir(ASM_DIR)):
        if not filename.endswith('.asm'): continue
        current_pc = 0x8000
        with open(os.path.join(ASM_DIR, filename), 'r') as f:
            for line in f:
                clean = line.split(';')[0].strip()
                if not clean: continue
                
                # Update PC
                org_match = re.match(r'^\.org\s+\$([0-9A-Fa-f]+)', clean)
                if org_match:
                    current_pc = int(org_match.group(1), 16)
                    continue
                
                # Labels/Alias
                alias_match = re.match(r'^\.alias\s+(\w+)\s+\$?([0-9A-Fa-f]+)', clean)
                if alias_match:
                    global_symbols[alias_match.group(1)] = f"0x{alias_match.group(2).lower()}"
                    continue
                
                label_match = re.match(r'^(\w+):', clean)
                if label_match:
                    global_symbols[label_match.group(1)] = hex(current_pc).lower()
                
                # Instruction size tracking
                ins_match = re.match(r'^\s*(\w{3})\s*(.*)$', clean)
                if ins_match:
                    current_pc += get_ins_size(ins_match.group(1), ins_match.group(2))
                elif ".byte" in clean or ".db" in clean:
                    current_pc += len(clean.split(','))

def convert_all():
    if not os.path.exists(OUT_DIR): os.makedirs(OUT_DIR)
    
    banks = []
    for filename in sorted(os.listdir(ASM_DIR)):
        if not filename.endswith('.asm') or "Defines" in filename: continue
        
        bank_name = os.path.splitext(filename)[0].replace("-", "_")
        banks.append(bank_name)
        
        print(f"Converting {filename} -> {bank_name}.cpp")
        
        with open(os.path.join(ASM_DIR, filename), 'r') as f:
            lines = f.readlines()

        with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
            out.write('#include "cpu_shared.h"\n\n')
            out.write(f'namespace {bank_name} {{\n    void execute() {{\n        switch(reg_PC) {{\n')
            
            current_pc = 0x8000
            for line in lines:
                clean = line.split(';')[0].strip()
                if not clean: continue
                
                org_match = re.match(r'^\.org\s+\$([0-9A-Fa-f]+)', clean)
                if org_match:
                    current_pc = int(org_match.group(1), 16)
                    continue

                code_match = re.match(r'^(?:\w+:)?\s*(\w{3})\s*(.*)$', clean)
                if code_match:
                    opcode, op = code_match.groups()
                    code, size = translate_line(opcode, op)
                    
                    out.write(f'            case 0x{current_pc:04x}:\n')
                    out.write(f'                {code}')
                    if "return" not in code:
                        out.write(f' reg_PC += {size}; return;')
                    out.write('\n')
                    current_pc += size
                elif ".byte" in clean or ".db" in clean:
                    # Increment PC for data but don't create case labels
                    current_pc += len(clean.split(','))

            out.write('            default: return;\n        }\n    }\n}\n')

    # Generate the Dispatcher
    with open(os.path.join(OUT_DIR, "dispatcher.cpp"), 'w') as out:
        out.write('#include "cpu_shared.h"\n')
        for b in banks: out.write(f'namespace {b} {{ void execute(); }}\n')
        out.write('\nnamespace Dispatcher {\n    void execute() {\n')
        out.write('        uint16_t bank = get_current_prg_bank();\n')
        out.write('        if (reg_PC >= 0xC000) { Bank03::execute(); } // Fixed Bank\n')
        out.write('        else {\n            switch(bank) {\n')
        for i, b in enumerate(banks):
            if i < 3: # Assuming 0, 1, 2 are switchable
                out.write(f'                case {i}: {b}::execute(); break;\n')
        out.write('            }\n        }\n    }\n}\n')

if __name__ == "__main__":
    pre_parse_symbols()
    convert_all()
