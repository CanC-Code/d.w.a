import os
import re

# Configuration
ASM_DIR = "source_files"
OUT_DIR = "app/src/main/cpp/recompiled"

global_symbols = {}

def get_ins_size(opcode, op):
    opcode = opcode.lower()
    if opcode in ['clc', 'cld', 'cli', 'clv', 'dex', 'dey', 'inx', 'iny', 'nop', 
                  'pha', 'php', 'pla', 'plp', 'rti', 'rts', 'sec', 'sed', 'sei', 
                  'tax', 'tay', 'tsx', 'txa', 'txs', 'tya', 'asl', 'lsr', 'rol', 'ror']:
        if not op or op.strip().upper() == 'A': return 1
    
    if not op or op.strip() == "": return 1
    
    # Addressing Mode Detection
    if op.startswith('#'): return 2
    if '(' in op: return 2 # Indirect is almost always ZP in NES games
    
    if ',' in op:
        clean_op = re.sub(r'[#$(),\sXY%]', '', op)
        return 2 if len(clean_op) <= 2 else 3
    
    if opcode in ['bcc', 'bcs', 'beq', 'bmi', 'bne', 'bpl', 'bvc', 'bvs']:
        return 2

    clean_op = re.sub(r'[#$(),\sXY%]', '', op)
    if clean_op in global_symbols:
        val = int(global_symbols[clean_op], 16)
        return 2 if val <= 0xFF else 3
    
    return 2 if len(clean_op) <= 2 else 3

def resolve_symbol(symbol):
    if not symbol: return "0x00"
    if '%' in symbol:
        bin_match = re.search(r'%([01]+)', symbol)
        if bin_match: return hex(int(bin_match.group(1), 2))

    clean_sym = re.sub(r'[#$(),\sXY]', '', symbol)
    if clean_sym in global_symbols: return global_symbols[clean_sym]
    
    hex_match = re.search(r'([0-9A-Fa-f]+)', clean_sym)
    if hex_match:
        val = int(hex_match.group(1), 16)
        return hex(val)
    return "0x00"

def translate_line(opcode, op, current_pc):
    opcode = opcode.lower()
    val_raw = resolve_symbol(op)
    size = get_ins_size(opcode, op)
    
    # 1. IMMEDIATE (#val)
    if op.startswith('#'):
        imm = int(val_raw, 16) & 0xFF
        imm_s = f"0x{imm:02x}"
        imm_map = {
            'lda': f'reg_A = {imm_s}; update_nz(reg_A);',
            'ldx': f'reg_X = {imm_s}; update_nz(reg_X);',
            'ldy': f'reg_Y = {imm_s}; update_nz(reg_Y);',
            'cmp': f'update_flags_cmp(reg_A, {imm_s});',
            'cpx': f'update_flags_cmp(reg_X, {imm_s});',
            'cpy': f'update_flags_cmp(reg_Y, {imm_s});',
            'ora': f'reg_A |= {imm_s}; update_nz(reg_A);',
            'and': f'reg_A &= {imm_s}; update_nz(reg_A);',
            'eor': f'reg_A ^= {imm_s}; update_nz(reg_A);',
            'adc': f'cpu_adc({imm_s});',
            'sbc': f'cpu_sbc({imm_s});',
        }
        if opcode in imm_map: return imm_map[opcode], size

    # 2. INDIRECT ADDRESSING (Critical for Dragon Warrior)
    if '(' in op:
        addr = int(val_raw, 16) & 0xFF
        if opcode == 'jmp': # Indirect JMP
            return f'reg_PC = read_pointer(0x{addr:04x}); return;', size
        
        # Indirect Indexed: LDA ($addr),Y
        if '),Y' in op.upper():
            code_map = {
                'lda': f'reg_A = bus_read(read_pointer_indexed_y({hex(addr)})); update_nz(reg_A);',
                'sta': f'bus_write(read_pointer_indexed_y({hex(addr)}), reg_A);',
                'cmp': f'update_flags_cmp(reg_A, bus_read(read_pointer_indexed_y({hex(addr)})));',
            }
            if opcode in code_map: return code_map[opcode], size
        
        # Indexed Indirect: LDA ($addr,X)
        if ',X)' in op.upper():
            return f'reg_A = bus_read(read_pointer_indexed_x({hex(addr)})); update_nz(reg_A);', size

    # 3. INDEXED / ABSOLUTE / ZERO PAGE
    addr_val = int(val_raw, 16) & 0xFFFF
    m_str = f"0x{addr_val:04x}"
    
    if ',' in op:
        reg = 'reg_X' if ',X' in op.upper() else 'reg_Y'
        final_addr = f"(uint16_t)({m_str} + {reg})" if addr_val > 0xFF else f"(uint8_t)({addr_val} + {reg})"
    else:
        final_addr = m_str

    mapping = {
        'lda': f'reg_A = bus_read({final_addr}); update_nz(reg_A);',
        'ldx': f'reg_X = bus_read({final_addr}); update_nz(reg_X);',
        'ldy': f'reg_Y = bus_read({final_addr}); update_nz(reg_Y);',
        'sta': f'bus_write({final_addr}, reg_A);',
        'stx': f'bus_write({final_addr}, reg_X);',
        'sty': f'bus_write({final_addr}, reg_Y);',
        'bit': f'cpu_bit(bus_read({final_addr}));',
        'cmp': f'update_flags_cmp(reg_A, bus_read({final_addr}));',
        'cpx': f'update_flags_cmp(reg_X, bus_read({final_addr}));',
        'cpy': f'update_flags_cmp(reg_Y, bus_read({final_addr}));',
        'jmp': f'reg_PC = {final_addr}; return;',
        'jsr': f'{{ uint16_t ret = (reg_PC + {size} - 1); push_stack(ret >> 8); push_stack(ret & 0xFF); reg_PC = {final_addr}; return; }}',
        'inc': f'{{ uint8_t v = bus_read({final_addr}) + 1; bus_write({final_addr}, v); update_nz(v); }}',
        'dec': f'{{ uint8_t v = bus_read({final_addr}) - 1; bus_write({final_addr}, v); update_nz(v); }}',
        'asl': f'{{ if ("{op}" == "A") reg_A = cpu_asl(reg_A); else bus_write({final_addr}, cpu_asl(bus_read({final_addr}))); }}',
        'lsr': f'{{ if ("{op}" == "A") reg_A = cpu_lsr(reg_A); else bus_write({final_addr}, cpu_lsr(bus_read({final_addr}))); }}',
        'rol': f'{{ if ("{op}" == "A") reg_A = cpu_rol(reg_A); else bus_write({final_addr}, cpu_rol(bus_read({final_addr}))); }}',
        'ror': f'{{ if ("{op}" == "A") reg_A = cpu_ror(reg_A); else bus_write({final_addr}, cpu_ror(bus_read({final_addr}))); }}',
    }
    
    # Branching
    branch_map = {
        'beq': 'reg_P & FLAG_Z', 'bne': '!(reg_P & FLAG_Z)',
        'bcs': 'reg_P & FLAG_C', 'bcc': '!(reg_P & FLAG_C)',
        'bmi': 'reg_P & FLAG_N', 'bpl': '!(reg_P & FLAG_N)',
    }
    if opcode in branch_map:
        return f'if ({branch_map[opcode]}) {{ reg_PC = {m_str}; return; }}', size

    # Single-byte / Stack
    standalone = {
        'rts': 'reg_PC = (pop_stack() | (pop_stack() << 8)) + 1; return;',
        'rti': 'reg_P = pop_stack(); reg_PC = (pop_stack() | (pop_stack() << 8)); return;',
        'tax': 'reg_X = reg_A; update_nz(reg_X);', 'txa': 'reg_A = reg_X; update_nz(reg_A);',
        'tay': 'reg_Y = reg_A; update_nz(reg_Y);', 'tya': 'reg_A = reg_Y; update_nz(reg_A);',
        'txs': 'reg_S = reg_X;', 'tsx': 'reg_X = reg_S; update_nz(reg_X);',
        'pha': 'push_stack(reg_A);', 'pla': 'reg_A = pop_stack(); update_nz(reg_A);',
        'php': 'push_stack(reg_P | 0x30);', 'plp': 'reg_P = (pop_stack() & 0xEF) | 0x20;',
        'clc': 'reg_P &= ~FLAG_C;', 'sec': 'reg_P |= FLAG_C;',
        'cli': 'reg_P &= ~FLAG_I;', 'sei': 'reg_P |= FLAG_I;',
        'dex': 'reg_X--; update_nz(reg_X);', 'inx': 'reg_X++; update_nz(reg_X);',
        'dey': 'reg_Y--; update_nz(reg_Y);', 'iny': 'reg_Y++; update_nz(reg_Y);',
        'nop': '// NOP',
    }

    if opcode in standalone: return standalone[opcode], size
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
                label_match = re.match(r'^(\w+):', line)
                if label_match:
                    global_symbols[label_match.group(1)] = hex(current_pc & 0xFFFF)
                    line = re.sub(r'^\w+:\s*', '', line).strip()
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
                        out.write(f'            case 0x{current_pc & 0xFFFF:04x}: {code}')
                        if "return" not in code: 
                            out.write(f' reg_PC += {size}; return;')
                        out.write(f' // {clean}\n')
                        current_pc += size
            out.write('default: return; }}}')

if __name__ == "__main__":
    convert_all()
