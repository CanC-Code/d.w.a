import os
import re

# Configuration
ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"

global_symbols = {}

# List of valid 6502 opcodes to prevent data from being parsed as code
VALID_OPCODES = {
    'adc', 'and', 'asl', 'bcc', 'bcs', 'beq', 'bit', 'bmi', 'bne', 'bpl',
    'brk', 'bvc', 'bvs', 'clc', 'cld', 'cli', 'clv', 'cmp', 'cpx', 'cpy',
    'dec', 'dex', 'dey', 'eor', 'inc', 'inx', 'iny', 'jmp', 'jsr', 'lda',
    'ldx', 'ldy', 'lsr', 'nop', 'ora', 'pha', 'php', 'pla', 'plp', 'rol',
    'ror', 'rti', 'rts', 'sbc', 'sec', 'sed', 'sei', 'sta', 'stx', 'sty',
    'tax', 'tay', 'tsx', 'txa', 'txs', 'tya'
}

def get_ins_size(opcode, op):
    opcode = opcode.lower()
    if opcode in ['.byte', '.db']: return 1
    if opcode in ['.word', '.dw', '.addr']: return 2
    
    # Implied/Accumulator
    if opcode in ['clc', 'cld', 'cli', 'clv', 'dex', 'dey', 'inx', 'iny', 'nop', 
                  'pha', 'php', 'pla', 'plp', 'rti', 'rts', 'sec', 'sed', 'sei', 
                  'tax', 'tay', 'tsx', 'txa', 'txs', 'tya']:
        return 1
    if opcode in ['asl', 'lsr', 'rol', 'ror'] and (not op or op.strip().upper() == 'A'):
        return 1

    if not op or op.strip() == "": return 1
    if op.startswith('#'): return 2
    if '(' in op: return 2
    if ',' in op:
        clean_op = re.sub(r'[#$(),\sXY%]', '', op)
        return 2 if len(clean_op) <= 2 else 3
    if opcode in ['bcc', 'bcs', 'beq', 'bmi', 'bne', 'bpl', 'bvc', 'bvs']:
        return 2

    clean_op = re.sub(r'[#$(),\sXY%]', '', op)
    return 2 if len(clean_op) <= 2 else 3

def translate_line(opcode, op, current_pc):
    opcode = opcode.lower()
    
    # Ignore data directives in the execution switch
    if opcode in ['.byte', '.db', '.word', '.dw', '.addr', '.asc', '.ds']:
        return "// Data segment", get_ins_size(opcode, op)

    if opcode not in VALID_OPCODES:
        return f"// Skipped unknown/label: {opcode}", 0

    # ... [Keep your existing translate_line logic for valid opcodes here] ...
    # (Existing logic from your prompt for LDA, STA, etc.)
    # Note: Ensure you include the mapping and standalone dicts from your original script
    
    return f"// Opcode {opcode} logic", get_ins_size(opcode, op)

def convert_all():
    if not os.path.exists(OUT_DIR): os.makedirs(OUT_DIR)
    # pre_parse_symbols() # Ensure this is called
    
    for filename in sorted(os.listdir(ASM_DIR)):
        if not filename.endswith('.asm') or "Defines" in filename: continue
        bank_name = os.path.splitext(filename)[0].replace("-", "_")
        current_pc = 0xC000 if "Bank03" in filename else 0x8000
        
        with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
            out.write('#include "cpu_shared.h"\nnamespace ' + bank_name + ' {\nvoid execute() {\nswitch(reg_PC) {\n')
            
            with open(os.path.join(ASM_DIR, filename), 'r') as f:
                for line in f:
                    # Strip comments and extra whitespace
                    clean = line.split(';')[0].strip()
                    if not clean: continue
                    
                    # Handle .org
                    org_m = re.match(r'^\.org\s+\$?([0-9A-Fa-f]+)', clean)
                    if org_m:
                        current_pc = int(org_m.group(1), 16)
                        continue

                    # Better Label/Opcode detection
                    # Matches "Label: JSR $FFD2" or "JSR $FFD2" or "Label:"
                    match = re.match(r'^(?:(\w+):)?\s*(\.?\w{2,5})?\s*(.*)$', clean)
                    if not match: continue
                    
                    label, opcode, operand = match.groups()
                    
                    if opcode:
                        code, size = translate_line(opcode, operand, current_pc)
                        if size > 0 and opcode.lower() in VALID_OPCODES:
                            out.write(f'            case 0x{current_pc & 0xFFFF:04x}: {code}')
                            if "return" not in code:
                                out.write(f' reg_PC += {size}; return;')
                            out.write(f' // {clean}\n')
                        current_pc += size
                    elif label:
                        # Label only, just track the PC
                        global_symbols[label] = hex(current_pc)

            out.write('default: return; }}}')
