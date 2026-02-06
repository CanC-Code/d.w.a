import os
import re

# Configuration
ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"

# Hardware / Symbol State
global_symbols = {}

def get_ins_size(opcode, op):
    opcode = opcode.lower()
    # Implied/Single byte instructions
    if opcode in ['clc', 'cld', 'cli', 'clv', 'dex', 'dey', 'inx', 'iny', 'nop', 
                  'pha', 'php', 'pla', 'plp', 'rti', 'rts', 'sec', 'sed', 'sei', 
                  'tax', 'tay', 'tsx', 'txa', 'txs', 'tya']:
        return 1
    if not op or op.strip() == "": return 1
    if op.startswith('#'): return 2 # Immediate
    if '(' in op: return 2 # Indirect (usually ZP)

    # Branching (6502 branches are always 2 bytes)
    if opcode in ['bcc', 'bcs', 'beq', 'bmi', 'bne', 'bpl', 'bvc', 'bvs']:
        return 2

    # Resolve labels vs absolute addresses
    clean_op = re.sub(r'[#$(),\sXY]', '', op)
    if clean_op in global_symbols:
        val = int(global_symbols[clean_op], 16)
        return 2 if val <= 0xFF else 3
    
    # Hex detection
    if re.match(r'^[0-9A-Fa-f]{1,2}$', clean_op): return 2
    return 3 # Default to absolute

def resolve_symbol(symbol):
    if not symbol: return "0x0000"
    # Clean symbol from addressing mode syntax
    clean_sym = re.sub(r'[#$(),\sXY]', '', symbol)
    
    if clean_sym in global_symbols:
        return global_symbols[clean_sym]
    
    # Check for hex ($FD86 -> 0xfd86)
    hex_match = re.search(r'([0-9A-Fa-f]{2,4})', clean_sym)
    if hex_match:
        return f"0x{hex_match.group(1).lower()}"
    
    return "0x0000"

def translate_line(opcode, op, current_pc):
    opcode = opcode.lower()
    addr = resolve_symbol(op)
    size = get_ins_size(opcode, op)

    # --- 1. Immediate Addressing ---
    if op.startswith('#'):
        imm = addr
        imm_map = {
            'lda': f'reg_A = {imm}; update_nz(reg_A);',
            'ldx': f'reg_X = {imm}; update_nz(reg_X);',
            'ldy': f'reg_Y = {imm}; update_nz(reg_Y);',
            'cmp': f'update_flags_cmp(reg_A, {imm});',
            'cpx': f'update_flags_cmp(reg_X, {imm});',
            'cpy': f'update_flags_cmp(reg_Y, {imm});',
            'ora': f'reg_A |= {imm}; update_nz(reg_A);',
            'and': f'reg_A &= {imm}; update_nz(reg_A);',
            'eor': f'reg_A ^= {imm}; update_nz(reg_A);',
            'adc': f'cpu_adc({imm});',
            'sbc': f'cpu_sbc({imm});',
        }
        if opcode in imm_map: return imm_map[opcode], size

    # --- 2. Branching Logic (PC Relative to Absolute) ---
    branch_map = {
        'beq': 'reg_P & FLAG_Z',
        'bne': '!(reg_P & FLAG_Z)',
        'bcs': 'reg_P & FLAG_C',
        'bcc': '!(reg_P & FLAG_C)',
        'bmi': 'reg_P & FLAG_N',
        'bpl': '!(reg_P & FLAG_N)',
    }
    if opcode in branch_map:
        # Hardware Hack: If reading PPU status ($2002) then branching, 
        # prevent infinite frame-locks by returning control to Dispatcher
        return f'if ({branch_map[opcode]}) {{ reg_PC = {addr}; return; }}', size

    # --- 3. Memory / Register Operations ---
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
        'jsr': f'{{ uint16_t ret = reg_PC + {size}; push_stack(ret >> 8); push_stack(ret & 0xFF); reg_PC = {addr}; return; }}',
        'rts': 'reg_PC = (pop_stack() | (pop_stack() << 8)) + 1; return;',
        'rti': 'reg_P = pop_stack(); reg_PC = (pop_stack() | (pop_stack() << 8)); return;',
        'inc': f'{{ uint8_t v = bus_read({addr}) + 1; bus_write({addr}, v); update_nz(v); }}',
        'dec': f'{{ uint8_t v = bus_read({addr}) - 1; bus_write({addr}, v); update_nz(v); }}',
        'tax': 'reg_X = reg_A; update_nz(reg_X);',
        'txa': 'reg_A = reg_X; update_nz(reg_A);',
        'tay': 'reg_Y = reg_A; update_nz(reg_Y);',
        'tya': 'reg_A = reg_Y; update_nz(reg_A);',
        'txs': 'reg_S = reg_X;',
        'tsx': 'reg_X = reg_S; update_nz(reg_X);',
        'pha': 'push_stack(reg_A);',
        'pla': 'reg_A = pop_stack(); update_nz(reg_A);',
        'clc': 'reg_P &= ~FLAG_C;',
        'sec': 'reg_P |= FLAG_C;',
        'cld': 'reg_P &= ~FLAG_D;',
        'sei': 'reg_P |= FLAG_I;',
        'nop': '// NOP',
    }

    code = mapping.get(opcode, f"// Unsupported Opcode: {opcode}")
    return code, size

def pre_parse_symbols():
    print("Pass 1: Pre-parsing labels and calculating addresses...")
    for filename in sorted(os.listdir(ASM_DIR)):
        if not filename.endswith('.asm'): continue
        # MMC1 Logic: Bank03 is usually $C000, others map to $8000
        current_pc = 0xC000 if "Bank03" in filename else 0x8000

        with open(os.path.join(ASM_DIR, filename), 'r') as f:
            for line in f:
                # Remove comments and whitespace
                line = line.split(';')[0].strip()
                if not line: continue

                # .org handling
                org_match = re.match(r'^\.org\s+\$?([0-9A-Fa-f]+)', line)
                if org_match:
                    current_pc = int(org_match.group(1), 16)
                    continue

                # Label handling (support Label: or Label )
                label_match = re.match(r'^(\w+):?', line)
                if label_match and not re.match(r'^\w{3}\s', line):
                    label = label_match.group(1)
                    if label.lower() not in ['lda', 'sta', 'jmp', 'jsr']: # Avoid opcode confusion
                        global_symbols[label] = hex(current_pc)
                        # Remove label from line to check if instruction follows
                        line = re.sub(r'^\w+:?\s*', '', line).strip()
                
                if not line: continue

                # Instruction size calculation
                ins_match = re.match(r'^(\w{3})\s*(.*)$', line)
                if ins_match:
                    current_pc += get_ins_size(ins_match.group(1), ins_match.group(2))
                elif ".byte" in line or ".db" in line:
                    current_pc += len(line.split(','))

def convert_all():
    if not os.path.exists(OUT_DIR): os.makedirs(OUT_DIR)
    pre_parse_symbols()

    for filename in sorted(os.listdir(ASM_DIR)):
        if not filename.endswith('.asm') or "Defines" in filename: continue
        bank_name = os.path.splitext(filename)[0].replace("-", "_")

        print(f"Pass 2: Converting {filename} to {bank_name}.cpp")
        current_pc = 0xC000 if "Bank03" in filename else 0x8000

        with open(os.path.join(ASM_DIR, filename), 'r') as f:
            lines = f.readlines()

        with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
            out.write('#include "cpu_shared.h"\n\n')
            out.write(f'namespace {bank_name} {{\n    void execute() {{\n        switch(reg_PC) {{\n')

            for line in lines:
                # Basic line cleanup
                comment_split = line.split(';')
                clean = comment_split[0].strip()
                if not clean: continue

                # .org sync
                org_match = re.match(r'^\.org\s+\$?([0-9A-Fa-f]+)', clean)
                if org_match:
                    current_pc = int(org_match.group(1), 16)
                    continue

                # Process instruction
                # (Optional: extract label for comment)
                code_match = re.match(r'^(?:\w+:)?\s*(\w{3})\s*(.*)$', clean)
                if code_match:
                    opcode, op = code_match.groups()
                    code, size = translate_line(opcode, op, current_pc)
                    
                    out.write(f'            case 0x{current_pc:04x}: {code}')
                    if "return" not in code: 
                        out.write(f' reg_PC += {size}; return;')
                    out.write(f' // {clean}\n')
                    
                    current_pc += size
                elif ".byte" in clean or ".db" in clean:
                    current_pc += len(clean.split(','))

            out.write('            default: return;\n        }\n    }\n}\n')

if __name__ == "__main__":
    convert_all()
    print("Done! Check app/src/main/cpp/recompiled/")
