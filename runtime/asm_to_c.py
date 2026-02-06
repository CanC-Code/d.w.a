import os
import re

# Configuration
ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"

# Hardware / Symbol State
global_symbols = {}

def get_ins_size(opcode, op):
    opcode = opcode.lower()
    if opcode in ['clc', 'cld', 'cli', 'clv', 'dex', 'dey', 'inx', 'iny', 'nop', 
                  'pha', 'php', 'pla', 'plp', 'rti', 'rts', 'sec', 'sed', 'sei', 
                  'tax', 'tay', 'tsx', 'txa', 'txs', 'tya']:
        return 1
    if not op or op.strip() == "": return 1
    if op.startswith('#'): return 2 
    if '(' in op: return 2 
    if opcode in ['bcc', 'bcs', 'beq', 'bmi', 'bne', 'bpl', 'bvc', 'bvs']:
        return 2

    clean_op = re.sub(r'[#$(),\sXY%]', '', op)
    if clean_op in global_symbols:
        val = int(global_symbols[clean_op], 16)
        return 2 if val <= 0xFF else 3
    
    if re.match(r'^[0-9A-Fa-f]{1,2}$', clean_op): return 2
    return 3 

def resolve_symbol(symbol):
    if not symbol: return "0x00"
    
    # Handle Binary Literals (e.g., #%10001001) found in your logs
    if '%' in symbol:
        bin_match = re.search(r'([01]{8})', symbol)
        if bin_match:
            return hex(int(bin_match.group(1), 2))

    clean_sym = re.sub(r'[#$(),\sXY%]', '', symbol)
    
    if clean_sym in global_symbols:
        return global_symbols[clean_sym]
    
    hex_match = re.search(r'([0-9A-Fa-f]{2,4})', clean_sym)
    if hex_match:
        val = int(hex_match.group(1), 16)
        return f"0x{val:02x}" if val <= 0xFF else f"0x{val:04x}"
    
    return "0x00"

def translate_line(opcode, op, current_pc):
    opcode = opcode.lower()
    addr = resolve_symbol(op)
    size = get_ins_size(opcode, op)

    # --- 1. Immediate Addressing (Forcing 8-bit to stop Wconstant-conversion) ---
    if op.startswith('#'):
        imm = int(addr, 16) & 0xFF
        imm_str = f"0x{imm:02x}"
        imm_map = {
            'lda': f'reg_A = {imm_str}; update_nz(reg_A);',
            'ldx': f'reg_X = {imm_str}; update_nz(reg_X);',
            'ldy': f'reg_Y = {imm_str}; update_nz(reg_Y);',
            'cmp': f'update_flags_cmp(reg_A, {imm_str});',
            'cpx': f'update_flags_cmp(reg_X, {imm_str});',
            'cpy': f'update_flags_cmp(reg_Y, {imm_str});',
            'ora': f'reg_A |= {imm_str}; update_nz(reg_A);',
            'and': f'reg_A &= {imm_str}; update_nz(reg_A);',
            'eor': f'reg_A ^= {imm_str}; update_nz(reg_A);',
            'adc': f'cpu_adc({imm_str});',
            'sbc': f'cpu_sbc({imm_str});',
        }
        if opcode in imm_map: return imm_map[opcode], size

    # --- 2. Branching Logic ---
    branch_map = {
        'beq': 'reg_P & FLAG_Z', 'bne': '!(reg_P & FLAG_Z)',
        'bcs': 'reg_P & FLAG_C', 'bcc': '!(reg_P & FLAG_C)',
        'bmi': 'reg_P & FLAG_N', 'bpl': '!(reg_P & FLAG_N)',
    }
    if opcode in branch_map:
        # Mask the target address to 16-bit
        target = int(addr, 16) & 0xFFFF
        return f'if ({branch_map[opcode]}) {{ reg_PC = 0x{target:04x}; return; }}', size

    # --- 3. Memory / Register Operations ---
    # Ensure all jumps and writes are 16-bit masked
    mem_addr = int(addr, 16) & 0xFFFF
    m_str = f"0x{mem_addr:04x}"

    mapping = {
        'lda': f'reg_A = bus_read({m_str}); update_nz(reg_A);',
        'ldx': f'reg_X = bus_read({m_str}); update_nz(reg_X);',
        'ldy': f'reg_Y = bus_read({m_str}); update_nz(reg_Y);',
        'sta': f'bus_write({m_str}, reg_A);',
        'stx': f'bus_write({m_str}, reg_X);',
        'sty': f'bus_write({m_str}, reg_Y);',
        'cmp': f'update_flags_cmp(reg_A, bus_read({m_str}));',
        'cpx': f'update_flags_cmp(reg_X, bus_read({m_str}));',
        'cpy': f'update_flags_cmp(reg_Y, bus_read({m_str}));',
        'jmp': f'reg_PC = {m_str}; return;',
        'jsr': f'{{ uint16_t ret = (reg_PC + {size} - 1); push_stack(ret >> 8); push_stack(ret & 0xFF); reg_PC = {m_str}; return; }}',
        'rts': 'reg_PC = (pop_stack() | (pop_stack() << 8)) + 1; return;',
        'rti': 'reg_P = pop_stack(); reg_PC = (pop_stack() | (pop_stack() << 8)); return;',
        'inc': f'{{ uint8_t v = bus_read({m_str}) + 1; bus_write({m_str}, v); update_nz(v); }}',
        'dec': f'{{ uint8_t v = bus_read({m_str}) - 1; bus_write({m_str}, v); update_nz(v); }}',
        'tax': 'reg_X = reg_A; update_nz(reg_X);', 'txa': 'reg_A = reg_X; update_nz(reg_A);',
        'tay': 'reg_Y = reg_A; update_nz(reg_Y);', 'tya': 'reg_A = reg_Y; update_nz(reg_A);',
        'txs': 'reg_S = reg_X;', 'tsx': 'reg_X = reg_S; update_nz(reg_X);',
        'pha': 'push_stack(reg_A);', 'pla': 'reg_A = pop_stack(); update_nz(reg_A);',
        'clc': 'reg_P &= ~FLAG_C;', 'sec': 'reg_P |= FLAG_C;', 'sei': 'reg_P |= FLAG_I;',
        'nop': '// NOP',
    }

    code = mapping.get(opcode, f"// Unsupported Opcode: {opcode}")
    return code, size

def pre_parse_symbols():
    for filename in sorted(os.listdir(ASM_DIR)):
        if not filename.endswith('.asm'): continue
        current_pc = 0xC000 if "Bank03" in filename else 0x8000
        with open(os.path.join(ASM_DIR, filename), 'r') as f:
            for line in f:
                line = line.split(';')[0].strip()
                if not line: continue
                org_match = re.match(r'^\.org\s+\$?([0-9A-Fa-f]+)', line)
                if org_match:
                    current_pc = int(org_match.group(1), 16)
                    continue
                label_match = re.match(r'^(\w+):?', line)
                if label_match and not re.match(r'^\w{3}\s', line):
                    label = label_match.group(1)
                    global_symbols[label] = hex(current_pc & 0xFFFF)
                    line = re.sub(r'^\w+:?\s*', '', line).strip()
                if not line: continue
                ins_match = re.match(r'^(\w{3})\s*(.*)$', line)
                if ins_match:
                    current_pc += get_ins_size(ins_match.group(1), ins_match.group(2))

def convert_all():
    if not os.path.exists(OUT_DIR): os.makedirs(OUT_DIR)
    pre_parse_symbols()
    for filename in sorted(os.listdir(ASM_DIR)):
        if not filename.endswith('.asm') or "Defines" in filename: continue
        bank_name = os.path.splitext(filename)[0].replace("-", "_")
        current_pc = 0xC000 if "Bank03" in filename else 0x8000
        with open(os.path.join(OUT_DIR, f"{bank_name}.cpp"), 'w') as out:
            out.write('#include "cpu_shared.h"\nnamespace ' + bank_name + ' {\nvoid execute() {\nswitch(reg_PC) {\n')
            with open(os.path.join(ASM_DIR, filename), 'r') as f:
                for line in f:
                    clean = line.split(';')[0].strip()
                    if not clean: continue
                    org_m = re.match(r'^\.org\s+\$?([0-9A-Fa-f]+)', clean)
                    if org_m: current_pc = int(org_m.group(1), 16); continue
                    ins_m = re.match(r'^(?:\w+:)?\s*(\w{3})\s*(.*)$', clean)
                    if ins_m:
                        code, size = translate_line(ins_m.group(1), ins_m.group(2), current_pc)
                        # CRITICAL: Force 16-bit address for the switch case
                        out.write(f'            case 0x{current_pc & 0xFFFF:04x}: {code}')
                        if "return" not in code: 
                            out.write(f' reg_PC += {size}; return;')
                        out.write(f' // {clean}\n')
                        current_pc += size
            out.write('default: return; }}}')

if __name__ == "__main__":
    convert_all()
