#!/usr/bin/env python3
"""
Dragon Warrior ASM to C++ Converter
Converts 6502 assembly to C++ switch-case recompiled code
"""

import os
import re
import sys

# Paths
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
SOURCE_DIR = os.path.join(SCRIPT_DIR, "..", "source_files")
OUTPUT_DIR = os.path.join(SCRIPT_DIR, "..", "app", "src", "main", "cpp", "recompiled")

# Global symbol table (label -> address)
global_symbols = {}

# Valid 6502 opcodes
VALID_OPCODES = {
    'adc', 'and', 'asl', 'bcc', 'bcs', 'beq', 'bit', 'bmi', 'bne', 'bpl',
    'brk', 'bvc', 'bvs', 'clc', 'cld', 'cli', 'clv', 'cmp', 'cpx', 'cpy',
    'dec', 'dex', 'dey', 'eor', 'inc', 'inx', 'iny', 'jmp', 'jsr', 'lda',
    'ldx', 'ldy', 'lsr', 'nop', 'ora', 'pha', 'php', 'pla', 'plp', 'rol',
    'ror', 'rti', 'rts', 'sbc', 'sec', 'sed', 'sei', 'sta', 'stx', 'sty',
    'tax', 'tay', 'tsx', 'txa', 'txs', 'tya'
}

def get_instruction_size(opcode, operand):
    """Calculate instruction size in bytes"""
    opcode = opcode.lower()
    
    # Data directives
    if opcode in ['.byte', '.db']: return 1
    if opcode in ['.word', '.dw', '.addr']: return 2
    
    # Implied addressing (1 byte)
    implied_opcodes = {
        'brk', 'clc', 'cld', 'cli', 'clv', 'dex', 'dey', 'inx', 'iny',
        'nop', 'pha', 'php', 'pla', 'plp', 'rti', 'rts', 'sec', 'sed',
        'sei', 'tax', 'tay', 'tsx', 'txa', 'txs', 'tya'
    }
    if opcode in implied_opcodes:
        return 1
    
    # Accumulator addressing (1 byte)
    if opcode in ['asl', 'lsr', 'rol', 'ror']:
        if not operand or operand.strip().upper() == 'A':
            return 1
    
    if not operand or operand.strip() == "":
        return 1
    
    # Immediate (2 bytes)
    if operand.startswith('#'):
        return 2
    
    # Indirect (2 bytes)
    if '(' in operand:
        return 2
    
    # Relative branches (2 bytes)
    if opcode in ['bcc', 'bcs', 'beq', 'bmi', 'bne', 'bpl', 'bvc', 'bvs']:
        return 2
    
    # Indexed addressing
    if ',' in operand:
        # Remove symbols and check length
        clean = re.sub(r'[#$(),\sXY%]', '', operand)
        return 2 if len(clean) <= 2 else 3
    
    # Absolute/Zero Page
    clean = re.sub(r'[#$(),\sXY%]', '', operand)
    
    # Check if it's a known symbol
    if clean in global_symbols:
        val = int(global_symbols[clean], 16)
        return 2 if val <= 0xFF else 3
    
    # Default based on operand length
    return 2 if len(clean) <= 2 else 3

def resolve_symbol(symbol):
    """Resolve a symbol to its hex value"""
    if not symbol:
        return "0x00"
    
    # Binary literal
    if '%' in symbol:
        match = re.search(r'%([01]+)', symbol)
        if match:
            return hex(int(match.group(1), 2))
    
    # Clean the symbol
    clean = re.sub(r'[#$(),\sXY]', '', symbol)
    
    # Look up in symbol table
    if clean in global_symbols:
        return global_symbols[clean]
    
    # Hex literal
    hex_match = re.search(r'([0-9A-Fa-f]+)', clean)
    if hex_match:
        return hex(int(hex_match.group(1), 16))
    
    return "0x00"

def translate_instruction(opcode, operand, pc):
    """Translate a single 6502 instruction to C++ code"""
    opcode = opcode.lower()
    
    # Skip data directives
    if opcode in ['.byte', '.db', '.word', '.dw', '.addr', '.asc', '.ds']:
        return "// Data directive", get_instruction_size(opcode, operand)
    
    # Skip non-opcode lines
    if opcode not in VALID_OPCODES:
        return f"// Unknown: {opcode}", 0
    
    size = get_instruction_size(opcode, operand)
    val = resolve_symbol(operand)
    
    # === IMMEDIATE ADDRESSING ===
    if operand and operand.startswith('#'):
        imm = int(val, 16) & 0xFF
        imm_hex = f"0x{imm:02X}"
        
        immediate_ops = {
            'lda': f'reg_A = {imm_hex}; update_nz(reg_A);',
            'ldx': f'reg_X = {imm_hex}; update_nz(reg_X);',
            'ldy': f'reg_Y = {imm_hex}; update_nz(reg_Y);',
            'cmp': f'update_flags_cmp(reg_A, {imm_hex});',
            'cpx': f'update_flags_cmp(reg_X, {imm_hex});',
            'cpy': f'update_flags_cmp(reg_Y, {imm_hex});',
            'ora': f'reg_A |= {imm_hex}; update_nz(reg_A);',
            'and': f'reg_A &= {imm_hex}; update_nz(reg_A);',
            'eor': f'reg_A ^= {imm_hex}; update_nz(reg_A);',
            'adc': f'cpu_adc({imm_hex});',
            'sbc': f'cpu_sbc({imm_hex});',
        }
        
        if opcode in immediate_ops:
            return immediate_ops[opcode], size
    
    # === INDIRECT ADDRESSING ===
    if operand and '(' in operand:
        addr = int(val, 16) & 0xFF
        
        if opcode == 'jmp':
            return f'reg_PC = read_pointer(0x{addr:04X}); return;', size
        
        if '),Y' in operand.upper():
            indirect_y_ops = {
                'lda': f'reg_A = bus_read(read_pointer_indexed_y(0x{addr:02X})); update_nz(reg_A);',
                'sta': f'bus_write(read_pointer_indexed_y(0x{addr:02X}), reg_A);',
                'cmp': f'update_flags_cmp(reg_A, bus_read(read_pointer_indexed_y(0x{addr:02X})));',
            }
            if opcode in indirect_y_ops:
                return indirect_y_ops[opcode], size
        
        if ',X)' in operand.upper():
            return f'reg_A = bus_read(read_pointer_indexed_x(0x{addr:02X})); update_nz(reg_A);', size
    
    # === ABSOLUTE/INDEXED/ZERO PAGE ===
    addr = int(val, 16) & 0xFFFF
    addr_str = f"0x{addr:04X}"
    
    # Check for accumulator addressing
    is_accumulator = (not operand or operand.strip().upper() == 'A') and opcode in ['asl', 'lsr', 'rol', 'ror']
    
    # Build final address expression
    if operand and ',' in operand:
        if ',X' in operand.upper():
            final_addr = f"(uint16_t)({addr_str} + reg_X)"
        else:  # ,Y
            final_addr = f"(uint16_t)({addr_str} + reg_Y)"
    else:
        final_addr = addr_str
    
    # Memory operations
    memory_ops = {
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
        'jsr': f'{{ uint16_t ret = reg_PC + {size - 1}; push_stack(ret >> 8); push_stack(ret & 0xFF); reg_PC = {final_addr}; return; }}',
        'inc': f'{{ uint8_t v = bus_read({final_addr}) + 1; bus_write({final_addr}, v); update_nz(v); }}',
        'dec': f'{{ uint8_t v = bus_read({final_addr}) - 1; bus_write({final_addr}, v); update_nz(v); }}',
        'adc': f'cpu_adc(bus_read({final_addr}));',
        'sbc': f'cpu_sbc(bus_read({final_addr}));',
        'ora': f'reg_A |= bus_read({final_addr}); update_nz(reg_A);',
        'and': f'reg_A &= bus_read({final_addr}); update_nz(reg_A);',
        'eor': f'reg_A ^= bus_read({final_addr}); update_nz(reg_A);',
    }
    
    # Shift/rotate operations
    if opcode == 'asl':
        return ('reg_A = cpu_asl(reg_A);' if is_accumulator 
                else f'{{ uint8_t v = cpu_asl(bus_read({final_addr})); bus_write({final_addr}, v); }}'), size
    elif opcode == 'lsr':
        return ('reg_A = cpu_lsr(reg_A);' if is_accumulator
                else f'{{ uint8_t v = cpu_lsr(bus_read({final_addr})); bus_write({final_addr}, v); }}'), size
    elif opcode == 'rol':
        return ('reg_A = cpu_rol(reg_A);' if is_accumulator
                else f'{{ uint8_t v = cpu_rol(bus_read({final_addr})); bus_write({final_addr}, v); }}'), size
    elif opcode == 'ror':
        return ('reg_A = cpu_ror(reg_A);' if is_accumulator
                else f'{{ uint8_t v = cpu_ror(bus_read({final_addr})); bus_write({final_addr}, v); }}'), size
    
    # Branch instructions
    branch_ops = {
        'beq': 'reg_P & FLAG_Z',
        'bne': '!(reg_P & FLAG_Z)',
        'bcs': 'reg_P & FLAG_C',
        'bcc': '!(reg_P & FLAG_C)',
        'bmi': 'reg_P & FLAG_N',
        'bpl': '!(reg_P & FLAG_N)',
        'bvs': 'reg_P & FLAG_V',
        'bvc': '!(reg_P & FLAG_V)',
    }
    
    if opcode in branch_ops:
        return f'if ({branch_ops[opcode]}) {{ reg_PC = {addr_str}; return; }}', size
    
    # Standalone instructions
    standalone_ops = {
        'rts': 'reg_PC = (pop_stack() | (pop_stack() << 8)) + 1; return;',
        'rti': 'reg_P = pop_stack(); reg_PC = (pop_stack() | (pop_stack() << 8)); return;',
        'brk': 'push_stack((reg_PC + 2) >> 8); push_stack((reg_PC + 2) & 0xFF); push_stack(reg_P | 0x10); reg_P |= FLAG_I; reg_PC = read_pointer(0xFFFE); return;',
        'tax': 'reg_X = reg_A; update_nz(reg_X);',
        'txa': 'reg_A = reg_X; update_nz(reg_A);',
        'tay': 'reg_Y = reg_A; update_nz(reg_Y);',
        'tya': 'reg_A = reg_Y; update_nz(reg_A);',
        'tsx': 'reg_X = reg_S; update_nz(reg_X);',
        'txs': 'reg_S = reg_X;',
        'clc': 'reg_P &= ~FLAG_C;',
        'sec': 'reg_P |= FLAG_C;',
        'cli': 'reg_P &= ~FLAG_I;',
        'sei': 'reg_P |= FLAG_I;',
        'clv': 'reg_P &= ~FLAG_V;',
        'cld': 'reg_P &= ~FLAG_D;',
        'sed': 'reg_P |= FLAG_D;',
        'dex': 'reg_X--; update_nz(reg_X);',
        'inx': 'reg_X++; update_nz(reg_X);',
        'dey': 'reg_Y--; update_nz(reg_Y);',
        'iny': 'reg_Y++; update_nz(reg_Y);',
        'pha': 'push_stack(reg_A);',
        'pla': 'reg_A = pop_stack(); update_nz(reg_A);',
        'php': 'push_stack(reg_P | 0x10);',
        'plp': 'reg_P = pop_stack();',
        'nop': '// NOP',
    }
    
    if opcode in standalone_ops:
        return standalone_ops[opcode], size
    
    # Memory operations
    if opcode in memory_ops:
        return memory_ops[opcode], size
    
    return f"// Unsupported: {opcode} {operand}", size

def pre_parse_symbols(asm_files):
    """First pass: build symbol table"""
    print("Building symbol table...")
    
    for filename in sorted(asm_files):
        filepath = os.path.join(SOURCE_DIR, filename)
        
        # Determine starting address
        if "Bank03" in filename:
            current_pc = 0xC000
        else:
            current_pc = 0x8000
        
        with open(filepath, 'r') as f:
            for line in f:
                line = line.split(';')[0].strip()
                if not line:
                    continue
                
                # Check for .org directive
                org_match = re.match(r'^\.org\s+\$?([0-9A-Fa-f]+)', line)
                if org_match:
                    current_pc = int(org_match.group(1), 16)
                    continue
                
                # Check for label
                label_match = re.match(r'^(\w+):', line)
                if label_match:
                    label = label_match.group(1)
                    global_symbols[label] = hex(current_pc & 0xFFFF)
                    # Remove label from line
                    line = re.sub(r'^\w+:\s*', '', line).strip()
                
                if not line:
                    continue
                
                # Parse instruction
                match = re.match(r'^(\.?\w+)\s*(.*)$', line)
                if match:
                    opcode, operand = match.groups()
                    current_pc += get_instruction_size(opcode, operand)
    
    print(f"Found {len(global_symbols)} symbols")

def convert_asm_file(filename):
    """Convert a single ASM file to C++"""
    filepath = os.path.join(SOURCE_DIR, filename)
    bank_name = os.path.splitext(filename)[0].replace("-", "_")
    
    # Determine starting address
    if "Bank03" in filename:
        current_pc = 0xC000
    else:
        current_pc = 0x8000
    
    output_path = os.path.join(OUTPUT_DIR, f"{bank_name}.cpp")
    
    print(f"Converting {filename} -> {bank_name}.cpp")
    
    with open(output_path, 'w') as out:
        # Write header
        out.write('#include "cpu_shared.h"\n\n')
        out.write(f'namespace {bank_name} {{\n')
        out.write('void execute() {\n')
        out.write('    switch (reg_PC) {\n')
        
        # Process file
        with open(filepath, 'r') as f:
            for line in f:
                clean = line.split(';')[0].strip()
                if not clean:
                    continue
                
                # Check for .org
                org_match = re.match(r'^\.org\s+\$?([0-9A-Fa-f]+)', clean)
                if org_match:
                    current_pc = int(org_match.group(1), 16)
                    continue
                
                # Parse line (label + instruction)
                match = re.match(r'^(?:(\w+):)?\s*(\.?\w+)?\s*(.*)$', clean)
                if not match:
                    continue
                
                label, opcode, operand = match.groups()
                
                if not opcode:
                    continue
                
                # Translate instruction
                code, size = translate_instruction(opcode, operand, current_pc)
                
                if size > 0 and opcode.lower() in VALID_OPCODES:
                    out.write(f'        case 0x{current_pc & 0xFFFF:04X}: ')
                    out.write(code)
                    
                    # Add PC increment if no control flow change
                    if 'return' not in code:
                        out.write(f' reg_PC += {size}; return;')
                    
                    out.write(f' // {clean}\n')
                
                current_pc += size
        
        # Write footer
        out.write('        default: reg_PC++; return;\n')
        out.write('    }\n')
        out.write('}\n')
        out.write('}\n')

def main():
    print("=" * 60)
    print("Dragon Warrior ASM to C++ Converter")
    print("=" * 60)
    
    # Check directories
    if not os.path.exists(SOURCE_DIR):
        print(f"ERROR: Source directory not found: {SOURCE_DIR}")
        return 1
    
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    
    # Find ASM files
    asm_files = [f for f in os.listdir(SOURCE_DIR) 
                 if f.endswith('.asm') and "Defines" not in f and "Header" not in f]
    
    if not asm_files:
        print(f"ERROR: No ASM files found in {SOURCE_DIR}")
        return 1
    
    print(f"Found {len(asm_files)} ASM files")
    
    # First pass: build symbol table
    pre_parse_symbols(asm_files)
    
    # Second pass: convert files
    for filename in asm_files:
        convert_asm_file(filename)
    
    print("=" * 60)
    print("Conversion complete!")
    print("=" * 60)
    return 0

if __name__ == "__main__":
    sys.exit(main())
