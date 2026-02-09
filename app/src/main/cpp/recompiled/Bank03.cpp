#include "cpu_shared.h"
namespace Bank03 {
void execute_at(uint16_t pc) {
    switch (pc) {
        case 0xC000: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LC000:  JSR $C5E0
        case 0xC003: cycles_to_run -= 3; reg_PC = 0x0000; return; // LC003:  JMP $C009
        case 0xC006: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC006:  JSR CalcAttribAddr
        case 0xC009: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LC009:  TYA
        case 0xC00A: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LC00A:  PHA
        case 0xC00B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC00B:  LDA NTYPos
        case 0xC00E: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC00D:  AND #$02
        case 0xC010: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LC00F:  ASL
        case 0xC011: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC010:  STA GenByte3D
        case 0xC014: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC012:  LDA NTXPos
        case 0xC017: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC014:  AND #$02
        case 0xC019: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC016:  CLC
        case 0xC01A: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LC017:  ADC GenByte3D
        case 0xC01D: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LC019:  TAY
        case 0xC01E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC01A:  LDA #$FC
        case 0xC020: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LC01C:  CPY #$00
        case 0xC022: cycles_to_run -= 2; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LC01E:  BEQ SetAttribBits
        case 0xC024: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LC020:  SEC
        case 0xC025: cycles_to_run -= 2; reg_A = cpu_rol(reg_A); reg_PC += 1; return; // LC021:  ROL
        case 0xC026: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LC022:  ASL PPUDataByte
        case 0xC029: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LC024:  DEY
        case 0xC02A: cycles_to_run -= 2; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC025:  BNE AttribLoop
        case 0xC02C: cycles_to_run -= 2; reg_A &= bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LC027:  AND (PPUBufPtr),Y
        case 0xC02E: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC029:  ORA PPUDataByte
        case 0xC031: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LC02B:  STA (PPUBufPtr),Y
        case 0xC033: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC02D:  STA PPUDataByte
        case 0xC036: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LC02F:  PLA
        case 0xC037: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LC030:  TAY
        case 0xC038: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC031:  RTS
        case 0xC039: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC032:  CMP #DIR_UP
        case 0xC03B: cycles_to_run -= 2; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC034:  BNE ChkPlayerRight
        case 0xC03D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC036:  LDA #DIR_DOWN
        case 0xC03F: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC038:  RTS
        case 0xC040: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC039:  CMP #DIR_RIGHT
        case 0xC042: cycles_to_run -= 2; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC03B:  BNE ChkPlayerDown
        case 0xC044: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC03D:  LDA #DIR_LEFT
        case 0xC046: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC03F:  RTS
        case 0xC047: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC040:  CMP #DIR_DOWN
        case 0xC049: cycles_to_run -= 2; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC042:  BNE PlayerLeft
        case 0xC04B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC044:  LDA #DIR_UP
        case 0xC04D: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC046:  RTS
        case 0xC04E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC047:  LDA #DIR_RIGHT
        case 0xC050: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC049:  RTS
        case 0xC051: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC04A:  STA NPCNumber
        case 0xC054: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LC04C:  TYA
        case 0xC055: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LC04D:  PHA
        case 0xC056: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LC04E:  TXA
        case 0xC057: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LC04F:  PHA
        case 0xC058: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LC050:  LDX NPCNumber
        case 0xC05B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC052:  LDA CharDirection
        case 0xC05E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC055:  JSR NPCNewDir
        case 0xC061: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC058:  STA NPCNewFace
        case 0xC064: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LC05A:  LSR
        case 0xC065: cycles_to_run -= 2; reg_A = cpu_ror(reg_A); reg_PC += 1; return; // LC05B:  ROR
        case 0xC066: cycles_to_run -= 2; reg_A = cpu_ror(reg_A); reg_PC += 1; return; // LC05C:  ROR
        case 0xC067: cycles_to_run -= 2; reg_A = cpu_ror(reg_A); reg_PC += 1; return; // LC05D:  ROR
        case 0xC068: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC05E:  STA GenByte24
        case 0xC06B: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LC060:  LDA NPCYPos,X
        case 0xC06E: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC062:  AND #$9F
        case 0xC070: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC064:  ORA GenByte24
        case 0xC073: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LC066:  STA NPCYPos,X
        case 0xC076: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC068:  LDA #$00
        case 0xC078: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC06A:  STA NPCSpriteCntr
        case 0xC07B: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LC06C:  LDY SpriteRAM
        case 0xC07E: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LC06F:  LDX SpriteRAM+3
        case 0xC081: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC072:  LDA CharDirection
        case 0xC084: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC075:  BNE ChkPlyrRight
        case 0xC086: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LC077:  TYA
        case 0xC087: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LC078:  SEC
        case 0xC088: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LC079:  SBC #$10
        case 0xC08A: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LC07B:  TAY
        case 0xC08B: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LC07C:  JMP CheckNPCPosition
        case 0xC08E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC07F:  CMP #DIR_RIGHT
        case 0xC090: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC081:  BNE ChkPlyrDown
        case 0xC092: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LC083:  TXA
        case 0xC093: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC084:  CLC
        case 0xC094: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LC085:  ADC #$10
        case 0xC096: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LC087:  TAX
        case 0xC097: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LC088:  JMP CheckNPCPosition
        case 0xC09A: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC08B:  CMP #DIR_DOWN
        case 0xC09C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC08D:  BNE PlyrLeft
        case 0xC09E: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LC08F:  TYA
        case 0xC09F: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC090:  CLC
        case 0xC0A0: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LC091:  ADC #$10
        case 0xC0A2: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LC093:  TAY
        case 0xC0A3: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LC094:  JMP CheckNPCPosition
        case 0xC0A6: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LC097:  TXA
        case 0xC0A7: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LC098:  SEC
        case 0xC0A8: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LC099:  SBC #$10
        case 0xC0AA: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LC09B:  TAX
        case 0xC0AB: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LC09C:  STX NPCXCheck
        case 0xC0AE: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LC09E:  STY NPCYCheck
        case 0xC0B1: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LC0A0:  LDY #$10
        case 0xC0B3: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LC0A2:  LDA SpriteRAM,Y
        case 0xC0B6: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LC0A5:  CMP NPCYCheck
        case 0xC0B9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LC0A7:  BNE +
        case 0xC0BB: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LC0A9:  LDA SpriteRAM+3,Y
        case 0xC0BE: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LC0AC:  CMP NPCXCheck
        case 0xC0C1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LC0AE:  BEQ NPCFound
        case 0xC0C3: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC0B1:  CLC
        case 0xC0C4: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LC0B2:  ADC #$10
        case 0xC0C6: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LC0B4:  TAY
        case 0xC0C7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC0B5:  BNE NPCSearchLoop
        case 0xC0C9: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LC0B7:  LDX NPCXCheck
        case 0xC0CC: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LC0B9:  LDY NPCYCheck
        case 0xC0CF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC0BB:  LDA NPCSpriteCntr
        case 0xC0D2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC0BD:  BNE NPCDirEnd
        case 0xC0D4: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC0BF:  LDA #$01
        case 0xC0D6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC0C1:  STA NPCSpriteCntr
        case 0xC0D9: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LC0C3:  JMP ChkPlyrDirection
        case 0xC0DC: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LC0C6:  STY NPCSprtRAMInd
        case 0xC0DF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC0C8:  LDA #$04
        case 0xC0E1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC0CA:  STA NPCSpriteCntr
        case 0xC0E4: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LC0CC:  LDX NPCNumber
        case 0xC0E7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC0CE:  JSR GetNPCSpriteIndex
        case 0xC0EA: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LC0D1:  TAY
        case 0xC0EB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC0D2:  LDA NPCNewFace
        case 0xC0EE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LC0D4:  JSR SprtFacingBaseAddr
        case 0xC0F1: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LC0D7:  LDX NPCSprtRAMInd
        case 0xC0F4: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LC0D9:  LDA (GenPtr22),Y
        case 0xC0F6: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LC0DB:  STA SpriteRAM+1,X
        case 0xC0F9: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LC0DE:  INY
        case 0xC0FA: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LC0DF:  LDA (GenPtr22),Y
        case 0xC0FC: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LC0E1:  DEY
        case 0xC0FD: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LC0E2:  STA SpriteRAM+2,X
        case 0xC100: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LC0E5:  INX
        case 0xC101: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LC0E6:  INX
        case 0xC102: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LC0E7:  INX
        case 0xC103: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LC0E8:  INX
        case 0xC104: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LC0E9:  INY
        case 0xC105: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LC0EA:  INY
        case 0xC106: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LC0EB:  DEC NPCSpriteCntr
        case 0xC109: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC0ED:  BNE NPCLoadNxtSprt
        case 0xC10B: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LC0EF:  PLA
        case 0xC10C: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LC0F0:  TAX
        case 0xC10D: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LC0F1:  PLA
        case 0xC10E: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LC0F2:  TAY
        case 0xC10F: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC0F3:  RTS
        case 0xC110: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LC0F4:  LDA NPCXPos,X
        case 0xC113: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC0F6:  AND #$E0
        case 0xC115: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LC0F8:  LSR
        case 0xC116: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC0F9:  STA GenByte24
        case 0xC119: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC0FB:  CMP #$60
        case 0xC11B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC0FD:  BNE ChkWizardNPC
        case 0xC11D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC0FF:  LDA MapNumber
        case 0xC120: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC101:  CMP #MAP_TANTCSTL_GF
        case 0xC122: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC103:  BNE ChkThroneRoomNPC
        case 0xC124: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC105:  LDA StoryFlags
        case 0xC127: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC107:  AND #F_DGNLRD_DEAD
        case 0xC129: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC109:  BNE SetPrincessNPC
        case 0xC12B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC10B:  LDA MapNumber
        case 0xC12E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC10D:  CMP #MAP_THRONEROOM
        case 0xC130: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC10F:  BNE NPCWalkAnim
        case 0xC132: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC111:  LDA #$D0
        case 0xC134: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC113:  STA GenByte24
        case 0xC137: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC115:  BNE NPCWalkAnim
        case 0xC139: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC117:  LDA GenByte24
        case 0xC13C: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC119:  CMP #$50
        case 0xC13E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC11B:  BNE ChkGuardNPC
        case 0xC140: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC11D:  LDA MapNumber
        case 0xC143: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC11F:  CMP #MAP_TANTCSTL_GF
        case 0xC145: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC121:  BNE ChkDrgnLordNPC
        case 0xC147: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC123:  LDA StoryFlags
        case 0xC14A: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC125:  AND #F_DGNLRD_DEAD
        case 0xC14C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LC127:  BEQ ChkDrgnLordNPC
        case 0xC14E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC129:  LDA #$F0
        case 0xC150: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC12B:  STA GenByte24
        case 0xC153: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC12D:  LDA DisplayedLevel
        case 0xC156: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC12F:  CMP #$FF
        case 0xC158: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC131:  BNE EndNPCSpclType
        case 0xC15A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC133:  LDA GenByte24
        case 0xC15D: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC135:  ORA #$08
        case 0xC15F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC137:  STA GenByte24
        case 0xC162: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC139:  BNE EndNPCSpclType
        case 0xC164: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC13B:  LDA MapNumber
        case 0xC167: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC13D:  CMP #MAP_DLCSTL_BF
        case 0xC169: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC13F:  BNE NPCWalkAnim
        case 0xC16B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC141:  LDA #$E0
        case 0xC16D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC143:  STA GenByte24
        case 0xC170: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC145:  BNE NPCWalkAnim
        case 0xC172: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC147:  CMP #$70
        case 0xC174: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LC149:  BEQ GetGuardType
        case 0xC176: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC14B:  LDA CharLeftRight
        case 0xC179: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC14D:  AND #$08
        case 0xC17B: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC14F:  ORA GenByte24
        case 0xC17E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC151:  STA GenByte24
        case 0xC181: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC153:  LDA GenByte24
        case 0xC184: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC155:  RTS
        case 0xC185: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LC156:  LDX #$3A
        case 0xC187: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC158:  LDA #LVL_30
        case 0xC189: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC15A:  STA DisplayedLevel
        case 0xC18C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC15C:  LDA ExpLB
        case 0xC18F: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LC15E:  SEC
        case 0xC190: cycles_to_run -= 2; cpu_sbc(bus_read(addr_abs_x(0xC07E, nullptr))); reg_PC += 3; return; // LC15F:  SBC LevelUpTbl,X
        case 0xC193: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC162:  LDA ExpUB
        case 0xC196: cycles_to_run -= 2; cpu_sbc(bus_read(addr_abs_x(0x0000, nullptr))); reg_PC += 3; return; // LC164:  SBC LevelUpTbl+1,X
        case 0xC199: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07B; return; } // LC167:  BCS PlayerStatEnd
        case 0xC19B: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LC169:  DEC DisplayedLevel
        case 0xC19E: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LC16B:  DEX
        case 0xC19F: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LC16C:  DEX
        case 0xC1A0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC16D:  BNE PlayerStatLoop
        case 0xC1A2: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC16F:  RTS
        case 0xC1A3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC170:  STA GenByte24
        case 0xC1A6: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LC175:  DEC GenByte24
        case 0xC1A9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LC177:  BPL -
        case 0xC1AB: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC179:  RTS
        case 0xC1AC: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LC17A:  PHA
        case 0xC1AD: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LC17B:  TXA
        case 0xC1AE: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LC17C:  PHA
        case 0xC1AF: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LC17D:  TYA
        case 0xC1B0: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LC17E:  PHA
        case 0xC1B1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LC17F:  JSR WaitForNMI
        case 0xC1B4: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC182:  LDA #%00001000
        case 0xC1B6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC184:  STA PPUControl0
        case 0xC1B9: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC187:  LDA #TL_BLANK_TILE1
        case 0xC1BB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC189:  STA PPUDataByte
        case 0xC1BE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC18B:  LDA PPUStatus
        case 0xC1C1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC18E:  LDA #NT_NAMETBL0_UB
        case 0xC1C3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC190:  STA PPUAddress
        case 0xC1C6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC193:  LDA #NT_NAMETBL0_LB
        case 0xC1C8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC195:  STA PPUAddress
        case 0xC1CB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC198:  JSR ClearNameTable
        case 0xC1CE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC19B:  LDA PPUStatus
        case 0xC1D1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC19E:  LDA #NT_NAMETBL1_UB
        case 0xC1D3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC1A0:  STA PPUAddress
        case 0xC1D6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC1A3:  LDA #NT_NAMETBL1_LB
        case 0xC1D8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC1A5:  STA PPUAddress
        case 0xC1DB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC1A8:  JSR ClearNameTable
        case 0xC1DE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC1AB:  LDA #%10001000
        case 0xC1E0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC1AD:  STA PPUControl0
        case 0xC1E3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LC1B0:  JSR WaitForNMI
        case 0xC1E6: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LC1B3:  PLA
        case 0xC1E7: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LC1B4:  TAY
        case 0xC1E8: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LC1B5:  PLA
        case 0xC1E9: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LC1B6:  TAX
        case 0xC1EA: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LC1B7:  PLA
        case 0xC1EB: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC1B8:  RTS
        case 0xC1EC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC1B9:  LDA PPUDataByte
        case 0xC1EF: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LC1BB:  LDX #$1E
        case 0xC1F1: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LC1BD:  LDY #$20
        case 0xC1F3: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LC1C2:  DEY
        case 0xC1F4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LC1C3:  BNE -
        case 0xC1F6: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LC1C5:  DEX
        case 0xC1F7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC1C6:  BNE ClearNTOuterLoop
        case 0xC1F9: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC1C8:  RTS
        case 0xC1FA: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC1C9:  LDA #$00
        case 0xC1FC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC1CB:  STA MultRsltLB
        case 0xC1FF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC1CD:  STA MultRsltUB
        case 0xC202: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC1CF:  LDA MultNum1LB
        case 0xC205: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC1D1:  ORA MultNum1UB
        case 0xC208: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LC1D3:  BEQ MultEnd
        case 0xC20A: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LC1D5:  LSR MultNum1UB
        case 0xC20D: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LC1D7:  ROR MultNum1LB
        case 0xC210: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LC1D9:  BCC +
        case 0xC212: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC1DB:  LDA MultNum2LB
        case 0xC215: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC1DD:  CLC
        case 0xC216: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LC1DE:  ADC MultRsltLB
        case 0xC219: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC1E0:  STA MultRsltLB
        case 0xC21C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC1E2:  LDA MultNum2UB
        case 0xC21F: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LC1E4:  ADC MultRsltUB
        case 0xC222: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC1E6:  STA MultRsltUB
        case 0xC225: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_rol(bus_read(a))); } reg_PC += 3; return; // LC1EA:  ROL MultNum2UB
        case 0xC228: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LC1EC:  JMP MultiplyLoop
        case 0xC22B: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC1EF:  RTS
        case 0xC22C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC1F0:  LDA #$00
        case 0xC22E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC1F2:  STA DivNmu1UB
        case 0xC231: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LC1F4:  LDY #$10
        case 0xC233: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC1F6:  LDA #$00
        case 0xC235: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LC1F8:  ASL DivNum1LB
        case 0xC238: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_rol(bus_read(a))); } reg_PC += 3; return; // LC1FA:  ROL DivNmu1UB
        case 0xC23B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC1FC:  STA DivRemainder
        case 0xC23E: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LC1FE:  ADC DivRemainder
        case 0xC241: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LC200:  INC DivQuotient
        case 0xC244: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LC202:  SEC
        case 0xC245: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LC203:  SBC DivNum2
        case 0xC248: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LC205:  BCS +
        case 0xC24A: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC207:  CLC
        case 0xC24B: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LC208:  ADC DivNum2
        case 0xC24E: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LC20A:  DEC DivQuotient
        case 0xC251: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC20D:  BNE DivLoop
        case 0xC253: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC20F:  STA DivRemainder
        case 0xC256: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC211:  RTS
        case 0xC257: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC212:  LDA #$00
        case 0xC259: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC214:  STA PalModByte
        case 0xC25C: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LC216:  LDX #$04
        case 0xC25E: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LC21B:  DEX
        case 0xC25F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LC21C:  BNE -
        case 0xC261: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC21E:  LDA SprtPalPtrLB
        case 0xC264: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC220:  STA PalPtrLB
        case 0xC267: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC222:  LDA SprtPalPtrUB
        case 0xC26A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC224:  STA PalPtrUB
        case 0xC26D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC226:  JSR PrepSPPalLoad
        case 0xC270: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC229:  LDA LoadBGPal
        case 0xC273: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LC22B:  BEQ +
        case 0xC275: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC22D:  LDA BGPalPtrLB
        case 0xC278: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC22F:  STA PalPtrLB
        case 0xC27B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC231:  LDA BGPalPtrUB
        case 0xC27E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC233:  STA PalPtrUB
        case 0xC281: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC235:  JSR PrepBGPalLoad
        case 0xC284: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC23A:  CLC
        case 0xC285: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LC23B:  ADC #$10
        case 0xC287: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC23D:  STA PalModByte
        case 0xC28A: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC23F:  CMP #$50
        case 0xC28C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC241:  BNE FadeOutLoop
        case 0xC28E: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC243:  RTS
        case 0xC28F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC244:  LDA NTBlockX
        case 0xC292: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LC246:  ASL
        case 0xC293: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC247:  CLC
        case 0xC294: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LC248:  ADC XPosFromCenter
        case 0xC297: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC24A:  AND #$3F
        case 0xC299: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LC24C:  PHA
        case 0xC29A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC24D:  LDA NTBlockY
        case 0xC29D: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LC24F:  ASL
        case 0xC29E: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC250:  CLC
        case 0xC29F: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LC251:  ADC YPosFromCenter
        case 0xC2A2: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC253:  CLC
        case 0xC2A3: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LC254:  ADC #$1E
        case 0xC2A5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC256:  STA DivNum1LB
        case 0xC2A8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC258:  LDA #$1E
        case 0xC2AA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC25A:  STA DivNum2
        case 0xC2AD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC25C:  JSR ByteDivide
        case 0xC2B0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC25F:  LDA DivRemainder
        case 0xC2B3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC261:  STA NTYPos
        case 0xC2B6: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LC263:  PLA
        case 0xC2B7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC264:  STA NTXPos
        case 0xC2BA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC266:  JSR PrepClearAttrib
        case 0xC2BD: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC269:  RTS
        case 0xC2BE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LC26A:  JSR $C5E0
        case 0xC2C1: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LC26D:  JMP ClearAttrib
        case 0xC2C4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC270:  JSR CalcAttribAddr
        case 0xC2C7: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LC273:  TYA
        case 0xC2C8: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LC274:  PHA
        case 0xC2C9: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LC275:  LDY #$00
        case 0xC2CB: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LC277:  LDA (PPUBufPtr),Y
        case 0xC2CD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC279:  STA PPUDataByte
        case 0xC2D0: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LC27B:  PLA
        case 0xC2D1: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LC27C:  TAY
        case 0xC2D2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC27D:  LDA PPUAddrUB
        case 0xC2D5: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC27F:  CLC
        case 0xC2D6: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LC280:  ADC #$20
        case 0xC2D8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC282:  STA PPUAddrUB
        case 0xC2DB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC284:  JSR AddPPUBufEntry
        case 0xC2DE: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC287:  RTS
        case 0xC2DF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC529:  LDA #$30
        case 0xC2E1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC52B:  STA PalModByte
        case 0xC2E4: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LC52D:  LDX #$04
        case 0xC2E6: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LC532:  DEX
        case 0xC2E7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LC533:  BNE -
        case 0xC2E9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC535:  LDA SprtPalPtrLB
        case 0xC2EC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC537:  STA PalPtrLB
        case 0xC2EF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC539:  LDA SprtPalPtrUB
        case 0xC2F2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC53B:  STA PalPtrUB
        case 0xC2F5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC53D:  JSR PrepSPPalLoad
        case 0xC2F8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC540:  LDA LoadBGPal
        case 0xC2FB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LC542:  BEQ +
        case 0xC2FD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC544:  LDA BGPalPtrLB
        case 0xC300: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC546:  STA PalPtrLB
        case 0xC303: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC548:  LDA BGPalPtrUB
        case 0xC306: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC54A:  STA PalPtrUB
        case 0xC309: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC54C:  JSR PrepBGPalLoad
        case 0xC30C: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LC551:  SEC
        case 0xC30D: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LC552:  SBC #$10
        case 0xC30F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC554:  STA PalModByte
        case 0xC312: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC556:  CMP #$F0
        case 0xC314: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC558:  BNE FadeInLoop
        case 0xC316: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC55A:  RTS
        case 0xC317: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC55B:  LDA RandNumUB
        case 0xC31A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC55D:  STA GenWord3CUB
        case 0xC31D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC55F:  LDA RandNumLB
        case 0xC320: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC561:  STA GenWord3CLB
        case 0xC323: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LC563:  ASL RandNumLB
        case 0xC326: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_rol(bus_read(a))); } reg_PC += 3; return; // LC565:  ROL RandNumUB
        case 0xC329: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC567:  CLC
        case 0xC32A: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LC568:  ADC RandNumLB
        case 0xC32D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC56A:  STA RandNumLB
        case 0xC330: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC56C:  LDA RandNumUB
        case 0xC333: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LC56E:  ADC GenWord3CUB
        case 0xC336: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC570:  STA RandNumUB
        case 0xC339: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC572:  LDA RandNumLB
        case 0xC33C: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC574:  CLC
        case 0xC33D: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LC575:  ADC RandNumUB
        case 0xC340: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC577:  STA RandNumUB
        case 0xC343: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC579:  LDA RandNumLB
        case 0xC346: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC57B:  CLC
        case 0xC347: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LC57C:  ADC #$81
        case 0xC349: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC57E:  STA RandNumLB
        case 0xC34C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC580:  LDA RandNumUB
        case 0xC34F: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LC582:  ADC #$00
        case 0xC351: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC584:  STA RandNumUB
        case 0xC354: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC586:  RTS
        case 0xC355: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC587:  STA GenByte24
        case 0xC358: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC589:  LDA PPUBufCount
        case 0xC35B: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LC58B:  CMP GenByte24
        case 0xC35E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07B; return; } // LC58D:  BCC WaitForPPUBufEnd
        case 0xC360: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LC58F:  JSR WaitForNMI
        case 0xC363: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LC592:  JMP WaitForPPUBufLoop
        case 0xC366: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC595:  RTS
        case 0xC367: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC596:  LDA #$40
        case 0xC369: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC598:  ORA XPosFromLeft
        case 0xC36C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC59A:  STA XPosFromLeft
        case 0xC36F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC59C:  BNE Block2TileConv
        case 0xC371: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC59E:  LDA #$80
        case 0xC373: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC5A0:  ORA XPosFromLeft
        case 0xC376: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC5A2:  STA XPosFromLeft
        case 0xC379: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC5A4:  BNE DoAddrCalc
        case 0xC37B: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LC5A6:  ASL XPosFromLeft
        case 0xC37E: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LC5A8:  ASL YPosFromTop
        case 0xC381: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC5AA:  LDA YPosFromTop
        case 0xC384: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC5AC:  STA PPUBufPtrUB
        case 0xC387: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC5AE:  LDA #$00
        case 0xC389: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC5B0:  STA PPUBufPtrLB
        case 0xC38C: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LC5B2:  LSR PPUBufPtrUB
        case 0xC38F: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LC5B4:  ROR PPUBufPtrLB
        case 0xC392: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LC5B6:  LSR PPUBufPtrUB
        case 0xC395: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LC5B8:  ROR PPUBufPtrLB
        case 0xC398: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LC5BA:  LSR PPUBufPtrUB
        case 0xC39B: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LC5BC:  ROR PPUBufPtrLB
        case 0xC39E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC5BE:  LDA XPosFromLeft
        case 0xC3A1: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC5C0:  AND #$1F
        case 0xC3A3: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC5C2:  CLC
        case 0xC3A4: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LC5C3:  ADC PPUBufPtrLB
        case 0xC3A7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC5C5:  STA PPUBufPtrLB
        case 0xC3AA: cycles_to_run -= 3; // Unsupported php reg_PC += 1; return; // LC5C7:  PHP
        case 0xC3AB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC5C8:  LDA XPosFromLeft
        case 0xC3AE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LC5CA:  BPL +
        case 0xC3B0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC5CC:  LDA #$04
        case 0xC3B2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC5CE:  BNE EndPPUCalcAddr
        case 0xC3B4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LC5D2:  BNE +
        case 0xC3B6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC5D4:  LDA #NT_NAMETBL0_UB
        case 0xC3B8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC5D6:  BNE EndPPUCalcAddr
        case 0xC3BA: cycles_to_run -= 4; // Unsupported plp reg_PC += 1; return; // LC5DA:  PLP
        case 0xC3BB: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LC5DB:  ADC PPUAddrUB
        case 0xC3BE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC5DD:  STA PPUAddrUB
        case 0xC3C1: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC5DF:  RTS
        case 0xC3C2: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LC5E0:  ASL NTXPos
        case 0xC3C5: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LC5E2:  ASL NTYPos
        case 0xC3C8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC5E4:  LDA NTYPos
        case 0xC3CB: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC5E6:  AND #$FC
        case 0xC3CD: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LC5E8:  ASL
        case 0xC3CE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC5E9:  STA PPUAddrLB
        case 0xC3D1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC5EB:  LDA NTXPos
        case 0xC3D4: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC5ED:  AND #$1F
        case 0xC3D6: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LC5EF:  LSR
        case 0xC3D7: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LC5F0:  LSR
        case 0xC3D8: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC5F1:  CLC
        case 0xC3D9: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LC5F2:  ADC PPUAddrLB
        case 0xC3DC: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC5F4:  CLC
        case 0xC3DD: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LC5F5:  ADC #$C0
        case 0xC3DF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC5F7:  STA PPUAddrLB
        case 0xC3E2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC5F9:  LDA NTXPos
        case 0xC3E5: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC5FB:  AND #$20
        case 0xC3E7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LC5FD:  BNE +
        case 0xC3E9: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC5FF:  LDA #$03
        case 0xC3EB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC601:  BNE ExitAttribCalc
        case 0xC3ED: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC605:  STA PPUAddrUB
        case 0xC3F0: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC607:  RTS
        case 0xC3F1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC608:  LDA GenByte3C
        case 0xC3F4: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LC60A:  PHA
        case 0xC3F5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC60B:  LDA GenByte3D
        case 0xC3F8: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LC60D:  PHA
        case 0xC3F9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC60E:  JSR UpdateRandNum
        case 0xC3FC: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LC611:  PLA
        case 0xC3FD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC612:  STA GenByte3D
        case 0xC400: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LC614:  PLA
        case 0xC401: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC615:  STA GenByte3C
        case 0xC404: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC617:  LDA #$01
        case 0xC406: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC619:  STA CPUJoyPad1
        case 0xC409: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC61C:  LDA #$00
        case 0xC40B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC61E:  STA CPUJoyPad1
        case 0xC40E: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LC621:  LDY #$08
        case 0xC410: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC623:  LDA CPUJoyPad1
        case 0xC413: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC626:  STA JoypadBit
        case 0xC416: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LC628:  LSR
        case 0xC417: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC629:  ORA JoypadBit
        case 0xC41A: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LC62B:  LSR
        case 0xC41B: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LC62C:  ROR JoypadBtns
        case 0xC41E: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LC62E:  DEY
        case 0xC41F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC62F:  BNE JoypadReadLoop
        case 0xC421: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC631:  RTS
        case 0xC422: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC632:  LDA #$31
        case 0xC424: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC634:  JSR WaitForPPUBufSpace
        case 0xC427: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC637:  LDA #PAL_SPR_LB
        case 0xC429: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC639:  STA PPUAddrLB
        case 0xC42C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC63B:  BNE LoadPalData
        case 0xC42E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC63D:  LDA #$61
        case 0xC430: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC63F:  JSR WaitForPPUBufSpace
        case 0xC433: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC642:  LDA #PAL_BKG_LB
        case 0xC435: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC644:  STA PPUAddrLB
        case 0xC438: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC646:  LDA #PAL_UB
        case 0xC43A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC648:  STA PPUAddrUB
        case 0xC43D: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LC64A:  LDY #$00
        case 0xC43F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC64C:  LDA #PAL_BLACK
        case 0xC441: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC64E:  STA PPUDataByte
        case 0xC444: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC650:  JSR AddPPUBufEntry
        case 0xC447: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC653:  JSR AddPalByte
        case 0xC44A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC656:  JSR AddPalByte
        case 0xC44D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC659:  JSR AddPalByte
        case 0xC450: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LC65C:  CPY #$0C
        case 0xC452: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC65E:  BNE PalDataLoop
        case 0xC454: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC660:  RTS
        case 0xC455: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC661:  LDA PPUAddrLB
        case 0xC458: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC663:  CMP #$01
        case 0xC45A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LC665:  BEQ ChkLowHPPal
        case 0xC45C: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC667:  CMP #$03
        case 0xC45E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC669:  BNE ChkPalFade
        case 0xC460: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC66B:  LDA EnNumber
        case 0xC463: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC66D:  CMP #EN_DRAGONLORD2
        case 0xC465: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC66F:  BNE ChkPalFade
        case 0xC467: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC671:  LDA DisplayedMaxHP
        case 0xC46A: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LC673:  LSR
        case 0xC46B: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LC674:  LSR
        case 0xC46C: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC675:  CLC
        case 0xC46D: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LC676:  ADC #$01
        case 0xC46F: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LC678:  CMP HitPoints
        case 0xC472: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07B; return; } // LC67A:  BCC ChkPalFade
        case 0xC474: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC67C:  LDA #$26
        case 0xC476: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LC67E:  BNE +
        case 0xC478: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LC680:  LDA (PalPtrLB),Y
        case 0xC47A: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LC683:  SBC PalModByte
        case 0xC47D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LC685:  BCS +
        case 0xC47F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC687:  LDA #PAL_BLACK
        case 0xC481: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC68B:  JSR AddPPUBufEntry
        case 0xC484: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LC68E:  INY
        case 0xC485: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC68F:  RTS
        case 0xC486: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LC690:  LDX PPUBufCount
        case 0xC489: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LC692:  CPX #$B0
        case 0xC48B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07B; return; } // LC694:  BCC PutPPUBufDat
        case 0xC48D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LC696:  JSR WaitForNMI
        case 0xC490: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LC699:  JMP AddPPUBufEntry
        case 0xC493: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LC69C:  LDX PPUBufCount
        case 0xC496: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC69E:  LDA PPUAddrUB
        case 0xC499: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LC6A0:  STA BlockRAM,X
        case 0xC49C: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LC6A3:  INX
        case 0xC49D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC6A4:  LDA PPUAddrLB
        case 0xC4A0: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LC6A6:  STA BlockRAM,X
        case 0xC4A3: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LC6A9:  INX
        case 0xC4A4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC6AA:  LDA PPUDataByte
        case 0xC4A7: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LC6AC:  STA BlockRAM,X
        case 0xC4AA: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LC6AF:  INX
        case 0xC4AB: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LC6B0:  INC PPUEntCount
        case 0xC4AE: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LC6B2:  STX PPUBufCount
        case 0xC4B1: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LC6B4:  INC PPUAddrLB
        case 0xC4B4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LC6B6:  BNE +
        case 0xC4B6: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LC6B8:  INC PPUAddrUB
        case 0xC4B9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LC6BB:  JSR WaitForNMI
        case 0xC4BC: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LC6BE:  LDX #$00
        case 0xC4BE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC6C0:  LDA #$F0
        case 0xC4C0: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LC6C5:  INX
        case 0xC4C1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LC6C6:  BNE -
        case 0xC4C3: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC6C8:  RTS
        case 0xC4C4: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LC6F0:  PLA
        case 0xC4C5: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC6F1:  CLC
        case 0xC4C6: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LC6F2:  ADC #$01
        case 0xC4C8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC6F4:  STA GenPtr3ELB
        case 0xC4CB: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LC6F6:  PLA
        case 0xC4CC: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LC6F7:  ADC #$00
        case 0xC4CE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC6F9:  STA GenPtr3EUB
        case 0xC4D1: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LC6FB:  PHA
        case 0xC4D2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC6FC:  LDA GenPtr3ELB
        case 0xC4D5: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LC6FE:  PHA
        case 0xC4D6: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LC6FF:  LDY #$00
        case 0xC4D8: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LC701:  LDA (GenPtr3E),Y
        case 0xC4DA: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LC703:  BRK
        case 0xC4DB: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC706:  RTS
        case 0xC4DC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC707:  LDA BlockClear
        case 0xC4DF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC709:  BNE BlankBlock
        case 0xC4E1: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LC70B:  LDY #$00
        case 0xC4E3: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LC70D:  LDA (BlockAddr),Y
        case 0xC4E5: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC70F:  CMP #$FF
        case 0xC4E7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LC711:  BEQ BlankBlock
        case 0xC4E9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC713:  CMP #$FE
        case 0xC4EB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LC715:  BEQ BlankBlock
        case 0xC4ED: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LC717:  JMP BattleBlock
        case 0xC4F0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC71A:  LDA #$00
        case 0xC4F2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC71C:  STA BlkRemoveFlgs
        case 0xC4F5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC71E:  STA PPUHorzVert
        case 0xC4F8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LC720:  JSR ModMapBlock
        case 0xC4FB: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LC723:  LDY #$00
        case 0xC4FD: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC725:  LDA #$FF
        case 0xC4FF: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LC727:  STA (BlockAddr),Y
        case 0xC501: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LC729:  INY
        case 0xC502: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LC72A:  STA (BlockAddr),Y
        case 0xC504: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LC72C:  LDY #$20
        case 0xC506: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LC72E:  STA (BlockAddr),Y
        case 0xC508: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LC730:  INY
        case 0xC509: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LC731:  STA (BlockAddr),Y
        case 0xC50B: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC733:  RTS
        case 0xC50C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC734:  LDA NTBlockY
        case 0xC50F: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LC736:  ASL
        case 0xC510: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LC737:  ADC YPosFromCenter
        case 0xC513: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC739:  CLC
        case 0xC514: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LC73A:  ADC #$1E
        case 0xC516: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC73C:  STA DivNum1LB
        case 0xC519: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC73E:  LDA #$1E
        case 0xC51B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC740:  STA DivNum2
        case 0xC51E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC742:  JSR ByteDivide
        case 0xC521: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC745:  LDA DivRemainder
        case 0xC524: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC747:  STA YPosFromTop
        case 0xC527: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC749:  STA YFromTopTemp
        case 0xC52A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC74B:  LDA NTBlockX
        case 0xC52D: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LC74D:  ASL
        case 0xC52E: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC74E:  CLC
        case 0xC52F: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LC74F:  ADC XPosFromCenter
        case 0xC532: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC751:  AND #$3F
        case 0xC534: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC753:  STA XPosFromLeft
        case 0xC537: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC755:  STA XFromLeftTemp
        case 0xC53A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC757:  JSR DoAddrCalc
        case 0xC53D: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LC75A:  LDY #$00
        case 0xC53F: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LC75C:  LDA (BlockAddr),Y
        case 0xC541: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC75E:  STA PPUDataByte
        case 0xC544: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC760:  JSR AddPPUBufEntry
        case 0xC547: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LC763:  INY
        case 0xC548: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LC764:  LDA (BlockAddr),Y
        case 0xC54A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC766:  STA PPUDataByte
        case 0xC54D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC768:  JSR AddPPUBufEntry
        case 0xC550: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC76B:  LDA PPUAddrLB
        case 0xC553: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC76D:  CLC
        case 0xC554: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LC76E:  ADC #$1E
        case 0xC556: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC770:  STA PPUAddrLB
        case 0xC559: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LC772:  BCC +
        case 0xC55B: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LC774:  INC PPUAddrUB
        case 0xC55E: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LC778:  LDA (BlockAddr),Y
        case 0xC560: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC77A:  STA PPUDataByte
        case 0xC563: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC77C:  JSR AddPPUBufEntry
        case 0xC566: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LC77F:  INY
        case 0xC567: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LC780:  LDA (BlockAddr),Y
        case 0xC569: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC782:  STA PPUDataByte
        case 0xC56C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC784:  JSR AddPPUBufEntry
        case 0xC56F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC787:  LDA XFromLeftTemp
        case 0xC572: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC789:  STA XPosFromLeft
        case 0xC575: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC78B:  LDA YFromTopTemp
        case 0xC578: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC78D:  STA YPosFromTop
        case 0xC57B: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LC78F:  LDY #$00
        case 0xC57D: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LC791:  LDA (BlockAddr),Y
        case 0xC57F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LC793:  CMP #$C1
        case 0xC581: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LC795:  BCS +
        case 0xC583: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC797:  LDA #$00
        case 0xC585: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LC799:  BEQ SetAttribVals
        case 0xC587: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LC79D:  BCS +
        case 0xC589: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC79F:  LDA #$01
        case 0xC58B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC7A1:  BNE SetAttribVals
        case 0xC58D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LC7A5:  BCS +
        case 0xC58F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC7A7:  LDA #$02
        case 0xC591: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LC7A9:  BNE SetAttribVals
        case 0xC593: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC7AD:  STA PPUDataByte
        case 0xC596: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC7AF:  JSR ModAttribBits
        case 0xC599: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC7B2:  LDA PPUAddrUB
        case 0xC59C: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC7B4:  CLC
        case 0xC59D: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LC7B5:  ADC #$20
        case 0xC59F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC7B7:  STA PPUAddrUB
        case 0xC5A2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC7B9:  JSR AddPPUBufEntry
        case 0xC5A5: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC7BC:  RTS
        case 0xC5A6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC7BD:  STA GenByte3C
        case 0xC5A9: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC7BF:  LDA #$00
        case 0xC5AB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC7C1:  STA GenByte3D
        case 0xC5AE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LC7C3:  BEQ SetDialogBytes
        case 0xC5B0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC7C5:  LDA #$01
        case 0xC5B2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC7C7:  STA GenByte3D
        case 0xC5B5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LC7C9:  BNE +
        case 0xC5B7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC7CB:  LDA #$00
        case 0xC5B9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC7CD:  STA GenByte3D
        case 0xC5BC: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LC7D0:  CLC
        case 0xC5BD: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LC7D1:  ADC #$01
        case 0xC5BF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC7D3:  STA GenPtr3ELB
        case 0xC5C2: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LC7D5:  PLA
        case 0xC5C3: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LC7D6:  ADC #$00
        case 0xC5C5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC7D8:  STA GenPtr3EUB
        case 0xC5C8: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LC7DA:  PHA
        case 0xC5C9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC7DB:  LDA GenPtr3ELB
        case 0xC5CC: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LC7DD:  PHA
        case 0xC5CD: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LC7DE:  LDY #$00
        case 0xC5CF: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LC7E0:  LDA (GenPtr3E),Y
        case 0xC5D1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC7E2:  STA GenByte3C
        case 0xC5D4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LC7E4:  LDA GenByte3C
        case 0xC5D7: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LC7E6:  LDX GenByte3D
        case 0xC5DA: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LC7E8:  BRK
        case 0xC5DB: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LC7EB:  RTS
        case 0xC5DC: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC9B5:  LDA #$00
        case 0xC5DE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LC9B7:  JSR SetPRGBankAndSwitch
        case 0xC5E1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC9BA:  LDA #$00
        case 0xC5E3: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LC9BC:  TAX
        case 0xC5E4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC9BD:  STA DrgnLrdPal
        case 0xC5E7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC9C0:  STA CharDirection
        case 0xC5EA: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LC9C6:  INX
        case 0xC5EB: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LC9C7:  CPX #$10
        case 0xC5ED: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LC9C9:  BCC -
        case 0xC5EF: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LC9CB:  BRK
        case 0xC5F0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LC9CE:  JSR Bank0ToNT0
        case 0xC5F3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LC9D1:  JSR Bank3ToNT1
        case 0xC5F6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC9D4:  LDA #$FF
        case 0xC5F8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC9D6:  STA HitPoints
        case 0xC5FB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC9D8:  LDA #$08
        case 0xC5FD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC9DA:  STA NTBlockX
        case 0xC600: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC9DC:  LDA #$07
        case 0xC602: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC9DE:  STA NTBlockY
        case 0xC605: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LC9E0:  JSR DoIntroGFX
        case 0xC608: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC9E3:  LDA #$01
        case 0xC60A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LC9E5:  JSR WaitForPPUBufSpace
        case 0xC60D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC9E8:  LDA #%00011000
        case 0xC60F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC9EA:  STA PPUControl1
        case 0xC612: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC9ED:  LDA #$00
        case 0xC614: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC9EF:  STA SndEngineStat
        case 0xC617: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LC9F2:  BRK
        case 0xC618: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LC9F5:  LDA #%00000000
        case 0xC61A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LC9F7:  STA PPUControl1
        case 0xC61D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LC9FA:  JSR WaitForNMI
        case 0xC620: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LC9FD:  JSR Bank1ToNT0
        case 0xC623: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCA00:  JSR Bank2ToNT1
        case 0xC626: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCA03:  LDA #MSC_VILLAGE
        case 0xC628: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LCA05:  BRK
        case 0xC629: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCA08:  JSR LoadSaveMenus
        case 0xC62C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCA0B:  LDA #$FA
        case 0xC62E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCA0D:  STA GameStarted
        case 0xC631: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCA0F:  LDA #$00
        case 0xC633: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LCA11:  TAX
        case 0xC634: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LCA15:  INX
        case 0xC635: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LCA16:  CPX #$20
        case 0xC637: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LCA18:  BNE -
        case 0xC639: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCA1A:  JSR StartAtThroneRoom
        case 0xC63C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCA1D:  LDA PlayerFlags
        case 0xC63F: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCA1F:  AND #F_LEFT_THROOM
        case 0xC641: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCA21:  BEQ FirstKingDialog
        case 0xC643: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCA23:  JSR DoDialogHiBlock
        case 0xC646: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCA27:  LDA DisplayedLevel
        case 0xC649: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCA29:  CMP #LVL_30
        case 0xC64B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCA2B:  BNE KingExpCalc
        case 0xC64D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCA2D:  JSR DoDialogLoBlock
        case 0xC650: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCA31:  JMP EndKingDialog
        case 0xC653: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCA34:  JSR GetExpRemaining
        case 0xC656: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCA37:  JSR DoDialogLoBlock
        case 0xC659: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCA3B:  JSR DoDialogHiBlock
        case 0xC65C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCA3F:  JSR DoDialogLoBlock
        case 0xC65F: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCA43:  JMP PlayerInitControl
        case 0xC662: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCA46:  JSR DoDialogHiBlock
        case 0xC665: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCA4A:  JSR WaitForBtnRelease
        case 0xC668: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCA4D:  LDA #WND_DIALOG
        case 0xC66A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCA4F:  JSR RemoveWindow
        case 0xC66D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCA52:  LDA #NPC_MOVE
        case 0xC66F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCA54:  STA StopNPCMove
        case 0xC672: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCA56:  LDA RadiantTimer
        case 0xC675: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCA58:  BEQ CheckRepelTimer
        case 0xC677: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LCA5A:  DEC RadiantTimer
        case 0xC67A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCA5C:  BNE CheckRepelTimer
        case 0xC67C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCA5E:  LDA LightDiameter
        case 0xC67F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCA60:  CMP #$01
        case 0xC681: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCA62:  BEQ CheckRepelTimer
        case 0xC683: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCA64:  LDA #$3C
        case 0xC685: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCA66:  STA RadiantTimer
        case 0xC688: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LCA68:  DEC LightDiameter
        case 0xC68B: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LCA6A:  DEC LightDiameter
        case 0xC68E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCA6C:  LDA RepelTimer
        case 0xC691: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCA6E:  BEQ JoypadCheckLoop
        case 0xC693: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LCA70:  DEC RepelTimer
        case 0xC696: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LCA72:  DEC RepelTimer
        case 0xC699: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCA74:  BEQ EndRepelTimer
        case 0xC69B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCA76:  LDA RepelTimer
        case 0xC69E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCA78:  CMP #$01
        case 0xC6A0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCA7A:  BNE JoypadCheckLoop
        case 0xC6A2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCA7C:  JSR WaitForNMI
        case 0xC6A5: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCA7F:  LDA #NPC_STOP
        case 0xC6A7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCA81:  STA StopNPCMove
        case 0xC6AA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCA83:  JSR Dowindow
        case 0xC6AD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCA87:  LDA RepelTimer
        case 0xC6B0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCA89:  BNE RepelEndMsg
        case 0xC6B2: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCA8B:  LDA #$37
        case 0xC6B4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LCA8D:  BNE +
        case 0xC6B6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCA8F:  LDA #$34
        case 0xC6B8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCA94:  JSR ResumeGamePlay
        case 0xC6BB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCA97:  LDA #$00
        case 0xC6BD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCA99:  STA RepelTimer
        case 0xC6C0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCA9B:  LDA #$00
        case 0xC6C2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCA9D:  STA FrameCounter
        case 0xC6C5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCA9F:  JSR GetJoypadStatus
        case 0xC6C8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCAA2:  LDA JoypadBtns
        case 0xC6CB: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCAA4:  AND #IN_START
        case 0xC6CD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCAA6:  BEQ CheckJoyA
        case 0xC6CF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCAA8:  JSR WaitForNMI
        case 0xC6D2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCAAB:  LDA FrameCounter
        case 0xC6D5: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCAAD:  AND #$0F
        case 0xC6D7: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCAAF:  CMP #$01
        case 0xC6D9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCAB1:  BEQ GamePaused
        case 0xC6DB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCAB3:  JSR DoSprites
        case 0xC6DE: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCAB6:  JMP PausePrepLoop
        case 0xC6E1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCAB9:  JSR GetJoypadStatus
        case 0xC6E4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCABC:  LDA JoypadBtns
        case 0xC6E7: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCABE:  AND #IN_START
        case 0xC6E9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCAC0:  BNE GamePaused
        case 0xC6EB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCAC5:  LDA JoypadBtns
        case 0xC6EE: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCAC7:  AND #IN_START
        case 0xC6F0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LCAC9:  BEQ -
        case 0xC6F2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCACE:  LDA JoypadBtns
        case 0xC6F5: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCAD0:  AND #IN_START
        case 0xC6F7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LCAD2:  BNE -
        case 0xC6F9: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCAD4:  JMP JoypadCheckLoop
        case 0xC6FC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCAD7:  LDA JoypadBtns
        case 0xC6FF: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LCAD9:  LSR
        case 0xC700: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07B; return; } // LCADA:  BCC CheckJoyUp
        case 0xC702: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCADC:  JSR DoNCCmdWindow
        case 0xC705: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCADF:  JMP JoypadCheckLoop
        case 0xC708: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCAE2:  LDA JoypadBtns
        case 0xC70B: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCAE4:  AND #IN_UP
        case 0xC70D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCAE6:  BEQ CheckJoyDown
        case 0xC70F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCAE8:  LDA #DIR_UP
        case 0xC711: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCAEA:  STA CharDirection
        case 0xC714: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCAED:  JSR DoJoyUp
        case 0xC717: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCAF0:  JSR ChkSpecialLoc
        case 0xC71A: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCAF3:  JMP GameEngineLoop
        case 0xC71D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCAF6:  LDA JoypadBtns
        case 0xC720: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCAF8:  AND #IN_DOWN
        case 0xC722: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCAFA:  BEQ CheckJoyLeft
        case 0xC724: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCAFC:  LDA #DIR_DOWN
        case 0xC726: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCAFE:  STA CharDirection
        case 0xC729: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCB01:  JSR DoJoyDown
        case 0xC72C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCB04:  JSR ChkSpecialLoc
        case 0xC72F: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCB07:  JMP GameEngineLoop
        case 0xC732: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCB0A:  LDA JoypadBtns
        case 0xC735: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCB0C:  AND #IN_LEFT
        case 0xC737: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCB0E:  BEQ CheckJoyRight
        case 0xC739: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCB10:  LDA #DIR_LEFT
        case 0xC73B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCB12:  STA CharDirection
        case 0xC73E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCB15:  JSR DoJoyLeft
        case 0xC741: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCB18:  JSR ChkSpecialLoc
        case 0xC744: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCB1B:  JMP GameEngineLoop
        case 0xC747: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCB1E:  LDA JoypadBtns
        case 0xC74A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0xC07B; return; } // LCB20:  BPL IdleUpdate
        case 0xC74C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCB22:  LDA #DIR_RIGHT
        case 0xC74E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCB24:  STA CharDirection
        case 0xC751: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCB27:  JSR DoJoyRight
        case 0xC754: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCB2A:  JSR ChkSpecialLoc
        case 0xC757: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCB2D:  JMP GameEngineLoop
        case 0xC75A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCB30:  JSR WaitForNMI
        case 0xC75D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCB33:  LDA FrameCounter
        case 0xC760: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCB35:  CMP #$31
        case 0xC762: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCB37:  BNE EngineLoopEnd
        case 0xC764: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCB39:  JSR Dowindow
        case 0xC767: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCB3D:  LDA #$32
        case 0xC769: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCB3F:  STA FrameCounter
        case 0xC76C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCB41:  JSR DoSprites
        case 0xC76F: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCB44:  JMP CheckInputs
        case 0xC772: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCB47:  JSR WaitForNMI
        case 0xC775: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCB4A:  LDA BlackPalPtr
        case 0xC778: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCB4D:  STA PalPtrLB
        case 0xC77B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCB4F:  LDA BlackPalPtr+1
        case 0xC77E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCB52:  STA PalPtrUB
        case 0xC781: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCB54:  LDA #$00
        case 0xC783: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCB56:  STA PalModByte
        case 0xC786: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCB58:  JSR PrepSPPalLoad
        case 0xC789: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCB5B:  LDA #$30
        case 0xC78B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCB5D:  STA PalModByte
        case 0xC78E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCB5F:  JSR PrepBGPalLoad
        case 0xC791: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCB62:  JSR WaitForNMI
        case 0xC794: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCB65:  JSR LoadStats
        case 0xC797: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCB68:  JSR Bank1ToNT0
        case 0xC79A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCB6B:  LDA ModsnSpells
        case 0xC79D: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCB6D:  AND #IS_CURSED
        case 0xC79F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LCB6F:  BEQ +
        case 0xC7A1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCB71:  LDA #$01
        case 0xC7A3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCB73:  STA HitPoints
        case 0xC7A6: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCB75:  JMP SetStartPos
        case 0xC7A9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCB7B:  CMP #STRT_FULL_HP
        case 0xC7AB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCB7D:  BNE SetStartPos
        case 0xC7AD: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LCB7F:  TXA
        case 0xC7AE: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LCB80:  PHA
        case 0xC7AF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCB81:  LDA DisplayedMaxHP
        case 0xC7B2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCB83:  STA HitPoints
        case 0xC7B5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCB85:  LDA DisplayedMaxMP
        case 0xC7B8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCB87:  STA MagicPoints
        case 0xC7BB: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LCB89:  LDX SaveNumber
        case 0xC7BE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCB8C:  LDA #STRT_NO_HP
        case 0xC7C0: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LCB8E:  STA StartStatus1,X
        case 0xC7C3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCB91:  STA ThisStrtStat
        case 0xC7C6: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LCB94:  PLA
        case 0xC7C7: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LCB95:  TAX
        case 0xC7C8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCB96:  LDA #$03
        case 0xC7CA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCB98:  STA CharXPos
        case 0xC7CD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCB9A:  STA _CharXPos
        case 0xC7D0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCB9C:  LDA #$04
        case 0xC7D2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCB9E:  STA CharYPos
        case 0xC7D5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCBA0:  STA _CharYPos
        case 0xC7D8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCBA2:  LDA #$30
        case 0xC7DA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCBA4:  STA CharXPixelsLB
        case 0xC7DD: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCBA6:  LDA #$40
        case 0xC7DF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCBA8:  STA CharYPixelsLB
        case 0xC7E2: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCBAA:  LDA #$00
        case 0xC7E4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCBAC:  STA RepeatCounter
        case 0xC7E7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCBAE:  STA RepelTimer
        case 0xC7EA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCBB0:  STA CharXPixelsUB
        case 0xC7ED: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCBB2:  STA CharYPixelsUB
        case 0xC7F0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCBB4:  LDA #$08
        case 0xC7F2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCBB6:  STA NTBlockX
        case 0xC7F5: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCBB8:  LDA #$07
        case 0xC7F7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCBBA:  STA NTBlockY
        case 0xC7FA: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCBBC:  LDA #MAP_THRONEROOM
        case 0xC7FC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCBBE:  STA MapNumber
        case 0xC7FF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCBC0:  JSR ClearWinBufRAM2
        case 0xC802: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCBC3:  JSR MapChngNoSound
        case 0xC805: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCBC6:  JSR WaitForNMI
        case 0xC808: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCBC9:  LDA #$00
        case 0xC80A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCBCB:  STA FrameCounter
        case 0xC80D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCBCD:  JSR GetJoypadStatus
        case 0xC810: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCBD0:  LDA JoypadBtns
        case 0xC813: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCBD2:  BNE FrameSyncLoop
        case 0xC815: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCBD4:  JSR WaitForNMI
        case 0xC818: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCBD7:  JSR DoSprites
        case 0xC81B: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCBDA:  JMP FirstInputLoop
        case 0xC81E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCBDD:  JSR WaitForNMI
        case 0xC821: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCBE0:  LDA FrameCounter
        case 0xC824: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCBE2:  AND #$0F
        case 0xC826: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCBE4:  CMP #$01
        case 0xC828: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LCBE6:  BEQ +
        case 0xC82A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCBE8:  JSR DoSprites
        case 0xC82D: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCBEB:  JMP FrameSyncLoop
        case 0xC830: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCBF0:  STA StopNPCMove
        case 0xC833: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCBF2:  JSR Dowindow
        case 0xC836: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LCBF6:  RTS
        case 0xC837: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCBF7:  LDA StoryFlags
        case 0xC83A: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCBF9:  AND #F_DGNLRD_DEAD
        case 0xC83C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCBFB:  BNE ChkCstlEnd
        case 0xC83E: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCBFD:  JMP MovementUpdates
        case 0xC841: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCC00:  LDA MapNumber
        case 0xC844: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCC02:  CMP #MAP_TANTCSTL_GF
        case 0xC846: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LCC04:  BNE +
        case 0xC848: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCC06:  LDA CharYPos
        case 0xC84B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCC08:  CMP #$08
        case 0xC84D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LCC0A:  BNE +
        case 0xC84F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCC0C:  LDA CharXPos
        case 0xC852: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCC0E:  CMP #$0A
        case 0xC854: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCC10:  BEQ EndGameTriggered
        case 0xC856: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCC12:  CMP #$0B
        case 0xC858: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCC14:  BEQ EndGameTriggered
        case 0xC85A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCC19:  LDA #MSC_NOSOUND
        case 0xC85C: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LCC1B:  BRK
        case 0xC85D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCC1E:  LDA #NPC_STOP
        case 0xC85F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCC20:  STA StopNPCMove
        case 0xC862: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCC22:  JSR Dowindow
        case 0xC865: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCC26:  JSR DoDialogHiBlock
        case 0xC868: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCC2A:  LDA PlayerFlags
        case 0xC86B: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCC2C:  AND #F_RTN_GWAELIN
        case 0xC86D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCC2E:  BEQ ChkCarryGwaelin
        case 0xC86F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCC30:  LDA #$C7
        case 0xC871: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCC32:  STA GwaelinXPos
        case 0xC874: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCC34:  LDA #$27
        case 0xC876: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCC36:  STA GwaelinYPos
        case 0xC879: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCC38:  LDA #$00
        case 0xC87B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCC3A:  STA GwaelinOffset
        case 0xC87E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCC3C:  JSR WaitForNMI
        case 0xC881: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCC3F:  JSR DoSprites
        case 0xC884: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCC42:  JSR DoDialogHiBlock
        case 0xC887: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCC46:  JSR WaitForNMI
        case 0xC88A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCC49:  JSR WaitForNMI
        case 0xC88D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCC4C:  LDA GwaelinOffset
        case 0xC890: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LCC4E:  CLC
        case 0xC891: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LCC4F:  ADC #$10
        case 0xC893: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCC51:  STA GwaelinOffset
        case 0xC896: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LCC53:  BCC +
        case 0xC898: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LCC55:  INC GwaelinXPos
        case 0xC89B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCC5A:  LDA GwaelinXPos
        case 0xC89E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCC5C:  CMP #$CA
        case 0xC8A0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCC5E:  BNE GwaelinMoveLoop
        case 0xC8A2: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCC60:  LDA #$47
        case 0xC8A4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCC62:  STA GwaelinYPos
        case 0xC8A7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCC64:  JSR DoSprites
        case 0xC8AA: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCC67:  JMP GwaelinJoin
        case 0xC8AD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCC6A:  LDA PlayerFlags
        case 0xC8B0: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LCC6C:  LSR
        case 0xC8B1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07B; return; } // LCC6D:  BCC TaleEnd
        case 0xC8B3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCC6F:  LDA PlayerFlags
        case 0xC8B6: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCC71:  AND #$FE
        case 0xC8B8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCC73:  STA PlayerFlags
        case 0xC8BB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCC75:  LDA #$CA
        case 0xC8BD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCC77:  STA GwaelinXPos
        case 0xC8C0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCC79:  LDA #$47
        case 0xC8C2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCC7B:  STA GwaelinYPos
        case 0xC8C5: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCC7D:  LDA #$00
        case 0xC8C7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCC7F:  STA GwaelinOffset
        case 0xC8CA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCC81:  JSR WaitForNMI
        case 0xC8CD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCC84:  JSR DoSprites
        case 0xC8D0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCC87:  JSR DoDialogHiBlock
        case 0xC8D3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCC8B:  JSR DoDialogHiBlock
        case 0xC8D6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCC8F:  JSR DoDialogHiBlock
        case 0xC8D9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCC93:  JSR Dowindow
        case 0xC8DC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCC97:  BEQ GwaelinAccept
        case 0xC8DE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCC99:  JSR DoDialogLoBlock
        case 0xC8E1: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCC9D:  JMP GwaelinDecline
        case 0xC8E4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCCA0:  JSR DoDialogLoBlock
        case 0xC8E7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCCA4:  LDA #$00
        case 0xC8E9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCCA6:  STA GwaelinXPos
        case 0xC8EC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCCA8:  STA GwaelinYPos
        case 0xC8EF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCCAA:  STA GwaelinOffset
        case 0xC8F2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCCAC:  LDA PlayerFlags
        case 0xC8F5: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCCAE:  ORA #F_GOT_GWAELIN
        case 0xC8F7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCCB0:  STA PlayerFlags
        case 0xC8FA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCCB2:  JSR WaitForNMI
        case 0xC8FD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCCB5:  JSR DoSprites
        case 0xC900: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCCB8:  JSR DoDialogHiBlock
        case 0xC903: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LCCBC:  LDX #$78
        case 0xC905: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LCCC1:  DEX
        case 0xC906: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LCCC2:  BNE -
        case 0xC908: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCCC4:  LDA #WND_DIALOG
        case 0xC90A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCCC6:  JSR RemoveWindow
        case 0xC90D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCCC9:  LDA #$01
        case 0xC90F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCCCB:  STA CharDirection
        case 0xC912: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCCCE:  JSR DoSprites
        case 0xC915: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCCD1:  LDA #$1E
        case 0xC917: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCCD3:  JSR WaitMultiNMIs
        case 0xC91A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCCD6:  LDA #$02
        case 0xC91C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCCD8:  STA CharDirection
        case 0xC91F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCCDB:  JSR DoSprites
        case 0xC922: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LCCDE:  LDX #$1E
        case 0xC924: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LCCE3:  DEX
        case 0xC925: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LCCE4:  BNE -
        case 0xC927: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCCE6:  LDA #$FF
        case 0xC929: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCCE8:  STA DisplayedLevel
        case 0xC92C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCCEA:  JSR DoSprites
        case 0xC92F: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LCCED:  BRK
        case 0xC930: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCCF0:  JSR MMCShutdown
        case 0xC933: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCCF3:  JMP Spinlock1
        case 0xC936: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCCF6:  LDA EqippedItems
        case 0xC939: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCCF8:  AND #AR_ARMOR
        case 0xC93B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCCFA:  CMP #AR_ERDK_ARMR
        case 0xC93D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCCFC:  BEQ MovmtIncHP
        case 0xC93F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCCFE:  CMP #AR_MAGIC_ARMR
        case 0xC941: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCD00:  BNE CheckTantCursed
        case 0xC943: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LCD02:  INC MjArmrHP
        case 0xC946: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCD04:  LDA MjArmrHP
        case 0xC949: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCD06:  AND #$03
        case 0xC94B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCD08:  BNE CheckTantCursed
        case 0xC94D: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LCD0A:  INC HitPoints
        case 0xC950: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCD0C:  LDA HitPoints
        case 0xC953: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LCD0E:  CMP DisplayedMaxHP
        case 0xC956: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07B; return; } // LCD10:  BCC ChkLowHP
        case 0xC958: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCD12:  LDA DisplayedMaxHP
        case 0xC95B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCD14:  STA HitPoints
        case 0xC95E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCD16:  LDA DisplayedMaxHP
        case 0xC961: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LCD18:  LSR
        case 0xC962: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LCD19:  LSR
        case 0xC963: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LCD1A:  CLC
        case 0xC964: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LCD1B:  ADC #$01
        case 0xC966: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LCD1D:  CMP HitPoints
        case 0xC969: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07B; return; } // LCD1F:  BCS CheckTantCursed
        case 0xC96B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCD21:  LDA #$01
        case 0xC96D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCD23:  STA PPUAddrLB
        case 0xC970: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCD25:  LDA #$3F
        case 0xC972: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCD27:  STA PPUAddrUB
        case 0xC975: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCD29:  LDA #$30
        case 0xC977: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCD2B:  STA PPUDataByte
        case 0xC97A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCD2D:  JSR AddPPUBufEntry
        case 0xC97D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCD30:  LDA MapNumber
        case 0xC980: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCD32:  CMP #MAP_TANTCSTL_GF
        case 0xC982: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCD34:  BNE CheckAxeKnight
        case 0xC984: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCD36:  LDA ModsnSpells
        case 0xC987: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCD38:  AND #$C0
        case 0xC989: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCD3A:  BEQ CheckAxeKnight
        case 0xC98B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCD3C:  LDA CharYPos
        case 0xC98E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCD3E:  CMP #$1B
        case 0xC990: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCD40:  BNE CheckAxeKnight
        case 0xC992: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCD42:  LDA #NPC_STOP
        case 0xC994: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCD44:  STA StopNPCMove
        case 0xC997: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCD46:  JSR Dowindow
        case 0xC99A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCD4A:  JSR DoDialogLoBlock
        case 0xC99D: cycles_to_run -= 3; reg_PC = 0x0000; return; // LCD4E:  JMP CheckMapExit
        case 0xC9A0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCD51:  LDA MapNumber
        case 0xC9A3: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCD53:  CMP #MAP_HAUKSNESS
        case 0xC9A5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCD55:  BNE CheckGrnDragon
        case 0xC9A7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCD57:  LDA CharXPos
        case 0xC9AA: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCD59:  CMP #$12
        case 0xC9AC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCD5B:  BNE CheckGrnDragon
        case 0xC9AE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCD5D:  LDA CharYPos
        case 0xC9B1: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCD5F:  CMP #$0C
        case 0xC9B3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCD61:  BNE CheckGrnDragon
        case 0xC9B5: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCD63:  LDA #EN_AXEKNIGHT
        case 0xC9B7: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LCD65:  JMP InitFight
        case 0xC9BA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCD68:  LDA MapNumber
        case 0xC9BD: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCD6A:  CMP #MAP_SWAMPCAVE
        case 0xC9BF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCD6C:  BNE CheckGolem
        case 0xC9C1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCD6E:  LDA CharXPos
        case 0xC9C4: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCD70:  CMP #$04
        case 0xC9C6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCD72:  BNE CheckGolem
        case 0xC9C8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCD74:  LDA CharYPos
        case 0xC9CB: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCD76:  CMP #$0E
        case 0xC9CD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCD78:  BNE CheckGolem
        case 0xC9CF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCD7A:  LDA StoryFlags
        case 0xC9D2: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCD7C:  AND #F_GDRG_DEAD
        case 0xC9D4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCD7E:  BNE CheckGolem
        case 0xC9D6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCD80:  LDA #EN_GDRAGON
        case 0xC9D8: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LCD82:  JMP InitFight
        case 0xC9DB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCD85:  LDA MapNumber
        case 0xC9DE: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCD87:  CMP #MAP_OVERWORLD
        case 0xC9E0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCD89:  BNE CheckBlockDmg
        case 0xC9E2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCD8B:  LDA CharXPos
        case 0xC9E5: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCD8D:  CMP #$49
        case 0xC9E7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCD8F:  BNE CheckBlockDmg
        case 0xC9E9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCD91:  LDA CharYPos
        case 0xC9EC: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCD93:  CMP #$64
        case 0xC9EE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCD95:  BNE CheckBlockDmg
        case 0xC9F0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCD97:  LDA StoryFlags
        case 0xC9F3: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCD99:  AND #F_GOLEM_DEAD
        case 0xC9F5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCD9B:  BNE CheckBlockDmg
        case 0xC9F7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCD9D:  LDA #EN_GOLEM
        case 0xC9F9: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LCD9F:  JMP InitFight
        case 0xC9FC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCDA2:  JSR UpdateRandNum
        case 0xC9FF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCDA5:  LDA CharXPos
        case 0xCA02: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCDA7:  STA XTarget
        case 0xCA05: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCDA9:  LDA CharYPos
        case 0xCA08: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCDAB:  STA YTarget
        case 0xCA0B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCDAD:  JSR GetBlockID
        case 0xCA0E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCDB0:  LDA TargetResults
        case 0xCA11: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCDB2:  STA ThisTile
        case 0xCA14: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCDB4:  CMP #BLK_TOWN
        case 0xCA16: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07B; return; } // LCDB6:  BCC ChkOtherBlocks
        case 0xCA18: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCDB8:  CMP #BLK_BRIDGE
        case 0xCA1A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07B; return; } // LCDBA:  BCS ChkOtherBlocks
        case 0xCA1C: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LCDBC:  JMP CalcNextMap
        case 0xCA1F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCDBF:  LDA StoryFlags
        case 0xCA22: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCDC1:  AND #F_DGNLRD_DEAD
        case 0xCA24: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCDC3:  BEQ NextBlockCheck
        case 0xCA26: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LCDC5:  RTS
        case 0xCA27: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCDC6:  LDA ThisTile
        case 0xCA2A: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCDC8:  CMP #BLK_SWAMP
        case 0xCA2C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCDCA:  BNE ChkBlkSand
        case 0xCA2E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCDCC:  LDA EqippedItems
        case 0xCA31: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCDCE:  AND #AR_ARMOR
        case 0xCA33: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCDD0:  CMP #AR_ERDK_ARMR
        case 0xCA35: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCDD2:  BEQ ChkFight
        case 0xCA37: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCDD4:  LDA #SFX_SWMP_DMG
        case 0xCA39: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LCDD6:  BRK
        case 0xCA3A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCDD9:  JSR RedFlashScreen
        case 0xCA3D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCDDC:  JSR WaitForNMI
        case 0xCA40: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCDDF:  LDA HitPoints
        case 0xCA43: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LCDE1:  SEC
        case 0xCA44: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LCDE2:  SBC #$02
        case 0xCA46: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07B; return; } // LCDE4:  BCS DoSwampDamage
        case 0xCA48: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCDE6:  LDA #$00
        case 0xCA4A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCDE8:  STA HitPoints
        case 0xCA4D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCDEA:  JSR WaitForNMI
        case 0xCA50: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCDED:  JSR LoadRegBGPal
        case 0xCA53: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCDF0:  LDA HitPoints
        case 0xCA56: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCDF2:  BNE ChkFight
        case 0xCA58: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCDF4:  JSR Dowindow
        case 0xCA5B: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LCDF8:  JMP InitDeathSequence
        case 0xCA5E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCDFB:  LDA #$0F
        case 0xCA60: cycles_to_run -= 2; reg_A &= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCDFD:  AND RandNumUB
        case 0xCA63: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCDFF:  BEQ DoRandomFight
        case 0xCA65: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LCE01:  RTS
        case 0xCA66: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCE02:  CMP #BLK_SAND
        case 0xCA68: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCE04:  BEQ ChkSandFight
        case 0xCA6A: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCE06:  CMP #BLK_HILL
        case 0xCA6C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCE08:  BNE ChkBlkTrees
        case 0xCA6E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCE0A:  JSR WaitForNMI
        case 0xCA71: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCE0D:  JSR WaitForNMI
        case 0xCA74: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCE10:  JSR WaitForNMI
        case 0xCA77: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCE13:  LDA #$07
        case 0xCA79: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCE15:  BNE ChkFight2
        case 0xCA7B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCE17:  CMP #BLK_TREES
        case 0xCA7D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCE19:  BEQ ChkRandomFight
        case 0xCA7F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCE1B:  CMP #BLK_BRICK
        case 0xCA81: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCE1D:  BEQ ChkRandomFight
        case 0xCA83: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCE1F:  CMP #BLK_FFIELD
        case 0xCA85: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCE21:  BNE ChkFight6
        case 0xCA87: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCE23:  LDA EqippedItems
        case 0xCA8A: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCE25:  AND #AR_ARMOR
        case 0xCA8C: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCE27:  CMP #AR_ERDK_ARMR
        case 0xCA8E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCE29:  BEQ ChkRandomFight
        case 0xCA90: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCE2B:  LDA #SFX_FFDAMAGE
        case 0xCA92: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LCE2D:  BRK
        case 0xCA93: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCE30:  LDA #$03
        case 0xCA95: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCE32:  STA GenByte42
        case 0xCA98: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCE37:  JSR RedFlashScreen
        case 0xCA9B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCE3A:  JSR WaitForNMI
        case 0xCA9E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCE3D:  JSR LoadRegBGPal
        case 0xCAA1: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LCE40:  DEC GenByte42
        case 0xCAA4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LCE42:  BNE -
        case 0xCAA6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCE44:  LDA HitPoints
        case 0xCAA9: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LCE46:  SEC
        case 0xCAAA: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LCE47:  SBC #$0F
        case 0xCAAC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LCE49:  BCS +
        case 0xCAAE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCE4B:  LDA #$00
        case 0xCAB0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LCE4D:  BEQ +
        case 0xCAB2: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCE51:  CMP #$00
        case 0xCAB4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCE53:  BNE ChkRandomFight
        case 0xCAB6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCE55:  JSR LoadRegBGPal
        case 0xCAB9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCE58:  JSR Dowindow
        case 0xCABC: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LCE5C:  JMP InitDeathSequence
        case 0xCABF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCE5F:  LDA #$0F
        case 0xCAC1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCE61:  BNE ChkFight2
        case 0xCAC3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCE63:  LDA CharXPos
        case 0xCAC6: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LCE65:  LSR
        case 0xCAC7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07B; return; } // LCE66:  BCS ChkFight5
        case 0xCAC9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCE68:  LDA CharYPos
        case 0xCACC: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LCE6A:  LSR
        case 0xCACD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07B; return; } // LCE6B:  BCC HighFightChance
        case 0xCACF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07B; return; } // LCE6D:  BCS NormFightChance
        case 0xCAD1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCE6F:  LDA CharYPos
        case 0xCAD4: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LCE71:  LSR
        case 0xCAD5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07B; return; } // LCE72:  BCC NormFightChance
        case 0xCAD7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCE74:  LDA #$1F
        case 0xCAD9: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCE78:  LDA #$0F
        case 0xCADB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LCE7A:  BNE -
        case 0xCADD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCE7C:  LDA MapNumber
        case 0xCAE0: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCE7E:  CMP #MAP_OVERWORLD
        case 0xCAE2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCE80:  BNE ChkDungeonFights
        case 0xCAE4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCE82:  LDA CharYPos
        case 0xCAE7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCE84:  STA DivNum1LB
        case 0xCAEA: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCE86:  LDA #$0F
        case 0xCAEC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCE88:  STA DivNum2
        case 0xCAEF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCE8A:  JSR ByteDivide
        case 0xCAF2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCE8D:  LDA DivNum1LB
        case 0xCAF5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCE8F:  STA GenByte42
        case 0xCAF8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCE91:  LDA CharXPos
        case 0xCAFB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCE93:  STA DivNum1LB
        case 0xCAFE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCE95:  LDA #$0F
        case 0xCB00: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCE97:  STA DivNum2
        case 0xCB03: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCE99:  JSR ByteDivide
        case 0xCB06: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCE9C:  LDA GenByte42
        case 0xCB09: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LCE9E:  ASL
        case 0xCB0A: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LCE9F:  ASL
        case 0xCB0B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCEA0:  STA EnemyOffset
        case 0xCB0E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCEA2:  LDA DivNum1LB
        case 0xCB11: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LCEA4:  LSR
        case 0xCB12: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LCEA5:  CLC
        case 0xCB13: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LCEA6:  ADC EnemyOffset
        case 0xCB16: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LCEA8:  TAX
        case 0xCB17: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0xC07E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LCEA9:  LDA OvrWrldEnGrid,X
        case 0xCB1A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCEAC:  STA EnemyOffset
        case 0xCB1D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCEAE:  LDA DivNum1LB
        case 0xCB20: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LCEB0:  LSR
        case 0xCB21: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LCEB1:  BCS +
        case 0xCB23: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LCEB3:  LSR EnemyOffset
        case 0xCB26: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LCEB5:  LSR EnemyOffset
        case 0xCB29: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LCEB7:  LSR EnemyOffset
        case 0xCB2C: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LCEB9:  LSR EnemyOffset
        case 0xCB2F: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCEBD:  AND #$0F
        case 0xCB31: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCEBF:  BNE GetEnemyRow
        case 0xCB33: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCEC1:  JSR UpdateRandNum
        case 0xCB36: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCEC4:  LDA ThisTile
        case 0xCB39: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCEC6:  CMP #BLK_HILL
        case 0xCB3B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCEC8:  BNE NormFightModifier
        case 0xCB3D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCECA:  LDA RandNumUB
        case 0xCB40: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCECC:  AND #$03
        case 0xCB42: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCECE:  BEQ GetEnemyRow
        case 0xCB44: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LCED0:  RTS
        case 0xCB45: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCED1:  LDA RandNumUB
        case 0xCB48: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCED3:  AND #$01
        case 0xCB4A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCED5:  BEQ GetEnemyRow
        case 0xCB4C: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LCED7:  RTS
        case 0xCB4D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCED8:  CMP #MAP_DLCSTL_GF
        case 0xCB4F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCEDA:  BNE ChkHauksnessFight
        case 0xCB51: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCEDC:  LDA #$10
        case 0xCB53: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCEDE:  BNE GetEnemyRow
        case 0xCB55: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCEE0:  CMP #MAP_HAUKSNESS
        case 0xCB57: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCEE2:  BNE ChkDLCastleFight
        case 0xCB59: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCEE4:  LDA #$0D
        case 0xCB5B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCEE6:  BNE GetEnemyRow
        case 0xCB5D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCEE8:  CMP #MAP_DLCSTL_BF
        case 0xCB5F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCEEA:  BNE ChkErdrickFight
        case 0xCB61: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCEEC:  LDA #$12
        case 0xCB63: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCEEE:  BNE GetEnemyRow
        case 0xCB65: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCEF0:  CMP #MAP_ERDRCK_B1
        case 0xCB67: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07B; return; } // LCEF2:  BCS NoEnemyMap
        case 0xCB69: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCEF4:  LDA MapType
        case 0xCB6C: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCEF6:  CMP #MAP_DUNGEON
        case 0xCB6E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCEF8:  BEQ DoDungeonEnemy
        case 0xCB70: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LCEFA:  RTS
        case 0xCB71: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCEFB:  LDA MapNumber
        case 0xCB74: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LCEFD:  SEC
        case 0xCB75: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LCEFE:  SBC #$0F
        case 0xCB77: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LCF00:  TAX
        case 0xCB78: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0xC07E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LCF01:  LDA CaveEnIndexTbl,X
        case 0xCB7B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCF04:  STA EnemyOffset
        case 0xCB7E: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LCF06:  ASL
        case 0xCB7F: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LCF07:  ASL
        case 0xCB80: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LCF08:  CLC
        case 0xCB81: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LCF09:  ADC EnemyOffset
        case 0xCB84: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCF0B:  STA EnemyOffset
        case 0xCB87: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCF0D:  JSR UpdateRandNum
        case 0xCB8A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCF10:  LDA RandNumUB
        case 0xCB8D: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCF12:  AND #$07
        case 0xCB8F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCF14:  CMP #$05
        case 0xCB91: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07B; return; } // LCF16:  BCS GetEnemyInRow
        case 0xCB93: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LCF18:  ADC EnemyOffset
        case 0xCB96: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LCF1A:  TAX
        case 0xCB97: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0xC07E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LCF1B:  LDA EnemyGroupsTbl,X
        case 0xCB9A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCF1E:  STA _EnNumber
        case 0xCB9D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCF20:  LDA MapNumber
        case 0xCBA0: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCF22:  CMP #MAP_OVERWORLD
        case 0xCBA2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCF24:  BNE ReadyFight
        case 0xCBA4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCF26:  LDA RepelTimer
        case 0xCBA7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCF28:  BEQ ReadyFight
        case 0xCBA9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCF2A:  LDA DisplayedDefns
        case 0xCBAC: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LCF2C:  LSR
        case 0xCBAD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCF2D:  STA GenByte3E
        case 0xCBB0: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LCF2F:  LDX _EnNumber
        case 0xCBB3: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0xC07E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LCF31:  LDA RepelTbl,X
        case 0xCBB6: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LCF34:  SEC
        case 0xCBB7: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LCF35:  SBC GenByte3E
        case 0xCBBA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07B; return; } // LCF37:  BCC RepelSucceeded
        case 0xCBBC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCF39:  STA GenByte3E
        case 0xCBBF: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0xC07E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LCF3B:  LDA RepelTbl,X
        case 0xCBC2: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LCF3E:  LSR
        case 0xCBC3: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LCF3F:  CMP GenByte3E
        case 0xCBC6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07B; return; } // LCF41:  BCC ReadyFight
        case 0xCBC8: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LCF43:  RTS
        case 0xCBC9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCF44:  LDA _EnNumber
        case 0xCBCC: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LCF46:  JMP InitFight
        case 0xCBCF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCF49:  JSR WaitForNMI
        case 0xCBD2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCF4C:  LDA FrameCounter
        case 0xCBD5: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCF4E:  AND #$0F
        case 0xCBD7: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCF50:  CMP #$01
        case 0xCBD9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCF52:  BEQ ShowNCCmdWindow
        case 0xCBDB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCF54:  JSR DoSprites
        case 0xCBDE: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCF57:  JMP DoNCCmdWindow
        case 0xCBE1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCF5A:  LDA #NPC_STOP
        case 0xCBE3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCF5C:  STA StopNPCMove
        case 0xCBE6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCF5E:  JSR Dowindow
        case 0xCBE9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCF62:  JSR Dowindow
        case 0xCBEC: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCF66:  CMP #WND_ABORT
        case 0xCBEE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCF68:  BNE NCCmdSelected
        case 0xCBF0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCF6A:  LDA #WND_CMD_NONCMB
        case 0xCBF2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCF6C:  JSR RemoveWindow
        case 0xCBF5: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCF6F:  LDA #NPC_MOVE
        case 0xCBF7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCF71:  STA StopNPCMove
        case 0xCBFA: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LCF73:  RTS
        case 0xCBFB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCF74:  LDA WndSelResults
        case 0xCBFE: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCF76:  CMP #NCC_STATUS
        case 0xCC00: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCF78:  BNE CheckCmdWndResults
        case 0xCC02: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCF7A:  JSR LoadStats
        case 0xCC05: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCF7D:  JSR IncDescBuffer
        case 0xCC08: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCF80:  LDA EqippedItems
        case 0xCC0B: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LCF82:  LSR
        case 0xCC0C: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LCF83:  LSR
        case 0xCC0D: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LCF84:  LSR
        case 0xCC0E: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LCF85:  LSR
        case 0xCC0F: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LCF86:  LSR
        case 0xCC10: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LCF87:  CLC
        case 0xCC11: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LCF88:  ADC #$09
        case 0xCC13: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCF8A:  STA DescBuf+$8
        case 0xCC16: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCF8C:  LDA EqippedItems
        case 0xCC19: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LCF8E:  LSR
        case 0xCC1A: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LCF8F:  LSR
        case 0xCC1B: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCF90:  AND #$07
        case 0xCC1D: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LCF92:  CLC
        case 0xCC1E: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LCF93:  ADC #$11
        case 0xCC20: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCF95:  STA DescBuf+$9
        case 0xCC23: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCF97:  LDA EqippedItems
        case 0xCC26: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCF99:  AND #$03
        case 0xCC28: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LCF9B:  CLC
        case 0xCC29: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LCF9C:  ADC #$19
        case 0xCC2B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LCF9E:  STA DescBuf+$A
        case 0xCC2E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCFA0:  JSR Dowindow
        case 0xCC31: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCFA4:  JSR WaitForBtnRelease
        case 0xCC34: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCFA7:  LDA #WND_STATUS
        case 0xCC36: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCFA9:  JSR RemoveWindow
        case 0xCC39: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCFAC:  JMP ClrNCCmdWnd
        case 0xCC3C: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCFAF:  CMP #NCC_TALK
        case 0xCC3E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCFB1:  BEQ CheckTalk
        case 0xCC40: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LCFB3:  CMP #NCC_STAIRS
        case 0xCC42: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LCFB5:  BNE +
        case 0xCC44: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LCFB7:  JMP CheckStairs
        case 0xCC47: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LCFBC:  BNE +
        case 0xCC49: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LCFBE:  JMP CheckDoor
        case 0xCC4C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LCFC3:  BNE +
        case 0xCC4E: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LCFC5:  JMP DoSpell
        case 0xCC51: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LCFCA:  BNE +
        case 0xCC53: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LCFCC:  JMP CheckInventory
        case 0xCC56: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LCFD1:  BNE +
        case 0xCC58: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LCFD3:  JMP DoSearch
        case 0xCC5B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCFD9:  JSR WaitForBtnRelease
        case 0xCC5E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LCFDC:  LDA #WND_DIALOG
        case 0xCC60: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCFDE:  JSR RemoveWindow
        case 0xCC63: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LCFE1:  JMP ClrNCCmdWnd
        case 0xCC66: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCFE4:  JSR WaitForNMI
        case 0xCC69: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCFE7:  JSR GetJoypadStatus
        case 0xCC6C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCFEA:  LDA JoypadBtns
        case 0xCC6F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LCFEC:  BNE WaitForBtnRelease
        case 0xCC71: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LCFEE:  JSR WaitForNMI
        case 0xCC74: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LCFF1:  JSR GetJoypadStatus
        case 0xCC77: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCFF4:  LDA JoypadBtns
        case 0xCC7A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07B; return; } // LCFF6:  BEQ WaitForBtnPress
        case 0xCC7C: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LCFF8:  RTS
        case 0xCC7D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LCFF9:  LDA CharDirection
        case 0xCC80: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LCFFC:  PHA
        case 0xCC81: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LCFFD:  JSR Dowindow
        case 0xCC84: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD001:  LDA CharXPos
        case 0xCC87: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD003:  STA XTarget
        case 0xCC8A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD005:  LDA CharYPos
        case 0xCC8D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD007:  STA YTarget
        case 0xCC90: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LD009:  PLA
        case 0xCC91: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LD00A:  BNE CheckFacingRight
        case 0xCC93: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LD00C:  DEC YTarget
        case 0xCC96: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD00E:  JSR GetBlockID
        case 0xCC99: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD011:  LDA TargetResults
        case 0xCC9C: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD013:  CMP #BLK_LRG_TILE
        case 0xCC9E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LD015:  BNE DoTalkResults
        case 0xCCA0: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LD017:  DEC _TargetY
        case 0xCCA3: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD019:  JMP DoTalkResults
        case 0xCCA6: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD01C:  CMP #DIR_RIGHT
        case 0xCCA8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LD01E:  BNE CheckFacingDown
        case 0xCCAA: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LD020:  INC XTarget
        case 0xCCAD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD022:  JSR GetBlockID
        case 0xCCB0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD025:  LDA TargetResults
        case 0xCCB3: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD027:  CMP #BLK_LRG_TILE
        case 0xCCB5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LD029:  BNE DoTalkResults
        case 0xCCB7: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LD02B:  INC _TargetX
        case 0xCCBA: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD02D:  JMP DoTalkResults
        case 0xCCBD: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD030:  CMP #DIR_DOWN
        case 0xCCBF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LD032:  BNE DoFacingLeft
        case 0xCCC1: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LD034:  INC YTarget
        case 0xCCC4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD036:  JSR GetBlockID
        case 0xCCC7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD039:  LDA TargetResults
        case 0xCCCA: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD03B:  CMP #BLK_LRG_TILE
        case 0xCCCC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LD03D:  BNE DoTalkResults
        case 0xCCCE: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LD03F:  INC _TargetY
        case 0xCCD1: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD041:  JMP DoTalkResults
        case 0xCCD4: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LD044:  DEC XTarget
        case 0xCCD7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD046:  JSR GetBlockID
        case 0xCCDA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD049:  LDA TargetResults
        case 0xCCDD: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD04B:  CMP #BLK_LRG_TILE
        case 0xCCDF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07B; return; } // LD04D:  BNE DoTalkResults
        case 0xCCE1: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LD04F:  DEC _TargetX
        case 0xCCE4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD051:  LDA TargetResults
        case 0xCCE7: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD053:  CMP #BLK_PRINCESS
        case 0xCCE9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD055:  BNE CheckNPCTalk
        case 0xCCEB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD057:  LDA _TargetX
        case 0xCCEE: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LD059:  PHA
        case 0xCCEF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD05A:  LDA _TargetY
        case 0xCCF2: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LD05C:  PHA
        case 0xCCF3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD05D:  JSR DoDialogLoBlock
        case 0xCCF6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD061:  JSR DoDialogLoBlock
        case 0xCCF9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD065:  JSR Dowindow
        case 0xCCFC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LD069:  BEQ +
        case 0xCCFE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD06B:  JSR DoDialogLoBlock
        case 0xCD01: cycles_to_run -= 3; reg_PC = 0xC07B; return; // JMP PrincessRescueLoop
        case 0xCD04: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD074:  ORA #F_GOT_GWAELIN
        case 0xCD06: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD076:  STA PlayerFlags
        case 0xCD09: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD078:  JSR WaitForNMI
        case 0xCD0C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD07B:  JSR DoSprites
        case 0xCD0F: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LD07E:  PLA
        case 0xCD10: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LD07F:  SEC
        case 0xCD11: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LD080:  SBC CharYPos
        case 0xCD14: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LD082:  ASL
        case 0xCD15: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD083:  STA YPosFromCenter
        case 0xCD18: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LD085:  PLA
        case 0xCD19: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LD086:  SEC
        case 0xCD1A: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LD087:  SBC CharXPos
        case 0xCD1D: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LD089:  ASL
        case 0xCD1E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD08A:  STA XPosFromCenter
        case 0xCD21: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD08C:  LDA #$00
        case 0xCD23: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD08E:  STA BlkRemoveFlgs
        case 0xCD26: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD090:  JSR ModMapBlock
        case 0xCD29: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD093:  JSR DoDialogLoBlock
        case 0xCD2C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD097:  LDA #MSC_PRNCS_LOVE
        case 0xCD2E: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LD099:  BRK
        case 0xCD2F: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LD09C:  BRK
        case 0xCD30: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD09F:  LDA #MSC_DUNGEON1
        case 0xCD32: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LD0A1:  BRK
        case 0xCD33: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD0A4:  LDA #$B8
        case 0xCD35: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD0A6:  JMP DoFinalDialog
        case 0xCD38: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LD0A9:  LDY #$00
        case 0xCD3A: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD0AB:  LDA _NPCXPos,Y
        case 0xCD3D: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD0AE:  AND #$1F
        case 0xCD3F: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LD0B0:  CMP _TargetX
        case 0xCD42: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD0B2:  BNE CheckNextNPC
        case 0xCD44: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD0B4:  LDA _NPCYPos,Y
        case 0xCD47: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD0B7:  AND #$1F
        case 0xCD49: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LD0B9:  CMP _TargetY
        case 0xCD4C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD0BB:  BNE CheckNextNPC
        case 0xCD4E: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD0BD:  LDA _NPCXPos,Y
        case 0xCD51: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD0C0:  BNE JmpValidateNPC
        case 0xCD53: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD0C2:  LDA _NPCYPos,Y
        case 0xCD56: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD0C5:  BNE JmpValidateNPC
        case 0xCD58: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD0C7:  LDA _NPCMidPos,Y
        case 0xCD5B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD0CA:  BNE JmpValidateNPC
        case 0xCD5D: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD0CC:  JMP NoTalk
        case 0xCD60: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD0CF:  JMP ValidateNPC
        case 0xCD63: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LD0D2:  INY
        case 0xCD64: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LD0D3:  INY
        case 0xCD65: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LD0D4:  INY
        case 0xCD66: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LD0D5:  CPY #$3C
        case 0xCD68: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD0D7:  BNE NPCTalkLoop
        case 0xCD6A: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD0D9:  JMP NoTalk
        case 0xCD6D: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LD0DC:  STY NPCOffset
        case 0xCD70: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LD0DE:  CPY #$1E
        case 0xCD72: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LD0E0:  BCC CheckMobNPC
        case 0xCD74: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LD0E2:  TYA
        case 0xCD75: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LD0E3:  SEC
        case 0xCD76: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LD0E4:  SBC #$1C
        case 0xCD78: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LD0E6:  TAY
        case 0xCD79: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD0E7:  LDA MapNumber
        case 0xCD7C: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LD0E9:  SEC
        case 0xCD7D: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LD0EA:  SBC #$04
        case 0xCD7F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD0EC:  CMP #$0B
        case 0xCD81: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LD0EE:  BCC GetStatNPCPtr
        case 0xCD83: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD0F0:  JMP NoTalk
        case 0xCD86: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LD0F3:  ASL
        case 0xCD87: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LD0F4:  TAX
        case 0xCD88: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD0F5:  LDA NPCStatPtrTbl,X
        case 0xCD8B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD0F8:  STA GenPtr3CLB
        case 0xCD8E: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD0FA:  LDA NPCStatPtrTbl+1,X
        case 0xCD91: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD0FD:  STA GenPtr3CUB
        case 0xCD94: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD0FF:  JMP PrepTalk
        case 0xCD97: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LD102:  INY
        case 0xCD98: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LD103:  INY
        case 0xCD99: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD104:  LDA MapNumber
        case 0xCD9C: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LD106:  SEC
        case 0xCD9D: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LD107:  SBC #$04
        case 0xCD9F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD109:  CMP #$0B
        case 0xCDA1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LD10B:  BCC GetMobNPCPtr
        case 0xCDA3: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD10D:  JMP NoTalk
        case 0xCDA6: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LD110:  ASL
        case 0xCDA7: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LD111:  TAX
        case 0xCDA8: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD112:  LDA NPCMobPtrTbl,X
        case 0xCDAB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD115:  STA GenPtr3CLB
        case 0xCDAE: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD117:  LDA NPCMobPtrTbl+1,X
        case 0xCDB1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD11A:  STA GenPtr3CUB
        case 0xCDB4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD11C:  LDA NPCOffset
        case 0xCDB7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD11E:  JSR NPCFacePlayer
        case 0xCDBA: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LD121:  TYA
        case 0xCDBB: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LD122:  PHA
        case 0xCDBC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD123:  LDA GenPtr3CLB
        case 0xCDBF: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LD125:  PHA
        case 0xCDC0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD126:  LDA GenPtr3CUB
        case 0xCDC3: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LD128:  PHA
        case 0xCDC4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD129:  JSR DoSprites
        case 0xCDC7: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LD12C:  PLA
        case 0xCDC8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD12D:  STA GenPtr3CUB
        case 0xCDCB: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LD12F:  PLA
        case 0xCDCC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD130:  STA GenPtr3CLB
        case 0xCDCF: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LD132:  PLA
        case 0xCDD0: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LD133:  TAY
        case 0xCDD1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD134:  LDA StoryFlags
        case 0xCDD4: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD136:  AND #F_DGNLRD_DEAD
        case 0xCDD6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD138:  BEQ RegularDialog
        case 0xCDD8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD13A:  LDA MapNumber
        case 0xCDDB: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD13C:  CMP #MAP_TANTCSTL_GF
        case 0xCDDD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD13E:  BNE TantEndDialog
        case 0xCDDF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD140:  JSR DoDialogHiBlock
        case 0xCDE2: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD144:  JMP ResumeGamePlay
        case 0xCDE5: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LD147:  LDA (GenPtr3C),Y
        case 0xCDE7: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD149:  CMP #$64
        case 0xCDE9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD14B:  BNE RandEndDialog
        case 0xCDEB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD14D:  JSR DoDialogHiBlock
        case 0xCDEE: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD151:  JMP ResumeGamePlay
        case 0xCDF1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD154:  JSR UpdateRandNum
        case 0xCDF4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD157:  LDA RandNumUB
        case 0xCDF7: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LD159:  LSR
        case 0xCDF8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LD15A:  BCC AlternateDialog
        case 0xCDFA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD15C:  JSR DoDialogHiBlock
        case 0xCDFD: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD160:  JMP ResumeGamePlay
        case 0xCE00: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD163:  JSR DoDialogHiBlock
        case 0xCE03: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD167:  JMP ResumeGamePlay
        case 0xCE06: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LD16A:  LDA (GenPtr3C),Y
        case 0xCE08: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD16C:  CMP #$07
        case 0xCE0A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LD16E:  BCS +
        case 0xCE0C: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD170:  JMP WeaponsDialog
        case 0xCE0F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LD175:  BCS +
        case 0xCE11: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD177:  JMP ToolsDialog
        case 0xCE14: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LD17C:  BCS +
        case 0xCE16: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD17E:  JMP KeysDialog
        case 0xCE19: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LD183:  BCS +
        case 0xCE1B: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD185:  JMP FairyDialog
        case 0xCE1E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LD18A:  BCS +
        case 0xCE20: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD18C:  JMP InnDialog
        case 0xCE23: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LD191:  BCS CheckYesNoDialog
        case 0xCE25: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LD193:  PHA
        case 0xCE26: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD194:  LDA PlayerFlags
        case 0xCE29: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD196:  AND #F_LEFT_THROOM
        case 0xCE2B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD198:  BNE DoVariousDialog1
        case 0xCE2D: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LD19A:  PLA
        case 0xCE2E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD19B:  CMP #$23
        case 0xCE30: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD19D:  BNE ThrnRmDialog2
        case 0xCE32: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD19F:  JSR DoDialogHiBlock
        case 0xCE35: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD1A3:  JMP ResumeGamePlay
        case 0xCE38: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD1A6:  CMP #$24
        case 0xCE3A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD1A8:  BNE DoVariousDialog2
        case 0xCE3C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD1AA:  JSR DoDialogHiBlock
        case 0xCE3F: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD1AE:  JMP ResumeGamePlay
        case 0xCE42: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LD1B1:  PLA
        case 0xCE43: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LD1B2:  PHA
        case 0xCE44: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LD1B3:  CLC
        case 0xCE45: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LD1B4:  ADC #$2F
        case 0xCE47: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD1B6:  JSR DoMidDialog
        case 0xCE4A: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LD1B9:  PLA
        case 0xCE4B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD1BA:  CMP #$1A
        case 0xCE4D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD1BC:  BNE EndVariousDialog
        case 0xCE4F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD1BE:  JSR DoDialogLoBlock
        case 0xCE52: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD1C2:  JMP ResumeGamePlay
        case 0xCE55: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD1C5:  CMP #$62
        case 0xCE57: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LD1C7:  BCS ChkPrncsDialog1
        case 0xCE59: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LD1C9:  CLC
        case 0xCE5A: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LD1CA:  ADC #$2F
        case 0xCE5C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD1CC:  STA DialogTemp
        case 0xCE5F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD1CE:  JSR DoMidDialog
        case 0xCE62: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD1D1:  JSR Dowindow
        case 0xCE65: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD1D5:  BNE NoRespDialog
        case 0xCE67: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD1D7:  LDA DialogTemp
        case 0xCE6A: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LD1D9:  CLC
        case 0xCE6B: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LD1DA:  ADC #$05
        case 0xCE6D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD1DC:  JSR DoMidDialog
        case 0xCE70: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD1DF:  JMP ResumeGamePlay
        case 0xCE73: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD1E2:  LDA DialogTemp
        case 0xCE76: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LD1E4:  CLC
        case 0xCE77: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LD1E5:  ADC #$0A
        case 0xCE79: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD1E7:  JSR DoMidDialog
        case 0xCE7C: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD1EA:  JMP ResumeGamePlay
        case 0xCE7F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD1ED:  JSR DoDialogLoBlock
        case 0xCE82: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD1F1:  JMP ResumeGamePlay
        case 0xCE85: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD1F4:  BNE ChkPrncsDialog2
        case 0xCE87: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD1F6:  LDA PlayerFlags
        case 0xCE8A: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD1F8:  AND #F_DONE_GWAELIN
        case 0xCE8C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD1FA:  BNE PrincessSaved1
        case 0xCE8E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD1FC:  LDA #$9B
        case 0xCE90: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD1FE:  BNE DoFinalDialog
        case 0xCE92: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD200:  LDA #$9C
        case 0xCE94: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD202:  BNE DoFinalDialog
        case 0xCE96: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD204:  CMP #$63
        case 0xCE98: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD206:  BNE ChkPrncsDialog3
        case 0xCE9A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD208:  LDA PlayerFlags
        case 0xCE9D: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD20A:  AND #F_DONE_GWAELIN
        case 0xCE9F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD20C:  BEQ PrincessNotSaved
        case 0xCEA1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD20E:  LDA #$9D
        case 0xCEA3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD210:  BNE DoFinalDialog
        case 0xCEA5: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD212:  CMP #$64
        case 0xCEA7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD214:  BNE ChkPrncsDialog4
        case 0xCEA9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD216:  LDA PlayerFlags
        case 0xCEAC: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD218:  AND #F_DONE_GWAELIN
        case 0xCEAE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD21A:  BNE PrincessSaved2
        case 0xCEB0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD21C:  LDA #$9E
        case 0xCEB2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD21E:  BNE DoFinalDialog
        case 0xCEB4: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD220:  LDA #$9F
        case 0xCEB6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD222:  BNE DoFinalDialog
        case 0xCEB8: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD224:  CMP #$65
        case 0xCEBA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD226:  BNE WzdGuardDialog
        case 0xCEBC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD228:  LDA PlayerFlags
        case 0xCEBF: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD22A:  AND #F_DONE_GWAELIN
        case 0xCEC1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD22C:  BNE PrincessSaved3
        case 0xCEC3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD22E:  JSR DoDialogLoBlock
        case 0xCEC6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD232:  JSR Dowindow
        case 0xCEC9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD236:  BEQ SavePrincessDialog
        case 0xCECB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD238:  JSR DoDialogLoBlock
        case 0xCECE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD23C:  LDA #$A2
        case 0xCED0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD23E:  BNE DoFinalDialog
        case 0xCED2: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD240:  LDA #$A3
        case 0xCED4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD242:  JSR DoMidDialog
        case 0xCED7: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD245:  JMP ResumeGamePlay
        case 0xCEDA: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD248:  CMP #$66
        case 0xCEDC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD24A:  BNE ChkCursedDialog
        case 0xCEDE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD24C:  LDA #ITM_STNS_SNLGHT
        case 0xCEE0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD24E:  JSR CheckForInvItem
        case 0xCEE3: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD251:  CMP #ITM_NOT_FOUND
        case 0xCEE5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD253:  BNE HaveUniqueItem
        case 0xCEE7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD255:  LDA #ITM_RNBW_DROP
        case 0xCEE9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD257:  JSR CheckForInvItem
        case 0xCEEC: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD25A:  CMP #ITM_NOT_FOUND
        case 0xCEEE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD25C:  BNE HaveUniqueItem
        case 0xCEF0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD25E:  JSR DoDialogLoBlock
        case 0xCEF3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD262:  LDA #$C6
        case 0xCEF5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD264:  BNE DoFinalDialog
        case 0xCEF7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD266:  LDA #$A5
        case 0xCEF9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD268:  BNE DoFinalDialog
        case 0xCEFB: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD26A:  CMP #$67
        case 0xCEFD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD26C:  BNE ChkWeaponDialog
        case 0xCEFF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD26E:  LDA ModsnSpells
        case 0xCF02: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD270:  AND #$C0
        case 0xCF04: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD272:  BNE CursedDialog
        case 0xCF06: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD274:  LDA #$A6
        case 0xCF08: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD276:  BNE DoFinalDialog
        case 0xCF0A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD278:  JSR DoDialogLoBlock
        case 0xCF0D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD27C:  LDA ModsnSpells
        case 0xCF10: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0xC07E; return; } // LD27E:  BPL RemoveCrsBelt
        case 0xCF12: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD280:  LDA #ITM_DTH_NEKLACE
        case 0xCF14: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD282:  JSR RemoveInvItem
        case 0xCF17: cycles_to_run -= 3; cpu_bit(bus_read(0x0000)); reg_PC += 3; return; // LD285:  BIT ModsnSpells
        case 0xCF1A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_V)) { reg_PC = 0xC07E; return; } // LD287:  BVC ClearCurseFlags
        case 0xCF1C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD289:  LDA #ITM_CRSD_BELT
        case 0xCF1E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD28B:  JSR RemoveInvItem
        case 0xCF21: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD28E:  LDA ModsnSpells
        case 0xCF24: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD290:  AND #$3F
        case 0xCF26: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD292:  STA ModsnSpells
        case 0xCF29: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD294:  LDA #$A8
        case 0xCF2B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD296:  BNE DoFinalDialog
        case 0xCF2D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD298:  CMP #$68
        case 0xCF2F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD29A:  BNE ChkRingDialog
        case 0xCF31: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD29C:  LDA EqippedItems
        case 0xCF34: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD29E:  AND #WP_WEAPONS
        case 0xCF36: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD2A0:  CMP #WP_ERDK_SWRD
        case 0xCF38: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD2A2:  BEQ GotSwordDialog
        case 0xCF3A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD2A4:  LDA #$A9
        case 0xCF3C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD2A6:  BNE DoFinalDialog
        case 0xCF3E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD2A8:  LDA #$AA
        case 0xCF40: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD2AA:  BNE DoFinalDialog
        case 0xCF42: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD2AC:  CMP #$69
        case 0xCF44: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD2AE:  BNE ChkMagicDialog
        case 0xCF46: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD2B0:  LDA #ITM_FTR_RING
        case 0xCF48: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD2B2:  JSR CheckForInvItem
        case 0xCF4B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD2B5:  CMP #ITM_NOT_FOUND
        case 0xCF4D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD2B7:  BNE RingInInventory
        case 0xCF4F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD2B9:  LDA ModsnSpells
        case 0xCF52: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD2BB:  AND #$DF
        case 0xCF54: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD2BD:  STA ModsnSpells
        case 0xCF57: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD2BF:  LDA ModsnSpells
        case 0xCF5A: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD2C1:  AND #F_FTR_RING
        case 0xCF5C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD2C3:  BNE WearingRing
        case 0xCF5E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD2C5:  LDA #$AC
        case 0xCF60: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD2C7:  JMP DoFinalDialog
        case 0xCF63: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD2CA:  LDA #$AB
        case 0xCF65: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD2CC:  JMP DoFinalDialog
        case 0xCF68: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD2CF:  CMP #$6A
        case 0xCF6A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD2D1:  BNE ErdTknDialog
        case 0xCF6C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD2D3:  JSR DoDialogLoBlock
        case 0xCF6F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD2D7:  JSR BWScreenFlash
        case 0xCF72: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD2DA:  LDA DisplayedMaxMP
        case 0xCF75: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD2DC:  STA MagicPoints
        case 0xCF78: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD2DE:  JSR Dowindow
        case 0xCF7B: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD2E2:  JMP ResumeGamePlay
        case 0xCF7E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD2E5:  CMP #$6B
        case 0xCF80: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD2E7:  BNE RainStaffDialog
        case 0xCF82: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD2E9:  JSR DoDialogLoBlock
        case 0xCF85: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD3ED:  JSR DoDialogLoBlock
        case 0xCF88: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD2F1:  JSR DoDialogLoBlock
        case 0xCF8B: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD2F5:  JMP ResumeGamePlay
        case 0xCF8E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD2F8:  CMP #$6C
        case 0xCF90: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD2FA:  BNE RnbwDrpDialog
        case 0xCF92: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD2FC:  LDA #ITM_RNBW_DROP
        case 0xCF94: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD2FE:  JSR CheckForInvItem
        case 0xCF97: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD301:  CMP #ITM_NOT_FOUND
        case 0xCF99: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD303:  BNE HaveItemDialog
        case 0xCF9B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD305:  LDA #ITM_STFF_RAIN
        case 0xCF9D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD307:  JSR CheckForInvItem
        case 0xCFA0: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD30A:  CMP #ITM_NOT_FOUND
        case 0xCFA2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD30C:  BEQ ChkSlvrHarp
        case 0xCFA4: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD30E:  LDA #$A5
        case 0xCFA6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD310:  JSR DoMidDialog
        case 0xCFA9: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD313:  JMP ResumeGamePlay
        case 0xCFAC: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD316:  LDA #ITM_SLVR_HARP
        case 0xCFAE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD318:  JSR CheckForInvItem
        case 0xCFB1: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD31B:  CMP #ITM_NOT_FOUND
        case 0xCFB3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD31D:  BEQ HarpNotFound
        case 0xCFB5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD31F:  JSR DoDialogLoBlock
        case 0xCFB8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD323:  JSR DoDialogLoBlock
        case 0xCFBB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD327:  JSR DoDialogLoBlock
        case 0xCFBE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD32B:  LDA #ITM_SLVR_HARP
        case 0xCFC0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD32D:  JSR RemoveInvItem
        case 0xCFC3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD330:  LDA #$00
        case 0xCFC5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD332:  STA NPCXPos+$1E
        case 0xCFC8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD334:  STA NPCYPos+$1E
        case 0xCFCB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD336:  STA NPCMidPos+$1E
        case 0xCFCE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD338:  JSR WaitForNMI
        case 0xCFD1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD33B:  JSR DoSprites
        case 0xCFD4: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD33E:  JMP ResumeGamePlay
        case 0xCFD7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD341:  LDA #$B1
        case 0xCFD9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD343:  BNE NoItemGive
        case 0xCFDB: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD345:  CMP #$6D
        case 0xCFDD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD347:  BNE KingDialog
        case 0xCFDF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD349:  LDA #ITM_RNBW_DROP
        case 0xCFE1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD34B:  JSR CheckForInvItem
        case 0xCFE4: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD34E:  CMP #ITM_NOT_FOUND
        case 0xCFE6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD350:  BNE HaveItemDialog
        case 0xCFE8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD352:  LDA #ITM_ERDRICK_TKN
        case 0xCFEA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD354:  JSR CheckForInvItem
        case 0xCFED: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD357:  CMP #ITM_NOT_FOUND
        case 0xCFEF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD359:  BNE HaveErdToken
        case 0xCFF1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD35B:  JSR DoDialogLoBlock
        case 0xCFF4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD35F:  JSR BWScreenFlash
        case 0xCFF7: cycles_to_run -= 3; reg_PC = 0x0000; return; // LD362:  JMP CheckMapExit
        case 0xCFFA: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD365:  LDA #ITM_STNS_SNLGHT
        case 0xCFFC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD367:  JSR CheckForInvItem
        case 0xCFFF: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD36A:  CMP #ITM_NOT_FOUND
        case 0xD001: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD36C:  BEQ NoRnbwDrpDialog
        case 0xD003: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD36E:  LDA #ITM_STFF_RAIN
        case 0xD005: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD370:  JSR CheckForInvItem
        case 0xD008: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD373:  CMP #ITM_NOT_FOUND
        case 0xD00A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD375:  BEQ NoRnbwDrpDialog
        case 0xD00C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD377:  JSR DoDialogLoBlock
        case 0xD00F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD37B:  LDA #ITM_STNS_SNLGHT
        case 0xD011: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD37D:  JSR RemoveInvItem
        case 0xD014: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD380:  LDA #ITM_STFF_RAIN
        case 0xD016: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD382:  JSR RemoveInvItem
        case 0xD019: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD385:  LDA #ITM_RNBW_DROP
        case 0xD01B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD387:  JSR AddInvItem
        case 0xD01E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD38A:  JSR WaitForNMI
        case 0xD021: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD38D:  LDA #%00011001
        case 0xD023: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD38F:  STA PPUControl1
        case 0xD026: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LD392:  LDX #$1E
        case 0xD028: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LD397:  DEX
        case 0xD029: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LD398:  BNE -
        case 0xD02B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD39A:  LDA #%00011000
        case 0xD02D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD39C:  STA PPUControl1
        case 0xD030: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD39F:  JMP ResumeGamePlay
        case 0xD033: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD3A2:  JSR DoDialogLoBlock
        case 0xD036: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD3A6:  LDA #$AE
        case 0xD038: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD3A8:  JMP NoItemGive
        case 0xD03B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD3AB:  CMP #$6E
        case 0xD03D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD3AD:  BEQ DoKingDialog
        case 0xD03F: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD3AF:  JMP PrincessDialog
        case 0xD042: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD3B2:  LDA PlayerFlags
        case 0xD045: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD3B4:  AND #F_GOT_GWAELIN
        case 0xD047: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD3B6:  BEQ KingDialog2
        case 0xD049: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD3B8:  JSR DoDialogLoBlock
        case 0xD04C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD3BC:  LDA #ITM_GWAELIN_LVE
        case 0xD04E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD3BE:  JSR AddInvItem
        case 0xD051: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LD3C1:  CPX #INV_FULL
        case 0xD053: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD3C3:  BNE KingPrncsDialog
        case 0xD055: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LD3C5:  LDX #$00
        case 0xD057: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0xC07E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD3C7:  LDA InvListTbl,X
        case 0xD05A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD3CA:  JSR CheckForInvItem
        case 0xD05D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD3CD:  CMP #ITM_NOT_FOUND
        case 0xD05F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD3CF:  BNE TakeItemFound
        case 0xD061: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LD3D1:  INX
        case 0xD062: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LD3D2:  CPX #$07
        case 0xD064: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD3D4:  BNE TakeItemLoop
        case 0xD066: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD3D6:  BEQ KingPrncsDialog
        case 0xD068: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0xC07E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD3D8:  LDA InvListTbl,X
        case 0xD06B: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LD3DB:  PHA
        case 0xD06C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD3DC:  JSR RemoveInvItem
        case 0xD06F: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LD3DF:  PLA
        case 0xD070: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LD3E0:  CLC
        case 0xD071: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LD3E1:  ADC #$31
        case 0xD073: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD3E3:  JSR GetDescriptionByte
        case 0xD076: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD3E6:  LDA #ITM_GWAELIN_LVE
        case 0xD078: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD3E8:  JSR AddInvItem
        case 0xD07B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD3EB:  JSR DoDialogLoBlock
        case 0xD07E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD3EF:  JSR DoDialogLoBlock
        case 0xD081: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD3F3:  JSR DoDialogLoBlock
        case 0xD084: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD3F7:  LDA PlayerFlags
        case 0xD087: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD3F9:  AND #$FC
        case 0xD089: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD3FB:  ORA #F_RTN_GWAELIN
        case 0xD08B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD3FD:  STA PlayerFlags
        case 0xD08E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD3FF:  LDA #$C6
        case 0xD090: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD401:  STA NPCXPos+$27
        case 0xD093: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD403:  LDA #$43
        case 0xD095: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD405:  STA NPCYPos+$27
        case 0xD098: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD407:  JSR WaitForNMI
        case 0xD09B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD40A:  JSR DoSprites
        case 0xD09E: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD40D:  JMP SaveDialog
        case 0xD0A1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD410:  LDA PlayerFlags
        case 0xD0A4: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD412:  AND #F_LEFT_THROOM
        case 0xD0A6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD414:  BNE LeftThRoom
        case 0xD0A8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD416:  LDA #$BF
        case 0xD0AA: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD418:  JMP DoFinalDialog
        case 0xD0AD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD41B:  JSR DoDialogLoBlock
        case 0xD0B0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD41F:  LDA DisplayedLevel
        case 0xD0B3: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD421:  CMP #LVL_30
        case 0xD0B5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD423:  BNE CalculateExp
        case 0xD0B7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD425:  JSR DoDialogLoBlock
        case 0xD0BA: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD429:  JMP SaveDialog
        case 0xD0BD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD42C:  JSR GetExpRemaining
        case 0xD0C0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD42F:  JSR DoDialogLoBlock
        case 0xD0C3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD433:  JSR DoDialogHiBlock
        case 0xD0C6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD437:  JSR Dowindow
        case 0xD0C9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD43B:  CMP #WND_YES
        case 0xD0CB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD43D:  BNE ContQuestDialog
        case 0xD0CD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD43F:  JSR PrepSaveGame
        case 0xD0D0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD442:  JSR DoDialogHiBlock
        case 0xD0D3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD446:  JSR DoDialogHiBlock
        case 0xD0D6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD44A:  JSR Dowindow
        case 0xD0D9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD44E:  CMP #$00
        case 0xD0DB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD450:  BEQ KingEndTalk
        case 0xD0DD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD452:  JSR DoDialogHiBlock
        case 0xD0E0: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LD456:  BRK
        case 0xD0E1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD459:  JSR MMCShutdown
        case 0xD0E4: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD45C:  JMP Spinlock2
        case 0xD0E7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD45F:  LDA #$C4
        case 0xD0E9: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD461:  JMP DoFinalDialog
        case 0xD0EC: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD464:  CMP #$6F
        case 0xD0EE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD466:  BNE DgrnLrdDialog
        case 0xD0F0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD468:  JSR UpdateRandNum
        case 0xD0F3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD46B:  LDA RandNumUB
        case 0xD0F6: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD46D:  AND #$60
        case 0xD0F8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD46F:  BNE PrncsRndDialog1
        case 0xD0FA: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD471:  LDA #$BB
        case 0xD0FC: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD473:  JMP DoFinalDialog
        case 0xD0FF: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD476:  CMP #$60
        case 0xD101: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD478:  BNE PrncsRndDialog2
        case 0xD103: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD47A:  LDA #$BD
        case 0xD105: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD47C:  JMP DoFinalDialog
        case 0xD108: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD47F:  JSR DoDialogLoBlock
        case 0xD10B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD483:  JSR Dowindow
        case 0xD10E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD487:  BEQ PrncsLoveDialog
        case 0xD110: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD489:  JSR DoDialogLoBlock
        case 0xD113: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD48D:  JMP PrncsRndDialog2
        case 0xD116: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD490:  JSR DoDialogLoBlock
        case 0xD119: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD494:  LDA #MSC_PRNCS_LOVE
        case 0xD11B: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LD496:  BRK
        case 0xD11C: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LD499:  BRK
        case 0xD11D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD49C:  LDA #MSC_THRN_ROOM
        case 0xD11F: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LD49E:  BRK
        case 0xD120: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD4A1:  JMP ResumeGamePlay
        case 0xD123: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD4A4:  CMP #$70
        case 0xD125: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD4A6:  BEQ DoDrgnLrdDialog
        case 0xD127: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD4A8:  JMP MiscDialog
        case 0xD12A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD4AB:  JSR DoDialogLoBlock
        case 0xD12D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD4AF:  JSR DoDialogLoBlock
        case 0xD130: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD4B3:  JSR DoDialogLoBlock
        case 0xD133: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD4B7:  JSR Dowindow
        case 0xD136: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD4BB:  BNE RefuseDglrdDialog
        case 0xD138: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD4BD:  JSR DoDialogHiBlock
        case 0xD13B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD4C1:  JSR Dowindow
        case 0xD13E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD4C5:  BEQ ChooseDrgnLrd
        case 0xD140: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD4C7:  JSR DoDialogLoBlock
        case 0xD143: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LD4CB:  LDX #$28
        case 0xD145: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LD4D0:  DEX
        case 0xD146: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LD4D1:  BNE -
        case 0xD148: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD4D3:  LDA #WND_DIALOG
        case 0xD14A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD4D5:  JSR RemoveWindow
        case 0xD14D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD4D8:  LDA #WND_CMD_NONCMB
        case 0xD14F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD4DA:  JSR RemoveWindow
        case 0xD152: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD4DD:  LDA #WND_POPUP
        case 0xD154: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD4DF:  JSR RemoveWindow
        case 0xD157: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD4E2:  LDA #EN_DRAGONLORD1
        case 0xD159: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD4E4:  JMP InitFight
        case 0xD15C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD4E7:  JSR DoDialogLoBlock
        case 0xD15F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD4EB:  JSR DoDialogLoBlock
        case 0xD162: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD4EF:  LDA #$00
        case 0xD164: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD4F1:  STA ExpLB
        case 0xD167: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD4F3:  STA ExpUB
        case 0xD16A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD4F5:  STA GoldLB
        case 0xD16D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD4F7:  STA GoldUB
        case 0xD170: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD4F9:  STA InventorySlot12
        case 0xD173: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD4FB:  STA InventorySlot34
        case 0xD176: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD4FD:  STA InventorySlot56
        case 0xD179: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD4FF:  STA InventorySlot78
        case 0xD17C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD501:  STA InventoryKeys
        case 0xD17F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD503:  STA InventoryHerbs
        case 0xD182: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD505:  STA EqippedItems
        case 0xD185: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD507:  STA ModsnSpells
        case 0xD188: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD509:  STA PlayerFlags
        case 0xD18B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD50B:  STA StoryFlags
        case 0xD18E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD50D:  JSR DoDialogLoBlock
        case 0xD191: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD511:  JSR DoDialogLoBlock
        case 0xD194: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD515:  LDA BadEndBGPalPtr
        case 0xD197: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD518:  STA PalPtrLB
        case 0xD19A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD51A:  LDA BadEndBGPalPtr+1
        case 0xD19D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD51D:  STA PalPtrUB
        case 0xD1A0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD51F:  LDA #$00
        case 0xD1A2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD521:  STA PalModByte
        case 0xD1A5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD523:  JSR PrepBGPalLoad
        case 0xD1A8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD526:  JSR WaitForNMI
        case 0xD1AB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD529:  JSR Dowindow
        case 0xD1AE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD52D:  JSR WaitForNMI
        case 0xD1B1: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD530:  JMP Spinlock3
        case 0xD1B4: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD533:  CMP #$71
        case 0xD1B6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD535:  BEQ GuardDialog1
        case 0xD1B8: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD537:  CMP #$72
        case 0xD1BA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD539:  BEQ GuardDialog2
        case 0xD1BC: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD53B:  JMP NoTalk
        case 0xD1BF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD53E:  JSR DoDialogLoBlock
        case 0xD1C2: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD542:  JMP ResumeGamePlay
        case 0xD1C5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD545:  JSR DoDialogLoBlock
        case 0xD1C8: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD549:  JMP ResumeGamePlay
        case 0xD1CB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD553:  STA DialogTemp
        case 0xD1CE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD555:  JSR DoDialogLoBlock
        case 0xD1D1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD559:  JSR Dowindow
        case 0xD1D4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LD55D:  BEQ WeapYesDialog
        case 0xD1D6: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD55F:  JMP WeapNoDialog
        case 0xD1D9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD562:  JSR DoDialogLoBlock
        case 0xD1DC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD566:  JSR GetShopItems
        case 0xD1DF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD569:  LDA WndSelResults
        case 0xD1E2: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD56B:  CMP #WND_ABORT
        case 0xD1E4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD56D:  BNE CheckBuyWeapon
        case 0xD1E6: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD56F:  JMP WeapNoDialog
        case 0xD1E9: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD572:  LDA ShopItemsTbl,X
        case 0xD1EC: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LD575:  PHA
        case 0xD1ED: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LD576:  CLC
        case 0xD1EE: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LD577:  ADC #$1B
        case 0xD1F0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD579:  JSR GetDescriptionByte
        case 0xD1F3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD57C:  JSR DoDialogLoBlock
        case 0xD1F6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD580:  LDA #$00
        case 0xD1F8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD582:  STA GenWrd00LB
        case 0xD1FB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD584:  STA GenWrd00UB
        case 0xD1FE: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LD586:  PLA
        case 0xD1FF: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LD587:  ASL
        case 0xD200: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LD588:  TAX
        case 0xD201: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD589:  LDA GoldLB
        case 0xD204: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LD58B:  SEC
        case 0xD205: cycles_to_run -= 2; cpu_sbc(bus_read(addr_abs_x(0x0000, nullptr))); reg_PC += 3; return; // LD58C:  SBC ItemCostTbl,X
        case 0xD208: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD58F:  LDA GoldUB
        case 0xD20B: cycles_to_run -= 2; cpu_sbc(bus_read(addr_abs_x(0x0000, nullptr))); reg_PC += 3; return; // LD591:  SBC ItemCostTbl+1,X
        case 0xD20E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LD594:  BCS DoWeapPurchase
        case 0xD210: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD596:  JMP NoMoneyDialog
        case 0xD213: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LD599:  TXA
        case 0xD214: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LD59A:  PHA
        case 0xD215: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD59B:  CMP #$0E
        case 0xD217: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LD59D:  BCS ChkItem
        case 0xD219: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD59F:  LDA EqippedItems
        case 0xD21C: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LD5A1:  LSR
        case 0xD21D: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LD5A2:  LSR
        case 0xD21E: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LD5A3:  LSR
        case 0xD21F: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LD5A4:  LSR
        case 0xD220: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LD5A5:  LSR
        case 0xD221: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD5A6:  BEQ ConfSaleDialog
        case 0xD223: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD5A8:  BNE GetBuybackPrice
        case 0xD225: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD5AA:  CMP #$1C
        case 0xD227: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LD5AC:  BCS ChkShield
        case 0xD229: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD5AE:  LDA EqippedItems
        case 0xD22C: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LD5B0:  LSR
        case 0xD22D: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LD5B1:  LSR
        case 0xD22E: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD5B2:  AND #$07
        case 0xD230: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD5B4:  BEQ ConfSaleDialog
        case 0xD232: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LD5B6:  CLC
        case 0xD233: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LD5B7:  ADC #$07
        case 0xD235: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD5B9:  BNE GetBuybackPrice
        case 0xD237: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD5BB:  LDA EqippedItems
        case 0xD23A: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD5BD:  AND #SH_SHIELDS
        case 0xD23C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD5BF:  BEQ ConfSaleDialog
        case 0xD23E: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LD5C1:  CLC
        case 0xD23F: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LD5C2:  ADC #$0E
        case 0xD241: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LD5C4:  ASL
        case 0xD242: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LD5C5:  TAY
        case 0xD243: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD5C6:  LDA ItemCostTbl-2,Y
        case 0xD246: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD5C9:  STA GenWrd00LB
        case 0xD249: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD5CB:  LDA ItemCostTbl-1,Y
        case 0xD24C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD5CE:  STA GenWrd00UB
        case 0xD24F: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LD5D0:  LSR GenWrd00UB
        case 0xD252: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LD5D2:  ROR GenWrd00LB
        case 0xD255: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LD5D4:  TYA
        case 0xD256: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LD5D5:  LSR
        case 0xD257: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LD5D6:  CLC
        case 0xD258: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LD5D7:  ADC #$1A
        case 0xD25A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD5D9:  JSR GetDescriptionByte
        case 0xD25D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD5DC:  JSR DoDialogLoBlock
        case 0xD260: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD5E0:  JSR DoDialogLoBlock
        case 0xD263: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD5E4:  JSR Dowindow
        case 0xD266: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LD5E8:  PLA
        case 0xD267: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LD5E9:  TAX
        case 0xD268: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD5EA:  LDA WndSelResults
        case 0xD26B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD5EC:  BEQ ComitWeapPurchase
        case 0xD26D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD5EE:  JSR DoDialogLoBlock
        case 0xD270: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD5F2:  JMP NextWeapDialog
        case 0xD273: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD5F5:  LDA GoldLB
        case 0xD276: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LD5F7:  SEC
        case 0xD277: cycles_to_run -= 2; cpu_sbc(bus_read(addr_abs_x(0x0000, nullptr))); reg_PC += 3; return; // LD5F8:  SBC ItemCostTbl,X
        case 0xD27A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD5FB:  STA GoldLB
        case 0xD27D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD5FD:  LDA GoldUB
        case 0xD280: cycles_to_run -= 2; cpu_sbc(bus_read(addr_abs_x(0x0000, nullptr))); reg_PC += 3; return; // LD5FF:  SBC ItemCostTbl+1,X
        case 0xD283: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD602:  STA GoldUB
        case 0xD286: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD604:  LDA GoldLB
        case 0xD289: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LD606:  CLC
        case 0xD28A: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LD607:  ADC GenWrd00LB
        case 0xD28D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD609:  STA GoldLB
        case 0xD290: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD60B:  LDA GoldUB
        case 0xD293: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LD60D:  ADC GenWrd00UB
        case 0xD296: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD60F:  STA GoldUB
        case 0xD299: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LD611:  BCC ApplyPurchase
        case 0xD29B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD613:  LDA #$FF
        case 0xD29D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD615:  STA GoldLB
        case 0xD2A0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD617:  STA GoldUB
        case 0xD2A3: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LD619:  TXA
        case 0xD2A4: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LD61A:  LSR
        case 0xD2A5: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD61B:  CMP #$07
        case 0xD2A7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LD61D:  BCS $D63C
        case 0xD2A9: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LD61F:  CLC
        case 0xD2AA: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LD620:  ADC #$01
        case 0xD2AC: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LD622:  ASL
        case 0xD2AD: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LD623:  ASL
        case 0xD2AE: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LD624:  ASL
        case 0xD2AF: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LD625:  ASL
        case 0xD2B0: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LD626:  ASL
        case 0xD2B1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD627:  STA GenByte3C
        case 0xD2B4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD629:  LDA EqippedItems
        case 0xD2B7: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD62B:  AND #$1F
        case 0xD2B9: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD62D:  ORA GenByte3C
        case 0xD2BC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD62F:  STA EqippedItems
        case 0xD2BF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD631:  JSR DoDialogLoBlock
        case 0xD2C2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD635:  JSR Dowindow
        case 0xD2C5: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD639:  JMP NextWeapDialog
        case 0xD2C8: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD63C:  CMP #$0E
        case 0xD2CA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LD63E:  BCS ApplyShield
        case 0xD2CC: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LD640:  SEC
        case 0xD2CD: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LD641:  SBC #$06
        case 0xD2CF: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LD643:  ASL
        case 0xD2D0: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LD644:  ASL
        case 0xD2D1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD645:  STA GenByte3C
        case 0xD2D4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD647:  LDA EqippedItems
        case 0xD2D7: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD649:  AND #$E3
        case 0xD2D9: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD64B:  ORA GenByte3C
        case 0xD2DC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD64D:  STA EqippedItems
        case 0xD2DF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD64F:  BNE CompWeapPurchase
        case 0xD2E1: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LD651:  SEC
        case 0xD2E2: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LD652:  SBC #$0D
        case 0xD2E4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD654:  STA GenByte3C
        case 0xD2E7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD656:  LDA EqippedItems
        case 0xD2EA: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD658:  AND #$FC
        case 0xD2EC: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD65A:  ORA GenByte3C
        case 0xD2EF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD65C:  STA EqippedItems
        case 0xD2F2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD65E:  BNE CompWeapPurchase
        case 0xD2F4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD660:  JSR DoDialogLoBlock
        case 0xD2F7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD664:  JSR DoDialogLoBlock
        case 0xD2FA: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD668:  JMP WpnDialogLoop
        case 0xD2FD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD66B:  JSR DoDialogLoBlock
        case 0xD300: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD66F:  JMP ResumeGamePlay
        case 0xD303: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LD672:  LDX #$00
        case 0xD305: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD674:  LDA DialogTemp
        case 0xD308: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD676:  STA ShopIndex
        case 0xD30B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD678:  BEQ ShopEntryFound
        case 0xD30D: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD67A:  LDA ShopItemsTbl,X
        case 0xD310: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LD67D:  INX
        case 0xD311: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD67E:  CMP #ITM_TBL_END
        case 0xD313: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD680:  BNE ShopEntryLoop
        case 0xD315: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LD682:  DEC ShopIndex
        case 0xD318: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD684:  BNE ShopEntryLoop
        case 0xD31A: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LD686:  TXA
        case 0xD31B: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LD687:  PHA
        case 0xD31C: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LD688:  LDY #$01
        case 0xD31E: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LD68A:  STY DescBuf
        case 0xD321: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD68C:  LDA ShopItemsTbl,X
        case 0xD324: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LD68F:  CLC
        case 0xD325: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LD690:  ADC #$02
        case 0xD327: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LD692:  STA _DescBuf,Y
        case 0xD32A: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD695:  CMP #ITM_TBL_END+2
        case 0xD32C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD697:  BEQ ShowShopInvWnd
        case 0xD32E: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LD699:  INX
        case 0xD32F: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LD69A:  INY
        case 0xD330: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD69B:  BNE ShopEntryLoad
        case 0xD332: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD69D:  JSR Dowindow
        case 0xD335: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LD6A1:  PLA
        case 0xD336: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LD6A2:  CLC
        case 0xD337: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LD6A3:  ADC WndSelResults
        case 0xD33A: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LD6A5:  TAX
        case 0xD33B: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LD6A6:  RTS
        case 0xD33C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD6A7:  STA DialogTemp
        case 0xD33F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD6A9:  JSR DoDialogLoBlock
        case 0xD342: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD6AD:  JSR Dowindow
        case 0xD345: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD6B1:  BEQ DoToolPurchase
        case 0xD347: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD6B3:  CMP #WND_SELL
        case 0xD349: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD6B5:  BNE ToolExitDialog
        case 0xD34B: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD6B7:  JMP DoToolSell
        case 0xD34E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD6BA:  JSR DoDialogLoBlock
        case 0xD351: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD6BE:  JMP ResumeGamePlay
        case 0xD354: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD6C1:  JSR DoDialogLoBlock
        case 0xD357: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD6C5:  JSR GetShopItems
        case 0xD35A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD6C8:  LDA WndSelResults
        case 0xD35D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD6CA:  CMP #WND_ABORT
        case 0xD35F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD6CC:  BEQ ToolExitDialog
        case 0xD361: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD6CE:  LDA ShopItemsTbl,X
        case 0xD364: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LD6D1:  PHA
        case 0xD365: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LD6D2:  CLC
        case 0xD366: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LD6D3:  ADC #$1F
        case 0xD368: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD6D5:  JSR GetDescriptionByte
        case 0xD36B: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LD6D8:  PLA
        case 0xD36C: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LD6D9:  ASL
        case 0xD36D: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LD6DA:  TAX
        case 0xD36E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD6DB:  LDA GoldLB
        case 0xD371: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LD6DD:  SEC
        case 0xD372: cycles_to_run -= 2; cpu_sbc(bus_read(addr_abs_x(0x0000, nullptr))); reg_PC += 3; return; // LD6DE:  SBC ItemCostTbl,X
        case 0xD375: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD6E1:  STA GenWord3CLB
        case 0xD378: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD6E3:  LDA GoldUB
        case 0xD37B: cycles_to_run -= 2; cpu_sbc(bus_read(addr_abs_x(0x0000, nullptr))); reg_PC += 3; return; // LD6E5:  SBC ItemCostTbl+1,X
        case 0xD37E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD6E8:  STA GenWord3CUB
        case 0xD381: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LD6EA:  BCS ChkToolPurchase
        case 0xD383: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD6EC:  JSR DoDialogLoBlock
        case 0xD386: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD6F0:  JMP NextToolDialog
        case 0xD389: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LD6F3:  CPX #$22
        case 0xD38B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD6F5:  BNE DoOthrToolPurchase
        case 0xD38D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD6F7:  LDA InventoryHerbs
        case 0xD390: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD6F9:  CMP #$06
        case 0xD392: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD6FB:  BNE PurchaseHerb
        case 0xD394: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD6FD:  JSR DoDialogLoBlock
        case 0xD397: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD701:  JMP NextToolDialog
        case 0xD39A: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LD704:  INC InventoryHerbs
        case 0xD39D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD706:  LDA GenWord3CLB
        case 0xD3A0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD708:  STA GoldLB
        case 0xD3A3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD70A:  LDA GenWord3CUB
        case 0xD3A6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD70C:  STA GoldUB
        case 0xD3A9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD70E:  JSR DoDialogLoBlock
        case 0xD3AC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD712:  JSR Dowindow
        case 0xD3AF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD716:  JSR DoDialogLoBlock
        case 0xD3B2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD71A:  JSR Dowindow
        case 0xD3B5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD71E:  BNE DoToolExit
        case 0xD3B7: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD720:  JMP DoToolPurchase
        case 0xD3BA: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD723:  JMP ToolExitDialog
        case 0xD3BD: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LD726:  TXA
        case 0xD3BE: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LD727:  LSR
        case 0xD3BF: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LD728:  SEC
        case 0xD3C0: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LD729:  SBC #$12
        case 0xD3C2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD72B:  JSR AddInvItem
        case 0xD3C5: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LD72E:  CPX #INV_FULL
        case 0xD3C7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD730:  BNE PurchaseTool
        case 0xD3C9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD732:  JSR DoDialogLoBlock
        case 0xD3CC: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD736:  JMP NextToolDialog
        case 0xD3CF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD739:  JSR CreateInvList
        case 0xD3D2: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LD73C:  CPX #$01
        case 0xD3D4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD73E:  BNE HaveToolsToSell
        case 0xD3D6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD740:  JSR DoDialogLoBlock
        case 0xD3D9: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD744:  JMP ToolExitDialog
        case 0xD3DC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD747:  JSR DoDialogLoBlock
        case 0xD3DF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD74B:  JSR Dowindow
        case 0xD3E2: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD74F:  CMP #WND_ABORT
        case 0xD3E4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD751:  BNE GetSellDesc
        case 0xD3E6: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD753:  JMP ToolExitDialog
        case 0xD3E9: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LD756:  TAX
        case 0xD3EA: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD757:  LDA DescBuf+1,X
        case 0xD3ED: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD759:  STA DescTemp
        case 0xD3F0: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LD75B:  CLC
        case 0xD3F1: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LD75C:  ADC #$2E
        case 0xD3F3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD75E:  JSR GetDescriptionByte
        case 0xD3F6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD761:  LDA DescTemp
        case 0xD3F9: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LD763:  CLC
        case 0xD3FA: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LD764:  ADC #$0F
        case 0xD3FC: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LD766:  ASL
        case 0xD3FD: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LD767:  TAX
        case 0xD3FE: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD768:  LDA ItemCostTbl,X
        case 0xD401: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD76B:  STA GenWrd00LB
        case 0xD404: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD76D:  LDA ItemCostTbl+1,X
        case 0xD407: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD770:  STA GenWrd00UB
        case 0xD40A: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD772:  ORA GenWrd00LB
        case 0xD40D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD774:  BNE SellableTool
        case 0xD40F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD776:  JSR DoDialogLoBlock
        case 0xD412: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD77A:  JSR DoDialogLoBlock
        case 0xD415: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD77E:  JSR Dowindow
        case 0xD418: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LD782:  BNE $D787
        case 0xD41A: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD784:  JMP DoToolSell
        case 0xD41D: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD787:  JMP ToolExitDialog
        case 0xD420: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 2; return; // LD78A:  LSR $01
        case 0xD422: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 2; return; // LD78C:  ROR $00
        case 0xD424: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD78E:  JSR DoDialogLoBlock
        case 0xD427: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD792:  JSR Dowindow
        case 0xD42A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD796:  BNE ItemSellLoop
        case 0xD42C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD798:  LDA DescTemp
        case 0xD42F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD79A:  CMP #$03
        case 0xD431: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD79C:  BNE ChkSellHerb
        case 0xD433: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LD79E:  DEC InventoryKeys
        case 0xD436: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD7A0:  LDA GoldLB
        case 0xD439: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LD7A2:  CLC
        case 0xD43A: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LD7A3:  ADC GenWrd00LB
        case 0xD43D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD7A5:  STA GoldLB
        case 0xD440: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD7A7:  LDA GoldUB
        case 0xD443: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LD7A9:  ADC GenWrd00UB
        case 0xD446: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD7AB:  STA GoldUB
        case 0xD449: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LD7AD:  BCC DoneSellingTool
        case 0xD44B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD7AF:  LDA #$FF
        case 0xD44D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD7B1:  STA GoldLB
        case 0xD450: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD7B3:  STA GoldUB
        case 0xD453: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD7B5:  JSR Dowindow
        case 0xD456: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD7B9:  JMP ItemSellLoop
        case 0xD459: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD7BC:  CMP #$02
        case 0xD45B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD7BE:  BNE ChkSellBelt
        case 0xD45D: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LD7C0:  DEC InventoryHerbs
        case 0xD460: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD7C2:  JMP GetSellGold
        case 0xD463: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD7C5:  CMP #$0C
        case 0xD465: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD7C7:  BNE ChkSellNecklace
        case 0xD467: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LD7C9:  PHA
        case 0xD468: cycles_to_run -= 3; cpu_bit(bus_read(0x0000)); reg_PC += 3; return; // LD7CA:  BIT ModsnSpells
        case 0xD46B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_V)) { reg_PC = 0xC07E; return; } // LD7CC:  BVC SellBelt
        case 0xD46D: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LD7CE:  PLA
        case 0xD46E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD7CF:  JSR DoDialogLoBlock
        case 0xD471: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD7D3:  JSR DoDialogLoBlock
        case 0xD474: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD7D7:  JMP ItemSellLoop
        case 0xD477: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD7DA:  CMP #$0E
        case 0xD479: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD7DC:  BNE DoSellTool
        case 0xD47B: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LD7DE:  PHA
        case 0xD47C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD7DF:  LDA ModsnSpells
        case 0xD47F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0xC07E; return; } // LD7E1:  BMI CantSellCrsdItm
        case 0xD481: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LD7E3:  PLA
        case 0xD482: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LD7E4:  SEC
        case 0xD483: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LD7E5:  SBC #$03
        case 0xD485: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD7E7:  JSR RemoveInvItem
        case 0xD488: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD7EA:  JMP GetSellGold
        case 0xD48B: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LD7ED:  SEC
        case 0xD48C: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LD7EE:  SBC #$0C
        case 0xD48E: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LD7F0:  TAX
        case 0xD48F: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD7F1:  LDA KeyCostTbl,X
        case 0xD492: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD7F4:  STA GenWrd00LB
        case 0xD495: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD7F6:  LDA #$00
        case 0xD497: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD7F8:  STA GenWrd00UB
        case 0xD49A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD7FA:  JSR DoDialogLoBlock
        case 0xD49D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD7FE:  JSR Dowindow
        case 0xD4A0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD802:  BEQ ChkBuyKey
        case 0xD4A2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD804:  JSR DoDialogLoBlock
        case 0xD4A5: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD808:  JMP ResumeGamePlay
        case 0xD4A8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD80B:  LDA InventoryKeys
        case 0xD4AB: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD80D:  CMP #$06
        case 0xD4AD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD80F:  BNE ChkBuyKeyGold
        case 0xD4AF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD811:  JSR DoDialogLoBlock
        case 0xD4B2: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD815:  JMP EndKeyDialog
        case 0xD4B5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD818:  LDA GoldLB
        case 0xD4B8: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LD81A:  SEC
        case 0xD4B9: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LD81B:  SBC GenWrd00LB
        case 0xD4BC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD81D:  STA GenWord3CLB
        case 0xD4BF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD81F:  LDA GoldUB
        case 0xD4C2: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LD821:  SBC GenWrd00UB
        case 0xD4C5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD823:  STA GenWord3CUB
        case 0xD4C8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LD825:  BCS BuyKey
        case 0xD4CA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD827:  JSR DoDialogLoBlock
        case 0xD4CD: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD82B:  JMP EndKeyDialog
        case 0xD4D0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD82E:  LDA GenWord3CLB
        case 0xD4D3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD830:  STA GoldLB
        case 0xD4D6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD832:  LDA GenWord3CUB
        case 0xD4D9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD834:  STA GoldUB
        case 0xD4DC: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LD836:  INC InventoryKeys
        case 0xD4DF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD838:  JSR Dowindow
        case 0xD4E2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD83C:  JSR DoDialogLoBlock
        case 0xD4E5: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD840:  JMP KeyDialogLoop
        case 0xD4E8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD843:  LDA #$26
        case 0xD4EA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD845:  STA GenWrd00LB
        case 0xD4ED: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD847:  LDA #$00
        case 0xD4EF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD849:  STA GenWrd00UB
        case 0xD4F2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD84B:  JSR DoDialogLoBlock
        case 0xD4F5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD84F:  JSR Dowindow
        case 0xD4F8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD853:  BEQ ChkBuyFryWtr
        case 0xD4FA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD855:  JSR DoDialogLoBlock
        case 0xD4FD: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD859:  JMP ResumeGamePlay
        case 0xD500: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD85C:  LDA GoldLB
        case 0xD503: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LD85E:  SEC
        case 0xD504: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LD85F:  SBC GenWrd00LB
        case 0xD507: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD861:  STA GenWord3CLB
        case 0xD50A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD863:  LDA GoldUB
        case 0xD50D: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LD865:  SBC GenWrd00UB
        case 0xD510: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD867:  STA GenWord3CUB
        case 0xD513: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LD869:  BCS ChkFryWtrInv
        case 0xD515: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD86B:  JSR DoDialogLoBlock
        case 0xD518: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD86F:  JMP EndFairyDialog
        case 0xD51B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD872:  LDA #ITM_FRY_WATER
        case 0xD51D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD874:  JSR AddInvItem
        case 0xD520: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LD877:  CPX #INV_FULL
        case 0xD522: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD879:  BNE BuyFairyWater
        case 0xD524: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD87B:  JSR DoDialogLoBlock
        case 0xD527: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD87F:  JMP EndFairyDialog
        case 0xD52A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD882:  LDA GenWord3CLB
        case 0xD52D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD884:  STA GoldLB
        case 0xD530: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD886:  LDA GenWord3CUB
        case 0xD533: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD888:  STA GoldUB
        case 0xD536: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD88A:  JSR Dowindow
        case 0xD539: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD88E:  JSR DoDialogLoBlock
        case 0xD53C: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD892:  JMP FairyDialogLoop
        case 0xD53F: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LD895:  SEC
        case 0xD540: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LD896:  SBC #$11
        case 0xD542: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LD898:  TAX
        case 0xD543: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD899:  LDA InnCostTbl,X
        case 0xD546: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD89C:  STA GenWrd00LB
        case 0xD549: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD89E:  LDA #$00
        case 0xD54B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD8A0:  STA GenWrd00UB
        case 0xD54E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD8A2:  JSR DoDialogLoBlock
        case 0xD551: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD8A6:  JSR Dowindow
        case 0xD554: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD8AA:  BEQ ChkBuyInnStay
        case 0xD556: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD8AC:  JSR DoDialogLoBlock
        case 0xD559: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD8B0:  JMP ResumeGamePlay
        case 0xD55C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD8B3:  LDA GoldLB
        case 0xD55F: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LD8B5:  SEC
        case 0xD560: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LD8B6:  SBC GenWrd00LB
        case 0xD563: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD8B8:  STA GenWord3CLB
        case 0xD566: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD8BA:  LDA GoldUB
        case 0xD569: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LD8BC:  SBC GenWrd00UB
        case 0xD56C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD8BE:  STA GenWord3CUB
        case 0xD56F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LD8C0:  BCS BuyInnStay
        case 0xD571: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD8C2:  JSR DoDialogLoBlock
        case 0xD574: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD8C6:  JMP InnExitDialog2
        case 0xD577: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD8C9:  LDA GenWord3CLB
        case 0xD57A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD8CB:  STA GoldLB
        case 0xD57D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD8CD:  LDA GenWord3CUB
        case 0xD580: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD8CF:  STA GoldUB
        case 0xD583: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD8D1:  JSR Dowindow
        case 0xD586: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD8D5:  JSR DoDialogLoBlock
        case 0xD589: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD8D9:  JSR $D915
        case 0xD58C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD8DC:  JSR PalFadeOut
        case 0xD58F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD8DF:  LDA #MSC_INN
        case 0xD591: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LD8E1:  BRK
        case 0xD592: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD8E4:  LDA DisplayedMaxHP
        case 0xD595: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD8E6:  STA HitPoints
        case 0xD598: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD8E8:  LDA DisplayedMaxMP
        case 0xD59B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD8EA:  STA MagicPoints
        case 0xD59E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD8EC:  JSR Dowindow
        case 0xD5A1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LD8F0:  JSR GetRegPalPtrs
        case 0xD5A4: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LD8F3:  BRK
        case 0xD5A5: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD8F6:  LDA #MSC_VILLAGE
        case 0xD5A7: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LD8F8:  BRK
        case 0xD5A8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD8FB:  JSR PalFadeIn
        case 0xD5AB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD8FE:  LDA PlayerFlags
        case 0xD5AE: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LD900:  LSR
        case 0xD5AF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LD901:  BCC SpecialInnDialog
        case 0xD5B1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD903:  JSR DoDialogLoBlock
        case 0xD5B4: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD907:  JMP InnExitDialog
        case 0xD5B7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD90A:  JSR DoDialogLoBlock
        case 0xD5BA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD90E:  JSR DoDialogLoBlock
        case 0xD5BD: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD912:  JMP ResumeGamePlay
        case 0xD5C0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD915:  LDA RegSPPalPtr
        case 0xD5C3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD918:  STA SprtPalPtrLB
        case 0xD5C6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD91A:  LDA RegSPPalPtr+1
        case 0xD5C9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD91D:  STA SprtPalPtrUB
        case 0xD5CC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD91F:  LDA TownPalPtr
        case 0xD5CF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD922:  STA BGPalPtrLB
        case 0xD5D2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD924:  LDA TownPalPtr+1
        case 0xD5D5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD927:  STA BGPalPtrUB
        case 0xD5D8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD929:  LDA #PAL_LOAD_BG
        case 0xD5DA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD92B:  STA LoadBGPal
        case 0xD5DD: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LD92D:  RTS
        case 0xD5DE: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LD92E:  LDX #$00
        case 0xD5E0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD930:  LDA #$01
        case 0xD5E2: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LD934:  INX
        case 0xD5E3: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LD935:  CLC
        case 0xD5E4: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LD936:  ADC #$01
        case 0xD5E6: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LD938:  CPX #$0B
        case 0xD5E8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LD93A:  BNE -
        case 0xD5EA: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LD93C:  RTS
        case 0xD5EB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD93D:  LDA #$00
        case 0xD5ED: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LD93F:  BEQ +
        case 0xD5EF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD941:  LDA #$01
        case 0xD5F1: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LD945:  LDX #$00
        case 0xD5F3: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LD947:  LDY #$00
        case 0xD5F5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD949:  LDA MapNumber
        case 0xD5F8: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(addr_abs_x(0xC07E, nullptr))); reg_PC += 3; return; // LD94B:  CMP MapEntryTbl,X
        case 0xD5FB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD94E:  BNE NextMapEntry1
        case 0xD5FD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD950:  LDA CharXPos
        case 0xD600: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(addr_abs_x(0x0000, nullptr))); reg_PC += 3; return; // LD952:  CMP MapEntryTbl+1,X
        case 0xD603: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD955:  BNE NextMapEntry1
        case 0xD605: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD957:  LDA CharYPos
        case 0xD608: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(addr_abs_x(0x0000, nullptr))); reg_PC += 3; return; // LD959:  CMP MapEntryTbl+2,X
        case 0xD60B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD95C:  BNE NextMapEntry1
        case 0xD60D: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0xC07E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD95E:  LDA MapTargetTbl,X
        case 0xD610: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD961:  STA MapNumber
        case 0xD613: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD963:  LDA MapTargetTbl+1,X
        case 0xD616: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD966:  STA CharXPos
        case 0xD619: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD968:  STA _CharXPos
        case 0xD61C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD96A:  STA CharXPixelsLB
        case 0xD61F: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD96C:  LDA MapTargetTbl+2,X
        case 0xD622: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD96F:  STA CharYPos
        case 0xD625: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD971:  STA _CharYPos
        case 0xD628: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD973:  STA CharYPixelsLB
        case 0xD62B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD975:  LDA GenByte2C
        case 0xD62E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LD977:  BEQ StairsFaceRight
        case 0xD630: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD979:  LDA MapEntryDirTbl,Y
        case 0xD633: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD97C:  JMP SetPlyrPixelLoc
        case 0xD636: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD97F:  LDA #DIR_RIGHT
        case 0xD638: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD981:  AND #$03
        case 0xD63A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD983:  STA CharDirection
        case 0xD63D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD986:  LDA #$00
        case 0xD63F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD988:  STA CharXPixelsUB
        case 0xD642: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD98A:  STA CharYPixelsUB
        case 0xD645: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LD98C:  LDX #$04
        case 0xD647: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_rol(bus_read(a))); } reg_PC += 3; return; // LD990:  ROL CharXPixelsUB
        case 0xD64A: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LD992:  ASL CharYPixelsLB
        case 0xD64D: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_rol(bus_read(a))); } reg_PC += 3; return; // LD994:  ROL CharYPixelsUB
        case 0xD650: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LD996:  DEX
        case 0xD651: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LD997:  BNE -
        case 0xD653: cycles_to_run -= 3; reg_PC = 0x0000; return; // LD999:  JMP MapChngWithSound
        case 0xD656: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LD99C:  INX
        case 0xD657: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LD99D:  INX
        case 0xD658: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LD99E:  INX
        case 0xD659: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LD99F:  INY
        case 0xD65A: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LD9A0:  CPX #$99
        case 0xD65C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD9A2:  BNE MapCheckLoop1
        case 0xD65E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD9A4:  JSR Dowindow
        case 0xD661: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LD9A8:  JSR DoDialogLoBlock
        case 0xD664: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LD9AC:  JMP ResumeGamePlay
        case 0xD667: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD9AF:  LDA CharXPos
        case 0xD66A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD9B1:  STA XTarget
        case 0xD66D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD9B3:  LDA CharYPos
        case 0xD670: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD9B5:  STA YTarget
        case 0xD673: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LD9B7:  JSR GetBlockID
        case 0xD676: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD9BA:  LDA TargetResults
        case 0xD679: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD9BC:  CMP #BLK_STAIR_DN
        case 0xD67B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD9BE:  BNE ChkStairsUp
        case 0xD67D: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD9C0:  JMP StairDownFound
        case 0xD680: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LD9C3:  CMP #BLK_STAIR_UP
        case 0xD682: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD9C5:  BNE NoStairsFound
        case 0xD684: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LD9C7:  LDX #$00
        case 0xD686: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LD9C9:  LDY #$00
        case 0xD688: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD9CB:  LDA MapNumber
        case 0xD68B: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(addr_abs_x(0xC07E, nullptr))); reg_PC += 3; return; // LD9CD:  CMP MapTargetTbl,X
        case 0xD68E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD9D0:  BNE NextMapEntry2
        case 0xD690: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD9D2:  LDA CharXPos
        case 0xD693: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(addr_abs_x(0x0000, nullptr))); reg_PC += 3; return; // LD9D4:  CMP MapTargetTbl+1,X
        case 0xD696: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD9D7:  BNE NextMapEntry2
        case 0xD698: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LD9D9:  LDA CharYPos
        case 0xD69B: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(addr_abs_x(0x0000, nullptr))); reg_PC += 3; return; // LD9DB:  CMP MapTargetTbl+2,X
        case 0xD69E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LD9DE:  BNE NextMapEntry2
        case 0xD6A0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LD9E0:  LDA #DIR_LEFT
        case 0xD6A2: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LD9E2:  PHA
        case 0xD6A3: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0xC07E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD9E3:  LDA MapEntryTbl,X
        case 0xD6A6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD9E6:  STA MapNumber
        case 0xD6A9: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD9E8:  LDA MapEntryTbl+1,X
        case 0xD6AC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD9EB:  STA CharXPos
        case 0xD6AF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD9ED:  STA _CharXPos
        case 0xD6B2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD9EF:  STA CharXPixelsLB
        case 0xD6B5: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LD9F1:  LDA MapEntryTbl+2,X
        case 0xD6B8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD9F4:  STA CharYPos
        case 0xD6BB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD9F6:  STA _CharYPos
        case 0xD6BE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LD9F8:  STA CharYPixelsLB
        case 0xD6C1: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LD9FA:  PLA
        case 0xD6C2: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LD9FB:  JMP SetPlyrPixelLoc
        case 0xD6C5: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LD9FE:  INX
        case 0xD6C6: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LD9FF:  INX
        case 0xD6C7: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LDA00:  INX
        case 0xD6C8: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LDA01:  INY
        case 0xD6C9: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LDA02:  CPX #$99
        case 0xD6CB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDA04:  BNE MapCheckLoop2
        case 0xD6CD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDA06:  JSR Dowindow
        case 0xD6D0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDA0A:  JSR DoDialogLoBlock
        case 0xD6D3: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDA0E:  JMP ResumeGamePlay
        case 0xD6D6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDA11:  LDA SpellFlags
        case 0xD6D9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDA13:  STA SpellFlagsLB
        case 0xD6DC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDA15:  LDA ModsnSpells
        case 0xD6DF: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDA17:  AND #$03
        case 0xD6E1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDA19:  STA SpellFlagsUB
        case 0xD6E4: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDA1B:  ORA SpellFlagsLB
        case 0xD6E7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LDA1D:  BNE +
        case 0xD6E9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDA1F:  JSR Dowindow
        case 0xD6EC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDA23:  JSR DoDialogLoBlock
        case 0xD6EF: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDA27:  JMP ResumeGamePlay
        case 0xD6F2: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDA2D:  CMP #WND_ABORT
        case 0xD6F4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LDA2F:  BNE +
        case 0xD6F6: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDA31:  JMP ClrNCCmdWnd
        case 0xD6F9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDA35:  JSR Dowindow
        case 0xD6FC: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LDA39:  PLA
        case 0xD6FD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDA3A:  JSR CheckMP
        case 0xD700: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDA3D:  CMP #$32
        case 0xD702: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDA3F:  BNE ChkHeal
        case 0xD704: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDA41:  JSR DoMidDialog
        case 0xD707: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDA44:  JMP ResumeGamePlay
        case 0xD70A: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDA47:  CMP #SPL_HEAL
        case 0xD70C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDA49:  BNE ChkHurt
        case 0xD70E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDA4B:  JSR DoHeal
        case 0xD711: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDA4E:  JMP ResumeGamePlay
        case 0xD714: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDA51:  CMP #SPL_HURT
        case 0xD716: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LDA53:  BNE +
        case 0xD718: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDA55:  JSR DoDialogLoBlock
        case 0xD71B: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDA59:  JMP ResumeGamePlay
        case 0xD71E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LDA5E:  BEQ SpellFizzle
        case 0xD720: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDA60:  CMP #SPL_RADIANT
        case 0xD722: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDA62:  BNE ChkRepel
        case 0xD724: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDA64:  LDA MapType
        case 0xD727: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDA66:  CMP #MAP_DUNGEON
        case 0xD729: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDA68:  BNE SpellFizzle
        case 0xD72B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDA6A:  LDA #$50
        case 0xD72D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDA6C:  STA RadiantTimer
        case 0xD730: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDA6E:  LDA #WND_DIALOG
        case 0xD732: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDA70:  JSR RemoveWindow
        case 0xD735: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDA73:  LDA #WND_CMD_NONCMB
        case 0xD737: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDA75:  JSR RemoveWindow
        case 0xD73A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDA78:  LDA #WND_POPUP
        case 0xD73C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDA7A:  JSR RemoveWindow
        case 0xD73F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDA7D:  LDA LightDiameter
        case 0xD742: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDA7F:  CMP #$07
        case 0xD744: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LDA81:  BNE +
        case 0xD746: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LDA83:  RTS
        case 0xD747: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LDA85:  ADC #$02
        case 0xD749: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDA87:  STA LightDiameter
        case 0xD74C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDA89:  LDA #SFX_RADIANT
        case 0xD74E: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDA8B:  BRK
        case 0xD74F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDA8E:  JSR PostMoveUpdate
        case 0xD752: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LDA91:  JMP LightIncreaseLoop
        case 0xD755: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDA94:  CMP #SPL_REPEL
        case 0xD757: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDA96:  BNE ChkOutside
        case 0xD759: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDA98:  LDA #$FF
        case 0xD75B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDA9A:  STA RepelTimer
        case 0xD75E: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDA9C:  JMP ResumeGamePlay
        case 0xD761: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDA9F:  CMP #SPL_OUTSIDE
        case 0xD763: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDAA1:  BNE ChkHealmore
        case 0xD765: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDAA3:  LDA MapNumber
        case 0xD768: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDAA5:  CMP #MAP_ERDRCK_B1
        case 0xD76A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LDAA7:  BCC ChkGarinhamCave
        case 0xD76C: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LDAA9:  LDX #$27
        case 0xD76E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDAAB:  LDA #DIR_DOWN
        case 0xD770: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LDAAD:  JMP ChangeMaps
        case 0xD773: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LDAB2:  BCC ChkRockMtn
        case 0xD775: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LDAB4:  LDX #$39
        case 0xD777: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDAB6:  LDA #DIR_DOWN
        case 0xD779: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LDAB8:  JMP ChangeMaps
        case 0xD77C: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDABB:  CMP #MAP_RCKMTN_B1
        case 0xD77E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LDABD:  BCC ChkSwampCave
        case 0xD780: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LDABF:  LDX #$18
        case 0xD782: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDAC1:  LDA #DIR_DOWN
        case 0xD784: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LDAC3:  JMP ChangeMaps
        case 0xD787: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDAC6:  CMP #MAP_SWAMPCAVE
        case 0xD789: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDAC8:  BNE ChkDLCastle
        case 0xD78B: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LDACA:  LDX #$0F
        case 0xD78D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDACC:  LDA #DIR_DOWN
        case 0xD78F: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LDACE:  JMP ChangeMaps
        case 0xD792: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDAD1:  CMP #MAP_DLCSTL_SL1
        case 0xD794: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LDAD3:  BCS OutsideDLCastle
        case 0xD796: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDAD5:  CMP #MAP_DLCSTL_BF
        case 0xD798: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LDAD7:  BEQ OutsideDLCastle
        case 0xD79A: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LDAD9:  JMP SpellFizzle
        case 0xD79D: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LDADC:  LDX #$12
        case 0xD79F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDADE:  LDA #DIR_DOWN
        case 0xD7A1: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LDAE0:  JMP ChangeMaps
        case 0xD7A4: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDAE3:  CMP #SPL_HEALMORE
        case 0xD7A6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDAE5:  BNE ChkReturn
        case 0xD7A8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDAE7:  JSR DoHealmore
        case 0xD7AB: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDAEA:  JMP ResumeGamePlay
        case 0xD7AE: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDAED:  CMP #SPL_RETURN
        case 0xD7B0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDAEF:  BNE UnknownSpell
        case 0xD7B2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDAF1:  LDA MapType
        case 0xD7B5: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDAF3:  CMP #MAP_DUNGEON
        case 0xD7B7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LDAF5:  BEQ ReturnFail
        case 0xD7B9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDAF7:  LDA MapNumber
        case 0xD7BC: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDAF9:  CMP #MAP_DLCSTL_BF
        case 0xD7BE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDAFB:  BNE DoReturn
        case 0xD7C0: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LDAFD:  JMP SpellFizzle
        case 0xD7C3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDB00:  LDA #MAP_OVERWORLD
        case 0xD7C5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDB02:  STA MapNumber
        case 0xD7C8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDB04:  LDA #$2A
        case 0xD7CA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDB06:  STA CharXPos
        case 0xD7CD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDB08:  STA _CharXPos
        case 0xD7D0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDB0A:  STA CharXPixelsLB
        case 0xD7D3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDB0C:  LDA #$2B
        case 0xD7D5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDB0E:  STA CharYPos
        case 0xD7D8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDB10:  STA _CharYPos
        case 0xD7DB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDB12:  STA CharYPixelsLB
        case 0xD7DE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDB14:  LDA #$00
        case 0xD7E0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDB16:  STA CharXPixelsUB
        case 0xD7E3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDB18:  STA CharYPixelsUB
        case 0xD7E6: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LDB1A:  LDX #$04
        case 0xD7E8: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_rol(bus_read(a))); } reg_PC += 3; return; // LDB1E:  ROL CharXPixelsUB
        case 0xD7EB: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LDB20:  ASL CharYPixelsLB
        case 0xD7EE: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_rol(bus_read(a))); } reg_PC += 3; return; // LDB22:  ROL CharYPixelsUB
        case 0xD7F1: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LDB24:  DEX
        case 0xD7F2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LDB25:  BNE -
        case 0xD7F4: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDB27:  LDA #SFX_WVRN_WNG
        case 0xD7F6: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDB29:  BRK
        case 0xD7F7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDB2C:  LDA #DIR_DOWN
        case 0xD7F9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDB2E:  STA CharDirection
        case 0xD7FC: cycles_to_run -= 3; reg_PC = 0x0000; return; // LDB31:  JMP MapChngFadeNoSound
        case 0xD7FF: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LDB34:  JMP SpellFizzle
        case 0xD802: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LDB37:  LDX #$08
        case 0xD804: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDB3C:  JSR WaitForNMI
        case 0xD807: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDB3F:  LDA #%00011001
        case 0xD809: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDB41:  STA PPUControl1
        case 0xD80C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDB44:  JSR WaitForNMI
        case 0xD80F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDB47:  JSR WaitForNMI
        case 0xD812: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDB4A:  JSR WaitForNMI
        case 0xD815: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDB4D:  LDA #%00011000
        case 0xD817: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDB4F:  STA PPUControl1
        case 0xD81A: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LDB52:  DEX
        case 0xD81B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LDB53:  BNE -
        case 0xD81D: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LDB55:  RTS
        case 0xD81E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDB56:  JSR IncDescBuffer
        case 0xD821: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDB59:  LDA #$02
        case 0xD823: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDB5B:  STA SpellDescByte
        case 0xD826: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LDB5D:  LDX #$01
        case 0xD828: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LDB5F:  LSR SpellFlagsUB
        case 0xD82B: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LDB61:  ROR SpellFlagsLB
        case 0xD82E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LDB63:  BCC nextSpell
        case 0xD830: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDB65:  LDA SpellDescByte
        case 0xD833: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LDB67:  STA DescBuf,X
        case 0xD836: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LDB69:  INX
        case 0xD837: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LDB6A:  INC SpellDescByte
        case 0xD83A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDB6C:  LDA SpellDescByte
        case 0xD83D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDB6E:  CMP #$0C
        case 0xD83F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDB70:  BNE GetSpellsLoop
        case 0xD841: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDB72:  LDA #DSC_END
        case 0xD843: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LDB74:  STA DescBuf,X
        case 0xD846: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDB76:  JSR Dowindow
        case 0xD849: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDB7A:  CMP #WND_ABORT
        case 0xD84B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LDB7C:  BEQ ShowSpellEnd
        case 0xD84D: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LDB7E:  TAX
        case 0xD84E: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LDB7F:  LDA DescBuf+1,X
        case 0xD851: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LDB81:  SEC
        case 0xD852: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LDB82:  SBC #$02
        case 0xD854: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LDB84:  RTS
        case 0xD855: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDB85:  STA SpellToCast
        case 0xD858: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LDB87:  LDX SpellToCast
        case 0xD85B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDB89:  LDA MagicPoints
        case 0xD85E: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(addr_abs_x(0x0000, nullptr))); reg_PC += 3; return; // LDB8B:  CMP SpellCostTbl,X
        case 0xD861: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LDB8E:  BCS PlyrCastSpell
        case 0xD863: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDB90:  LDA #$32
        case 0xD865: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LDB92:  RTS
        case 0xD866: cycles_to_run -= 2; cpu_sbc(bus_read(addr_abs_x(0x0000, nullptr))); reg_PC += 3; return; // LDB93:  SBC SpellCostTbl,X
        case 0xD869: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDB96:  STA MagicPoints
        case 0xD86C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDB98:  LDA SpellToCast
        case 0xD86F: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LDB9A:  CLC
        case 0xD870: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LDB9B:  ADC #$10
        case 0xD872: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDB9D:  JSR GetDescriptionByte
        case 0xD875: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDBA0:  JSR DoDialogLoBlock
        case 0xD878: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDBA4:  LDA #SFX_SPELL
        case 0xD87A: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDBA6:  BRK
        case 0xD87B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDBA9:  JSR BWScreenFlash
        case 0xD87E: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDBAC:  BRK
        case 0xD87F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDBAF:  LDA SpellToCast
        case 0xD882: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LDBB1:  PHA
        case 0xD883: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDBB2:  JSR Dowindow
        case 0xD886: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LDBB6:  PLA
        case 0xD887: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LDBB7:  RTS
        case 0xD888: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDBB8:  JSR UpdateRandNum
        case 0xD88B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDBBB:  LDA RandNumUB
        case 0xD88E: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDBBD:  AND #$07
        case 0xD890: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LDBBF:  CLC
        case 0xD891: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LDBC0:  ADC #$0A
        case 0xD893: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LDBC2:  CLC
        case 0xD894: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LDBC3:  ADC HitPoints
        case 0xD897: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LDBC5:  BCS PlyrMaxHP
        case 0xD899: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LDBC7:  CMP DisplayedMaxHP
        case 0xD89C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LDBC9:  BCC +
        case 0xD89E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDBCB:  LDA DisplayedMaxHP
        case 0xD8A1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDBCF:  JSR LoadRegBGPal
        case 0xD8A4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDBD2:  JSR Dowindow
        case 0xD8A7: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LDBD6:  RTS
        case 0xD8A8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDBD7:  JSR UpdateRandNum
        case 0xD8AB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDBDA:  LDA RandNumUB
        case 0xD8AE: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDBDC:  AND #$0F
        case 0xD8B0: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LDBDE:  CLC
        case 0xD8B1: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LDBDF:  ADC #$55
        case 0xD8B3: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LDBE1:  JMP PlyrAddHP
        case 0xD8B6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDBE4:  LDA #$08
        case 0xD8B8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDBE6:  STA GenPtr3CLB
        case 0xD8BB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDBE8:  LDA #$01
        case 0xD8BD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDBEA:  STA GenPtr3CUB
        case 0xD8C0: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LDBEC:  LDY #$00
        case 0xD8C2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LDBEE:  BEQ GetThisDescLoop
        case 0xD8C4: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LDBF0:  CLC
        case 0xD8C5: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LDBF1:  ADC #$03
        case 0xD8C7: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LDBF3:  TAX
        case 0xD8C8: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LDBF4:  INX
        case 0xD8C9: cycles_to_run -= 2; reg_A = bus_read(0xC07E); update_nz(reg_A); reg_PC += 3; return; // LDBF5:  LDA DescTblPtr
        case 0xD8CC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDBF8:  STA GenPtr3CLB
        case 0xD8CF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDBFA:  LDA DescTblPtr+1
        case 0xD8D2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDBFD:  STA GenPtr3CUB
        case 0xD8D5: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LDBFF:  LDY #$00
        case 0xD8D7: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LDC01:  LDA (GenPtr3C),Y
        case 0xD8D9: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LDC03:  INC GenPtr3CLB
        case 0xD8DC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LDC05:  BNE +
        case 0xD8DE: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LDC07:  INC GenPtr3CUB
        case 0xD8E1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDC0B:  BNE DescriptionLoop
        case 0xD8E3: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LDC0D:  DEX
        case 0xD8E4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDC0E:  BNE DescriptionLoop
        case 0xD8E6: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LDC10:  LDA (GenPtr3C),Y
        case 0xD8E8: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LDC12:  STA _DescBuf,Y
        case 0xD8EB: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LDC15:  INY
        case 0xD8EC: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDC16:  CMP #TXT_SUBEND
        case 0xD8EE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDC18:  BNE GetThisDescLoop
        case 0xD8F0: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LDC1A:  RTS
        case 0xD8F1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDC1B:  JSR CreateInvList
        case 0xD8F4: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LDC1E:  CPX #INV_NONE
        case 0xD8F6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDC20:  BNE ShowInventory
        case 0xD8F8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDC22:  JSR Dowindow
        case 0xD8FB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDC26:  JSR DoDialogLoBlock
        case 0xD8FE: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDC2A:  JMP ResumeGamePlay
        case 0xD901: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDC2D:  JSR Dowindow
        case 0xD904: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDC31:  CMP #WND_ABORT
        case 0xD906: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDC33:  BNE ChkItemUsed
        case 0xD908: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDC35:  JMP ClrNCCmdWnd
        case 0xD90B: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LDC38:  TAX
        case 0xD90C: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LDC39:  LDA DescBuf+1,X
        case 0xD90F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDC3B:  CMP #INV_KEY
        case 0xD911: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LDC3D:  BEQ CheckDoor
        case 0xD913: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LDC3F:  JMP ChkHerb
        case 0xD916: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDC42:  LDA CharXPos
        case 0xD919: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDC44:  STA XTarget
        case 0xD91C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDC46:  LDA CharYPos
        case 0xD91F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDC48:  STA YTarget
        case 0xD922: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LDC4A:  DEC YTarget
        case 0xD925: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDC4C:  JSR GetBlockID
        case 0xD928: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDC4F:  LDA TargetResults
        case 0xD92B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDC51:  CMP #BLK_DOOR
        case 0xD92D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LDC53:  BEQ DoorFound
        case 0xD92F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDC55:  LDA CharXPos
        case 0xD932: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDC57:  STA XTarget
        case 0xD935: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDC59:  LDA CharYPos
        case 0xD938: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDC5B:  STA YTarget
        case 0xD93B: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LDC5D:  INC YTarget
        case 0xD93E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDC5F:  JSR GetBlockID
        case 0xD941: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDC62:  LDA TargetResults
        case 0xD944: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDC64:  CMP #BLK_DOOR
        case 0xD946: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LDC66:  BEQ DoorFound
        case 0xD948: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDC68:  LDA CharXPos
        case 0xD94B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDC6A:  STA XTarget
        case 0xD94E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDC6C:  LDA CharYPos
        case 0xD951: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDC6E:  STA YTarget
        case 0xD954: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LDC70:  DEC XTarget
        case 0xD957: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDC72:  JSR GetBlockID
        case 0xD95A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDC75:  LDA TargetResults
        case 0xD95D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDC77:  CMP #BLK_DOOR
        case 0xD95F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LDC79:  BEQ DoorFound
        case 0xD961: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDC7B:  LDA CharXPos
        case 0xD964: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDC7D:  STA XTarget
        case 0xD967: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDC7F:  LDA CharYPos
        case 0xD96A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDC81:  STA YTarget
        case 0xD96D: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LDC83:  INC XTarget
        case 0xD970: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDC85:  JSR GetBlockID
        case 0xD973: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDC88:  LDA TargetResults
        case 0xD976: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDC8A:  CMP #BLK_DOOR
        case 0xD978: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LDC8C:  BEQ DoorFound
        case 0xD97A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDC8E:  JSR Dowindow
        case 0xD97D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDC92:  JSR DoDialogHiBlock
        case 0xD980: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDC96:  JMP ResumeGamePlay
        case 0xD983: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDC99:  LDA InventoryKeys
        case 0xD986: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDC9B:  BNE UseKey
        case 0xD988: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDC9D:  JSR Dowindow
        case 0xD98B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDCA1:  JSR DoDialogHiBlock
        case 0xD98E: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDCA5:  JMP ResumeGamePlay
        case 0xD991: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LDCA8:  DEC InventoryKeys
        case 0xD994: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LDCAA:  LDX #$00
        case 0xD996: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LDCAC:  LDA DoorXPos,X
        case 0xD999: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LDCAF:  BEQ DoorOpened
        case 0xD99B: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LDCB1:  INX
        case 0xD99C: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LDCB2:  INX
        case 0xD99D: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LDCB3:  CPX #$10
        case 0xD99F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDCB5:  BNE DoorCheckLoop
        case 0xD9A1: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDCB7:  JMP ResumeGamePlay
        case 0xD9A4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDCBA:  LDA _TargetX
        case 0xD9A7: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LDCBC:  STA DoorXPos,X
        case 0xD9AA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDCBF:  LDA _TargetY
        case 0xD9AD: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LDCC1:  STA DoorYPos,X
        case 0xD9B0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDCC4:  LDA _TargetX
        case 0xD9B3: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LDCC6:  SEC
        case 0xD9B4: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LDCC7:  SBC CharXPos
        case 0xD9B7: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LDCC9:  ASL
        case 0xD9B8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDCCA:  STA XPosFromCenter
        case 0xD9BB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDCCC:  LDA _TargetY
        case 0xD9BE: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LDCCE:  SEC
        case 0xD9BF: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LDCCF:  SBC CharYPos
        case 0xD9C2: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LDCD1:  ASL
        case 0xD9C3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDCD2:  STA YPosFromCenter
        case 0xD9C6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDCD4:  LDA #$00
        case 0xD9C8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDCD6:  STA BlkRemoveFlgs
        case 0xD9CB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDCD8:  LDA #SFX_DOOR
        case 0xD9CD: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDCDA:  BRK
        case 0xD9CE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDCDD:  JSR ModMapBlock
        case 0xD9D1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDCE3:  LDA JoypadBtns
        case 0xD9D4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LDCE5:  BNE -
        case 0xD9D6: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDCE7:  JMP ClrNCCmdWnd
        case 0xD9D9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDCEA:  CMP #INV_HERB
        case 0xD9DB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDCEC:  BNE ChkTorch
        case 0xD9DD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDCEE:  JSR Dowindow
        case 0xD9E0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDCF2:  JSR DoDialogLoBlock
        case 0xD9E3: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LDCF6:  DEC InventoryHerbs
        case 0xD9E6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDCF8:  JSR HerbHeal
        case 0xD9E9: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDCFB:  JMP ResumeGamePlay
        case 0xD9EC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDCFE:  JSR UpdateRandNum
        case 0xD9EF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDD01:  LDA RandNumUB
        case 0xD9F2: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDD03:  AND #$07
        case 0xD9F4: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LDD05:  CLC
        case 0xD9F5: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LDD06:  ADC #$17
        case 0xD9F7: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LDD08:  ADC HitPoints
        case 0xD9FA: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LDD0A:  CMP DisplayedMaxHP
        case 0xD9FD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LDD0C:  BCC +
        case 0xD9FF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDD0E:  LDA DisplayedMaxHP
        case 0xDA02: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDD12:  JSR LoadRegBGPal
        case 0xDA05: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDD15:  JSR Dowindow
        case 0xDA08: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LDD19:  RTS
        case 0xDA09: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDD1A:  CMP #INV_TORCH
        case 0xDA0B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDD1C:  BNE ChkFryWtr
        case 0xDA0D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDD1E:  LDA MapType
        case 0xDA10: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDD20:  CMP #MAP_DUNGEON
        case 0xDA12: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LDD22:  BEQ UseTorch
        case 0xDA14: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDD24:  JSR Dowindow
        case 0xDA17: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDD28:  JSR DoDialogLoBlock
        case 0xDA1A: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDD2C:  JMP ResumeGamePlay
        case 0xDA1D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDD2F:  LDA #ITM_TORCH
        case 0xDA1F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDD31:  JSR RemoveInvItem
        case 0xDA22: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDD34:  LDA #$00
        case 0xDA24: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDD36:  STA RadiantTimer
        case 0xDA27: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDD38:  LDA #WND_DIALOG
        case 0xDA29: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDD3A:  JSR RemoveWindow
        case 0xDA2C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDD3D:  LDA #WND_CMD_NONCMB
        case 0xDA2E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDD3F:  JSR RemoveWindow
        case 0xDA31: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDD42:  LDA #WND_POPUP
        case 0xDA33: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDD44:  JSR RemoveWindow
        case 0xDA36: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDD47:  LDA #$03
        case 0xDA38: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDD49:  STA LightDiameter
        case 0xDA3B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDD4B:  LDA #SFX_RADIANT
        case 0xDA3D: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDD4D:  BRK
        case 0xDA3E: cycles_to_run -= 3; reg_PC = 0x0000; return; // LDD50:  JMP PostMoveUpdate
        case 0xDA41: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDD53:  CMP #INV_FAIRY
        case 0xDA43: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDD55:  BNE ChkWings
        case 0xDA45: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDD57:  JSR Dowindow
        case 0xDA48: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDD5B:  JSR DoDialogLoBlock
        case 0xDA4B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDD5F:  LDA #ITM_FRY_WATER
        case 0xDA4D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDD61:  JSR RemoveInvItem
        case 0xDA50: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDD64:  LDA #$FE
        case 0xDA52: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDD66:  STA RepelTimer
        case 0xDA55: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDD68:  JMP ResumeGamePlay
        case 0xDA58: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDD6B:  CMP #INV_WINGS
        case 0xDA5A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDD6D:  BNE ChkDrgnScl
        case 0xDA5C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDD6F:  JSR Dowindow
        case 0xDA5F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDD73:  LDA MapType
        case 0xDA62: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDD75:  CMP #MAP_DUNGEON
        case 0xDA64: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LDD77:  BEQ WingsFail
        case 0xDA66: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDD79:  LDA MapNumber
        case 0xDA69: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDD7B:  CMP #MAP_DLCSTL_BF
        case 0xDA6B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDD7D:  BNE UseWings
        case 0xDA6D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDD7F:  JSR DoDialogLoBlock
        case 0xDA70: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDD83:  JMP ResumeGamePlay
        case 0xDA73: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDD86:  LDA #ITM_WINGS
        case 0xDA75: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDD88:  JSR RemoveInvItem
        case 0xDA78: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDD8B:  JSR DoDialogLoBlock
        case 0xDA7B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDD8F:  JSR BWScreenFlash
        case 0xDA7E: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LDD92:  JMP DoReturn
        case 0xDA81: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDD95:  CMP #INV_SCALE
        case 0xDA83: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDD97:  BNE ChkFryFlt
        case 0xDA85: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDD99:  JSR Dowindow
        case 0xDA88: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDD9D:  JSR ChkDragonScale
        case 0xDA8B: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDDA0:  JMP ResumeGamePlay
        case 0xDA8E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDDA3:  CMP #INV_FLUTE
        case 0xDA90: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDDA5:  BNE ChkFghtrRng
        case 0xDA92: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDDA7:  JSR Dowindow
        case 0xDA95: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDDAB:  JSR DoDialogLoBlock
        case 0xDA98: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDDAF:  LDA #MSC_FRY_FLUTE
        case 0xDA9A: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDDB1:  BRK
        case 0xDA9B: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDDB4:  BRK
        case 0xDA9C: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LDDB7:  LDX MapNumber
        case 0xDA9F: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LDDB9:  LDA ResumeMusicTbl,X
        case 0xDAA2: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDDBC:  BRK
        case 0xDAA3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDDBF:  JSR DoDialogLoBlock
        case 0xDAA6: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDDC3:  JMP ResumeGamePlay
        case 0xDAA9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDDC6:  CMP #INV_RING
        case 0xDAAB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDDC8:  BNE ChkToken
        case 0xDAAD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDDCA:  JSR Dowindow
        case 0xDAB0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDDCE:  JSR ChkRing
        case 0xDAB3: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDDD1:  JMP ResumeGamePlay
        case 0xDAB6: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDDD4:  CMP #INV_TOKEN
        case 0xDAB8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDDD6:  BNE ChkStones
        case 0xDABA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDDD8:  JSR Dowindow
        case 0xDABD: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDDDC:  LDA #$38
        case 0xDABF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDDDE:  JSR GetDescriptionByte
        case 0xDAC2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDDE1:  JSR DoDialogLoBlock
        case 0xDAC5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDDE5:  JSR DoDialogLoBlock
        case 0xDAC8: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDDE9:  JMP ResumeGamePlay
        case 0xDACB: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDDEC:  CMP #INV_STONES
        case 0xDACD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDDEE:  BNE ChkStaff
        case 0xDACF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDDF0:  JSR Dowindow
        case 0xDAD2: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDDF4:  LDA #$3D
        case 0xDAD4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDDF6:  BNE DoItemDescription
        case 0xDAD6: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDDF8:  CMP #INV_STAFF
        case 0xDAD8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDDFA:  BNE ChkHarp
        case 0xDADA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDDFC:  JSR Dowindow
        case 0xDADD: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDE00:  LDA #$3E
        case 0xDADF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDE02:  BNE DoItemDescription
        case 0xDAE1: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDE04:  CMP #INV_HARP
        case 0xDAE3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDE06:  BNE ChkBelt
        case 0xDAE5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDE08:  JSR Dowindow
        case 0xDAE8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDE0C:  JSR DoDialogLoBlock
        case 0xDAEB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDE10:  LDA #MSC_SILV_HARP
        case 0xDAED: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDE12:  BRK
        case 0xDAEE: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDE15:  BRK
        case 0xDAEF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDE18:  LDA MapNumber
        case 0xDAF2: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDE1A:  CMP #MAP_OVERWORLD
        case 0xDAF4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDE1C:  BNE HarpFail
        case 0xDAF6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDE1E:  JSR UpdateRandNum
        case 0xDAF9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDE21:  LDA RandNumUB
        case 0xDAFC: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDE23:  AND #$07
        case 0xDAFE: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDE25:  CMP #$05
        case 0xDB00: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LDE27:  BEQ HarpRNGLoop
        case 0xDB02: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDE29:  CMP #$07
        case 0xDB04: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LDE2B:  BEQ HarpRNGLoop
        case 0xDB06: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LDE2D:  PHA
        case 0xDB07: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDE2E:  LDA #WND_DIALOG
        case 0xDB09: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDE30:  JSR RemoveWindow
        case 0xDB0C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDE33:  LDA #WND_CMD_NONCMB
        case 0xDB0E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDE35:  JSR RemoveWindow
        case 0xDB11: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDE38:  LDA #WND_POPUP
        case 0xDB13: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDE3A:  JSR RemoveWindow
        case 0xDB16: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LDE3D:  PLA
        case 0xDB17: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LDE3E:  JMP InitFight
        case 0xDB1A: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LDE41:  LDX MapNumber
        case 0xDB1D: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LDE43:  LDA ResumeMusicTbl,X
        case 0xDB20: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDE46:  BRK
        case 0xDB21: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDE49:  JSR DoDialogLoBlock
        case 0xDB24: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDE4D:  JMP ResumeGamePlay
        case 0xDB27: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDE50:  CMP #INV_BELT
        case 0xDB29: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDE52:  BNE ChkNecklace
        case 0xDB2B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDE54:  JSR Dowindow
        case 0xDB2E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDE58:  JSR WearCursedItem
        case 0xDB31: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LDE5B:  LDX MapNumber
        case 0xDB34: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LDE5D:  LDA ResumeMusicTbl,X
        case 0xDB37: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDE60:  BRK
        case 0xDB38: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDE63:  JMP ResumeGamePlay
        case 0xDB3B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDE66:  CMP #INV_NECKLACE
        case 0xDB3D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDE68:  BNE ChkDrop
        case 0xDB3F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDE6A:  JSR Dowindow
        case 0xDB42: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDE6E:  JSR ChkDeathNecklace
        case 0xDB45: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LDE71:  LDX MapNumber
        case 0xDB48: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LDE73:  LDA ResumeMusicTbl,X
        case 0xDB4B: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDE76:  BRK
        case 0xDB4C: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDE79:  JMP ResumeGamePlay
        case 0xDB4F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDE7C:  CMP #INV_DROP
        case 0xDB51: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LDE7E:  BEQ +
        case 0xDB53: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LDE80:  JMP ChkLove
        case 0xDB56: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDE87:  JSR DoDialogLoBlock
        case 0xDB59: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDE8B:  LDA MapNumber
        case 0xDB5C: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDE8D:  CMP #MAP_OVERWORLD
        case 0xDB5E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDE8F:  BNE RainbowFail
        case 0xDB60: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDE91:  LDA CharXPos
        case 0xDB63: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDE93:  CMP #$41
        case 0xDB65: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDE95:  BNE RainbowFail
        case 0xDB67: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDE97:  LDA CharYPos
        case 0xDB6A: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDE99:  CMP #$31
        case 0xDB6C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDE9B:  BNE RainbowFail
        case 0xDB6E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDE9D:  LDA ModsnSpells
        case 0xDB71: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDE9F:  AND #F_RNBW_BRDG
        case 0xDB73: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDEA1:  BNE RainbowFail
        case 0xDB75: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDEA3:  LDA #WND_DIALOG
        case 0xDB77: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDEA5:  JSR RemoveWindow
        case 0xDB7A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDEA8:  LDA #WND_CMD_NONCMB
        case 0xDB7C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDEAA:  JSR RemoveWindow
        case 0xDB7F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDEAD:  LDA #WND_POPUP
        case 0xDB81: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDEAF:  JSR RemoveWindow
        case 0xDB84: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDEB2:  LDA #MSC_RNBW_BRDG
        case 0xDB86: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDEB4:  BRK
        case 0xDB87: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDEB7:  LDA ModsnSpells
        case 0xDB8A: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDEB9:  ORA #F_RNBW_BRDG
        case 0xDB8C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDEBB:  STA ModsnSpells
        case 0xDB8F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDEBD:  LDA #$FE
        case 0xDB91: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDEBF:  STA XPosFromCenter
        case 0xDB94: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDEC1:  LDA #$00
        case 0xDB96: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDEC3:  STA YPosFromCenter
        case 0xDB99: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDEC5:  LDA #$04
        case 0xDB9B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDEC7:  STA BridgeFlashCntr
        case 0xDB9E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDEC9:  LDA #$21
        case 0xDBA0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDECB:  STA PPUDataByte
        case 0xDBA3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDECD:  JSR WaitForNMI
        case 0xDBA6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDED0:  JSR WaitForNMI
        case 0xDBA9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDED3:  JSR WaitForNMI
        case 0xDBAC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDED6:  JSR WaitForNMI
        case 0xDBAF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDED9:  JSR WaitForNMI
        case 0xDBB2: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDEDC:  LDA #$03
        case 0xDBB4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDEDE:  STA PPUAddrLB
        case 0xDBB7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDEE0:  LDA #$3F
        case 0xDBB9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDEE2:  STA PPUAddrUB
        case 0xDBBC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDEE4:  JSR AddPPUBufEntry
        case 0xDBBF: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LDEE7:  INC PPUDataByte
        case 0xDBC2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDEE9:  LDA PPUDataByte
        case 0xDBC5: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDEEB:  CMP #$12
        case 0xDBC7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LDEED:  BEQ BridgeAnimDone
        case 0xDBC9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDEEF:  CMP #$2D
        case 0xDBCB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDEF1:  BNE BuildBridgeLoop1
        case 0xDBCD: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LDEF3:  DEC BridgeFlashCntr
        case 0xDBD0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDEF5:  BNE BuildBridgeLoop2
        case 0xDBD2: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDEF7:  LDA #$11
        case 0xDBD4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDEF9:  STA PPUDataByte
        case 0xDBD7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDEFB:  BNE BuildBridgeLoop1
        case 0xDBD9: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDEFD:  BRK
        case 0xDBDA: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDF00:  LDA #MSC_OUTDOOR
        case 0xDBDC: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDF02:  BRK
        case 0xDBDD: cycles_to_run -= 3; reg_PC = 0x0000; return; // LDF05:  JMP ModMapBlock
        case 0xDBE0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDF08:  LDA #$05
        case 0xDBE2: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LDF0A:  JMP DoFinalDialog
        case 0xDBE5: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDF0D:  CMP #INV_LOVE
        case 0xDBE7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDF0F:  BNE EndItemChecks
        case 0xDBE9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LDF11:  JSR Dowindow
        case 0xDBEC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDF15:  LDA DisplayedLevel
        case 0xDBEF: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDF17:  CMP #LVL_30
        case 0xDBF1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDF19:  BNE DoLoveExp
        case 0xDBF3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDF1B:  JSR DoDialogHiBlock
        case 0xDBF6: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LDF1F:  JMP ChkLoveMap
        case 0xDBF9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDF22:  JSR GetExpRemaining
        case 0xDBFC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDF25:  JSR DoDialogLoBlock
        case 0xDBFF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDF29:  LDA MapNumber
        case 0xDC02: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LDF2B:  CMP #MAP_OVERWORLD
        case 0xDC04: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDF2D:  BNE LastLoveDialog
        case 0xDC06: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDF2F:  JSR DoDialogLoBlock
        case 0xDC09: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDF33:  JSR DoDialogLoBlock
        case 0xDC0C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDF37:  LDA CharYPos
        case 0xDC0F: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LDF39:  SEC
        case 0xDC10: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LDF3A:  SBC #$2B
        case 0xDC12: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LDF3C:  BCS YDiffDialog
        case 0xDC14: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDF3E:  EOR #$FF
        case 0xDC16: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDF40:  STA GenByte00
        case 0xDC19: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LDF42:  INC GenByte00
        case 0xDC1C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDF44:  LDA #$DF
        case 0xDC1E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDF46:  BNE DoNorthSouthDialog
        case 0xDC20: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDF48:  STA GenByte00
        case 0xDC23: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDF4A:  LDA #$DE
        case 0xDC25: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LDF4C:  LDX #$00
        case 0xDC27: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LDF4E:  STX GenByte01
        case 0xDC2A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDF50:  JSR DoMidDialog
        case 0xDC2D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDF53:  LDA CharXPos
        case 0xDC30: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LDF55:  SEC
        case 0xDC31: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LDF56:  SBC #$2B
        case 0xDC33: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LDF58:  BCS XDiffDialog
        case 0xDC35: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDF5A:  EOR #$FF
        case 0xDC37: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDF5C:  STA GenByte00
        case 0xDC3A: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LDF5E:  INC GenByte00
        case 0xDC3D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDF60:  LDA #$E0
        case 0xDC3F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDF62:  BNE DoEastWestDialog
        case 0xDC41: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDF64:  STA GenByte00
        case 0xDC44: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDF66:  LDA #$E1
        case 0xDC46: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LDF68:  LDX #$00
        case 0xDC48: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LDF6A:  STX GenByte01
        case 0xDC4B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDF6C:  JSR DoMidDialog
        case 0xDC4E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDF6F:  LDA #$BD
        case 0xDC50: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LDF71:  JMP DoFinalDialog
        case 0xDC53: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LDF74:  JMP ResumeGamePlay
        case 0xDC56: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LDF77:  LDX #$00
        case 0xDC58: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDF79:  LDA #$01
        case 0xDC5A: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LDF7B:  STA DescBuf,X
        case 0xDC5D: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LDF7D:  INX
        case 0xDC5E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDF7E:  LDA InventoryHerbs
        case 0xDC61: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LDF80:  BEQ +
        case 0xDC63: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDF82:  LDA #$02
        case 0xDC65: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LDF84:  STA DescBuf,X
        case 0xDC68: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LDF86:  INX
        case 0xDC69: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LDF89:  BEQ +
        case 0xDC6B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDF8B:  LDA #$03
        case 0xDC6D: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LDF8D:  STA DescBuf,X
        case 0xDC70: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LDF8F:  INX
        case 0xDC71: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LDF92:  LDA InventoryPtr,Y
        case 0xDC74: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDF95:  AND #$0F
        case 0xDC76: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LDF97:  BEQ +
        case 0xDC78: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LDF99:  CLC
        case 0xDC79: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LDF9A:  ADC #$03
        case 0xDC7B: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LDF9C:  STA DescBuf,X
        case 0xDC7E: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LDF9E:  INX
        case 0xDC7F: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDFA2:  AND #$F0
        case 0xDC81: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LDFA4:  BEQ +
        case 0xDC83: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LDFA6:  LSR
        case 0xDC84: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LDFA7:  LSR
        case 0xDC85: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LDFA8:  LSR
        case 0xDC86: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LDFA9:  LSR
        case 0xDC87: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LDFAA:  ADC #$03
        case 0xDC89: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LDFAC:  STA DescBuf,X
        case 0xDC8C: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LDFAE:  INX
        case 0xDC8D: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LDFB0:  CPY #$04
        case 0xDC8F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDFB2:  BNE InvListLoop
        case 0xDC91: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDFB4:  LDA #$FF
        case 0xDC93: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LDFB6:  STA DescBuf,X
        case 0xDC96: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LDFB8:  RTS
        case 0xDC97: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDFB9:  LDA ModsnSpells
        case 0xDC9A: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDFBB:  AND #F_DRGSCALE
        case 0xDC9C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDFBD:  BNE DrgScaleDialog
        case 0xDC9E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDFBF:  LDA ModsnSpells
        case 0xDCA1: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDFC1:  ORA #F_DRGSCALE
        case 0xDCA3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDFC3:  STA ModsnSpells
        case 0xDCA6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDFC5:  JSR DoDialogLoBlock
        case 0xDCA9: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LDFC9:  JMP LoadStats
        case 0xDCAC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDFCC:  JSR DoDialogLoBlock
        case 0xDCAF: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LDFD0:  RTS
        case 0xDCB0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDFD1:  LDA ModsnSpells
        case 0xDCB3: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDFD3:  AND #F_FTR_RING
        case 0xDCB5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LDFD5:  BNE AlreadyWearingRing
        case 0xDCB7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDFD7:  LDA ModsnSpells
        case 0xDCBA: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDFD9:  ORA #F_FTR_RING
        case 0xDCBC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDFDB:  STA ModsnSpells
        case 0xDCBF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDFDD:  JSR DoDialogLoBlock
        case 0xDCC2: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LDFE1:  RTS
        case 0xDCC3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDFE2:  JSR DoDialogLoBlock
        case 0xDCC6: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LDFE6:  RTS
        case 0xDCC7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDFE7:  LDA #$3A
        case 0xDCC9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LDFE9:  JSR GetDescriptionByte
        case 0xDCCC: cycles_to_run -= 3; cpu_bit(bus_read(0x0000)); reg_PC += 3; return; // LDFEC:  BIT ModsnSpells
        case 0xDCCF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_V) { reg_PC = 0xC07E; return; } // LDFEE:  BVS DoCursedDialog
        case 0xDCD1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LDFF0:  LDA ModsnSpells
        case 0xDCD4: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDFF2:  ORA #F_CRSD_BELT
        case 0xDCD6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LDFF4:  STA ModsnSpells
        case 0xDCD9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LDFF6:  JSR DoDialogLoBlock
        case 0xDCDC: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LDFFA:  LDA #MSC_CURSED
        case 0xDCDE: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDFFC:  BRK
        case 0xDCDF: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LDFFF:  BRK
        case 0xDCE0: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LE002:  RTS
        case 0xDCE1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE003:  JSR DoDialogLoBlock
        case 0xDCE4: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE007:  JMP PlayCursedMusic
        case 0xDCE7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE00A:  LDA #$3C
        case 0xDCE9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE00C:  JSR GetDescriptionByte
        case 0xDCEC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE00F:  LDA ModsnSpells
        case 0xDCEF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0xC07E; return; } // LE011:  BMI DoCursedDialog
        case 0xDCF1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE013:  LDA ModsnSpells
        case 0xDCF4: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE015:  ORA #F_DTH_NECKLACE
        case 0xDCF6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE017:  STA ModsnSpells
        case 0xDCF9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE019:  BNE PlayerCursed
        case 0xDCFB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE01B:  STA GenByte3E
        case 0xDCFE: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LE01D:  LDX #$00
        case 0xDD00: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LE01F:  LDA InventorySlot12,X
        case 0xDD03: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE021:  AND #$0F
        case 0xDD05: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE023:  BNE ChkUpperInvNibble
        case 0xDD07: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LE025:  LDA InventorySlot12,X
        case 0xDD0A: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE027:  AND #$F0
        case 0xDD0C: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE029:  ORA GenByte3E
        case 0xDD0F: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LE02B:  STA InventorySlot12,X
        case 0xDD12: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LE02D:  RTS
        case 0xDD13: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LE02E:  LDA InventorySlot12,X
        case 0xDD16: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE030:  AND #$F0
        case 0xDD18: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE032:  BNE ChkNextInvSlot
        case 0xDD1A: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LE034:  ASL GenByte3E
        case 0xDD1D: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LE036:  ASL GenByte3E
        case 0xDD20: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LE038:  ASL GenByte3E
        case 0xDD23: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LE03A:  ASL GenByte3E
        case 0xDD26: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LE03C:  LDA InventorySlot12,X
        case 0xDD29: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE03E:  AND #$0F
        case 0xDD2B: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE040:  ORA GenByte3E
        case 0xDD2E: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LE042:  STA InventorySlot12,X
        case 0xDD31: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LE044:  RTS
        case 0xDD32: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LE045:  INX
        case 0xDD33: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LE046:  CPX #INV_FULL
        case 0xDD35: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE048:  BNE AddInvLoop
        case 0xDD37: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LE04A:  RTS
        case 0xDD38: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE04B:  JSR CheckForInvItem
        case 0xDD3B: cycles_to_run -= 2; reg_A &= bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LE04E:  AND InventoryPtr,Y
        case 0xDD3E: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LE051:  STA InventoryPtr,Y
        case 0xDD41: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LE054:  RTS
        case 0xDD42: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LE055:  LDY #$00
        case 0xDD44: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE057:  STA GenByte3C
        case 0xDD47: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LE059:  LDA InventoryPtr,Y
        case 0xDD4A: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE05C:  AND #NBL_LOWER
        case 0xDD4C: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LE05E:  CMP GenByte3C
        case 0xDD4F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE060:  BNE +
        case 0xDD51: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE062:  LDA #ITM_FOUND_LO
        case 0xDD53: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LE064:  RTS
        case 0xDD54: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE068:  LSR
        case 0xDD55: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE069:  LSR
        case 0xDD56: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE06A:  LSR
        case 0xDD57: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE06B:  LSR
        case 0xDD58: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LE06C:  CMP GenByte3C
        case 0xDD5B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE06E:  BNE +
        case 0xDD5D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE070:  LDA #ITM_FOUND_HI
        case 0xDD5F: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LE072:  RTS
        case 0xDD60: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LE074:  CPY #$04
        case 0xDD62: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE076:  BNE InvCheckLoop
        case 0xDD64: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE078:  LDA #ITM_NOT_FOUND
        case 0xDD66: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LE07A:  RTS
        case 0xDD67: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE07B:  STA DialogTemp
        case 0xDD6A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE07D:  JSR DoDialogLoBlock
        case 0xDD6D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE081:  JSR Dowindow
        case 0xDD70: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE085:  BEQ PlayerDiscards
        case 0xDD72: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE087:  LDA DialogTemp
        case 0xDD75: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LE089:  CLC
        case 0xDD76: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE08A:  ADC #$31
        case 0xDD78: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE08C:  JSR GetDescriptionByte
        case 0xDD7B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE08F:  LDA #$CD
        case 0xDD7D: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE091:  JMP DoFinalDialog
        case 0xDD80: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE094:  JSR DoDialogLoBlock
        case 0xDD83: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE098:  JSR CreateInvList
        case 0xDD86: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE09B:  JSR Dowindow
        case 0xDD89: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE09F:  CMP #WND_ABORT
        case 0xDD8B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE0A1:  BEQ PlayerNoDiscard
        case 0xDD8D: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LE0A3:  TAX
        case 0xDD8E: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LE0A4:  LDA DescBuf+1,X
        case 0xDD91: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LE0A6:  LDY #$00
        case 0xDD93: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(addr_abs_y(0xC07E, nullptr))); reg_PC += 3; return; // LE0A8:  CMP NonDiscardTbl,Y
        case 0xDD96: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE0AB:  BNE NextDiscardChk
        case 0xDD98: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE0AD:  JSR DoDialogLoBlock
        case 0xDD9B: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE0B1:  JMP PlayerDiscards
        case 0xDD9E: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LE0B4:  INY
        case 0xDD9F: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LE0B5:  CPY #$09
        case 0xDDA1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE0B7:  BNE DiscardChkLoop
        case 0xDDA3: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE0B9:  CMP #INV_BELT
        case 0xDDA5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE0BB:  BNE ChkDiscardNecklace
        case 0xDDA7: cycles_to_run -= 3; cpu_bit(bus_read(0x0000)); reg_PC += 3; return; // LE0BD:  BIT ModsnSpells
        case 0xDDAA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_V)) { reg_PC = 0xC07E; return; } // LE0BF:  BVC ChkDiscardNecklace
        case 0xDDAC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE0C1:  JSR DoDialogLoBlock
        case 0xDDAF: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE0C5:  JMP PlayerDiscards
        case 0xDDB2: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LE0C8:  LDA DescBuf+1,X
        case 0xDDB5: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE0CA:  CMP #INV_NECKLACE
        case 0xDDB7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE0CC:  BNE +
        case 0xDDB9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE0CE:  LDA ModsnSpells
        case 0xDDBC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0xC07E; return; } // LE0D0:  BMI BodyCursedDialog
        case 0xDDBE: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LE0D4:  PHA
        case 0xDDBF: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LE0D5:  CLC
        case 0xDDC0: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE0D6:  ADC #$2E
        case 0xDDC2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE0D8:  JSR GetDescriptionByte
        case 0xDDC5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE0DB:  JSR DoDialogLoBlock
        case 0xDDC8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE0DF:  LDA DialogTemp
        case 0xDDCB: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LE0E1:  CLC
        case 0xDDCC: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE0E2:  ADC #$31
        case 0xDDCE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE0E4:  JSR GetDescriptionByte
        case 0xDDD1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE0E7:  JSR DoDialogLoBlock
        case 0xDDD4: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LE0EB:  PLA
        case 0xDDD5: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LE0EC:  SEC
        case 0xDDD6: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LE0ED:  SBC #$03
        case 0xDDD8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE0EF:  JSR RemoveInvItem
        case 0xDDDB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE0F2:  LDA DescTemp
        case 0xDDDE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE0F4:  JSR AddInvItem
        case 0xDDE1: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LE0F7:  JMP ResumeGamePlay
        case 0xDDE4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE103:  JSR Dowindow
        case 0xDDE7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE107:  JSR DoDialogLoBlock
        case 0xDDEA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE10B:  LDA MapNumber
        case 0xDDED: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE10D:  CMP #MAP_OVERWORLD
        case 0xDDEF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE10F:  BNE NextSearch
        case 0xDDF1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE111:  LDA CharXPos
        case 0xDDF4: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE113:  CMP #$53
        case 0xDDF6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE115:  BNE NextSearch
        case 0xDDF8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE117:  LDA CharYPos
        case 0xDDFB: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE119:  CMP #$71
        case 0xDDFD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE11B:  BNE NextSearch
        case 0xDDFF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE11D:  LDA #ITM_ERDRICK_TKN
        case 0xDE01: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE11F:  STA DescTemp
        case 0xDE04: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE121:  JSR CheckForInvItem
        case 0xDE07: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE124:  CMP #ITM_NOT_FOUND
        case 0xDE09: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE126:  BEQ ErdrickTknFound
        case 0xDE0B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE128:  LDA #$D3
        case 0xDE0D: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE12A:  JMP DoFinalDialog
        case 0xDE10: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE12D:  LDA DescTemp
        case 0xDE13: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LE12F:  CLC
        case 0xDE14: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE130:  ADC #$31
        case 0xDE16: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE132:  JSR GetDescriptionByte
        case 0xDE19: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE135:  JSR DoDialogLoBlock
        case 0xDE1C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE139:  LDA DescTemp
        case 0xDE1F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE13B:  JSR AddInvItem
        case 0xDE22: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LE13E:  CPX #INV_FULL
        case 0xDE24: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LE140:  BEQ +
        case 0xDE26: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LE142:  JMP ResumeGamePlay
        case 0xDE29: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE147:  JMP DiscardItem
        case 0xDE2C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE14A:  LDA MapNumber
        case 0xDE2F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE14C:  CMP #MAP_KOL
        case 0xDE31: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE14E:  BNE SrchEdrckArmor
        case 0xDE33: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE150:  LDA CharXPos
        case 0xDE36: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE152:  CMP #$09
        case 0xDE38: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE154:  BNE SrchEdrckArmor
        case 0xDE3A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE156:  LDA CharYPos
        case 0xDE3D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE158:  CMP #$06
        case 0xDE3F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE15A:  BNE SrchEdrckArmor
        case 0xDE41: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE15C:  LDA #ITM_FRY_FLUTE
        case 0xDE43: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE15E:  BNE FoundItem
        case 0xDE45: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE160:  LDA MapNumber
        case 0xDE48: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE162:  CMP #MAP_HAUKSNESS
        case 0xDE4A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE164:  BNE SrchPassage
        case 0xDE4C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE166:  LDA CharXPos
        case 0xDE4F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE168:  CMP #$12
        case 0xDE51: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE16A:  BNE SrchPassage
        case 0xDE53: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE16C:  LDA CharYPos
        case 0xDE56: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE16E:  CMP #$0C
        case 0xDE58: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE170:  BNE SrchPassage
        case 0xDE5A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE172:  LDA EqippedItems
        case 0xDE5D: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE174:  AND #AR_ARMOR
        case 0xDE5F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE176:  CMP #AR_ERDK_ARMR
        case 0xDE61: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE178:  BEQ ItemAlreadyFound
        case 0xDE63: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE17A:  LDA EqippedItems
        case 0xDE66: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE17C:  ORA #AR_ERDK_ARMR
        case 0xDE68: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE17E:  STA EqippedItems
        case 0xDE6B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE180:  LDA #$28
        case 0xDE6D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE182:  JSR GetDescriptionByte
        case 0xDE70: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE185:  LDA #$D5
        case 0xDE72: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE187:  JMP DoFinalDialog
        case 0xDE75: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE18A:  LDA MapNumber
        case 0xDE78: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE18C:  CMP #MAP_DLCSTL_GF
        case 0xDE7A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE18E:  BNE ChkSearchTrsr
        case 0xDE7C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE190:  LDA CharXPos
        case 0xDE7F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE192:  CMP #$0A
        case 0xDE81: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE194:  BNE ChkSearchTrsr
        case 0xDE83: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE196:  LDA CharYPos
        case 0xDE86: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE198:  CMP #$03
        case 0xDE88: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE19A:  BNE +
        case 0xDE8A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE19C:  LDA #$D6
        case 0xDE8C: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE19E:  JMP DoFinalDialog
        case 0xDE8F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE1A3:  BNE ChkSearchTrsr
        case 0xDE91: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE1A5:  LDA ModsnSpells
        case 0xDE94: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE1A7:  AND #F_PSG_FOUND
        case 0xDE96: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE1A9:  BNE ChkSearchTrsr
        case 0xDE98: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE1AB:  LDA ModsnSpells
        case 0xDE9B: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE1AD:  ORA #F_PSG_FOUND
        case 0xDE9D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE1AF:  STA ModsnSpells
        case 0xDEA0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE1B1:  LDA #$0F
        case 0xDEA2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE1B3:  JSR GetDescriptionByte
        case 0xDEA5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE1B6:  JSR DoDialogLoBlock
        case 0xDEA8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE1BA:  LDA #$00
        case 0xDEAA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE1BC:  STA YPosFromCenter
        case 0xDEAD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE1BE:  STA XPosFromCenter
        case 0xDEB0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE1C0:  STA BlkRemoveFlgs
        case 0xDEB3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE1C2:  JSR ModMapBlock
        case 0xDEB6: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LE1C5:  JMP ResumeGamePlay
        case 0xDEB9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE1C8:  LDA CharXPos
        case 0xDEBC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE1CA:  STA XTarget
        case 0xDEBF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE1CC:  LDA CharYPos
        case 0xDEC2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE1CE:  STA YTarget
        case 0xDEC5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE1D0:  JSR GetBlockID
        case 0xDEC8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE1D3:  LDA TargetResults
        case 0xDECB: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE1D5:  CMP #BLK_CHEST
        case 0xDECD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE1D7:  BNE +
        case 0xDECF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE1D9:  LDA #$D4
        case 0xDED1: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE1DB:  JMP DoFinalDialog
        case 0xDED4: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE1E0:  JMP DoFinalDialog
        case 0xDED7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE1E3:  JSR Dowindow
        case 0xDEDA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE1E7:  LDA CharXPos
        case 0xDEDD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE1E9:  STA XTarget
        case 0xDEE0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE1EB:  LDA CharYPos
        case 0xDEE3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE1ED:  STA YTarget
        case 0xDEE6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE1EF:  JSR GetBlockID
        case 0xDEE9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE1F2:  LDA TargetResults
        case 0xDEEC: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE1F4:  CMP #BLK_CHEST
        case 0xDEEE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE1F6:  BEQ FoundTreasure
        case 0xDEF0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE1F8:  LDA #$D7
        case 0xDEF2: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE1FA:  JMP DoFinalDialog
        case 0xDEF5: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE1FD:  BRK
        case 0xDEF6: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LE200:  LDY #$00
        case 0xDEF8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE202:  LDA MapNumber
        case 0xDEFB: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(addr_abs_y(0x0000, nullptr))); reg_PC += 3; return; // LE204:  CMP TrsrArray,Y
        case 0xDEFE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE207:  BNE NextTrsrChest
        case 0xDF00: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE209:  LDA CharXPos
        case 0xDF03: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(addr_abs_y(0x0000, nullptr))); reg_PC += 3; return; // LE20B:  CMP TrsrArray+1,Y
        case 0xDF06: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE20E:  BNE NextTrsrChest
        case 0xDF08: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE210:  LDA CharYPos
        case 0xDF0B: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(addr_abs_y(0x0000, nullptr))); reg_PC += 3; return; // LE212:  CMP TrsrArray+2,Y
        case 0xDF0E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE215:  BEQ ChkTrsrKey
        case 0xDF10: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LE217:  INY
        case 0xDF11: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LE218:  INY
        case 0xDF12: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LE219:  INY
        case 0xDF13: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LE21A:  INY
        case 0xDF14: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LE21B:  CPY #$7C
        case 0xDF16: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE21D:  BNE ChkTrsrTblLoop
        case 0xDF18: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE21F:  BEQ NoTrsrChest
        case 0xDF1A: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LE221:  LDA TrsrArray+3,Y
        case 0xDF1D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE224:  STA DialogTemp
        case 0xDF20: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE226:  CMP #TRSR_KEY
        case 0xDF22: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE228:  BNE ChkTrsrHerb
        case 0xDF24: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE22A:  LDA InventoryKeys
        case 0xDF27: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE22C:  CMP #$06
        case 0xDF29: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE22E:  BNE TrsrGetKey
        case 0xDF2B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE230:  JSR GetTreasure
        case 0xDF2E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE233:  LDA #$DA
        case 0xDF30: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE235:  JMP DoFinalDialog
        case 0xDF33: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LE238:  INC InventoryKeys
        case 0xDF36: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE23A:  JSR GetTreasure
        case 0xDF39: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE23D:  LDA #$D9
        case 0xDF3B: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE23F:  JMP DoFinalDialog
        case 0xDF3E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE242:  CMP #TRSR_HERB
        case 0xDF40: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE244:  BNE ChkTrsrNecklace
        case 0xDF42: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE246:  LDA InventoryHerbs
        case 0xDF45: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE248:  CMP #$06
        case 0xDF47: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE24A:  BEQ GetTreasureChest1
        case 0xDF49: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LE24C:  INC InventoryHerbs
        case 0xDF4C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE24E:  BNE GetTreasureChest2
        case 0xDF4E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE250:  CMP #TRSR_NCK
        case 0xDF50: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE252:  BNE ChkTrsrErdSword
        case 0xDF52: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE254:  LDA PlayerFlags
        case 0xDF55: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE256:  AND #F_DTH_NCK_FOUND
        case 0xDF57: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE258:  BNE GetDthNeckGold
        case 0xDF59: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE25A:  JSR UpdateRandNum
        case 0xDF5C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE25D:  LDA RandNumUB
        case 0xDF5F: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE25F:  AND #$1F
        case 0xDF61: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE261:  BNE GetDthNeckGold
        case 0xDF63: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE263:  LDA PlayerFlags
        case 0xDF66: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE265:  ORA #F_DTH_NCK_FOUND
        case 0xDF68: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE267:  STA PlayerFlags
        case 0xDF6B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE269:  JSR GetTreasure
        case 0xDF6E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE26C:  LDA DescTemp
        case 0xDF71: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LE26E:  SEC
        case 0xDF72: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LE26F:  SBC #$03
        case 0xDF74: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE271:  STA DescTemp
        case 0xDF77: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE273:  JSR DoDialogLoBlock
        case 0xDF7A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE277:  LDA DescTemp
        case 0xDF7D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE279:  JSR AddInvItem
        case 0xDF80: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LE27C:  CPX #INV_FULL
        case 0xDF82: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE27E:  BEQ TrsrInvFull
        case 0xDF84: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LE280:  JMP ResumeGamePlay
        case 0xDF87: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE283:  LDA DescTemp
        case 0xDF8A: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE285:  JMP DiscardItem
        case 0xDF8D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE288:  LDA #$1F
        case 0xDF8F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE28A:  STA RndGoldBits
        case 0xDF92: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE28C:  LDA #$64
        case 0xDF94: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE28E:  STA TrsrGoldLB
        case 0xDF97: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE290:  LDA #$00
        case 0xDF99: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE292:  STA TrsrGoldUB
        case 0xDF9C: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE294:  JMP GetTrsrGold
        case 0xDF9F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE297:  CMP #TRSR_ERSD
        case 0xDFA1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE299:  BNE ChkTrsrHarp
        case 0xDFA3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE29B:  LDA EqippedItems
        case 0xDFA6: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE29D:  AND #WP_WEAPONS
        case 0xDFA8: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE29F:  CMP #WP_ERDK_SWRD
        case 0xDFAA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE2A1:  BNE +
        case 0xDFAC: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE2A3:  JMP GetTreasureChest1
        case 0xDFAF: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE2A8:  ORA #WP_ERDK_SWRD
        case 0xDFB1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE2AA:  STA EqippedItems
        case 0xDFB4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE2AC:  JSR GetTreasure
        case 0xDFB7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE2AF:  LDA #$21
        case 0xDFB9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE2B1:  JSR GetDescriptionByte
        case 0xDFBC: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE2B4:  LDA #$D9
        case 0xDFBE: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE2B6:  JMP DoFinalDialog
        case 0xDFC1: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE2B9:  CMP #TRSR_HARP
        case 0xDFC3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE2BB:  BNE ChkTrsrStones
        case 0xDFC5: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE2BD:  LDA #ITM_SLVR_HARP
        case 0xDFC7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE2BF:  JSR CheckForInvItem
        case 0xDFCA: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE2C2:  CMP #ITM_NOT_FOUND
        case 0xDFCC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE2C4:  BNE GetTreasureChest3
        case 0xDFCE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE2C6:  LDA #ITM_STFF_RAIN
        case 0xDFD0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE2C8:  JSR CheckForInvItem
        case 0xDFD3: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE2CB:  CMP #ITM_NOT_FOUND
        case 0xDFD5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE2CD:  BNE GetTreasureChest3
        case 0xDFD7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE2CF:  LDA #ITM_RNBW_DROP
        case 0xDFD9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE2D1:  JSR CheckForInvItem
        case 0xDFDC: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE2D4:  CMP #ITM_NOT_FOUND
        case 0xDFDE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE2D6:  BNE GetTreasureChest3
        case 0xDFE0: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE2D8:  JMP SetTrsrDescByte
        case 0xDFE3: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE2DB:  CMP #TRSR_SUN
        case 0xDFE5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE2DD:  BNE ChkTrsrNonGold
        case 0xDFE7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE2DF:  LDA #ITM_STNS_SNLGHT
        case 0xDFE9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE2E1:  JSR CheckForInvItem
        case 0xDFEC: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE2E4:  CMP #ITM_NOT_FOUND
        case 0xDFEE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE2E6:  BNE GetTreasureChest3
        case 0xDFF0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE2E8:  BEQ ChkRnbwDrop
        case 0xDFF2: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE2EA:  CMP #TRSR_ERSD
        case 0xDFF4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LE2EC:  BCS ChkTrsrGold1
        case 0xDFF6: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE2EE:  JMP SetTrsrDescByte
        case 0xDFF9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE2F1:  CMP #TRSR_GLD1
        case 0xDFFB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE2F3:  BNE ChkTrsrGold2
        case 0xDFFD: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE2F5:  LDA #$0F
        case 0xDFFF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE2F7:  STA RndGoldBits
        case 0xE002: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE2F9:  LDA #$05
        case 0xE004: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE2FB:  STA TrsrGoldLB
        case 0xE007: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE2FD:  LDA #$00
        case 0xE009: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE2FF:  STA TrsrGoldUB
        case 0xE00C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE301:  BEQ GetTrsrGold
        case 0xE00E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE303:  CMP #TRSR_GLD2
        case 0xE010: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE305:  BNE ChkTrsrGold3
        case 0xE012: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE307:  LDA #$07
        case 0xE014: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE309:  STA RndGoldBits
        case 0xE017: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE30B:  LDA #$06
        case 0xE019: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE30D:  STA TrsrGoldLB
        case 0xE01C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE30F:  LDA #$00
        case 0xE01E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE311:  STA TrsrGoldUB
        case 0xE021: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE313:  BEQ GetTrsrGold
        case 0xE023: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE315:  CMP #TRSR_GLD3
        case 0xE025: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE317:  BNE ChkTrsrGold4
        case 0xE027: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE319:  LDA #$07
        case 0xE029: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE31B:  STA RndGoldBits
        case 0xE02C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE31D:  LDA #$0A
        case 0xE02E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE31F:  STA TrsrGoldLB
        case 0xE031: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE321:  LDA #$00
        case 0xE033: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE323:  STA TrsrGoldUB
        case 0xE036: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE325:  BEQ GetTrsrGold
        case 0xE038: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE327:  CMP #TRSR_GLD4
        case 0xE03A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE329:  BNE ChkTrsrGold5
        case 0xE03C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE32B:  LDA #$FF
        case 0xE03E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE32D:  STA RndGoldBits
        case 0xE041: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE32F:  LDA #$F4
        case 0xE043: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE331:  STA TrsrGoldLB
        case 0xE046: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE333:  LDA #$01
        case 0xE048: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE335:  STA TrsrGoldUB
        case 0xE04B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE337:  BNE GetTrsrGold
        case 0xE04D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE339:  CMP #TRSR_GLD5
        case 0xE04F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE33B:  BNE ChkTrsrErdTablet
        case 0xE051: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE33D:  LDA #$00
        case 0xE053: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE33F:  STA RndGoldBits
        case 0xE056: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE341:  STA TrsrGoldUB
        case 0xE059: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE343:  LDA #$78
        case 0xE05B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE345:  STA TrsrGoldLB
        case 0xE05E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE347:  BNE GetTrsrGold
        case 0xE060: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE349:  JSR GetTreasure
        case 0xE063: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LE34C:  LDX #$00
        case 0xE065: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LE351:  STA DescBuf,X
        case 0xE068: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LE353:  INX
        case 0xE069: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LE354:  CPX #$02
        case 0xE06B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE356:  BNE -
        case 0xE06D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE358:  JSR DoDialogLoBlock
        case 0xE070: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE35C:  JSR DoDialogHiBlock
        case 0xE073: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LE360:  JMP ResumeGamePlay
        case 0xE076: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE365:  JSR UpdateRandNum
        case 0xE079: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE368:  LDA RandNumUB
        case 0xE07C: cycles_to_run -= 2; reg_A &= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE36A:  AND RndGoldBits
        case 0xE07F: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LE36C:  CLC
        case 0xE080: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LE36D:  ADC TrsrGoldLB
        case 0xE083: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE36F:  STA TrsrGoldLB
        case 0xE086: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE371:  LDA TrsrGoldUB
        case 0xE089: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE373:  ADC #$00
        case 0xE08B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE375:  STA TrsrGoldUB
        case 0xE08E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE377:  JSR GetTreasure
        case 0xE091: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE37A:  LDA GoldLB
        case 0xE094: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LE37C:  CLC
        case 0xE095: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LE37D:  ADC TrsrGoldLB
        case 0xE098: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE37F:  STA GoldLB
        case 0xE09B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE381:  LDA GoldUB
        case 0xE09E: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LE383:  ADC TrsrGoldUB
        case 0xE0A1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE385:  STA GoldUB
        case 0xE0A4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LE387:  BCC GainGoldDialog
        case 0xE0A6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE389:  LDA #$FF
        case 0xE0A8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE38B:  STA GoldLB
        case 0xE0AB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE38D:  STA GoldUB
        case 0xE0AE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE38F:  JSR DoDialogLoBlock
        case 0xE0B1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE393:  JSR Dowindow
        case 0xE0B4: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LE397:  JMP ResumeGamePlay
        case 0xE0B7: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LE39A:  LDX #$00
        case 0xE0B9: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LE39C:  LDA TrsrXPos,X
        case 0xE0BC: cycles_to_run -= 2; reg_A |= bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LE39F:  ORA TrsrYPos,X
        case 0xE0BF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE3A2:  BEQ TakeTreasure
        case 0xE0C1: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LE3A4:  INX
        case 0xE0C2: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LE3A5:  INX
        case 0xE0C3: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LE3A6:  CPX #$10
        case 0xE0C5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE3A8:  BNE ChkTrsrLoop
        case 0xE0C7: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LE3AA:  RTS
        case 0xE0C8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE3AB:  LDA CharXPos
        case 0xE0CB: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LE3AD:  STA TrsrXPos,X
        case 0xE0CE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE3B0:  LDA CharYPos
        case 0xE0D1: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LE3B2:  STA TrsrYPos,X
        case 0xE0D4: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE3B5:  LDA #$00
        case 0xE0D6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE3B7:  STA XPosFromCenter
        case 0xE0D9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE3B9:  STA YPosFromCenter
        case 0xE0DC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE3BB:  STA BlkRemoveFlgs
        case 0xE0DF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE3BD:  LDA #SFX_TRSR_CHEST
        case 0xE0E1: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE3BF:  BRK
        case 0xE0E2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE3C2:  JSR ModMapBlock
        case 0xE0E5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE3C5:  LDA DescTemp
        case 0xE0E8: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LE3C7:  CLC
        case 0xE0E9: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE3C8:  ADC #$2E
        case 0xE0EB: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE3CA:  JMP GetDescriptionByte
        case 0xE0EE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE3CD:  LDA EnNumber
        case 0xE0F1: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE3CF:  CMP #EN_DRAGONLORD2
        case 0xE0F3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE3D1:  BNE +
        case 0xE0F5: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LE3D3:  RTS
        case 0xE0F6: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LE3D6:  STX BlockClear
        case 0xE0F9: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LE3D8:  STX BlockCounter
        case 0xE0FC: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE3DA:  LDA #$0A
        case 0xE0FE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE3DC:  STA GenPtr3CLB
        case 0xE101: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE3DE:  LDA #$05
        case 0xE103: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE3E0:  STA GenPtr3CUB
        case 0xE106: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LE3E2:  LDY #$00
        case 0xE108: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE3E4:  LDA MapNumber
        case 0xE10B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE3E6:  CMP #MAP_OVERWORLD
        case 0xE10D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LE3E8:  BEQ +
        case 0xE10F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE3EA:  LDA #TL_BLANK_TILE1
        case 0xE111: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE3EC:  BNE ++
        case 0xE113: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LE3F3:  INX
        case 0xE114: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LE3F4:  INY
        case 0xE115: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LE3F5:  CPY #$0E
        case 0xE117: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE3F7:  BNE BGLoadRow
        case 0xE119: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE3F9:  LDA GenPtr3CLB
        case 0xE11C: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LE3FB:  CLC
        case 0xE11D: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE3FC:  ADC #$20
        case 0xE11F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE3FE:  STA GenPtr3CLB
        case 0xE122: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE400:  LDA GenPtr3CUB
        case 0xE125: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE402:  ADC #$00
        case 0xE127: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE404:  STA GenPtr3CUB
        case 0xE12A: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LE406:  CPX #$C4
        case 0xE12C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE408:  BNE BGGFXLoadLoop
        case 0xE12E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE40A:  JSR WaitForNMI
        case 0xE131: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE40D:  JSR DoSprites
        case 0xE134: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LE410:  LDX BlockCounter
        case 0xE137: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0xC07E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LE412:  LDA CmbtBGPlcmntTbl,X
        case 0xE13A: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE415:  LSR
        case 0xE13B: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE416:  LSR
        case 0xE13C: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE417:  LSR
        case 0xE13D: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE418:  LSR
        case 0xE13E: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LE419:  CLC
        case 0xE13F: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE41A:  ADC #$FA
        case 0xE141: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE41C:  STA XPosFromCenter
        case 0xE144: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LE41E:  CLC
        case 0xE145: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE41F:  ADC #$10
        case 0xE147: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE421:  STA XPosFromLeft
        case 0xE14A: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0xC07E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LE423:  LDA CmbtBGPlcmntTbl,X
        case 0xE14D: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE426:  AND #$0F
        case 0xE14F: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LE428:  CLC
        case 0xE150: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE429:  ADC #$FA
        case 0xE152: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE42B:  STA YPosFromCenter
        case 0xE155: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LE42D:  CLC
        case 0xE156: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE42E:  ADC #$0E
        case 0xE158: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE430:  STA YPosFromTop
        case 0xE15B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE432:  JSR CalcRAMBufAddr
        case 0xE15E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE435:  LDA PPUBufPtrLB
        case 0xE161: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE437:  STA BlockAddrLB
        case 0xE164: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE439:  LDA PPUBufPtrUB
        case 0xE167: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE43B:  STA BlockAddrUB
        case 0xE16A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE43D:  JSR WaitForNMI
        case 0xE16D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE440:  JSR AddBlocksToScreen
        case 0xE170: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LE443:  INC BlockCounter
        case 0xE173: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE445:  LDA BlockCounter
        case 0xE176: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE447:  CMP #$31
        case 0xE178: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE449:  BNE BGGFXScreenLoop
        case 0xE17A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE44B:  JSR WaitForNMI
        case 0xE17D: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LE44E:  RTS
        case 0xE17E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE44F:  LDA EnNumber
        case 0xE181: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE451:  CMP #EN_DRAGONLORD2
        case 0xE183: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE453:  BNE +
        case 0xE185: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE455:  LDA #$00
        case 0xE187: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE457:  STA EnNumber
        case 0xE18A: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE459:  BRK
        case 0xE18B: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LE45C:  RTS
        case 0xE18C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE45F:  STA RAMTrgtPtrLB
        case 0xE18F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE461:  STA CopyCounterUB
        case 0xE192: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE463:  LDA #$03
        case 0xE194: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE465:  STA RAMTrgtPtrUB
        case 0xE197: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE467:  LDA #$A0
        case 0xE199: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE469:  STA CopyCounterLB
        case 0xE19C: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE46B:  BRK
        case 0xE19D: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LE46E:  LDX #$10
        case 0xE19F: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LE470:  LDY #$00
        case 0xE1A1: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LE472:  STY EnSpritePtrLB
        case 0xE1A4: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE474:  LDA #$03
        case 0xE1A6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE476:  STA EnSpritePtrUB
        case 0xE1A9: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LE478:  LDA (EnSpritePtr),Y
        case 0xE1AB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE47A:  BEQ EnLoadPalData
        case 0xE1AD: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LE47C:  STA SpriteRAM+1,X
        case 0xE1B0: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LE47F:  INY
        case 0xE1B1: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LE480:  LDA (EnSpritePtr),Y
        case 0xE1B3: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE482:  AND #$3F
        case 0xE1B5: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LE484:  CLC
        case 0xE1B6: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE485:  ADC #$44
        case 0xE1B8: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LE487:  STA SpriteRAM,X
        case 0xE1BB: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LE48A:  LDA (EnSpritePtr),Y
        case 0xE1BD: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE48C:  AND #$C0
        case 0xE1BF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE48E:  STA EnSprtAttribDat
        case 0xE1C2: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LE490:  INY
        case 0xE1C3: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LE491:  LDA (EnSpritePtr),Y
        case 0xE1C5: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE493:  AND #$03
        case 0xE1C7: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE495:  ORA EnSprtAttribDat
        case 0xE1CA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE497:  STA EnSprtAttribDat
        case 0xE1CD: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LE499:  LDA (EnSpritePtr),Y
        case 0xE1CF: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE49B:  LSR
        case 0xE1D0: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE49C:  LSR
        case 0xE1D1: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LE49D:  SEC
        case 0xE1D2: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LE49E:  SBC #$1C
        case 0xE1D4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE4A0:  STA EnSprtXPos
        case 0xE1D7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE4A2:  LDA IsEnMirrored
        case 0xE1DA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE4A4:  BEQ SetEnSprtAttrib
        case 0xE1DC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE4A6:  LDA EnSprtXPos
        case 0xE1DF: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE4A8:  EOR #$FF
        case 0xE1E1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE4AA:  STA EnSprtXPos
        case 0xE1E4: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LE4AC:  INC EnSprtXPos
        case 0xE1E7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE4AE:  LDA EnSprtAttribDat
        case 0xE1EA: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE4B0:  EOR #$40
        case 0xE1EC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE4B2:  STA EnSprtAttribDat
        case 0xE1EF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE4B4:  LDA EnSprtAttribDat
        case 0xE1F2: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LE4B6:  STA SpriteRAM+2,X
        case 0xE1F5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE4B9:  LDA EnSprtXPos
        case 0xE1F8: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LE4BB:  CLC
        case 0xE1F9: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE4BC:  ADC #$84
        case 0xE1FB: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LE4BE:  STA SpriteRAM+3,X
        case 0xE1FE: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LE4C1:  INX
        case 0xE1FF: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LE4C2:  INX
        case 0xE200: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LE4C3:  INX
        case 0xE201: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LE4C4:  INX
        case 0xE202: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LE4C5:  INY
        case 0xE203: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE4C6:  BNE EnSpriteLoop
        case 0xE205: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE4C8:  JSR LoadEnPalette
        case 0xE208: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE4CB:  JSR Bank3ToNT1
        case 0xE20B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE4CE:  LDA #$00
        case 0xE20D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE4D0:  STA EnSprtXPos
        case 0xE210: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE4D2:  LDA #$30
        case 0xE212: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE4D4:  STA EnSprtAttribDat
        case 0xE215: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE4D6:  JSR PrepSPPalLoad
        case 0xE218: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE4D9:  JSR PalFadeIn
        case 0xE21B: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LE4DC:  JMP PalFadeIn
        case 0xE21E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE4DF:  STA EnNumber
        case 0xE221: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE4E1:  STA EnDatPtr
        case 0xE224: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE4E3:  CMP #EN_DRAGONLORD2
        case 0xE226: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE4E5:  BNE +
        case 0xE228: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE4E7:  LDA #MSC_END_BOSS
        case 0xE22A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE4E9:  BNE LoadFightMusic
        case 0xE22C: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE4ED:  BRK
        case 0xE22D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE4F0:  LDA #$00
        case 0xE22F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE4F2:  STA EnDatPtrUB
        case 0xE232: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LE4F4:  ASL EnDatPtrLB
        case 0xE235: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_rol(bus_read(a))); } reg_PC += 3; return; // LE4F6:  ROL EnDatPtrUB
        case 0xE238: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LE4F8:  ASL EnDatPtrLB
        case 0xE23B: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_rol(bus_read(a))); } reg_PC += 3; return; // LE4FA:  ROL EnDatPtrUB
        case 0xE23E: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LE4FC:  ASL EnDatPtrLB
        case 0xE241: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_rol(bus_read(a))); } reg_PC += 3; return; // LE4FE:  ROL EnDatPtrUB
        case 0xE244: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LE500:  ASL EnDatPtrLB
        case 0xE247: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_rol(bus_read(a))); } reg_PC += 3; return; // LE502:  ROL EnDatPtrUB
        case 0xE24A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE504:  LDA EnNumber
        case 0xE24D: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LE506:  PHA
        case 0xE24E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE507:  LDA #$00
        case 0xE250: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE509:  STA EnNumber
        case 0xE253: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE50B:  BRK
        case 0xE254: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LE50E:  PLA
        case 0xE255: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE50F:  STA EnNumber
        case 0xE258: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE511:  CMP #EN_RDRAGON
        case 0xE25A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE513:  BNE +
        case 0xE25C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE515:  LDA #$46
        case 0xE25E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE517:  STA Stack
        case 0xE261: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE51A:  LDA #$FA
        case 0xE263: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE51C:  STA Stack+1
        case 0xE266: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE51F:  BNE ContInitFight
        case 0xE268: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE523:  STA Stack
        case 0xE26B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE526:  JSR DoSprites
        case 0xE26E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE529:  LDA PlayerFlags
        case 0xE271: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE52B:  AND #$0F
        case 0xE273: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE52D:  STA PlayerFlags
        case 0xE276: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE52F:  LDA EnNumber
        case 0xE279: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LE531:  PHA
        case 0xE27A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE532:  LDA #$00
        case 0xE27C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE534:  STA EnNumber
        case 0xE27F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE536:  JSR LoadStats
        case 0xE282: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LE539:  PLA
        case 0xE283: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE53A:  STA EnNumber
        case 0xE286: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LE53C:  ASL
        case 0xE287: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LE53D:  TAY
        case 0xE288: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LE53E:  LDX #$22
        case 0xE28A: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE540:  BRK
        case 0xE28B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE543:  LDA #PRG_BANK_1
        case 0xE28D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE545:  JSR GetBankDataByte
        case 0xE290: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LE548:  CLC
        case 0xE291: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE549:  ADC #$00
        case 0xE293: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE54B:  STA ROMSrcPtrLB
        case 0xE296: cycles_to_run -= 3; // Unsupported php reg_PC += 1; return; // LE54D:  PHP
        case 0xE297: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LE54E:  INY
        case 0xE298: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE54F:  LDA #PRG_BANK_1
        case 0xE29A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE551:  JSR GetBankDataByte
        case 0xE29D: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LE554:  TAY
        case 0xE29E: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE555:  AND #$7F
        case 0xE2A0: cycles_to_run -= 4; // Unsupported plp reg_PC += 1; return; // LE557:  PLP
        case 0xE2A1: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE558:  ADC #$80
        case 0xE2A3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE55A:  STA ROMSrcPtrUB
        case 0xE2A6: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LE55C:  TYA
        case 0xE2A7: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LE55D:  PHA
        case 0xE2A8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE55E:  LDA ROMSrcPtrLB
        case 0xE2AB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE560:  STA NotUsed26
        case 0xE2AE: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LE562:  PHA
        case 0xE2AF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE563:  LDA ROMSrcPtrUB
        case 0xE2B2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE565:  STA NotUsed27
        case 0xE2B5: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LE567:  PHA
        case 0xE2B6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE568:  JSR LoadCombatBckgrnd
        case 0xE2B9: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LE56B:  PLA
        case 0xE2BA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE56C:  STA ROMSrcPtrUB
        case 0xE2BD: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LE56E:  PLA
        case 0xE2BE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE56F:  STA ROMSrcPtrLB
        case 0xE2C1: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LE571:  PLA
        case 0xE2C2: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE572:  AND #$80
        case 0xE2C4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE574:  STA IsEnMirrored
        case 0xE2C7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE576:  LDA NPCUpdateCntr
        case 0xE2CA: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE578:  ORA #$80
        case 0xE2CC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE57A:  STA NPCUpdateCntr
        case 0xE2CF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE57C:  JSR LoadEnemyGFX
        case 0xE2D2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE57F:  JSR Dowindow
        case 0xE2D5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE583:  LDA EnNumber
        case 0xE2D8: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE585:  CMP #EN_DRAGONLORD2
        case 0xE2DA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE587:  BNE EnAppearText
        case 0xE2DC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE589:  JSR DoDialogHiBlock
        case 0xE2DF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE58D:  LDA EnBaseHP
        case 0xE2E2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE590:  BNE SetEnHP
        case 0xE2E4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE592:  JSR CopyEnUpperBytes
        case 0xE2E7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE595:  JSR DoDialogLoBlock
        case 0xE2EA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE599:  LDA EnBaseHP
        case 0xE2ED: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE59C:  STA MultNum2LB
        case 0xE2F0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE59E:  JSR UpdateRandNum
        case 0xE2F3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE5A1:  LDA RandNumUB
        case 0xE2F6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE5A3:  STA MultNum1LB
        case 0xE2F9: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE5A5:  LDA #$00
        case 0xE2FB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE5A7:  STA MultNum1UB
        case 0xE2FE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE5A9:  STA MultNum2UB
        case 0xE301: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE5AB:  JSR WordMultiply
        case 0xE304: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE5AE:  LDA MultRsltUB
        case 0xE307: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE5B0:  LSR
        case 0xE308: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE5B1:  LSR
        case 0xE309: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE5B2:  STA MultRsltLB
        case 0xE30C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE5B4:  LDA EnBaseHP
        case 0xE30F: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LE5B7:  SEC
        case 0xE310: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LE5B8:  SBC MultRsltLB
        case 0xE313: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE5BA:  STA EnCurntHP
        case 0xE316: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE5BC:  JSR CheckEnRun
        case 0xE319: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE5BF:  JSR CalcWhoIsNext
        case 0xE31C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LE5C2:  BCS StartPlayerTurn
        case 0xE31E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE5C4:  JSR CopyEnUpperBytes
        case 0xE321: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE5C7:  JSR DoDialogLoBlock
        case 0xE324: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE5CB:  JMP StartEnemyTurn
        case 0xE327: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE5CE:  JSR Dowindow
        case 0xE32A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE5D2:  LDA PlayerFlags
        case 0xE32D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0xC07E; return; } // LE5D4:  BPL ShowCmbtCmd
        case 0xE32F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE5D6:  JSR UpdateRandNum
        case 0xE332: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE5D9:  LDA RandNumUB
        case 0xE335: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE5DB:  LSR
        case 0xE336: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LE5DC:  BCS PlayerAwakes
        case 0xE338: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE5DE:  JSR DoDialogHiBlock
        case 0xE33B: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE5E2:  JMP StartEnemyTurn
        case 0xE33E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE5E5:  LDA PlayerFlags
        case 0xE341: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE5E7:  AND #$7F
        case 0xE343: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE5E9:  STA PlayerFlags
        case 0xE346: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE5EB:  JSR DoDialogHiBlock
        case 0xE349: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE5EF:  JSR DoDialogLoBlock
        case 0xE34C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE5F3:  JSR Dowindow
        case 0xE34F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE5F7:  LDA WndSelResults
        case 0xE352: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE5F9:  BEQ PlyrFight
        case 0xE354: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE5FB:  JMP ChkPlyrSpell
        case 0xE357: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE5FE:  LDA #WND_CMD_CMB
        case 0xE359: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE600:  JSR RemoveWindow
        case 0xE35C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE603:  LDA #SFX_ATTACK
        case 0xE35E: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE605:  BRK
        case 0xE35F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE608:  JSR DoDialogLoBlock
        case 0xE362: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE60C:  LDA DisplayedAttck
        case 0xE365: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE60E:  STA AttackStat
        case 0xE368: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE610:  LDA EnBaseDef
        case 0xE36B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE613:  STA DefenseStat
        case 0xE36E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE615:  LDA EnNumber
        case 0xE371: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE617:  CMP #EN_DRAGONLORD1
        case 0xE373: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE619:  BEQ ChkPlyrMiss
        case 0xE375: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE61B:  CMP #EN_DRAGONLORD2
        case 0xE377: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE61D:  BEQ ChkPlyrMiss
        case 0xE379: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE61F:  JSR UpdateRandNum
        case 0xE37C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE622:  LDA RandNumUB
        case 0xE37F: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE624:  AND #$1F
        case 0xE381: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE626:  BNE ChkPlyrMiss
        case 0xE383: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE628:  LDA #SFX_EXCLNT_MOVE
        case 0xE385: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE62A:  BRK
        case 0xE386: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE62D:  JSR CopyEnUpperBytes
        case 0xE389: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE630:  JSR DoDialogHiBlock
        case 0xE38C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE634:  JSR UpdateRandNum
        case 0xE38F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE637:  LDA RandNumUB
        case 0xE392: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE639:  STA MultNum1LB
        case 0xE395: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE63B:  LDA DisplayedAttck
        case 0xE398: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE63D:  LSR
        case 0xE399: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE63E:  STA MultNum2LB
        case 0xE39C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE640:  LDA #$00
        case 0xE39E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE642:  STA MultNum1UB
        case 0xE3A1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE644:  STA MultNum2UB
        case 0xE3A4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE646:  JSR WordMultiply
        case 0xE3A7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE649:  LDA DisplayedAttck
        case 0xE3AA: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LE64B:  SEC
        case 0xE3AB: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LE64C:  SBC MultRsltUB
        case 0xE3AE: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE64E:  JMP SetEnDmg1
        case 0xE3B1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE651:  JSR PlyrCalcHitDmg
        case 0xE3B4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE654:  LDA CalcDamage
        case 0xE3B7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE656:  BNE SetEnDmg1
        case 0xE3B9: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE658:  LDA #SFX_MISSED1
        case 0xE3BB: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE65A:  BRK
        case 0xE3BC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE65D:  JSR DoDialogLoBlock
        case 0xE3BF: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE661:  JMP StartEnemyTurn
        case 0xE3C2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE664:  STA EnDamage
        case 0xE3C5: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE666:  LDA #$00
        case 0xE3C7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE668:  STA DmgNotUsed
        case 0xE3CA: cycles_to_run -= 3; cpu_bit(bus_read(0x0000)); reg_PC += 3; return; // LE66A:  BIT PlayerFlags
        case 0xE3CD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_V) { reg_PC = 0xC07E; return; } // LE66C:  BVS PlyrHitEn
        case 0xE3CF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE66E:  JSR UpdateRandNum
        case 0xE3D2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE671:  LDA RandNumUB
        case 0xE3D5: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE673:  AND #$3F
        case 0xE3D7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE675:  STA RandNumUB
        case 0xE3DA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE677:  LDA EnBaseMDef
        case 0xE3DD: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE67A:  AND #$0F
        case 0xE3DF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE67C:  BEQ PlyrHitEn
        case 0xE3E1: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LE67E:  SEC
        case 0xE3E2: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LE67F:  SBC #$01
        case 0xE3E4: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LE681:  CMP RandNumUB
        case 0xE3E7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LE683:  BCC PlyrHitEn
        case 0xE3E9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE685:  JSR CopyEnUpperBytes
        case 0xE3EC: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE688:  LDA #SFX_MISSED1
        case 0xE3EE: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE68A:  BRK
        case 0xE3EF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE68D:  JSR DoDialogHiBlock
        case 0xE3F2: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE691:  JMP StartEnemyTurn
        case 0xE3F5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE694:  STA EnDamage
        case 0xE3F8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE696:  LDA #$00
        case 0xE3FA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE698:  STA DmgNotUsed
        case 0xE3FD: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE69A:  LDA #SFX_ENMY_HIT
        case 0xE3FF: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE69C:  BRK
        case 0xE400: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE69F:  LDA RedFlashPalPtr
        case 0xE403: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE6A2:  STA GenPtr42LB
        case 0xE406: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE6A4:  LDA RedFlashPalPtr+1
        case 0xE409: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE6A7:  STA GenPtr42UB
        case 0xE40C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE6A9:  JSR PaletteFlash
        case 0xE40F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE6AC:  JSR CopyEnUpperBytes
        case 0xE412: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE6AF:  JSR DoDialogLoBlock
        case 0xE415: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE6B3:  JMP UpdateEnHP
        case 0xE418: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE6B6:  CMP #CC_SPELL
        case 0xE41A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE6B8:  BEQ PlyrSpell
        case 0xE41C: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE6BA:  JMP ChkPlyrItem
        case 0xE41F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE6BD:  LDA SpellFlags
        case 0xE422: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE6BF:  STA SpellFlagsLB
        case 0xE425: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE6C1:  LDA ModsnSpells
        case 0xE428: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE6C3:  AND #$03
        case 0xE42A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE6C5:  STA SpellFlagsUB
        case 0xE42D: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE6C7:  ORA SpellFlagsLB
        case 0xE430: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE6C9:  BNE ShowSpellWnd
        case 0xE432: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE6CB:  LDA #WND_CMD_CMB
        case 0xE434: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE6CD:  JSR RemoveWindow
        case 0xE437: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE6D0:  JSR DoDialogLoBlock
        case 0xE43A: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE6D4:  JMP StartPlayerTurn
        case 0xE43D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE6D7:  JSR ShowSpells
        case 0xE440: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE6DA:  CMP #WND_ABORT
        case 0xE442: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE6DC:  BNE PlyrSpellChosen
        case 0xE444: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE6DE:  LDA #WND_CMD_CMB
        case 0xE446: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE6E0:  JSR RemoveWindow
        case 0xE449: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE6E3:  JMP StartPlayerTurn
        case 0xE44C: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LE6E6:  PHA
        case 0xE44D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE6E7:  LDA #WND_CMD_CMB
        case 0xE44F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE6E9:  JSR RemoveWindow
        case 0xE452: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LE6EC:  PLA
        case 0xE453: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE6ED:  CMP #SPL_RADIANT
        case 0xE455: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE6EF:  BEQ InvalidCombatSpell
        case 0xE457: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE6F1:  CMP #SPL_REPEL
        case 0xE459: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE6F3:  BEQ InvalidCombatSpell
        case 0xE45B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE6F5:  CMP #SPL_OUTSIDE
        case 0xE45D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE6F7:  BEQ InvalidCombatSpell
        case 0xE45F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE6F9:  CMP #SPL_RETURN
        case 0xE461: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE6FB:  BNE ValidCombatSpell
        case 0xE463: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE6FD:  JSR DoDialogLoBlock
        case 0xE466: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE701:  JMP StartPlayerTurn
        case 0xE469: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE704:  JSR CheckMP
        case 0xE46C: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE707:  CMP #$32
        case 0xE46E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE709:  BNE +
        case 0xE470: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE70B:  JSR DoMidDialog
        case 0xE473: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE70E:  JMP StartPlayerTurn
        case 0xE476: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE713:  LDA PlayerFlags
        case 0xE479: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE715:  AND #F_PLR_STOPSPEL
        case 0xE47B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE717:  BEQ PlyrPrepSpell
        case 0xE47D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE719:  JSR DoDialogLoBlock
        case 0xE480: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE71D:  JMP StartEnemyTurn
        case 0xE483: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE720:  LDA SpellToCast
        case 0xE486: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE722:  CMP #SPL_HEAL
        case 0xE488: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE724:  BNE +
        case 0xE48A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE726:  JSR DoHeal
        case 0xE48D: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE729:  JMP StartEnemyTurn
        case 0xE490: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE72E:  BNE +
        case 0xE492: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE730:  JSR DoHealmore
        case 0xE495: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE733:  JMP StartEnemyTurn
        case 0xE498: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE738:  BNE +
        case 0xE49A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE73A:  LDA EnBaseMDef
        case 0xE49D: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE73D:  LSR
        case 0xE49E: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE73E:  LSR
        case 0xE49F: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE73F:  LSR
        case 0xE4A0: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE740:  LSR
        case 0xE4A1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE741:  JSR ChkSpellFail
        case 0xE4A4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE744:  JSR UpdateRandNum
        case 0xE4A7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE747:  LDA RandNumUB
        case 0xE4AA: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE749:  AND #$07
        case 0xE4AC: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LE74B:  CLC
        case 0xE4AD: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE74C:  ADC #$05
        case 0xE4AF: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE74E:  JMP SetSpellDmg
        case 0xE4B2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE753:  BNE +
        case 0xE4B4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE755:  LDA EnBaseMDef
        case 0xE4B7: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE758:  LSR
        case 0xE4B8: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE759:  LSR
        case 0xE4B9: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE75A:  LSR
        case 0xE4BA: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE75B:  LSR
        case 0xE4BB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE75C:  JSR ChkSpellFail
        case 0xE4BE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE75F:  JSR UpdateRandNum
        case 0xE4C1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE762:  LDA RandNumUB
        case 0xE4C4: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE764:  AND #$07
        case 0xE4C6: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LE766:  CLC
        case 0xE4C7: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE767:  ADC #$3A
        case 0xE4C9: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE769:  JMP SetSpellDmg
        case 0xE4CC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE76E:  BNE +
        case 0xE4CE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE770:  LDA EnBaseAgi
        case 0xE4D1: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE773:  LSR
        case 0xE4D2: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE774:  LSR
        case 0xE4D3: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE775:  LSR
        case 0xE4D4: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LE776:  LSR
        case 0xE4D5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE777:  JSR ChkSpellFail
        case 0xE4D8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE77A:  JSR CopyEnUpperBytes
        case 0xE4DB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE77D:  JSR DoDialogLoBlock
        case 0xE4DE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE781:  LDA PlayerFlags
        case 0xE4E1: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE783:  ORA #F_EN_ASLEEP
        case 0xE4E3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE785:  STA PlayerFlags
        case 0xE4E6: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE787:  JMP EnStillAsleep
        case 0xE4E9: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE78D:  AND #$0F
        case 0xE4EB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE78F:  JSR ChkSpellFail
        case 0xE4EE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE792:  JSR CopyEnUpperBytes
        case 0xE4F1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE795:  JSR DoDialogLoBlock
        case 0xE4F4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE799:  LDA PlayerFlags
        case 0xE4F7: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE79B:  ORA #F_EN_STOPSPEL
        case 0xE4F9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE79D:  STA PlayerFlags
        case 0xE4FC: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE79F:  JMP StartEnemyTurn
        case 0xE4FF: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE7A2:  CMP #CC_ITEM
        case 0xE501: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE7A4:  BEQ PlyrItem
        case 0xE503: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE7A6:  JMP ChkPlyrRun
        case 0xE506: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE7A9:  JSR CreateInvList
        case 0xE509: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LE7AC:  CPX #INV_NONE
        case 0xE50B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE7AE:  BNE PlyrShowInv
        case 0xE50D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE7B0:  LDA #WND_CMD_CMB
        case 0xE50F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE7B2:  JSR RemoveWindow
        case 0xE512: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE7B5:  JSR DoDialogLoBlock
        case 0xE515: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE7B9:  JMP StartPlayerTurn
        case 0xE518: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE7BC:  JSR Dowindow
        case 0xE51B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE7C0:  CMP #WND_ABORT
        case 0xE51D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE7C2:  BNE ChkCmbtItemUsed
        case 0xE51F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE7C4:  LDA #WND_CMD_CMB
        case 0xE521: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE7C6:  JSR RemoveWindow
        case 0xE524: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE7C9:  JMP StartPlayerTurn
        case 0xE527: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LE7CC:  PHA
        case 0xE528: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE7CD:  LDA #WND_CMD_CMB
        case 0xE52A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE7CF:  JSR RemoveWindow
        case 0xE52D: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LE7D2:  PLA
        case 0xE52E: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LE7D3:  TAX
        case 0xE52F: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LE7D4:  LDA DescBuf+1,X
        case 0xE532: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE7D6:  CMP #INV_HERB
        case 0xE534: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE7D8:  BNE ChkUseFlute
        case 0xE536: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LE7DA:  DEC InventoryHerbs
        case 0xE539: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE7DC:  JSR DoDialogLoBlock
        case 0xE53C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE7E0:  JSR HerbHeal
        case 0xE53F: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE7E3:  JMP StartEnemyTurn
        case 0xE542: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE7E6:  CMP #INV_FLUTE
        case 0xE544: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE7E8:  BNE ChkUseHarp
        case 0xE546: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE7EA:  JSR DoDialogLoBlock
        case 0xE549: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE7EE:  LDA #MSC_FRY_FLUTE
        case 0xE54B: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE7F0:  BRK
        case 0xE54C: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE7F3:  BRK
        case 0xE54D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE7F6:  LDA EnNumber
        case 0xE550: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE7F8:  CMP #EN_DRAGONLORD2
        case 0xE552: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE7FA:  BEQ PlayEndBossMusic1
        case 0xE554: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE7FC:  LDA #MSC_REG_FGHT
        case 0xE556: cycles_to_run -= 3; reg_PC = 0x0000; return; // LE7FE:  JMP +
        case 0xE559: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE801:  LDA #MSC_END_BOSS
        case 0xE55B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE806:  LDA EnNumber
        case 0xE55E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE808:  CMP #EN_GOLEM
        case 0xE560: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE80A:  BNE FluteFail
        case 0xE562: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE80C:  JSR DoDialogLoBlock
        case 0xE565: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE810:  LDA PlayerFlags
        case 0xE568: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE812:  ORA #F_EN_ASLEEP
        case 0xE56A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE814:  STA PlayerFlags
        case 0xE56D: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE816:  JMP EnStillAsleep
        case 0xE570: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE819:  JSR DoDialogLoBlock
        case 0xE573: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE81D:  JMP StartEnemyTurn
        case 0xE576: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE820:  CMP #INV_HARP
        case 0xE578: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE822:  BNE ChkUseScale
        case 0xE57A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE824:  JSR DoDialogLoBlock
        case 0xE57D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE828:  LDA #MSC_SILV_HARP
        case 0xE57F: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE829:  BRK
        case 0xE580: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE82D:  BRK
        case 0xE581: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE830:  LDA EnNumber
        case 0xE584: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE832:  CMP #EN_DRAGONLORD2
        case 0xE586: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE834:  BEQ PlayEndBossMusic2
        case 0xE588: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE836:  LDA #MSC_REG_FGHT
        case 0xE58A: cycles_to_run -= 3; reg_PC = 0x0000; return; // LE838:  JMP +
        case 0xE58D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE83B:  LDA #MSC_END_BOSS
        case 0xE58F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE840:  JSR CopyEnUpperBytes
        case 0xE592: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE843:  JSR DoDialogLoBlock
        case 0xE595: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE847:  JMP StartEnemyTurn
        case 0xE598: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE84A:  CMP #INV_SCALE
        case 0xE59A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE84C:  BNE ChkUseRing
        case 0xE59C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE84E:  JSR ChkDragonScale
        case 0xE59F: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE851:  JMP StartEnemyTurn
        case 0xE5A2: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE854:  CMP #INV_RING
        case 0xE5A4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE856:  BNE ChkUseBelt
        case 0xE5A6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE858:  JSR ChkRing
        case 0xE5A9: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE85B:  JMP StartEnemyTurn
        case 0xE5AC: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE85E:  CMP #INV_BELT
        case 0xE5AE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE860:  BNE ChkUseNecklace
        case 0xE5B0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE862:  JSR WearCursedItem
        case 0xE5B3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE865:  LDA #MSC_REG_FGHT
        case 0xE5B5: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE867:  BRK
        case 0xE5B6: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE86A:  JMP StartEnemyTurn
        case 0xE5B9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE86D:  CMP #INV_NECKLACE
        case 0xE5BB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE86F:  BNE UseInvalidItem
        case 0xE5BD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE871:  JSR ChkDeathNecklace
        case 0xE5C0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE874:  LDA #MSC_REG_FGHT
        case 0xE5C2: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE876:  BRK
        case 0xE5C3: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE879:  JMP StartEnemyTurn
        case 0xE5C6: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE87C:  JMP InvalidCombatSpell
        case 0xE5C9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE87F:  CMP #CC_RUN
        case 0xE5CB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE881:  BEQ PlyrRun
        case 0xE5CD: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE883:  JMP ShowCmbtCmd
        case 0xE5D0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE886:  LDA #WND_CMD_CMB
        case 0xE5D2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE888:  JSR RemoveWindow
        case 0xE5D5: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE88B:  LDA #SFX_RUN
        case 0xE5D7: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE88D:  BRK
        case 0xE5D8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE890:  JSR DoDialogLoBlock
        case 0xE5DB: cycles_to_run -= 3; cpu_bit(bus_read(0x0000)); reg_PC += 3; return; // LE894:  BIT PlayerFlags
        case 0xE5DE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_V) { reg_PC = 0xC07E; return; } // LE896:  BVS RunSuccessful
        case 0xE5E0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE898:  JSR TryRun
        case 0xE5E3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LE89B:  BCS RunSuccessful
        case 0xE5E5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE89D:  JSR DoDialogLoBlock
        case 0xE5E8: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE8A1:  JMP StartEnemyTurn
        case 0xE5EB: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LE8A4:  LDX MapNumber
        case 0xE5EE: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LE8A6:  LDA ResumeMusicTbl,X
        case 0xE5F1: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE8A9:  BRK
        case 0xE5F2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE8AC:  LDA EnNumber
        case 0xE5F5: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE8AE:  CMP #EN_DRAGONLORD2
        case 0xE5F7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE8B0:  BNE ChkMapHauksness
        case 0xE5F9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE8B2:  JSR WaitForNMI
        case 0xE5FC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE8B5:  LDA BlackPalPtr
        case 0xE5FF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE8B8:  STA PalPtrLB
        case 0xE602: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE8BA:  LDA BlackPalPtr+1
        case 0xE605: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE8BD:  STA PalPtrUB
        case 0xE608: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE8BF:  LDA #$00
        case 0xE60A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE8C1:  STA PalModByte
        case 0xE60D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE8C3:  STA EnNumber
        case 0xE610: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE8C5:  JSR PrepSPPalLoad
        case 0xE613: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE8C8:  JSR PrepBGPalLoad
        case 0xE616: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE8CB:  JSR WaitForNMI
        case 0xE619: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LE8CE:  JSR ClearWinBufRAM2
        case 0xE61C: cycles_to_run -= 3; reg_PC = 0x0000; return; // LE8D1:  JMP MapChngNoSound
        case 0xE61F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE8D4:  LDA MapNumber
        case 0xE622: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE8D6:  CMP #MAP_HAUKSNESS
        case 0xE624: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE8D8:  BNE ChkMapSwampCave
        case 0xE626: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE8DA:  LDA CharXPos
        case 0xE629: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE8DC:  CMP #$12
        case 0xE62B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE8DE:  BNE ChkMapSwampCave
        case 0xE62D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE8E0:  LDA CharYPos
        case 0xE630: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE8E2:  CMP #$0C
        case 0xE632: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE8E4:  BNE ChkMapSwampCave
        case 0xE634: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE8E6:  JSR ClearSpriteRAM
        case 0xE637: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LE8E9:  DEC CharXPos
        case 0xE63A: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LE8EB:  DEC _CharXPos
        case 0xE63D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE8ED:  LDA CharXPixelsLB
        case 0xE640: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LE8EF:  SEC
        case 0xE641: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LE8F0:  SBC #$10
        case 0xE643: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE8F2:  STA CharXPixelsLB
        case 0xE646: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LE8F4:  BCS +
        case 0xE648: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LE8F6:  DEC CharXPixelsUB
        case 0xE64B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE8FB:  LDA MapNumber
        case 0xE64E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE8FD:  CMP #MAP_SWAMPCAVE
        case 0xE650: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE8FF:  BNE ChkMapOverworld
        case 0xE652: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE901:  LDA CharXPos
        case 0xE655: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE903:  CMP #$04
        case 0xE657: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE905:  BNE ChkMapOverworld
        case 0xE659: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE907:  LDA CharYPos
        case 0xE65C: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE909:  CMP #$0E
        case 0xE65E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE90B:  BNE ChkMapOverworld
        case 0xE660: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE90D:  LDA StoryFlags
        case 0xE663: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE90F:  AND #F_GDRG_DEAD
        case 0xE665: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE911:  BNE ChkMapOverworld
        case 0xE667: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE913:  JSR ClearSpriteRAM
        case 0xE66A: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LE916:  DEC CharYPos
        case 0xE66D: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LE918:  DEC _CharYPos
        case 0xE670: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE91A:  LDA CharYPixelsLB
        case 0xE673: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LE91C:  SEC
        case 0xE674: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LE91D:  SBC #$10
        case 0xE676: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE91F:  STA CharYPixelsLB
        case 0xE679: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LE921:  BCS +
        case 0xE67B: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LE923:  DEC CharYPixelsUB
        case 0xE67E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE928:  LDA MapNumber
        case 0xE681: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE92A:  CMP #MAP_OVERWORLD
        case 0xE683: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE92C:  BNE ChkMapsDone
        case 0xE685: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE92E:  LDA CharXPos
        case 0xE688: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE930:  CMP #$49
        case 0xE68A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE932:  BNE ChkMapsDone
        case 0xE68C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE934:  LDA CharYPos
        case 0xE68F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE936:  CMP #$64
        case 0xE691: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE938:  BNE ChkMapsDone
        case 0xE693: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE93A:  LDA StoryFlags
        case 0xE696: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE93C:  AND #F_GOLEM_DEAD
        case 0xE698: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE93E:  BEQ MovePlyrUp1Block
        case 0xE69A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE940:  JSR ClearSpriteRAM
        case 0xE69D: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE943:  JMP ExitFight2
        case 0xE6A0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE946:  STA GenByte3E
        case 0xE6A3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE948:  JSR UpdateRandNum
        case 0xE6A6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE94B:  LDA RandNumUB
        case 0xE6A9: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE94D:  AND #$0F
        case 0xE6AB: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LE94F:  CMP GenByte3E
        case 0xE6AE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LE951:  BCC SpellFailed
        case 0xE6B0: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LE953:  RTS
        case 0xE6B1: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LE954:  PLA
        case 0xE6B2: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LE955:  PLA
        case 0xE6B3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE956:  JSR DoDialogLoBlock
        case 0xE6B6: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE95A:  JMP StartEnemyTurn
        case 0xE6B9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE95D:  LDA EnCurntHP
        case 0xE6BC: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LE95F:  SEC
        case 0xE6BD: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LE960:  SBC EnDamage
        case 0xE6C0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE962:  STA EnCurntHP
        case 0xE6C3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LE964:  BCC EnemyDefeated
        case 0xE6C5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LE966:  BEQ EnemyDefeated
        case 0xE6C7: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE968:  JMP StartEnemyTurn
        case 0xE6CA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE96B:  LDA EnNumber
        case 0xE6CD: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE96D:  CMP #EN_GDRAGON
        case 0xE6CF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE96F:  BNE ChkGolemKilled
        case 0xE6D1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE971:  LDA MapNumber
        case 0xE6D4: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE973:  CMP #MAP_SWAMPCAVE
        case 0xE6D6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE975:  BNE ChkGolemKilled
        case 0xE6D8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE977:  LDA StoryFlags
        case 0xE6DB: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE979:  ORA #F_GDRG_DEAD
        case 0xE6DD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE97B:  STA StoryFlags
        case 0xE6E0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE97D:  BNE ContEnDefeated
        case 0xE6E2: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE97F:  CMP #EN_GOLEM
        case 0xE6E4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE981:  BNE ContEnDefeated
        case 0xE6E6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE983:  LDA MapNumber
        case 0xE6E9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE985:  CMP #MAP_OVERWORLD
        case 0xE6EB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE987:  BNE ContEnDefeated
        case 0xE6ED: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE989:  LDA StoryFlags
        case 0xE6F0: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE98B:  ORA #F_GOLEM_DEAD
        case 0xE6F2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE98D:  STA StoryFlags
        case 0xE6F5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE98F:  JSR CopyEnUpperBytes
        case 0xE6F8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE992:  JSR DoDialogLoBlock
        case 0xE6FB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE996:  JSR ClearSpriteRAM
        case 0xE6FE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE999:  LDA #MSC_VICTORY
        case 0xE700: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LE99B:  BRK
        case 0xE701: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE99E:  LDA EnNumber
        case 0xE704: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE9A0:  CMP #EN_DRAGONLORD1
        case 0xE706: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE9A2:  BNE NotDL1Defeated
        case 0xE708: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LE9A4:  LDX #$50
        case 0xE70A: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LE9A9:  DEX
        case 0xE70B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE9AA:  BNE -
        case 0xE70D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE9AC:  LDA #EN_DRAGONLORD2
        case 0xE70F: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LE9AE:  JMP InitFight
        case 0xE712: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LE9B1:  CMP #EN_DRAGONLORD2
        case 0xE714: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE9B3:  BNE RegEnDefeated
        case 0xE716: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE9B5:  STA DrgnLrdPal
        case 0xE719: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE9B8:  LDA #DIR_DOWN
        case 0xE71B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE9BA:  STA CharDirection
        case 0xE71E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE9BD:  LDA #$2C
        case 0xE720: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE9BF:  STA PPUAddrLB
        case 0xE723: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE9C1:  LDA #$21
        case 0xE725: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE9C3:  STA PPUAddrUB
        case 0xE728: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE9C5:  LDA #TL_BLANK_TILE1
        case 0xE72A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE9C7:  STA PPUDataByte
        case 0xE72D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE9C9:  LDA #$06
        case 0xE72F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE9CB:  STA GenByte3C
        case 0xE732: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LE9CD:  LDY #$07
        case 0xE734: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LE9D2:  DEY
        case 0xE735: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LE9D3:  BNE -
        case 0xE737: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE9D5:  LDA PPUAddrLB
        case 0xE73A: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LE9D7:  CLC
        case 0xE73B: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LE9D8:  ADC #$19
        case 0xE73D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE9DA:  STA PPUAddrLB
        case 0xE740: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LE9DC:  BCC +
        case 0xE742: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LE9DE:  INC PPUAddrUB
        case 0xE745: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LE9E2:  BNE EndBossEraseLoop
        case 0xE747: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE9E4:  JSR WaitForNMI
        case 0xE74A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LE9E7:  JSR WaitForNMI
        case 0xE74D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE9EA:  LDA #$00
        case 0xE74F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE9EC:  STA EnNumber
        case 0xE752: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE9EE:  JSR DoDialogHiBlock
        case 0xE755: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE9F2:  LDA StoryFlags
        case 0xE758: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LE9F4:  ORA #F_DGNLRD_DEAD
        case 0xE75A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE9F6:  STA StoryFlags
        case 0xE75D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LE9F8:  JSR WaitForBtnRelease
        case 0xE760: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE9FB:  LDA DisplayedMaxHP
        case 0xE763: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LE9FD:  STA HitPoints
        case 0xE766: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LE9FF:  LDA DisplayedMaxMP
        case 0xE769: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEA01:  STA MagicPoints
        case 0xE76C: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LEA03:  LDX #$12
        case 0xE76E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEA05:  LDA #DIR_DOWN
        case 0xE770: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEA07:  JMP ChangeMaps
        case 0xE773: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEA0A:  LDA EnBaseExp
        case 0xE776: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEA0D:  STA FightExpLB
        case 0xE779: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEA0F:  LDA #$00
        case 0xE77B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEA11:  STA FightExpUB
        case 0xE77E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEA13:  JSR DoDialogLoBlock
        case 0xE781: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEA17:  LDA FightExpLB
        case 0xE784: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LEA19:  CLC
        case 0xE785: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LEA1A:  ADC ExpLB
        case 0xE788: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEA1C:  STA ExpLB
        case 0xE78B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LEA1E:  BCC CalcPlyrGold
        case 0xE78D: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LEA20:  INC ExpUB
        case 0xE790: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LEA22:  BNE CalcPlyrGold
        case 0xE792: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEA24:  LDA #$FF
        case 0xE794: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEA26:  STA ExpLB
        case 0xE797: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEA28:  STA ExpUB
        case 0xE79A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEA2A:  LDA EnBaseGld
        case 0xE79D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEA2D:  STA MultNum2LB
        case 0xE7A0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEA2F:  JSR UpdateRandNum
        case 0xE7A3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEA32:  LDA RandNumUB
        case 0xE7A6: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEA34:  AND #$3F
        case 0xE7A8: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LEA36:  CLC
        case 0xE7A9: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LEA37:  ADC #$C0
        case 0xE7AB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEA39:  STA MultNum1LB
        case 0xE7AE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEA3B:  LDA #$00
        case 0xE7B0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEA3D:  STA MultNum1UB
        case 0xE7B3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEA3F:  STA MultNum2UB
        case 0xE7B6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEA41:  JSR WordMultiply
        case 0xE7B9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEA44:  LDA MultRsltUB
        case 0xE7BC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEA46:  STA FightGoldLB
        case 0xE7BF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEA48:  LDA #$00
        case 0xE7C1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEA4A:  STA FightGoldUB
        case 0xE7C4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEA4C:  JSR DoDialogLoBlock
        case 0xE7C7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 2; return; // LEA50:  LDA $00
        case 0xE7C9: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LEA52:  CLC
        case 0xE7CA: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LEA53:  ADC GoldLB
        case 0xE7CD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEA55:  STA GoldLB
        case 0xE7D0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LEA57:  BCC ChkLevelUp
        case 0xE7D2: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LEA59:  INC GoldUB
        case 0xE7D5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LEA5B:  BNE ChkLevelUp
        case 0xE7D7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEA5D:  LDA #$FF
        case 0xE7D9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEA5F:  STA GoldLB
        case 0xE7DC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEA61:  STA GoldUB
        case 0xE7DF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LEA63:  JSR Dowindow
        case 0xE7E2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEA67:  LDA DisplayedMaxMP
        case 0xE7E5: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LEA69:  PHA
        case 0xE7E6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEA6A:  LDA DisplayedMaxHP
        case 0xE7E9: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LEA6C:  PHA
        case 0xE7EA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEA6D:  LDA DisplayedAgi
        case 0xE7ED: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LEA6F:  PHA
        case 0xE7EE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEA70:  LDA DisplayedStr
        case 0xE7F1: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LEA72:  PHA
        case 0xE7F2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEA73:  LDA DisplayedLevel
        case 0xE7F5: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LEA75:  PHA
        case 0xE7F6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEA76:  JSR LoadStats
        case 0xE7F9: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LEA79:  PLA
        case 0xE7FA: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LEA7A:  CMP DisplayedLevel
        case 0xE7FD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LEA7C:  BNE DoLevelUp
        case 0xE7FF: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LEA7E:  BRK
        case 0xE800: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LEA81:  LDX MapNumber
        case 0xE803: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LEA83:  LDA ResumeMusicTbl,X
        case 0xE806: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LEA86:  BRK
        case 0xE807: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LEA89:  PLA
        case 0xE808: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LEA8A:  PLA
        case 0xE809: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LEA8B:  PLA
        case 0xE80A: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LEA8C:  PLA
        case 0xE80B: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEA8D:  JMP ExitFight
        case 0xE80E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEA90:  LDA #MSC_LEVEL_UP
        case 0xE810: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LEA92:  BRK
        case 0xE811: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LEA95:  BRK
        case 0xE812: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LEA98:  LDX MapNumber
        case 0xE815: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LEA9A:  LDA ResumeMusicTbl,X
        case 0xE818: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LEA9D:  BRK
        case 0xE819: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEAA0:  JSR DoDialogLoBlock
        case 0xE81C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEAA4:  LDA #$00
        case 0xE81E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEAA6:  STA AmountUB
        case 0xE821: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LEAA8:  PLA
        case 0xE822: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEAA9:  STA PlyrTempStat
        case 0xE825: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEAAB:  LDA DisplayedStr
        case 0xE828: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LEAAD:  SEC
        case 0xE829: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LEAAE:  SBC PlyrTempStat
        case 0xE82C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LEAB0:  BEQ ChkAgilityUp
        case 0xE82E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEAB2:  STA AmountLB
        case 0xE831: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEAB4:  JSR DoDialogHiBlock
        case 0xE834: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LEAB8:  PLA
        case 0xE835: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEAB9:  STA PlyrTempStat
        case 0xE838: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEABB:  LDA DisplayedAgi
        case 0xE83B: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LEABD:  SEC
        case 0xE83C: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LEABE:  SBC PlyrTempStat
        case 0xE83F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LEAC0:  BEQ ChkHPUp
        case 0xE841: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEAC2:  STA AmountLB
        case 0xE844: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEAC4:  JSR DoDialogHiBlock
        case 0xE847: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LEAC8:  PLA
        case 0xE848: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEAC9:  STA PlyrTempStat
        case 0xE84B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEACB:  LDA DisplayedMaxHP
        case 0xE84E: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LEACD:  SEC
        case 0xE84F: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LEACE:  SBC PlyrTempStat
        case 0xE852: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEAD0:  STA AmountLB
        case 0xE855: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEAD2:  JSR DoDialogHiBlock
        case 0xE858: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LEAD6:  PLA
        case 0xE859: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEAD7:  STA PlyrTempStat
        case 0xE85C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEAD9:  LDA DisplayedMaxMP
        case 0xE85F: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LEADB:  SEC
        case 0xE860: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LEADC:  SBC PlyrTempStat
        case 0xE863: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LEADE:  BEQ ChkNewSpell
        case 0xE865: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEAE0:  STA AmountLB
        case 0xE868: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEAE2:  JSR DoDialogHiBlock
        case 0xE86B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEAE6:  LDA DisplayedLevel
        case 0xE86E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEAE8:  CMP #LVL_03
        case 0xE870: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LEAEA:  BEQ NewSpellDialog
        case 0xE872: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEAEC:  CMP #LVL_04
        case 0xE874: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LEAEE:  BEQ NewSpellDialog
        case 0xE876: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEAF0:  CMP #LVL_07
        case 0xE878: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LEAF2:  BEQ NewSpellDialog
        case 0xE87A: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEAF4:  CMP #LVL_09
        case 0xE87C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LEAF6:  BEQ NewSpellDialog
        case 0xE87E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEAF8:  CMP #LVL_10
        case 0xE880: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LEAFA:  BEQ NewSpellDialog
        case 0xE882: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEAFC:  CMP #LVL_12
        case 0xE884: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LEAFE:  BEQ NewSpellDialog
        case 0xE886: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEB00:  CMP #LVL_13
        case 0xE888: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LEB02:  BEQ NewSpellDialog
        case 0xE88A: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEB04:  CMP #LVL_15
        case 0xE88C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LEB06:  BEQ NewSpellDialog
        case 0xE88E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEB08:  CMP #LVL_17
        case 0xE890: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LEB0A:  BEQ NewSpellDialog
        case 0xE892: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEB0C:  CMP #LVL_19
        case 0xE894: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LEB0E:  BNE +
        case 0xE896: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEB10:  JSR DoDialogLoBlock
        case 0xE899: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEB18:  JMP ExitFight
        case 0xE89C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEB1B:  LDA PlayerFlags
        case 0xE89F: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEB1D:  AND #F_EN_ASLEEP
        case 0xE8A1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LEB1F:  BEQ DoEnemyAttack
        case 0xE8A3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEB24:  LDA RandNumUB
        case 0xE8A6: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEB26:  AND #$03
        case 0xE8A8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LEB28:  BEQ -
        case 0xE8AA: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEB2A:  CMP #$01
        case 0xE8AC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LEB2C:  BNE EnStillAsleep
        case 0xE8AE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEB2E:  LDA PlayerFlags
        case 0xE8B1: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEB30:  AND #$BF
        case 0xE8B3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEB32:  STA PlayerFlags
        case 0xE8B6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEB34:  JSR CopyEnUpperBytes
        case 0xE8B9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEB37:  JSR DoDialogLoBlock
        case 0xE8BC: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEB3B:  JMP DoEnemyAttack
        case 0xE8BF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEB3E:  JSR CopyEnUpperBytes
        case 0xE8C2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEB41:  JSR DoDialogLoBlock
        case 0xE8C5: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEB45:  JMP StartPlayerTurn
        case 0xE8C8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEB48:  JSR CheckEnRun
        case 0xE8CB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEB4B:  JSR UpdateRandNum
        case 0xE8CE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEB4E:  LDA EnSpell
        case 0xE8D1: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEB51:  AND #$30
        case 0xE8D3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEB53:  STA GenByte3C
        case 0xE8D6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEB55:  LDA RandNumUB
        case 0xE8D9: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEB57:  AND #$30
        case 0xE8DB: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LEB59:  CMP GenByte3C
        case 0xE8DE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LEB5B:  BCS EnCheckHurtFire
        case 0xE8E0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEB5D:  LDA EnSpell
        case 0xE8E3: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEB60:  AND #$C0
        case 0xE8E5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LEB62:  BNE +
        case 0xE8E7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEB64:  LDA PlayerFlags
        case 0xE8EA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0xC07E; return; } // LEB66:  BMI EnCheckHurtFire
        case 0xE8EC: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEB68:  JMP EnCastSleep
        case 0xE8EF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LEB6D:  BNE +
        case 0xE8F1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEB6F:  LDA PlayerFlags
        case 0xE8F4: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEB71:  AND #F_PLR_STOPSPEL
        case 0xE8F6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LEB73:  BNE EnCheckHurtFire
        case 0xE8F8: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEB75:  JMP EnCastStopspell
        case 0xE8FB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LEB7A:  BNE +
        case 0xE8FD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEB7C:  LDA EnBaseHP
        case 0xE900: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LEB7F:  LSR
        case 0xE901: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LEB80:  LSR
        case 0xE902: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LEB81:  CMP EnCurntHP
        case 0xE905: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LEB83:  BCC EnCheckHurtFire
        case 0xE907: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEB85:  JMP EnCastHeal
        case 0xE90A: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LEB8B:  LSR
        case 0xE90B: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LEB8C:  LSR
        case 0xE90C: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LEB8D:  CMP EnCurntHP
        case 0xE90F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LEB8F:  BCC EnCheckHurtFire
        case 0xE911: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEB91:  JMP EnCastHealmore
        case 0xE914: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEB94:  JSR UpdateRandNum
        case 0xE917: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEB97:  LDA EnSpell
        case 0xE91A: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEB9A:  AND #$03
        case 0xE91C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEB9C:  STA GenByte3C
        case 0xE91F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEB9E:  LDA RandNumUB
        case 0xE922: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEBA0:  AND #$03
        case 0xE924: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LEBA2:  CMP GenByte3C
        case 0xE927: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LEBA4:  BCS EnPhysAttack
        case 0xE929: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEBA6:  LDA EnSpell
        case 0xE92C: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEBA9:  AND #$0C
        case 0xE92E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LEBAB:  BNE +
        case 0xE930: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEBAD:  JMP EnCastHurt
        case 0xE933: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LEBB2:  BNE +
        case 0xE935: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEBB4:  JMP EnCastHurtmore
        case 0xE938: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LEBB9:  BNE +
        case 0xE93A: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEBBB:  JMP EnCastFire1
        case 0xE93D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEBC1:  LDA #SFX_ATCK_PREP
        case 0xE93F: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LEBC3:  BRK
        case 0xE940: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEBC6:  JSR CopyEnUpperBytes
        case 0xE943: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEBC9:  JSR DoDialogLoBlock
        case 0xE946: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEBCD:  LDA EnBaseAtt
        case 0xE949: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEBD0:  STA AttackStat
        case 0xE94C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEBD2:  LDA DisplayedDefns
        case 0xE94F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEBD4:  STA DefenseStat
        case 0xE952: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEBD6:  JSR EnCalcHitDmg
        case 0xE955: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEBD9:  LDA CalcDamage
        case 0xE958: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LEBDB:  BNE EnHitsPlayer
        case 0xE95A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEBDD:  LDA #SFX_MISSED2
        case 0xE95C: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LEBDF:  BRK
        case 0xE95D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEBE2:  JSR DoDialogLoBlock
        case 0xE960: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEBE6:  JMP StartPlayerTurn
        case 0xE963: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEBE9:  STA GenByte00
        case 0xE966: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEBEB:  JMP PlayerHit
        case 0xE969: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEBEE:  JSR CopyEnUpperBytes
        case 0xE96C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEBF1:  JSR DoDialogLoBlock
        case 0xE96F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEBF5:  LDA SpellToCast
        case 0xE972: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEBF7:  JSR GetDescriptionByte
        case 0xE975: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEBFA:  JSR DoDialogLoBlock
        case 0xE978: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEBFE:  LDA #SFX_SPELL
        case 0xE97A: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LEC00:  BRK
        case 0xE97B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEC03:  LDA SplFlshBGPalPtr
        case 0xE97E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEC06:  STA GenPtr42LB
        case 0xE981: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEC08:  LDA SplFlshBGPalPtr+1
        case 0xE984: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEC0B:  STA GenPtr42UB
        case 0xE987: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEC0D:  JSR PaletteFlash
        case 0xE98A: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LEC10:  BRK
        case 0xE98B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEC13:  LDA PlayerFlags
        case 0xE98E: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEC15:  AND #F_EN_STOPSPEL
        case 0xE990: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LEC17:  BNE EnStopSplDialog
        case 0xE992: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LEC19:  RTS
        case 0xE993: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEC1A:  JSR DoDialogLoBlock
        case 0xE996: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LEC1E:  PLA
        case 0xE997: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LEC1F:  PLA
        case 0xE998: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEC20:  JMP StartPlayerTurn
        case 0xE99B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEC23:  LDA #DSC_HURT-4
        case 0xE99D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEC25:  STA SpellToCast
        case 0xE9A0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEC27:  JSR EnCastSpell
        case 0xE9A3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEC2A:  JSR UpdateRandNum
        case 0xE9A6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEC2D:  LDA RandNumUB
        case 0xE9A9: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEC2F:  AND #$07
        case 0xE9AB: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LEC31:  CLC
        case 0xE9AC: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LEC32:  ADC #$03
        case 0xE9AE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEC34:  STA PlyrDamage
        case 0xE9B1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEC36:  LDA EqippedItems
        case 0xE9B4: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEC38:  AND #AR_ARMOR
        case 0xE9B6: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEC3A:  CMP #AR_ERDK_ARMR
        case 0xE9B8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LEC3C:  BEQ ReducedSpellDmg
        case 0xE9BA: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEC3E:  CMP #AR_MAGIC_ARMR
        case 0xE9BC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LEC40:  BNE DoPlyrDmg
        case 0xE9BE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEC42:  LDA PlyrDamage
        case 0xE9C1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEC44:  STA DivNum1LB
        case 0xE9C4: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEC46:  LDA #$03
        case 0xE9C6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEC48:  STA DivNum2
        case 0xE9C9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEC4A:  JSR ByteDivide
        case 0xE9CC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEC4D:  LDA DivQuotient
        case 0xE9CF: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LEC4F:  ASL
        case 0xE9D0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEC50:  STA PlyrDamage
        case 0xE9D3: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEC52:  JMP PlayerHit
        case 0xE9D6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEC55:  LDA #DSC_HURTMORE-4
        case 0xE9D8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEC57:  STA SpellToCast
        case 0xE9DB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEC59:  JSR EnCastSpell
        case 0xE9DE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEC5C:  JSR UpdateRandNum
        case 0xE9E1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEC5F:  LDA RandNumUB
        case 0xE9E4: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEC61:  AND #$0F
        case 0xE9E6: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LEC63:  CLC
        case 0xE9E7: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LEC64:  ADC #$1E
        case 0xE9E9: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEC66:  JMP EnCalcSpllDmg
        case 0xE9EC: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEC69:  LDA #DSC_STOPSPELL-4
        case 0xE9EE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEC6B:  STA SpellToCast
        case 0xE9F1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEC6D:  JSR EnCastSpell
        case 0xE9F4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEC70:  LDA EqippedItems
        case 0xE9F7: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEC72:  AND #AR_ARMOR
        case 0xE9F9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEC74:  CMP #AR_ERDK_ARMR
        case 0xE9FB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LEC76:  BEQ BlockStopSpell
        case 0xE9FD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEC78:  JSR UpdateRandNum
        case 0xEA00: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEC7B:  LDA RandNumUB
        case 0xEA03: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LEC7D:  LSR
        case 0xEA04: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LEC7E:  BCC BlockStopSpell
        case 0xEA06: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEC80:  LDA PlayerFlags
        case 0xEA09: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEC82:  ORA #F_PLR_STOPSPEL
        case 0xEA0B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEC84:  STA PlayerFlags
        case 0xEA0E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEC86:  LDA #$FE
        case 0xEA10: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEC8B:  JMP StartPlayerTurn
        case 0xEA13: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEC8E:  LDA #$EB
        case 0xEA15: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LEC90:  BNE -
        case 0xEA17: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEC92:  LDA #DSC_SLEEP-4
        case 0xEA19: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEC94:  STA SpellToCast
        case 0xEA1C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEC96:  JSR EnCastSpell
        case 0xEA1F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEC99:  LDA PlayerFlags
        case 0xEA22: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEC9B:  ORA #F_PLR_ASLEEP
        case 0xEA24: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEC9D:  STA PlayerFlags
        case 0xEA27: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEC9F:  JSR DoDialogHiBlock
        case 0xEA2A: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LECA3:  JMP PlayerAsleepDialog
        case 0xEA2D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LECA6:  LDA #DSC_HEAL-4
        case 0xEA2F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LECA8:  STA SpellToCast
        case 0xEA32: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LECAA:  JSR EnCastSpell
        case 0xEA35: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LECAD:  JSR UpdateRandNum
        case 0xEA38: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LECB0:  LDA RandNumUB
        case 0xEA3B: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LECB2:  AND #$07
        case 0xEA3D: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LECB4:  CLC
        case 0xEA3E: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LECB5:  ADC #$14
        case 0xEA40: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LECB7:  CLC
        case 0xEA41: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LECB8:  ADC EnCurntHP
        case 0xEA44: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LECBA:  CMP EnBaseHP
        case 0xEA47: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LECBD:  BCC +
        case 0xEA49: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LECBF:  LDA EnBaseHP
        case 0xEA4C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LECC4:  JSR CopyEnUpperBytes
        case 0xEA4F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LECC7:  JSR DoDialogHiBlock
        case 0xEA52: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LECCB:  JMP StartPlayerTurn
        case 0xEA55: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LECCE:  LDA #DSC_HEALMORE-4
        case 0xEA57: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LECD0:  STA SpellToCast
        case 0xEA5A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LECD2:  JSR EnCastSpell
        case 0xEA5D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LECD5:  JSR UpdateRandNum
        case 0xEA60: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LECD8:  LDA RandNumUB
        case 0xEA63: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LECDA:  AND #$0F
        case 0xEA65: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LECDC:  CLC
        case 0xEA66: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LECDD:  ADC #$55
        case 0xEA68: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LECDF:  BNE EnemyAddHP
        case 0xEA6A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LECE1:  JSR UpdateRandNum
        case 0xEA6D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LECE4:  LDA RandNumUB
        case 0xEA70: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LECE6:  AND #$07
        case 0xEA72: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LECE8:  CLC
        case 0xEA73: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LECE9:  ADC #$41
        case 0xEA75: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LECEB:  BNE CalcPlyrDmg
        case 0xEA77: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LECED:  JSR UpdateRandNum
        case 0xEA7A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LECF0:  LDA RandNumUB
        case 0xEA7D: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LECF2:  AND #$07
        case 0xEA7F: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LECF4:  ORA #$10
        case 0xEA81: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LECF6:  STA PlyrDamage
        case 0xEA84: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LECF8:  LDA #$00
        case 0xEA86: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LECFA:  STA DmgNotUsed
        case 0xEA89: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LECFC:  LDA EqippedItems
        case 0xEA8C: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LECFE:  AND #AR_ARMOR
        case 0xEA8E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LED00:  CMP #AR_ERDK_ARMR
        case 0xEA90: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LED02:  BNE DoFireSFX
        case 0xEA92: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 2; return; // LED04:  LDA $00
        case 0xEA94: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LED06:  STA DivNum1LB
        case 0xEA97: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LED08:  LDA #$03
        case 0xEA99: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LED0A:  STA DivNum2
        case 0xEA9C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LED0C:  JSR ByteDivide
        case 0xEA9F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LED0F:  LDA DivQuotient
        case 0xEAA2: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LED11:  ASL
        case 0xEAA3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 2; return; // LED12:  STA $00
        case 0xEAA5: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LED14:  LDA #SFX_FIRE
        case 0xEAA7: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LED16:  BRK
        case 0xEAA8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LED19:  JSR CopyEnUpperBytes
        case 0xEAAB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LED1C:  JSR DoDialogLoBlock
        case 0xEAAE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LED20:  LDA #SFX_PLYR_HIT1
        case 0xEAB0: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LED22:  BRK
        case 0xEAB1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LED25:  LDA #$00
        case 0xEAB3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LED27:  STA DmgNotUsed
        case 0xEAB6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LED29:  LDA HitPoints
        case 0xEAB9: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LED2B:  SEC
        case 0xEABA: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LED2C:  SBC PlyrDamage
        case 0xEABD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LED2E:  BCS +
        case 0xEABF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LED30:  LDA #$00
        case 0xEAC1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LED34:  LDA #$08
        case 0xEAC3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LED36:  STA ShakeCounter
        case 0xEAC6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LED38:  LDA ScrollX
        case 0xEAC9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LED3A:  STA ShakeX
        case 0xEACC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LED3C:  LDA ScrollY
        case 0xEACF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LED3E:  STA ShakeY
        case 0xEAD2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LED40:  JSR WaitForNMI
        case 0xEAD5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LED43:  LDA HitPoints
        case 0xEAD8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LED45:  BEQ +
        case 0xEADA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LED47:  JSR WaitForNMI
        case 0xEADD: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LED4A:  JMP ChkShakeCounter
        case 0xEAE0: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LED4F:  CMP #EN_DRAGONLORD2
        case 0xEAE2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LED51:  BEQ ChkShakeCounter
        case 0xEAE4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LED53:  JSR RedFlashScreen
        case 0xEAE7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LED56:  LDA ShakeCounter
        case 0xEAEA: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LED58:  AND #$01
        case 0xEAEC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LED5A:  BNE UpdateShakeY
        case 0xEAEE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LED5C:  LDA ShakeX
        case 0xEAF1: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LED5E:  CLC
        case 0xEAF2: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LED5F:  ADC #$02
        case 0xEAF4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LED61:  STA ScrollX
        case 0xEAF7: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LED63:  JMP DoShake
        case 0xEAFA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LED66:  LDA ShakeY
        case 0xEAFD: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LED68:  CLC
        case 0xEAFE: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LED69:  ADC #$02
        case 0xEB00: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LED6B:  STA ScrollY
        case 0xEB03: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LED6D:  LDA EnNumber
        case 0xEB06: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LED6F:  CMP #EN_DRAGONLORD2
        case 0xEB08: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LED71:  BNE +
        case 0xEB0A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LED73:  LDA ShakeX
        case 0xEB0D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LED75:  STA ScrollX
        case 0xEB10: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LED77:  LDA ShakeY
        case 0xEB13: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LED79:  STA ScrollY
        case 0xEB16: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LED7E:  JSR LoadRegBGPal
        case 0xEB19: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LED81:  LDA ShakeX
        case 0xEB1C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LED83:  STA ScrollX
        case 0xEB1F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LED85:  LDA ShakeY
        case 0xEB22: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LED87:  STA ScrollY
        case 0xEB25: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LED89:  DEC ShakeCounter
        case 0xEB28: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LED8B:  BNE ShakeScreenLoop
        case 0xEB2A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LED8D:  JSR DoDialogLoBlock
        case 0xEB2D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LED91:  JSR Dowindow
        case 0xEB30: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LED95:  LDA HitPoints
        case 0xEB33: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LED97:  BEQ PlayerHasDied
        case 0xEB35: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LED99:  JMP StartPlayerTurn
        case 0xEB38: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LED9C:  LDA #MSC_DEATH
        case 0xEB3A: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LED9E:  BRK
        case 0xEB3B: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LEDA1:  BRK
        case 0xEB3C: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEDA4:  JMP DeathText
        case 0xEB3F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEDA7:  LDA #MSC_DEATH
        case 0xEB41: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LEDA9:  BRK
        case 0xEB42: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LEDAC:  BRK
        case 0xEB43: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEDAF:  LDA #WND_DIALOG
        case 0xEB45: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEDB1:  JSR _DoWindow
        case 0xEB48: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEDB4:  JSR DoDialogLoBlock
        case 0xEB4B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEDB8:  LDA #STRT_FULL_HP
        case 0xEB4D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEDBA:  STA ThisStrtStat
        case 0xEB50: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEDBD:  LDA #$00
        case 0xEB52: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEDBF:  STA CharDirection
        case 0xEB55: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEDC5:  LDA JoypadBtns
        case 0xEB58: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEDC7:  AND #$09
        case 0xEB5A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LEDC9:  BEQ -
        case 0xEB5C: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LEDCB:  LSR GoldUB
        case 0xEB5F: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LEDCD:  ROR GoldLB
        case 0xEB62: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEDCF:  LDA PlayerFlags
        case 0xEB65: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEDD1:  AND #$FE
        case 0xEB67: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEDD3:  STA PlayerFlags
        case 0xEB6A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEDD5:  LDA EnNumber
        case 0xEB6D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEDD7:  STA DrgnLrdPal
        case 0xEB70: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEDDA:  LDA #$00
        case 0xEB72: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEDDC:  STA EnNumber
        case 0xEB75: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEDDE:  JSR StartAtThroneRoom
        case 0xEB78: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEDE1:  LDA ModsnSpells
        case 0xEB7B: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEDE3:  AND #$C0
        case 0xEB7D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LEDE5:  BEQ KingDeathDialog
        case 0xEB7F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEDE7:  JSR DoDialogHiBlock
        case 0xEB82: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LEDEB:  LDX #$0C
        case 0xEB84: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEDED:  LDA #DIR_DOWN
        case 0xEB86: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEDEF:  JMP ChangeMaps
        case 0xEB89: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEDF2:  JSR DoDialogHiBlock
        case 0xEB8C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEDF6:  LDA DisplayedLevel
        case 0xEB8F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEDF8:  CMP #LVL_30
        case 0xEB91: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LEDFA:  BEQ NowGoText
        case 0xEB93: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEDFC:  JSR GetExpRemaining
        case 0xEB96: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEDFF:  JSR DoDialogHiBlock
        case 0xEB99: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEE03:  JSR DoDialogHiBlock
        case 0xEB9C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEE07:  JSR WaitForBtnRelease
        case 0xEB9F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEE0A:  LDA #WND_DIALOG
        case 0xEBA1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LEE0C:  JSR RemoveWindow
        case 0xEBA4: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEE0F:  LDA #NPC_MOVE
        case 0xEBA6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEE11:  STA StopNPCMove
        case 0xEBA9: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LEE13:  RTS
        case 0xEBAA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEE14:  JSR WaitForNMI
        case 0xEBAD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEE17:  LDA RedFlashPalPtr
        case 0xEBB0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEE1A:  STA PalPtrLB
        case 0xEBB3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEE1C:  LDA RedFlashPalPtr+1
        case 0xEBB6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEE1F:  STA PalPtrUB
        case 0xEBB9: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEE21:  LDA #$00
        case 0xEBBB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEE23:  STA PalModByte
        case 0xEBBE: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LEE25:  JMP PrepBGPalLoad
        case 0xEBC1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEE28:  JSR WaitForNMI
        case 0xEBC4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEE2B:  LDA EnNumber
        case 0xEBC7: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEE2D:  CMP #EN_DRAGONLORD2
        case 0xEBC9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LEE2F:  BNE LoadRegMapPal
        case 0xEBCB: cycles_to_run -= 2; reg_A = bus_read(0xC07E); update_nz(reg_A); reg_PC += 3; return; // LEE31:  LDA FnlNormBGPalPtr
        case 0xEBCE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEE34:  STA PalPtrLB
        case 0xEBD1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEE36:  LDA FnlNormBGPalPtr+1
        case 0xEBD4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEE39:  STA PalPtrUB
        case 0xEBD7: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEE3B:  JMP FinishRegPalLoad
        case 0xEBDA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEE3E:  LDA OverworldPalPtr
        case 0xEBDD: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LEE41:  CLC
        case 0xEBDE: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LEE42:  ADC MapType
        case 0xEBE1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEE44:  STA PalPtrLB
        case 0xEBE4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEE46:  LDA OverworldPalPtr+1
        case 0xEBE7: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LEE49:  ADC #$00
        case 0xEBE9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEE4B:  STA PalPtrUB
        case 0xEBEC: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEE4D:  LDA #$00
        case 0xEBEE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEE4F:  STA PalModByte
        case 0xEBF1: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LEE51:  JMP PrepBGPalLoad
        case 0xEBF4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEE54:  JSR ClearSpriteRAM
        case 0xEBF7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEE57:  JSR WaitForBtnRelease
        case 0xEBFA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEE5A:  JSR WaitForNMI
        case 0xEBFD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEE5D:  LDA RegSPPalPtr
        case 0xEC00: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEE60:  STA PalPtrLB
        case 0xEC03: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEE62:  LDA RegSPPalPtr+1
        case 0xEC06: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEE65:  STA PalPtrUB
        case 0xEC09: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEE67:  LDA #$00
        case 0xEC0B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEE69:  STA PalModByte
        case 0xEC0E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEE6B:  JSR PrepSPPalLoad
        case 0xEC11: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEE6E:  LDA NPCUpdateCntr
        case 0xEC14: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEE70:  AND #$70
        case 0xEC16: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LEE72:  BEQ +
        case 0xEC18: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEE74:  LDA #$FF
        case 0xEC1A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEE78:  LDA #WND_DIALOG
        case 0xEC1C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LEE7A:  JSR RemoveWindow
        case 0xEC1F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEE7D:  LDA #WND_ALPHBT
        case 0xEC21: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LEE7F:  JSR RemoveWindow
        case 0xEC24: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEE82:  LDA #DIR_DOWN
        case 0xEC26: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEE84:  STA CharDirection
        case 0xEC29: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEE87:  JSR WaitForNMI
        case 0xEC2C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEE8A:  JSR Bank2ToNT1
        case 0xEC2F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LEE8D:  JSR DoSprites
        case 0xEC32: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LEE90:  RTS
        case 0xEC33: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEE91:  JSR UpdateRandNum
        case 0xEC36: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEE94:  LDA EnNumber
        case 0xEC39: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEE96:  CMP #EN_STONEMAN
        case 0xEC3B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LEE98:  BCC ChkGrDrgnRun
        case 0xEC3D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEE9A:  LDA RandNumUB
        case 0xEC40: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEE9C:  JMP CalcNextOdds
        case 0xEC43: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEE9F:  CMP #EN_GDRAGON
        case 0xEC45: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LEEA1:  BCC ChkDrollMRun
        case 0xEC47: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEEA3:  LDA RandNumUB
        case 0xEC4A: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEEA5:  AND #$7F
        case 0xEC4C: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEEA7:  JMP CalcNextOdds
        case 0xEC4F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEEAA:  CMP #EN_DROLLMAGI
        case 0xEC51: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LEEAC:  BCC CalcWhoIsNext
        case 0xEC53: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEEAE:  LDA RandNumUB
        case 0xEC56: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEEB0:  AND #$3F
        case 0xEC58: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEEB2:  STA MultNum2LB
        case 0xEC5B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEEB4:  JSR UpdateRandNum
        case 0xEC5E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEEB7:  LDA RandNumUB
        case 0xEC61: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEEB9:  AND #$1F
        case 0xEC63: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LEEBB:  ADC MultNum2LB
        case 0xEC66: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEEBD:  JMP CalcNextOdds
        case 0xEC69: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEEC0:  JSR UpdateRandNum
        case 0xEC6C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEEC3:  LDA RandNumUB
        case 0xEC6F: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEEC5:  AND #$3F
        case 0xEC71: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEEC7:  STA MultNum1LB
        case 0xEC74: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEEC9:  LDA EnBaseDef
        case 0xEC77: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEECC:  STA MultNum2LB
        case 0xEC7A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEECE:  LDA #$00
        case 0xEC7C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEED0:  STA MultNum1UB
        case 0xEC7F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEED2:  STA MultNum2UB
        case 0xEC82: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEED4:  JSR WordMultiply
        case 0xEC85: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEED7:  LDA MultRsltLB
        case 0xEC88: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEED9:  STA GenWord42LB
        case 0xEC8B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEEDB:  LDA MultRsltUB
        case 0xEC8E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEEDD:  STA GenWord42UB
        case 0xEC91: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEEDF:  JSR UpdateRandNum
        case 0xEC94: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEEE2:  LDA RandNumUB
        case 0xEC97: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEEE4:  STA MultNum1LB
        case 0xEC9A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEEE6:  LDA DisplayedAgi
        case 0xEC9D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEEE8:  STA MultNum2LB
        case 0xECA0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEEEA:  LDA #$00
        case 0xECA2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEEEC:  STA MultNum1UB
        case 0xECA5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEEEE:  STA MultNum2UB
        case 0xECA8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEEF0:  JSR WordMultiply
        case 0xECAB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEEF3:  LDA MultRsltLB
        case 0xECAE: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LEEF5:  SEC
        case 0xECAF: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LEEF6:  SBC GenWord42LB
        case 0xECB2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEEF8:  LDA MultRsltUB
        case 0xECB5: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LEEFA:  SBC GenWord42UB
        case 0xECB8: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LEEFC:  RTS
        case 0xECB9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEEFD:  LDA EnNumber
        case 0xECBC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEEFF:  STA MultNum1LB
        case 0xECBF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEF01:  LDA #$0C
        case 0xECC1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF03:  STA MultNum2LB
        case 0xECC4: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEF05:  LDA #$00
        case 0xECC6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF07:  STA MultNum1UB
        case 0xECC9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF09:  STA MultNum2UB
        case 0xECCC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEF0B:  JSR WordMultiply
        case 0xECCF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEF0E:  LDA MultRsltLB
        case 0xECD2: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LEF10:  CLC
        case 0xECD3: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LEF11:  ADC EnSPPalsPtr
        case 0xECD6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF14:  STA GenPtr3CLB
        case 0xECD9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEF16:  LDA MultRsltUB
        case 0xECDC: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LEF18:  ADC EnSPPalsPtr+1
        case 0xECDF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF1B:  STA GenPtr3CUB
        case 0xECE2: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LEF1D:  TYA
        case 0xECE3: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LEF1E:  PHA
        case 0xECE4: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LEF1F:  LDY #$0B
        case 0xECE6: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LEF21:  LDA (GenPtr3C),Y
        case 0xECE8: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LEF23:  STA EnPalData,Y
        case 0xECEB: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LEF26:  DEY
        case 0xECEC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0xC07E; return; } // LEF27:  BPL EnPalLoop
        case 0xECEE: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LEF29:  PLA
        case 0xECEF: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LEF2A:  TAY
        case 0xECF0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEF2B:  LDA #$03
        case 0xECF2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF2D:  STA SprtPalPtrUB
        case 0xECF5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF2F:  STA PalPtrUB
        case 0xECF8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEF31:  LDA #$A0
        case 0xECFA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF33:  STA SprtPalPtrLB
        case 0xECFD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF35:  STA PalPtrLB
        case 0xED00: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LEF37:  RTS
        case 0xED01: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEF38:  LDA #$05
        case 0xED03: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF3A:  STA PalFlashCntr
        case 0xED06: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEF3C:  LDA GenPtr42LB
        case 0xED09: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF3E:  STA PalPtrLB
        case 0xED0C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEF40:  LDA GenPtr42UB
        case 0xED0F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF42:  STA PalPtrUB
        case 0xED12: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEF44:  JSR WaitForNMI
        case 0xED15: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEF47:  JSR WaitForNMI
        case 0xED18: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEF4A:  JSR WaitForNMI
        case 0xED1B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEF4D:  LDA #$00
        case 0xED1D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF4F:  STA PalModByte
        case 0xED20: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEF51:  JSR PrepSPPalLoad
        case 0xED23: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEF54:  LDA EnNumber
        case 0xED26: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEF56:  CMP #EN_DRAGONLORD2
        case 0xED28: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LEF58:  BNE +
        case 0xED2A: cycles_to_run -= 2; reg_A = bus_read(0xC07E); update_nz(reg_A); reg_PC += 3; return; // LEF5A:  LDA FnlRedBGPalPtr
        case 0xED2D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF5D:  STA PalPtrLB
        case 0xED30: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEF5F:  LDA FnlRedBGPalPtr+1
        case 0xED33: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF62:  STA PalPtrUB
        case 0xED36: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEF64:  JSR PrepBGPalLoad
        case 0xED39: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEF6A:  JSR WaitForNMI
        case 0xED3C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEF6D:  LDA GenPtr42LB
        case 0xED3F: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LEF6F:  PHA
        case 0xED40: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEF70:  LDA GenPtr42UB
        case 0xED43: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LEF72:  PHA
        case 0xED44: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEF73:  JSR LoadEnPalette
        case 0xED47: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LEF76:  PLA
        case 0xED48: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF77:  STA GenPtr42UB
        case 0xED4B: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LEF79:  PLA
        case 0xED4C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF7A:  STA GenPtr42LB
        case 0xED4F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEF7C:  LDA #$00
        case 0xED51: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF7E:  STA PalModByte
        case 0xED54: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEF80:  JSR PrepSPPalLoad
        case 0xED57: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEF83:  LDA EnNumber
        case 0xED5A: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEF85:  CMP #EN_DRAGONLORD2
        case 0xED5C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LEF87:  BNE +
        case 0xED5E: cycles_to_run -= 2; reg_A = bus_read(0xC07E); update_nz(reg_A); reg_PC += 3; return; // LEF89:  LDA FnlNormBGPalPtr
        case 0xED61: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF8C:  STA PalPtrLB
        case 0xED64: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEF8E:  LDA FnlNormBGPalPtr+1
        case 0xED67: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEF91:  STA PalPtrUB
        case 0xED6A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEF93:  JSR PrepBGPalLoad
        case 0xED6D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LEF98:  BNE PalFlashLoop
        case 0xED6F: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LEF9A:  RTS
        case 0xED70: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEFB7:  LDA DisplayedStr
        case 0xED73: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LEFB9:  LSR
        case 0xED74: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LEFBA:  CMP EnBaseAtt
        case 0xED77: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LEFBD:  BCC EnRunExit
        case 0xED79: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEFBF:  JSR UpdateRandNum
        case 0xED7C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEFC2:  LDA RandNumUB
        case 0xED7F: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEFC4:  AND #$03
        case 0xED81: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LEFC6:  BNE EnRunExit
        case 0xED83: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEFC8:  JSR ClearSpriteRAM
        case 0xED86: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LEFCB:  LDA #SFX_RUN
        case 0xED88: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LEFCD:  BRK
        case 0xED89: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LEFD0:  JSR CopyEnUpperBytes
        case 0xED8C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LEFD3:  JSR DoDialogLoBlock
        case 0xED8F: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LEFD7:  LDX MapNumber
        case 0xED92: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LEFD9:  LDA ResumeMusicTbl,X
        case 0xED95: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LEFDC:  BRK
        case 0xED96: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LEFDF:  PLA
        case 0xED97: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LEFE0:  PLA
        case 0xED98: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LEFE1:  JMP ExitFight
        case 0xED9B: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LEFE4:  RTS
        case 0xED9C: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LEFE5:  LSR DefenseStat
        case 0xED9F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEFE7:  LDA AttackStat
        case 0xEDA2: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LEFE9:  SEC
        case 0xEDA3: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LEFEA:  SBC DefenseStat
        case 0xEDA6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LEFEC:  BCC PlyrWeakAttack
        case 0xEDA8: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LEFEE:  CMP #$02
        case 0xEDAA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LEFF0:  BCS NormalAttack
        case 0xEDAC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LEFF2:  BCC PlyrWeakAttack
        case 0xEDAE: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LEFF4:  LSR DefenseStat
        case 0xEDB1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEFF6:  LDA AttackStat
        case 0xEDB4: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LEFF8:  LSR
        case 0xEDB5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LEFF9:  STA MultNum2LB
        case 0xEDB8: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LEFFB:  INC MultNum2LB
        case 0xEDBB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LEFFD:  LDA AttackStat
        case 0xEDBE: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LEFFF:  SEC
        case 0xEDBF: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LF000:  SBC DefenseStat
        case 0xEDC2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LF002:  BCC +
        case 0xEDC4: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LF004:  CMP MultNum2LB
        case 0xEDC7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LF006:  BCS NormalAttack
        case 0xEDC9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF00B:  LDA RandNumUB
        case 0xEDCC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF00D:  STA MultNum1LB
        case 0xEDCF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF00F:  LDA #$00
        case 0xEDD1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF011:  STA MultNum1UB
        case 0xEDD4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF013:  STA MultNum2UB
        case 0xEDD7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LF015:  JSR WordMultiply
        case 0xEDDA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF018:  LDA MultRsltUB
        case 0xEDDD: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LF01A:  CLC
        case 0xEDDE: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LF01B:  ADC #$02
        case 0xEDE0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF01D:  STA DivNum1LB
        case 0xEDE3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF01F:  LDA #$03
        case 0xEDE5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF021:  STA DivNum2
        case 0xEDE8: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LF023:  JMP ByteDivide
        case 0xEDEB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LF026:  JSR UpdateRandNum
        case 0xEDEE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF029:  LDA RandNumUB
        case 0xEDF1: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF02B:  AND #$01
        case 0xEDF3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF02D:  STA CalcDamage
        case 0xEDF6: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LF02F:  RTS
        case 0xEDF7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF030:  STA BaseAttack
        case 0xEDFA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF032:  STA MultNum2LB
        case 0xEDFD: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LF034:  INC MultNum2LB
        case 0xEE00: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LF036:  JSR UpdateRandNum
        case 0xEE03: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF039:  LDA RandNumUB
        case 0xEE06: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF03B:  STA MultNum1LB
        case 0xEE09: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF03D:  LDA #$00
        case 0xEE0B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF03F:  STA MultNum1UB
        case 0xEE0E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF041:  STA MultNum2UB
        case 0xEE11: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LF043:  JSR WordMultiply
        case 0xEE14: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF046:  LDA MultRsltUB
        case 0xEE17: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LF048:  CLC
        case 0xEE18: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LF049:  ADC BaseAttack
        case 0xEE1B: cycles_to_run -= 2; reg_A = cpu_ror(reg_A); reg_PC += 1; return; // LF04B:  ROR
        case 0xEE1C: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LF04C:  LSR
        case 0xEE1D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF04D:  STA CalcDamage
        case 0xEE20: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LF04F:  RTS
        case 0xEE21: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LF050:  LDX #LVL_TBL_LAST
        case 0xEE23: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF052:  LDA #LVL_30
        case 0xEE25: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF054:  STA DisplayedLevel
        case 0xEE28: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF056:  LDA ExpLB
        case 0xEE2B: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LF058:  SEC
        case 0xEE2C: cycles_to_run -= 2; cpu_sbc(bus_read(addr_abs_x(0xC07E, nullptr))); reg_PC += 3; return; // LF059:  SBC LevelUpTbl,X
        case 0xEE2F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF05C:  LDA ExpUB
        case 0xEE32: cycles_to_run -= 2; cpu_sbc(bus_read(addr_abs_x(0x0000, nullptr))); reg_PC += 3; return; // LF05E:  SBC LevelUpTbl+1,X
        case 0xEE35: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LF061:  BCS LevelFound
        case 0xEE37: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LF063:  DEC DisplayedLevel
        case 0xEE3A: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LF065:  DEX
        case 0xEE3B: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LF066:  DEX
        case 0xEE3C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF067:  BNE GetLevelLoop
        case 0xEE3E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF069:  LDA DisplayedLevel
        case 0xEE41: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LF06B:  SEC
        case 0xEE42: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LF06C:  SBC #$01
        case 0xEE44: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LF06E:  ASL
        case 0xEE45: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF06F:  STA LevelDatPtr
        case 0xEE48: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LF071:  ASL
        case 0xEE49: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LF072:  CLC
        case 0xEE4A: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LF073:  ADC LevelDatPtr
        case 0xEE4D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF075:  STA LevelDatPtr
        case 0xEE50: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF077:  LDA #$FF
        case 0xEE52: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF079:  STA NMIStatus
        case 0xEE55: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF07E:  LDA NMIStatus
        case 0xEE58: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LF080:  BNE -
        case 0xEE5A: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LF082:  BRK
        case 0xEE5B: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LF085:  LDX #$04
        case 0xEE5D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF087:  LDA #$00
        case 0xEE5F: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LF089:  CLC
        case 0xEE60: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LF08D:  DEX
        case 0xEE61: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF08E:  BNE NameAddLoop
        case 0xEE63: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF090:  STA StatBonus
        case 0xEE66: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF092:  AND #$03
        case 0xEE68: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF094:  STA StatPenalty
        case 0xEE6B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF096:  LDA StatBonus
        case 0xEE6E: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LF098:  LSR
        case 0xEE6F: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LF099:  LSR
        case 0xEE70: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF09A:  AND #$03
        case 0xEE72: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF09C:  STA StatBonus
        case 0xEE75: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF09E:  LDA StatPenalty
        case 0xEE78: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LF0A0:  LSR
        case 0xEE79: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LF0A1:  BCS MaxMPPenalty
        case 0xEE7B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF0A3:  LDA DisplayedStr
        case 0xEE7E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF0A5:  JSR ReduceStat
        case 0xEE81: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF0A8:  STA DisplayedStr
        case 0xEE84: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF0AA:  JMP ChkAgiPenalty
        case 0xEE87: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF0AD:  LDA DisplayedMaxMP
        case 0xEE8A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LF0AF:  BEQ ChkAgiPenalty
        case 0xEE8C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF0B1:  JSR ReduceStat
        case 0xEE8F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF0B4:  STA DisplayedMaxMP
        case 0xEE92: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF0B6:  LDA StatPenalty
        case 0xEE95: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF0B8:  AND #$02
        case 0xEE97: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF0BA:  BNE MaxHPPenalty
        case 0xEE99: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF0BC:  LDA DisplayedAgi
        case 0xEE9C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF0BE:  JSR ReduceStat
        case 0xEE9F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF0C1:  STA DisplayedAgi
        case 0xEEA2: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF0C3:  JMP AddItemBonuses
        case 0xEEA5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF0C6:  LDA DisplayedMaxHP
        case 0xEEA8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF0C8:  JSR ReduceStat
        case 0xEEAB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF0CB:  STA DisplayedMaxHP
        case 0xEEAE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF0CD:  LDA EqippedItems
        case 0xEEB1: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LF0CF:  LSR
        case 0xEEB2: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LF0D0:  LSR
        case 0xEEB3: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LF0D1:  LSR
        case 0xEEB4: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LF0D2:  LSR
        case 0xEEB5: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LF0D3:  LSR
        case 0xEEB6: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LF0D4:  TAX
        case 0xEEB7: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LF0D5:  LDA WeaponsBonusTbl,X
        case 0xEEBA: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LF0D8:  CLC
        case 0xEEBB: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LF0D9:  ADC DisplayedStr
        case 0xEEBE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF0DB:  STA DisplayedAttck
        case 0xEEC1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF0DD:  LDA DisplayedAgi
        case 0xEEC4: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LF0DF:  LSR
        case 0xEEC5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF0E0:  STA DisplayedDefns
        case 0xEEC8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF0E2:  LDA EqippedItems
        case 0xEECB: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LF0E4:  LSR
        case 0xEECC: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LF0E5:  LSR
        case 0xEECD: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF0E6:  AND #AR_ARMOR/4
        case 0xEECF: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LF0E8:  TAX
        case 0xEED0: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LF0E9:  LDA ArmorBonusTbl,X
        case 0xEED3: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LF0EC:  CLC
        case 0xEED4: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LF0ED:  ADC DisplayedDefns
        case 0xEED7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF0EF:  STA DisplayedDefns
        case 0xEEDA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF0F1:  LDA EqippedItems
        case 0xEEDD: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF0F3:  AND #SH_SHIELDS
        case 0xEEDF: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LF0F5:  TAX
        case 0xEEE0: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LF0F6:  LDA ShieldBonusTbl,X
        case 0xEEE3: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LF0F9:  CLC
        case 0xEEE4: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LF0FA:  ADC DisplayedDefns
        case 0xEEE7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF0FC:  STA DisplayedDefns
        case 0xEEEA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF0FE:  LDA ModsnSpells
        case 0xEEED: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF100:  AND #F_DRGSCALE
        case 0xEEEF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LF102:  BEQ +
        case 0xEEF1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF104:  LDA DisplayedDefns
        case 0xEEF4: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LF106:  CLC
        case 0xEEF5: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LF107:  ADC #$02
        case 0xEEF7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF109:  STA DisplayedDefns
        case 0xEEFA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF10C:  STA MultNum1LB
        case 0xEEFD: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF10E:  LDA #$09
        case 0xEEFF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF110:  STA MultNum2LB
        case 0xEF02: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF112:  LDA #$00
        case 0xEF04: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF114:  STA MultNum1UB
        case 0xEF07: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF116:  STA MultNum2UB
        case 0xEF0A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LF118:  JSR WordMultiply
        case 0xEF0D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF11B:  LDA MultRsltLB
        case 0xEF10: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF11D:  STA DivNum1LB
        case 0xEF13: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF11F:  LDA MultRsltUB
        case 0xEF16: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF121:  STA DivNmu1UB
        case 0xEF19: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF123:  LDA #$0A
        case 0xEF1B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF125:  STA DivNum2
        case 0xEF1E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF127:  LDA #$00
        case 0xEF20: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF129:  STA DivNum2NU
        case 0xEF23: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LF12B:  JSR WordDivide
        case 0xEF26: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF12E:  LDA DivQuotient
        case 0xEF29: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LF130:  CLC
        case 0xEF2A: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LF131:  ADC StatBonus
        case 0xEF2D: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LF133:  RTS
        case 0xEF2E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF134:  LDA DisplayedLevel
        case 0xEF31: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LF136:  ASL
        case 0xEF32: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LF137:  TAX
        case 0xEF33: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0xC07E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LF138:  LDA LevelUpTbl,X
        case 0xEF36: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LF13B:  SEC
        case 0xEF37: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LF13C:  SBC ExpLB
        case 0xEF3A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF13E:  STA GenWrd00LB
        case 0xEF3D: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LF140:  LDA LevelUpTbl+1,X
        case 0xEF40: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LF143:  SBC ExpUB
        case 0xEF43: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF145:  STA GenWrd00UB
        case 0xEF46: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LF147:  RTS
        case 0xEF47: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF148:  LDA #$01
        case 0xEF49: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LF14A:  JSR WaitForPPUBufSpace
        case 0xEF4C: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF14D:  JMP SaveCurrentGame
        case 0xEF4F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF678:  JSR ShowStartGame
        case 0xEF52: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LF67B:  LDX SaveNumber
        case 0xEF55: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LF67E:  LDA StartStatus1,X
        case 0xEF58: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF681:  STA ThisStrtStat
        case 0xEF5B: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LF684:  RTS
        case 0xEF5C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF685:  LDA SaveSelected
        case 0xEF5F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF688:  JSR Copy100Times
        case 0xEF62: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LF68B:  JSR GetPlayerStatPtr
        case 0xEF65: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LF68E:  RTS
        case 0xEF66: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF68F:  LDA #$00
        case 0xEF68: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF691:  STA ExpLB
        case 0xEF6B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF693:  STA ExpUB
        case 0xEF6E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF695:  STA GoldLB
        case 0xEF71: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF697:  STA GoldUB
        case 0xEF74: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF699:  STA InventorySlot12
        case 0xEF77: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF69B:  STA InventorySlot34
        case 0xEF7A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF69D:  STA InventorySlot56
        case 0xEF7D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF69F:  STA InventorySlot78
        case 0xEF80: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF6A1:  STA InventoryKeys
        case 0xEF83: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF6A3:  STA InventoryHerbs
        case 0xEF86: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF6A5:  STA EqippedItems
        case 0xEF89: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF6A7:  STA ModsnSpells
        case 0xEF8C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF6A9:  STA PlayerFlags
        case 0xEF8F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF6AB:  STA StoryFlags
        case 0xEF92: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF6AD:  STA HitPoints
        case 0xEF95: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF6AF:  STA MagicPoints
        case 0xEF98: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LF6B1:  LDX SaveNumber
        case 0xEF9B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF6B4:  LDA #STRT_FULL_HP
        case 0xEF9D: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LF6B6:  STA StartStatus1,X
        case 0xEFA0: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LF6B9:  RTS
        case 0xEFA1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF6BA:  LDA OpnSltSelected
        case 0xEFA4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF6BD:  JSR GetSaveGameBase
        case 0xEFA7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF6C0:  LDA GameDatPtrLB
        case 0xEFAA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF6C2:  STA RAMTrgtPtrLB
        case 0xEFAD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF6C4:  LDA GameDatPtrUB
        case 0xEFB0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF6C6:  STA RAMTrgtPtrUB
        case 0xEFB3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF6C8:  LDA SaveNumber
        case 0xEFB6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF6CB:  JSR GetSaveGameBase
        case 0xEFB9: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LF6CE:  LDY #$00
        case 0xEFBB: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LF6D2:  STA (RAMTrgtPtr),Y
        case 0xEFBD: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LF6D4:  DEY
        case 0xEFBE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LF6D5:  BNE -
        case 0xEFC0: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LF6D7:  LDX GameDatPtrUB
        case 0xEFC3: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LF6D9:  INX
        case 0xEFC4: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LF6DA:  STX GameDatPtrUB
        case 0xEFC7: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LF6DC:  LDX RAMTrgtPtrUB
        case 0xEFCA: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LF6DE:  INX
        case 0xEFCB: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LF6DF:  STX RAMTrgtPtrUB
        case 0xEFCE: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LF6E3:  STA (RAMTrgtPtr),Y
        case 0xEFD0: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LF6E5:  INY
        case 0xEFD1: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LF6E6:  CPY #$40
        case 0xEFD3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LF6E8:  BNE -
        case 0xEFD5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF6EA:  LDA OpnSltSelected
        case 0xEFD8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF6ED:  STA SaveNumber
        case 0xEFDB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF6F0:  JSR CreateNewSave
        case 0xEFDE: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LF6F3:  RTS
        case 0xEFDF: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LF6F4:  LDX #$09
        case 0xEFE1: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(addr_abs_x(0xC07E, nullptr))); reg_PC += 3; return; // LF6F9:  CMP KenMasutaTbl,X
        case 0xEFE4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF6FC:  BNE InitSaves
        case 0xEFE6: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LF6FE:  DEX
        case 0xEFE7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LF6FF:  BPL -
        case 0xEFE9: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF701:  JMP WriteKenStrings
        case 0xEFEC: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LF704:  LDX #$09
        case 0xEFEE: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(addr_abs_x(0xC07E, nullptr))); reg_PC += 3; return; // LF709:  CMP KenMasutaTbl,X
        case 0xEFF1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF70C:  BNE ClearSaveGameRAM
        case 0xEFF3: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LF70E:  DEX
        case 0xEFF4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LF70F:  BPL -
        case 0xEFF6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0xC07E; return; } // LF711:  BMI WriteKenStrings
        case 0xEFF8: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LF713:  INC Unused6435
        case 0xEFFB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF716:  LDA #$00
        case 0xEFFD: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LF718:  LDX #$00
        case 0xEFFF: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LF71D:  STA SavedGame2,X
        case 0xF002: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LF720:  STA SavedGame3,X
        case 0xF005: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LF723:  DEX
        case 0xF006: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LF724:  BNE -
        case 0xF008: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LF726:  LDX #$00
        case 0xF00A: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LF72B:  STA SavedGame2+$100,X
        case 0xF00D: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LF72E:  STA SavedGame3+$100,X
        case 0xF010: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LF731:  INX
        case 0xF011: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LF732:  CPX #$40
        case 0xF013: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LF734:  BCC -
        case 0xF015: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF736:  LDA #$00
        case 0xF017: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF738:  STA ValidSave1
        case 0xF01A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF73B:  STA ValidSave2
        case 0xF01D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF73E:  STA ValidSave3
        case 0xF020: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF741:  STA StartStatus1
        case 0xF023: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF744:  STA StartStatus2
        case 0xF026: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF747:  STA StartStatus3
        case 0xF029: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF74A:  STA CRCFail1
        case 0xF02C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF74D:  STA CRCFail2
        case 0xF02F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF750:  STA CRCFail3
        case 0xF032: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LF753:  LDX #$09
        case 0xF034: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LF758:  STA KenMasuta1,X
        case 0xF037: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LF75B:  STA KenMasuta2,X
        case 0xF03A: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LF75E:  DEX
        case 0xF03B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LF75F:  BPL -
        case 0xF03D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF761:  LDA #$00
        case 0xF03F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF763:  STA SaveBitMask
        case 0xF042: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF766:  JSR VerifyValidSaves
        case 0xF045: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF769:  LDA SaveBitMask
        case 0xF048: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF76C:  STA OpnSltSelected
        case 0xF04B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF76F:  LDA #$02
        case 0xF04D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF771:  STA SaveGameCntr
        case 0xF050: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF774:  LDA #$00
        case 0xF052: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF776:  STA SaveNumber
        case 0xF055: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LF779:  LSR OpnSltSelected
        case 0xF058: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LF77C:  BCC DoneValidateSave
        case 0xF05A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF77E:  LDA SaveNumber
        case 0xF05D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF781:  JSR Copy100Times
        case 0xF060: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF784:  CMP #$00
        case 0xF062: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LF786:  BEQ DoneValidateSave
        case 0xF064: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF788:  LDA SaveNumber
        case 0xF067: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF78B:  JSR ClearValidSaves
        case 0xF06A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LF78E:  JSR Dowindow
        case 0xF06D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF792:  LDA SaveNumber
        case 0xF070: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LF795:  CLC
        case 0xF071: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LF796:  ADC #$01
        case 0xF073: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF798:  STA GenWrd00LB
        case 0xF076: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF79A:  LDA #$00
        case 0xF078: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF79C:  STA GenWrd00UB
        case 0xF07B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LF79E:  JSR DoDialogHiBlock
        case 0xF07E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF7A2:  LDA #$1E
        case 0xF080: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LF7A4:  JSR WaitMultiNMIs
        case 0xF083: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF7A7:  LDA #$02
        case 0xF085: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LF7A9:  BRK
        case 0xF086: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LF7AC:  INC SaveNumber
        case 0xF089: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LF7AF:  DEC SaveGameCntr
        case 0xF08C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0xC07E; return; } // LF7B2:  BPL CheckValidSlot
        case 0xF08E: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LF7B4:  RTS
        case 0xF08F: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LF7B5:  TXA
        case 0xF090: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LF7B6:  PHA
        case 0xF091: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF7B7:  LDA #$00
        case 0xF093: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LF7B9:  LDX ValidSave1
        case 0xF096: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LF7BC:  CPX #$C8
        case 0xF098: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF7BE:  BNE ChkBitMask2
        case 0xF09A: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF7C0:  ORA #$01
        case 0xF09C: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LF7C2:  LDX ValidSave2
        case 0xF09F: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LF7C5:  CPX #$C8
        case 0xF0A1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF7C7:  BNE ChkBitMask3
        case 0xF0A3: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF7C9:  ORA #$02
        case 0xF0A5: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LF7CB:  LDX ValidSave3
        case 0xF0A8: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LF7CE:  CPX #$C8
        case 0xF0AA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF7D0:  BNE UpdtSaveBitMask
        case 0xF0AC: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF7D2:  ORA #$04
        case 0xF0AE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF7D4:  STA SaveBitMask
        case 0xF0B1: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LF7D7:  PLA
        case 0xF0B2: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LF7D8:  TAX
        case 0xF0B3: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LF7D9:  RTS
        case 0xF0B4: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LF7DA:  PHA
        case 0xF0B5: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LF7DB:  TXA
        case 0xF0B6: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LF7DC:  PHA
        case 0xF0B7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF7DD:  LDA SaveBitMask
        case 0xF0BA: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LF7E0:  LDX SaveNumber
        case 0xF0BD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LF7E3:  BEQ SaveAtSlot1
        case 0xF0BF: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LF7E5:  CPX #$01
        case 0xF0C1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LF7E7:  BEQ SaveAtSlot2
        case 0xF0C3: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LF7E9:  CPX #$02
        case 0xF0C5: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF7EB:  ORA #$04
        case 0xF0C7: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LF7ED:  LDX #$C8
        case 0xF0C9: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LF7EF:  STX ValidSave3
        case 0xF0CC: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF7F2:  JMP UpdateSaveBitMask
        case 0xF0CF: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF7F5:  ORA #$02
        case 0xF0D1: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LF7F7:  LDX #$C8
        case 0xF0D3: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LF7F9:  STX ValidSave2
        case 0xF0D6: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF7FC:  JMP UpdateSaveBitMask
        case 0xF0D9: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF7FF:  ORA #$01
        case 0xF0DB: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LF801:  LDX #$C8
        case 0xF0DD: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LF803:  STX ValidSave1
        case 0xF0E0: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF806:  AND #$07
        case 0xF0E2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF808:  STA SaveBitMask
        case 0xF0E5: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LF80B:  PLA
        case 0xF0E6: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LF80C:  TAX
        case 0xF0E7: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LF80D:  PLA
        case 0xF0E8: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LF80E:  RTS
        case 0xF0E9: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LF80F:  PHA
        case 0xF0EA: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LF810:  TXA
        case 0xF0EB: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LF811:  PHA
        case 0xF0EC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF812:  LDA SaveBitMask
        case 0xF0EF: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LF815:  LDX SaveNumber
        case 0xF0F2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LF818:  BEQ ClearValidSave1
        case 0xF0F4: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LF81A:  CPX #$01
        case 0xF0F6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LF81C:  BEQ ClearValidSave2
        case 0xF0F8: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LF81E:  CPX #$02
        case 0xF0FA: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF820:  AND #$03
        case 0xF0FC: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LF822:  LDX #$00
        case 0xF0FE: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LF824:  STX ValidSave3
        case 0xF101: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF827:  JMP ClearValisSaveDone
        case 0xF104: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF82A:  AND #$05
        case 0xF106: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LF82C:  LDX #$00
        case 0xF108: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LF82E:  STX ValidSave2
        case 0xF10B: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF831:  JMP ClearValisSaveDone
        case 0xF10E: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF834:  AND #$06
        case 0xF110: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LF836:  LDX #$00
        case 0xF112: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LF838:  STX ValidSave1
        case 0xF115: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF83B:  STA SaveBitMask
        case 0xF118: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LF83E:  PLA
        case 0xF119: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LF83F:  TAX
        case 0xF11A: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LF840:  PLA
        case 0xF11B: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LF841:  RTS
        case 0xF11C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF842:  JSR ClearWinBufRAM
        case 0xF11F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LF845:  JSR ClearSpriteRAM
        case 0xF122: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LF848:  JSR ClearPPU
        case 0xF125: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF84B:  LDA #%00011000
        case 0xF127: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF84D:  STA PPUControl1
        case 0xF12A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF850:  JSR WaitForNMI
        case 0xF12D: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LF853:  BRK
        case 0xF12E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF856:  JSR GetValidSaves
        case 0xF131: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF859:  LDA SaveBitMask
        case 0xF134: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF85C:  AND #$07
        case 0xF136: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LF85E:  BEQ PreNoSaves
        case 0xF138: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF860:  CMP #$07
        case 0xF13A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF862:  BNE PreUnusedSaves
        case 0xF13C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LF864:  JSR Dowindow
        case 0xF13F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF868:  CMP #$00
        case 0xF141: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF86A:  BNE ChkChngMsgSpeed
        case 0xF143: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF86C:  JMP DoContinueGame
        case 0xF146: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF86F:  CMP #$01
        case 0xF148: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF871:  BNE ChkEraseGame
        case 0xF14A: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF873:  JMP DoChngMsgSpeed
        case 0xF14D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF876:  CMP #$02
        case 0xF14F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF878:  BNE PreSavesUsed
        case 0xF151: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF87A:  JMP DoEraseGame
        case 0xF154: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LF87D:  JSR Dowindow
        case 0xF157: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF881:  CMP #$00
        case 0xF159: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF883:  BNE ChkChngMsgSpeed2
        case 0xF15B: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF885:  JMP DoContinueGame
        case 0xF15E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF888:  CMP #$01
        case 0xF160: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF88A:  BNE ChkNewQuest
        case 0xF162: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF88C:  JMP DoChngMsgSpeed
        case 0xF165: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF88F:  CMP #$02
        case 0xF167: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF891:  BNE ChkCopyQuest
        case 0xF169: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF893:  JMP DoNewQuest
        case 0xF16C: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF896:  CMP #$03
        case 0xF16E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF898:  BNE ChkEraseGame2
        case 0xF170: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF89A:  JMP DoCopyGame
        case 0xF173: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF89D:  CMP #$04
        case 0xF175: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF89F:  BNE PreUnusedSaves
        case 0xF177: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF8A1:  JMP DoEraseGame
        case 0xF17A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LF8A4:  JSR Dowindow
        case 0xF17D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF8A8:  CMP #$00
        case 0xF17F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF8AA:  BNE PreNoSaves
        case 0xF181: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF8AC:  JMP DoNewQuest
        case 0xF184: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF8AF:  LDA #$00
        case 0xF186: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF8B1:  JSR ShowUsedLogs
        case 0xF189: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF8B4:  CMP #WND_ABORT
        case 0xF18B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF8B6:  BNE PrepContinueGame
        case 0xF18D: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF8B8:  JMP WndBackToMain
        case 0xF190: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF8BB:  STA SaveNumber
        case 0xF193: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF8BE:  JSR Copy100Times
        case 0xF196: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LF8C1:  RTS
        case 0xF197: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF8C2:  LDA #$FF
        case 0xF199: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF8C4:  JSR ShowOpenLogs
        case 0xF19C: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF8C7:  CMP #WND_ABORT
        case 0xF19E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF8C9:  BNE NewQuest
        case 0xF1A0: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF8CB:  JMP WndBackToMain
        case 0xF1A3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF8CE:  STA SaveNumber
        case 0xF1A6: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LF8D1:  BRK
        case 0xF1A7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF8D4:  LDA #MSG_NORMAL
        case 0xF1A9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF8D6:  STA MessageSpeed
        case 0xF1AC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LF8D8:  JSR Dowindow
        case 0xF1AF: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF8DC:  CMP #WND_ABORT
        case 0xF1B1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF8DE:  BNE InitNewGame
        case 0xF1B3: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF8E0:  JMP WndBackToMain
        case 0xF1B6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF8E3:  STA MessageSpeed
        case 0xF1B9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF8E5:  JSR SGZeroStats
        case 0xF1BC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF8E8:  JSR SaveCurrentGame
        case 0xF1BF: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LF8EB:  RTS
        case 0xF1C0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF8EC:  LDA #$00
        case 0xF1C2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF8EE:  JSR ShowUsedLogs
        case 0xF1C5: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF8F1:  CMP #WND_ABORT
        case 0xF1C7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF8F3:  BNE ShowChngMsgSpeed
        case 0xF1C9: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF8F5:  JMP WndBackToMain
        case 0xF1CC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF8F8:  STA SaveNumber
        case 0xF1CF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF8FB:  JSR Copy100Times
        case 0xF1D2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LF8FE:  JSR Dowindow
        case 0xF1D5: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF902:  CMP #WND_ABORT
        case 0xF1D7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF904:  BNE ChngMsgSpeed
        case 0xF1D9: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF906:  JMP WndBackToMain
        case 0xF1DC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF909:  STA MessageSpeed
        case 0xF1DF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF90B:  JSR SaveCurrentGame
        case 0xF1E2: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF90E:  JMP WndBackToMain
        case 0xF1E5: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF911:  LDA #$00
        case 0xF1E7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF913:  JSR ShowUsedLogs
        case 0xF1EA: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF916:  CMP #WND_ABORT
        case 0xF1EC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF918:  BNE VerifyErase
        case 0xF1EE: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF91A:  JMP WndBackToMain
        case 0xF1F1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF91D:  STA SaveNumber
        case 0xF1F4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF920:  STA SaveSelected
        case 0xF1F7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LF923:  JSR Dowindow
        case 0xF1FA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LF927:  JSR Dowindow
        case 0xF1FD: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF92B:  CMP #WND_YES
        case 0xF1FF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LF92D:  BEQ EraseGame
        case 0xF201: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF92F:  JMP WndBackToMain
        case 0xF204: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF932:  LDA SaveNumber
        case 0xF207: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF935:  JSR ClearValidSaves
        case 0xF20A: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF938:  JMP WndBackToMain
        case 0xF20D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF93B:  LDA #$00
        case 0xF20F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF93D:  JSR ShowUsedLogs
        case 0xF212: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF940:  CMP #WND_ABORT
        case 0xF214: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF942:  BNE ShowOpenSlots
        case 0xF216: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF944:  JMP WndBackToMain
        case 0xF219: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF947:  STA SaveNumber
        case 0xF21C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF94A:  LDA #$FF
        case 0xF21E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF94C:  JSR ShowOpenLogs
        case 0xF221: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF94F:  CMP #WND_ABORT
        case 0xF223: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF951:  BNE ConfirmCopy
        case 0xF225: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF953:  JMP WndBackToMain
        case 0xF228: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF956:  STA OpnSltSelected
        case 0xF22B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LF959:  JSR Dowindow
        case 0xF22E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF95D:  CMP #$00
        case 0xF230: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LF95F:  BEQ DoCopyGameDat
        case 0xF232: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF961:  JMP WndBackToMain
        case 0xF235: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF964:  JSR CopyGame
        case 0xF238: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF967:  JMP WndBackToMain
        case 0xF23B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF96A:  LDA #$FF
        case 0xF23D: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LF96C:  BRK
        case 0xF23E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF96F:  LDA SaveBitMask
        case 0xF241: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF972:  AND #$07
        case 0xF243: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LF974:  BEQ JmpNoSaves
        case 0xF245: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF976:  CMP #$07
        case 0xF247: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF978:  BNE JmpSomeSaves
        case 0xF249: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF97A:  JMP PreSavesUsed
        case 0xF24C: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF97D:  JMP PreUnusedSaves
        case 0xF24F: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF980:  JMP PreNoSaves
        case 0xF252: cycles_to_run -= 2; reg_A ^= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF983:  EOR SaveBitMask
        case 0xF255: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF986:  AND #$07
        case 0xF257: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LF988:  BEQ NoOpenGames
        case 0xF259: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF98A:  STA _SaveBitMask
        case 0xF25C: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LF98D:  CLC
        case 0xF25D: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LF98E:  ADC #$11
        case 0xF25F: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LF990:  BRK
        case 0xF260: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF993:  CMP #WND_ABORT
        case 0xF262: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF995:  BNE ShowOpenLogsExit
        case 0xF264: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LF997:  PLA
        case 0xF265: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LF998:  PLA
        case 0xF266: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF999:  JMP WndBackToMain
        case 0xF269: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF99C:  JMP CalcSelectedSlot
        case 0xF26C: cycles_to_run -= 2; reg_A ^= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF99F:  EOR SaveBitMask
        case 0xF26F: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF9A2:  AND #$07
        case 0xF271: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LF9A4:  BEQ NoSavedGames
        case 0xF273: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF9A6:  STA _SaveBitMask
        case 0xF276: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LF9A9:  CLC
        case 0xF277: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LF9AA:  ADC #$18
        case 0xF279: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LF9AC:  BRK
        case 0xF27A: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF9AF:  CMP #WND_ABORT
        case 0xF27C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF9B1:  BNE ShowUsedLogsExit
        case 0xF27E: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LF9B3:  PLA
        case 0xF27F: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LF9B4:  PLA
        case 0xF280: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF9B5:  JMP WndBackToMain
        case 0xF283: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LF9B8:  JMP CalcSelectedSlot
        case 0xF286: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LF9BB:  LDX _SaveBitMask
        case 0xF289: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LF9BE:  CPX #$02
        case 0xF28B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF9C0:  BNE ChkUsedSlot3
        case 0xF28D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF9C2:  LDA #$01
        case 0xF28F: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LF9C4:  RTS
        case 0xF290: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LF9C5:  CPX #$04
        case 0xF292: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF9C7:  BNE ChkOpenSlot2
        case 0xF294: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF9C9:  LDA #$02
        case 0xF296: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LF9CB:  RTS
        case 0xF297: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LF9CC:  CPX #$05
        case 0xF299: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF9CE:  BNE ChkOpenSlot1
        case 0xF29B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LF9D0:  CMP #$01
        case 0xF29D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF9D2:  BNE CalcSlotExit
        case 0xF29F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF9D4:  LDA #$02
        case 0xF2A1: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LF9D6:  RTS
        case 0xF2A2: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LF9D7:  CPX #$06
        case 0xF2A4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LF9D9:  BNE CalcSlotExit
        case 0xF2A6: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LF9DB:  CLC
        case 0xF2A7: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LF9DC:  ADC #$01
        case 0xF2A9: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LF9DE:  RTS
        case 0xF2AA: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LF9DF:  PHA
        case 0xF2AB: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LF9E0:  TXA
        case 0xF2AC: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LF9E1:  PHA
        case 0xF2AD: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LF9E2:  TYA
        case 0xF2AE: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LF9E3:  PHA
        case 0xF2AF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF9E4:  LDA SaveNumber
        case 0xF2B2: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LF9E7:  AND #$07
        case 0xF2B4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF9E9:  STA SaveNumber
        case 0xF2B7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF9EC:  JSR CreateNewSave
        case 0xF2BA: cycles_to_run -= 2; reg_A = bus_read(0xC07E); update_nz(reg_A); reg_PC += 3; return; // LF9EF:  LDA CrntGamePtr
        case 0xF2BD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF9F2:  STA GameDatPtrLB
        case 0xF2C0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LF9F4:  LDA CrntGamePtr+1
        case 0xF2C3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LF9F7:  STA GameDatPtrUB
        case 0xF2C6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF9F9:  JSR SaveData
        case 0xF2C9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LF9FC:  JSR GetCRC
        case 0xF2CC: cycles_to_run -= 2; reg_A = bus_read(0xC07E); update_nz(reg_A); reg_PC += 3; return; // LF9FF:  LDA CrntGamePtr
        case 0xF2CF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFA02:  STA ROMSrcPtrLB
        case 0xF2D2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA04:  LDA CrntGamePtr+1
        case 0xF2D5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFA07:  STA ROMSrcPtrUB
        case 0xF2D8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA09:  LDA SaveNumber
        case 0xF2DB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFA0C:  JSR GetSaveGameBase
        case 0xF2DE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFA0F:  JSR Save10Times
        case 0xF2E1: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFA12:  PLA
        case 0xF2E2: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LFA13:  TAY
        case 0xF2E3: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFA14:  PLA
        case 0xF2E4: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LFA15:  TAX
        case 0xF2E5: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFA16:  PLA
        case 0xF2E6: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFA17:  RTS
        case 0xF2E7: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LFA18:  LDY #$00
        case 0xF2E9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA1A:  LDA ExpLB
        case 0xF2EC: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA1C:  STA (GameDatPtr),Y
        case 0xF2EE: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA1E:  INY
        case 0xF2EF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA1F:  LDA ExpUB
        case 0xF2F2: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA21:  STA (GameDatPtr),Y
        case 0xF2F4: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA23:  INY
        case 0xF2F5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA24:  LDA GoldLB
        case 0xF2F8: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA26:  STA (GameDatPtr),Y
        case 0xF2FA: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA28:  INY
        case 0xF2FB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA29:  LDA GoldUB
        case 0xF2FE: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA2B:  STA (GameDatPtr),Y
        case 0xF300: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA2D:  INY
        case 0xF301: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA2E:  LDA InventorySlot12
        case 0xF304: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA30:  STA (GameDatPtr),Y
        case 0xF306: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA32:  INY
        case 0xF307: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA33:  LDA InventorySlot34
        case 0xF30A: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA35:  STA (GameDatPtr),Y
        case 0xF30C: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA37:  INY
        case 0xF30D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA38:  LDA InventorySlot56
        case 0xF310: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA3A:  STA (GameDatPtr),Y
        case 0xF312: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA3C:  INY
        case 0xF313: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA3D:  LDA InventorySlot78
        case 0xF316: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA3F:  STA (GameDatPtr),Y
        case 0xF318: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA41:  INY
        case 0xF319: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA42:  LDA InventoryKeys
        case 0xF31C: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFA44:  AND #$0F
        case 0xF31E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LFA46:  CMP #$07
        case 0xF320: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LFA48:  BCC +
        case 0xF322: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFA4A:  LDA #$06
        case 0xF324: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA4E:  INY
        case 0xF325: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA4F:  LDA InventoryHerbs
        case 0xF328: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA51:  STA (GameDatPtr),Y
        case 0xF32A: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA53:  INY
        case 0xF32B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA54:  LDA EqippedItems
        case 0xF32E: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA56:  STA (GameDatPtr),Y
        case 0xF330: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA58:  INY
        case 0xF331: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA59:  LDA ModsnSpells
        case 0xF334: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA5B:  STA (GameDatPtr),Y
        case 0xF336: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA5D:  INY
        case 0xF337: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA5E:  LDA PlayerFlags
        case 0xF33A: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA60:  STA (GameDatPtr),Y
        case 0xF33C: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA62:  INY
        case 0xF33D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA63:  LDA StoryFlags
        case 0xF340: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA65:  STA (GameDatPtr),Y
        case 0xF342: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA67:  INY
        case 0xF343: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LFA68:  LDX #$03
        case 0xF345: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA6C:  STA (GameDatPtr),Y
        case 0xF347: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA6E:  INY
        case 0xF348: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LFA6F:  DEX
        case 0xF349: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LFA70:  BPL -
        case 0xF34B: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LFA72:  LDX #$03
        case 0xF34D: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA77:  STA (GameDatPtr),Y
        case 0xF34F: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA79:  INY
        case 0xF350: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LFA7A:  DEX
        case 0xF351: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LFA7B:  BPL -
        case 0xF353: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA7D:  LDA MessageSpeed
        case 0xF356: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA7F:  STA (GameDatPtr),Y
        case 0xF358: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA81:  INY
        case 0xF359: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA82:  LDA HitPoints
        case 0xF35C: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA84:  STA (GameDatPtr),Y
        case 0xF35E: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA86:  INY
        case 0xF35F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFA87:  LDA MagicPoints
        case 0xF362: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA89:  STA (GameDatPtr),Y
        case 0xF364: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LFA8B:  LDX SaveNumber
        case 0xF367: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LFA8E:  LDA StartStatus1,X
        case 0xF36A: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA91:  INY
        case 0xF36B: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA92:  STA (GameDatPtr),Y
        case 0xF36D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFA94:  LDA #$C8
        case 0xF36F: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA96:  INY
        case 0xF370: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA97:  STA (GameDatPtr),Y
        case 0xF372: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA99:  INY
        case 0xF373: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA9A:  STA (GameDatPtr),Y
        case 0xF375: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA9C:  INY
        case 0xF376: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFA9D:  STA (GameDatPtr),Y
        case 0xF378: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFA9F:  INY
        case 0xF379: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFAA0:  STA (GameDatPtr),Y
        case 0xF37B: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFAA2:  RTS
        case 0xF37C: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LFAA3:  LDX #$09
        case 0xF37E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFAA5:  JSR StoreGamedData
        case 0xF381: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFAA8:  LDA GameDatPtrLB
        case 0xF384: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LFAAA:  CLC
        case 0xF385: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LFAAB:  ADC #$20
        case 0xF387: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFAAD:  STA GameDatPtrLB
        case 0xF38A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LFAAF:  BCC +
        case 0xF38C: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LFAB1:  INC GameDatPtrUB
        case 0xF38F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0xC07E; return; } // LFAB4:  BPL Save10Loop
        case 0xF391: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFAB6:  RTS
        case 0xF392: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LFAB7:  LDY #$1F
        case 0xF394: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFABB:  STA (GameDatPtr),Y
        case 0xF396: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LFABD:  DEY
        case 0xF397: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LFABE:  BPL -
        case 0xF399: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFAC0:  RTS
        case 0xF39A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFAC1:  STA GenByte3E
        case 0xF39D: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LFAC3:  TXA
        case 0xF39E: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFAC4:  PHA
        case 0xF39F: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LFAC5:  TYA
        case 0xF3A0: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFAC6:  PHA
        case 0xF3A1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFAC7:  LDA GenByte3E
        case 0xF3A4: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFAC9:  AND #$07
        case 0xF3A6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFACB:  STA GenByte3E
        case 0xF3A9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFACD:  JSR GetSaveGameBase
        case 0xF3AC: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LFAD0:  LDX #$0A
        case 0xF3AE: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LFAD2:  TXA
        case 0xF3AF: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFAD3:  PHA
        case 0xF3B0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFAD4:  JSR CheckValidCRC
        case 0xF3B3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LFAD7:  BCS CRCCheckFail
        case 0xF3B5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFAD9:  JSR LoadSavedData
        case 0xF3B8: cycles_to_run -= 2; reg_A = bus_read(0xC07E); update_nz(reg_A); reg_PC += 3; return; // LFADC:  LDA CrntGamePtr
        case 0xF3BB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFADF:  STA GenPtr3CLB
        case 0xF3BE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFAE1:  LDA CrntGamePtr+1
        case 0xF3C1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFAE4:  STA GenPtr3CUB
        case 0xF3C4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFAE6:  JSR MakeWorkingCopy
        case 0xF3C7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFAE9:  JSR Copy10Times
        case 0xF3CA: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFAEC:  PLA
        case 0xF3CB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFAED:  LDA #$00
        case 0xF3CD: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LFAEF:  JMP FinishGameCopy
        case 0xF3D0: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LFAF2:  TXA
        case 0xF3D1: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LFAF3:  LDX SaveNumber
        case 0xF3D4: cycles_to_run -= 5; { uint16_t a = addr_abs_x(0x0000, nullptr); uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LFAF6:  INC CRCFail1,X
        case 0xF3D7: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LFAF9:  TAX
        case 0xF3D8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFAFA:  LDA GameDatPtrLB
        case 0xF3DB: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LFAFC:  CLC
        case 0xF3DC: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LFAFD:  ADC #$20
        case 0xF3DE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFAFF:  STA GameDatPtrLB
        case 0xF3E1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LFB01:  BCC NextCopy
        case 0xF3E3: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LFB03:  INC GameDatPtrUB
        case 0xF3E6: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFB05:  PLA
        case 0xF3E7: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LFB06:  TAX
        case 0xF3E8: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LFB07:  DEX
        case 0xF3E9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LFB08:  BNE GameCopyLoop
        case 0xF3EB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFB0A:  LDA #$FF
        case 0xF3ED: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFB0C:  STA GenByte3E
        case 0xF3F0: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFB0E:  PLA
        case 0xF3F1: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LFB0F:  TAY
        case 0xF3F2: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFB10:  PLA
        case 0xF3F3: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LFB11:  TAX
        case 0xF3F4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFB12:  LDA GenByte3E
        case 0xF3F7: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFB14:  RTS
        case 0xF3F8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFB15:  LDA GenByte3E
        case 0xF3FB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFB17:  JSR GetSaveGameBase
        case 0xF3FE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFB1A:  LDA GameDatPtrLB
        case 0xF401: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFB1C:  STA GenPtr3CLB
        case 0xF404: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFB1E:  LDA GameDatPtrUB
        case 0xF407: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFB20:  STA GenPtr3CUB
        case 0xF40A: cycles_to_run -= 2; reg_A = bus_read(0xC07E); update_nz(reg_A); reg_PC += 3; return; // LFB22:  LDA CrntGamePtr
        case 0xF40D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFB25:  STA GameDatPtrLB
        case 0xF410: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFB27:  LDA CrntGamePtr+1
        case 0xF413: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFB2A:  STA GameDatPtrUB
        case 0xF416: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LFB2C:  LDX #$0A
        case 0xF418: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFB2E:  JSR MakeWorkingCopy
        case 0xF41B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFB31:  LDA GenByte3C
        case 0xF41E: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LFB33:  CLC
        case 0xF41F: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LFB34:  ADC #$20
        case 0xF421: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFB36:  STA GenByte3C
        case 0xF424: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LFB38:  BCC Check10Copies
        case 0xF426: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LFB3A:  INC GenByte3D
        case 0xF429: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LFB3C:  DEX
        case 0xF42A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LFB3D:  BNE Copy10Loop
        case 0xF42C: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFB3F:  RTS
        case 0xF42D: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LFB40:  LDY #$1F
        case 0xF42F: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFB44:  STA (GenPtr3C),Y
        case 0xF431: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LFB46:  DEY
        case 0xF432: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LFB47:  BPL -
        case 0xF434: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFB49:  RTS
        case 0xF435: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LFB4A:  LDY #$1E
        case 0xF437: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFB4C:  LDA (GameDatPtr),Y
        case 0xF439: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFB4E:  STA CRCCopyLB
        case 0xF43C: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFB50:  INY
        case 0xF43D: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFB51:  LDA (GameDatPtr),Y
        case 0xF43F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFB53:  STA CRCCopyUB
        case 0xF442: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFB55:  JSR GetCRC
        case 0xF445: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LFB58:  LDY #$1E
        case 0xF447: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFB5A:  LDA CRCCopyLB
        case 0xF44A: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(read_pointer_indexed_y(0x0000, nullptr))); reg_PC += 2; return; // LFB5C:  CMP (GameDatPtr),Y
        case 0xF44C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LFB5E:  BNE InvalidCRC
        case 0xF44E: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFB60:  INY
        case 0xF44F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFB61:  LDA CRCCopyUB
        case 0xF452: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(read_pointer_indexed_y(0x0000, nullptr))); reg_PC += 2; return; // LFB63:  CMP (GameDatPtr),Y
        case 0xF454: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LFB65:  BEQ ValidCRC
        case 0xF456: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LFB67:  SEC
        case 0xF457: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFB68:  RTS
        case 0xF458: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LFB69:  CLC
        case 0xF459: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFB6A:  RTS
        case 0xF45A: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LFB6B:  LDY #$00
        case 0xF45C: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFB6D:  LDA (GameDatPtr),Y
        case 0xF45E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFB6F:  STA ExpLB
        case 0xF461: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFB71:  INY
        case 0xF462: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFB72:  LDA (GameDatPtr),Y
        case 0xF464: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFB74:  STA ExpUB
        case 0xF467: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFB76:  INY
        case 0xF468: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFB77:  LDA (GameDatPtr),Y
        case 0xF46A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFB79:  STA GoldLB
        case 0xF46D: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFB7B:  INY
        case 0xF46E: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFB7C:  LDA (GameDatPtr),Y
        case 0xF470: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFB7E:  STA GoldUB
        case 0xF473: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFB80:  INY
        case 0xF474: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFB81:  LDA (GameDatPtr),Y
        case 0xF476: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFB83:  STA InventorySlot12
        case 0xF479: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFB85:  INY
        case 0xF47A: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFB86:  LDA (GameDatPtr),Y
        case 0xF47C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFB88:  STA InventorySlot34
        case 0xF47F: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFB8A:  INY
        case 0xF480: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFB8B:  LDA (GameDatPtr),Y
        case 0xF482: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFB8D:  STA InventorySlot56
        case 0xF485: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFB8F:  INY
        case 0xF486: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFB90:  LDA (GameDatPtr),Y
        case 0xF488: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFB92:  STA InventorySlot78
        case 0xF48B: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFB94:  INY
        case 0xF48C: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFB95:  LDA (GameDatPtr),Y
        case 0xF48E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFB97:  STA InventoryKeys
        case 0xF491: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFB99:  INY
        case 0xF492: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFB9A:  LDA (GameDatPtr),Y
        case 0xF494: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFB9C:  STA InventoryHerbs
        case 0xF497: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFB9E:  INY
        case 0xF498: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFB9F:  LDA (GameDatPtr),Y
        case 0xF49A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFBA1:  STA EqippedItems
        case 0xF49D: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFBA3:  INY
        case 0xF49E: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFBA4:  LDA (GameDatPtr),Y
        case 0xF4A0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFBA6:  STA ModsnSpells
        case 0xF4A3: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFBA8:  INY
        case 0xF4A4: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFBA9:  LDA (GameDatPtr),Y
        case 0xF4A6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFBAB:  STA PlayerFlags
        case 0xF4A9: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFBAD:  INY
        case 0xF4AA: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFBAE:  LDA (GameDatPtr),Y
        case 0xF4AC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFBB0:  STA StoryFlags
        case 0xF4AF: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFBB2:  INY
        case 0xF4B0: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LFBB3:  LDX #$03
        case 0xF4B2: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LFBB7:  STA DispName0,X
        case 0xF4B5: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFBB9:  INY
        case 0xF4B6: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LFBBA:  DEX
        case 0xF4B7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LFBBB:  BPL -
        case 0xF4B9: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LFBBD:  LDX #$03
        case 0xF4BB: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LFBC1:  STA DispName4,X
        case 0xF4BE: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFBC4:  INY
        case 0xF4BF: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LFBC5:  DEX
        case 0xF4C0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LFBC6:  BPL -
        case 0xF4C2: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFBC8:  LDA (GameDatPtr),Y
        case 0xF4C4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFBCA:  STA MessageSpeed
        case 0xF4C7: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFBCC:  INY
        case 0xF4C8: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFBCD:  LDA (GameDatPtr),Y
        case 0xF4CA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFBCF:  STA HitPoints
        case 0xF4CD: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFBD1:  INY
        case 0xF4CE: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFBD2:  LDA (GameDatPtr),Y
        case 0xF4D0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFBD4:  STA MagicPoints
        case 0xF4D3: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFBD6:  INY
        case 0xF4D4: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFBD7:  LDA (GameDatPtr),Y
        case 0xF4D6: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LFBD9:  LDX SaveNumber
        case 0xF4D9: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LFBDC:  STA StartStatus1,X
        case 0xF4DC: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFBDF:  RTS
        case 0xF4DD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFBE0:  JSR DoCRC
        case 0xF4E0: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LFBE3:  LDY #$1E
        case 0xF4E2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFBE5:  LDA CRCLB
        case 0xF4E5: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFBE7:  STA (GameDatPtr),Y
        case 0xF4E7: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LFBE9:  INY
        case 0xF4E8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFBEA:  LDA CRCUB
        case 0xF4EB: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LFBEC:  STA (GameDatPtr),Y
        case 0xF4ED: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFBEE:  RTS
        case 0xF4EE: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LFBEF:  LDY #$1D
        case 0xF4F0: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LFBF1:  STY CRCLB
        case 0xF4F3: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LFBF3:  STY CRCUB
        case 0xF4F6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFBF7:  STA GenByte3C
        case 0xF4F9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFBF9:  JSR DoLFSR
        case 0xF4FC: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LFBFC:  DEY
        case 0xF4FD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LFBFD:  BPL -
        case 0xF4FF: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFBFF:  RTS
        case 0xF500: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFC00:  STA GenByte22
        case 0xF503: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LFC02:  TXA
        case 0xF504: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFC03:  PHA
        case 0xF505: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFC04:  LDA GenByte22
        case 0xF508: cycles_to_run -= 2; reg_X = bus_read(0xC07E); update_nz(reg_X); reg_PC += 3; return; // LFC06:  LDX SvdGamePtr
        case 0xF50B: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LFC09:  STX GameDatPtrLB
        case 0xF50E: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LFC0B:  LDX SvdGamePtr+1
        case 0xF511: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LFC0E:  STX GameDatPtrUB
        case 0xF514: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LFC10:  TAX
        case 0xF515: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LFC11:  BEQ FoundSaveGameBase
        case 0xF517: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LFC16:  DEX
        case 0xF518: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LFC17:  BNE -
        case 0xF51A: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFC19:  PLA
        case 0xF51B: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LFC1A:  TAX
        case 0xF51C: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFC1B:  RTS
        case 0xF51D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFC1C:  LDA GameDatPtrLB
        case 0xF520: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LFC1E:  CLC
        case 0xF521: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LFC1F:  ADC #$40
        case 0xF523: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFC21:  STA GameDatPtrLB
        case 0xF526: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFC23:  LDA GameDatPtrUB
        case 0xF529: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LFC25:  ADC #$01
        case 0xF52B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFC27:  STA GameDatPtrUB
        case 0xF52E: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFC29:  RTS
        case 0xF52F: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LFC2A:  TYA
        case 0xF530: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFC2B:  PHA
        case 0xF531: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LFC2C:  LDY #$08
        case 0xF533: cycles_to_run -= 2; reg_A ^= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFC30:  EOR GenByte3C
        case 0xF536: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LFC32:  ASL CRCLB
        case 0xF539: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_rol(bus_read(a))); } reg_PC += 3; return; // LFC34:  ROL CRCUB
        case 0xF53C: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LFC36:  ASL GenByte3C
        case 0xF53F: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LFC38:  ASL
        case 0xF540: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LFC39:  BCC +
        case 0xF542: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFC3B:  LDA CRCLB
        case 0xF545: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFC3D:  EOR #$21
        case 0xF547: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFC3F:  STA CRCLB
        case 0xF54A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFC41:  LDA CRCUB
        case 0xF54D: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFC43:  EOR #$10
        case 0xF54F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFC45:  STA CRCUB
        case 0xF552: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LFC48:  BNE --
        case 0xF554: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFC4A:  PLA
        case 0xF555: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LFC4B:  TAY
        case 0xF556: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFC4C:  RTS
        case 0xF557: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFC4D:  LDA #TL_BLANK_TILE1
        case 0xF559: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LFC4F:  LDX #$00
        case 0xF55B: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LFC54:  DEX
        case 0xF55C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LFC55:  BNE -
        case 0xF55E: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LFC57:  LDX #$00
        case 0xF560: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LFC5C:  DEX
        case 0xF561: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LFC5D:  BNE -
        case 0xF563: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LFC5F:  LDX #$00
        case 0xF565: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LFC64:  DEX
        case 0xF566: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LFC65:  BNE -
        case 0xF568: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LFC67:  LDX #$00
        case 0xF56A: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LFC6C:  INX
        case 0xF56B: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LFC6D:  CPX #$C0
        case 0xF56D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LFC6F:  BCC -
        case 0xF56F: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFC71:  RTS
        case 0xF570: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFC80:  PHA
        case 0xF571: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFC81:  LDA #PRG_BANK_3
        case 0xF573: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFC83:  JSR MMC1LoadPRG
        case 0xF576: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFC86:  PLA
        case 0xF577: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFC87:  RTS
        case 0xF578: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFC88:  PHA
        case 0xF579: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFC89:  LDA #PRG_B3_NO_RAM
        case 0xF57B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFC8B:  STA ActiveBank
        case 0xF57E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFC8E:  JSR MMC1LoadPRG
        case 0xF581: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFC91:  LDA #%00001000
        case 0xF583: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFC93:  STA PPUControl0
        case 0xF586: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFC96:  PLA
        case 0xF587: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFC97:  RTS
        case 0xF588: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFC98:  PHA
        case 0xF589: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFC99:  LDA #CHR_BANK_1
        case 0xF58B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFC9B:  STA ActiveNT0
        case 0xF58E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFC9E:  JSR MMC1LoadNT0
        case 0xF591: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFCA1:  PLA
        case 0xF592: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFCA2:  RTS
        case 0xF593: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFCA3:  PHA
        case 0xF594: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFCA4:  LDA #CHR_BANK_0
        case 0xF596: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LFCA6:  BEQ SetActiveNT0
        case 0xF598: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFCA8:  PHA
        case 0xF599: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFCA9:  LDA #CHR_BANK_0
        case 0xF59B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LFCAB:  BEQ SetActiveNT1
        case 0xF59D: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFCAD:  PHA
        case 0xF59E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFCAE:  LDA #CHR_BANK_2
        case 0xF5A0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFCB0:  STA ActiveNT1
        case 0xF5A3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFCB3:  JSR MMC1LoadNT1
        case 0xF5A6: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFCB6:  PLA
        case 0xF5A7: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFCB7:  RTS
        case 0xF5A8: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFCB8:  PHA
        case 0xF5A9: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFCB9:  LDA #CHR_BANK_3
        case 0xF5AB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LFCBB:  BNE SetActiveNT1
        case 0xF5AD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFCBD:  STA IRQStoreA
        case 0xF5B0: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LFCBF:  STX IRQStoreX
        case 0xF5B3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFCC1:  LDA ActiveBank
        case 0xF5B6: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFCC4:  PHA
        case 0xF5B7: cycles_to_run -= 3; // Unsupported php reg_PC += 1; return; // LFCC5:  PHP
        case 0xF5B8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFCC6:  LDA ActiveBank
        case 0xF5BB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFCC9:  STA WndUnused6006
        case 0xF5BE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFCCC:  JSR GetDataPtr
        case 0xF5C1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFCCF:  LDA #$4C
        case 0xF5C3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFCD1:  STA _JMPFuncPtr
        case 0xF5C6: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LFCD3:  LDX IRQStoreX
        case 0xF5C9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFCD5:  LDA IRQStoreA
        case 0xF5CC: cycles_to_run -= 4; // Unsupported plp reg_PC += 1; return; // LFCD7:  PLP
        case 0xF5CD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LFCD8:  JSR JMPFuncPtr
        case 0xF5D0: cycles_to_run -= 3; // Unsupported php reg_PC += 1; return; // LFCDB:  PHP
        case 0xF5D1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFCDC:  STA IRQStoreA
        case 0xF5D4: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFCDE:  PLA
        case 0xF5D5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFCDF:  STA NewPRGBank
        case 0xF5D8: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFCE1:  PLA
        case 0xF5D9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFCE2:  JSR SetPRGBankAndSwitch
        case 0xF5DC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFCE5:  LDA NewPRGBank
        case 0xF5DF: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFCE7:  PHA
        case 0xF5E0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFCE8:  LDA IRQStoreA
        case 0xF5E3: cycles_to_run -= 4; // Unsupported plp reg_PC += 1; return; // LFCEA:  PLP
        case 0xF5E4: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFCEB:  RTS
        case 0xF5E5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFCEC:  LDA NewPRGBank
        case 0xF5E8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFCEE:  JSR SetPRGBankAndSwitch
        case 0xF5EB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFCF1:  LDA BankPtrIndex
        case 0xF5EE: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LFCF3:  ASL
        case 0xF5EF: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LFCF4:  TAX
        case 0xF5F0: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LFCF5:  LDA BankPointers,X
        case 0xF5F3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFCF8:  STA BankPntrLB
        case 0xF5F6: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LFCFA:  LDA BankPointers+1,X
        case 0xF5F9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFCFD:  STA BankPntrUB
        case 0xF5FC: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFCFF:  RTS
        case 0xF5FD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFD00:  STA IRQStoreA
        case 0xF600: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LFD02:  STX IRQStoreX
        case 0xF603: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFD04:  LDA ActiveBank
        case 0xF606: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFD07:  PHA
        case 0xF607: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFD08:  JSR GetDataPtr
        case 0xF60A: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFD0B:  PLA
        case 0xF60B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFD0C:  JSR SetPRGBankAndSwitch
        case 0xF60E: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LFD0F:  LDX IRQStoreX
        case 0xF611: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFD11:  LDA BankPntrLB
        case 0xF614: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LFD13:  STA GenPtr00LB,X
        case 0xF617: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFD15:  LDA BankPntrUB
        case 0xF61A: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LFD17:  STA GenPtr00UB,X
        case 0xF61D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFD19:  LDA IRQStoreA
        case 0xF620: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFD1B:  RTS
        case 0xF621: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFD1C:  STA IRQStoreA
        case 0xF624: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFD1E:  LDA ActiveBank
        case 0xF627: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFD21:  PHA
        case 0xF628: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFD22:  LDA IRQStoreA
        case 0xF62B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFD24:  JSR SetPRGBankAndSwitch
        case 0xF62E: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LFD27:  LDA GenPtr00LB,X
        case 0xF631: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFD29:  STA BankDatPtrLB
        case 0xF634: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LFD2B:  LDA GenPtr00UB,X
        case 0xF637: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFD2D:  STA BankDatPtrUB
        case 0xF63A: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFD2F:  LDA (BankDatPtr),Y
        case 0xF63C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFD31:  STA IRQStoreA
        case 0xF63F: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFD33:  PLA
        case 0xF640: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFD34:  JSR SetPRGBankAndSwitch
        case 0xF643: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFD37:  LDA IRQStoreA
        case 0xF646: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFD39:  RTS
        case 0xF647: cycles_to_run -= 2; reg_P |= FLAG_I; reg_PC += 1; return; // LFD3A:  SEI
        case 0xF648: cycles_to_run -= 3; // Unsupported php reg_PC += 1; return; // LFD3B:  PHP
        case 0xF649: cycles_to_run -= 3; cpu_bit(bus_read(0x0000)); reg_PC += 3; return; // LFD3C:  BIT APUCommonCntrl0
        case 0xF64C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFD3F:  STA IRQStoreA
        case 0xF64F: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LFD41:  STX IRQStoreX
        case 0xF652: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LFD43:  STY IRQStoreY
        case 0xF655: cycles_to_run -= 2; // Unsupported tsx reg_PC += 1; return; // LFD45:  TSX
        case 0xF656: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LFD46:  LDA BankFuncDatLB,X
        case 0xF659: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LFD49:  SEC
        case 0xF65A: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LFD4A:  SBC #$01
        case 0xF65C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFD4C:  STA _BankFuncDatLB
        case 0xF65F: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LFD4E:  LDA BankFuncDatUB,X
        case 0xF662: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LFD51:  SBC #$00
        case 0xF664: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFD53:  STA _BankFuncDatUB
        case 0xF667: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LFD55:  LDY #$01
        case 0xF669: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFD57:  LDA (_BankFuncDatPtr),Y
        case 0xF66B: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFD59:  PHA
        case 0xF66C: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFD5A:  AND #$08
        case 0xF66E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LFD5C:  CMP #$08
        case 0xF670: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFD5E:  PLA
        case 0xF671: cycles_to_run -= 2; reg_A = cpu_ror(reg_A); reg_PC += 1; return; // LFD5F:  ROR
        case 0xF672: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFD60:  LSR
        case 0xF673: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFD61:  LSR
        case 0xF674: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFD62:  LSR
        case 0xF675: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFD63:  STA NewPRGBank
        case 0xF678: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LFD65:  DEY
        case 0xF679: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFD66:  LDA (_BankFuncDatPtr),Y
        case 0xF67B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0xC07E; return; } // LFD68:  BMI GetBankData
        case 0xF67D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFD6A:  STA BankPtrIndex
        case 0xF680: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LFD6C:  LDY IRQStoreY
        case 0xF683: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LFD6E:  LDX IRQStoreX
        case 0xF686: cycles_to_run -= 4; // Unsupported plp reg_PC += 1; return; // LFD70:  PLP
        case 0xF687: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFD71:  PLA
        case 0xF688: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFD72:  LDA IRQStoreA
        case 0xF68B: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LFD74:  JMP RunBankFunction
        case 0xF68E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFD79:  STA BankPtrIndex
        case 0xF691: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LFD7B:  LDY IRQStoreY
        case 0xF694: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LFD7D:  LDX IRQStoreX
        case 0xF697: cycles_to_run -= 4; // Unsupported plp reg_PC += 1; return; // LFD7F:  PLP
        case 0xF698: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFD80:  PLA
        case 0xF699: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFD81:  LDA IRQStoreA
        case 0xF69C: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LFD83:  JMP GetAndStrDatPtr
        case 0xF69F: cycles_to_run -= 2; // Unsupported cld reg_PC += 1; return; // LFD86:  CLD
        case 0xF6A0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFD87:  LDA #%00010000
        case 0xF6A2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFD89:  STA PPUControl0
        case 0xF6A5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0x0000; return; } // LFD8F:  BMI -
        case 0xF6A7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LFD94:  BPL -
        case 0xF6A9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0x0000; return; } // LFD99:  BMI -
        case 0xF6AB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFD9B:  LDA #%00000000
        case 0xF6AD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFD9D:  STA PPUControl1
        case 0xF6B0: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LFDA0:  LDX #$FF
        case 0xF6B2: cycles_to_run -= 2; // Unsupported txs reg_PC += 1; return; // LFDA2:  TXS
        case 0xF6B3: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LFDA3:  TAX
        case 0xF6B4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFDA4:  STA UpdateBGTiles
        case 0xF6B7: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LFDA9:  STA BlockRAM,X
        case 0xF6BA: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LFDAC:  STA WinBufRAM,X
        case 0xF6BD: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LFDAF:  STA WinBufRAM+$0100,X
        case 0xF6C0: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LFDB2:  STA WinBufRAM+$0200,X
        case 0xF6C3: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LFDB5:  STA WinBufRAM+$0300,X
        case 0xF6C6: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LFDB8:  INX
        case 0xF6C7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LFDB9:  BNE -
        case 0xF6C9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFDBB:  JSR LoadMMCPRGBank3
        case 0xF6CC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFDBE:  STA ActiveBank
        case 0xF6CF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFDC1:  LDA #%00011110
        case 0xF6D1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFDC3:  STA MMC1Config
        case 0xF6D4: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFDC6:  LDA #$00
        case 0xF6D6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFDC8:  STA ActiveNT0
        case 0xF6D9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFDCB:  STA ActiveNT1
        case 0xF6DC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFDCE:  JSR DoMMC1
        case 0xF6DF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFDD1:  LDA PPUStatus
        case 0xF6E2: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFDD4:  LDA #$10
        case 0xF6E4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFDD6:  STA PPUAddress
        case 0xF6E7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFDD9:  LDA #$00
        case 0xF6E9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFDDB:  STA PPUAddress
        case 0xF6EC: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LFDDE:  LDX #$10
        case 0xF6EE: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LFDE3:  DEX
        case 0xF6EF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LFDE4:  BNE -
        case 0xF6F1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFDE6:  LDA #%10001000
        case 0xF6F3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFDE8:  STA PPUControl0
        case 0xF6F6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07B; return; } // LFDEB:  JSR ClearSpriteRAM
        case 0xF6F9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFDEE:  JSR WaitForNMI
        case 0xF6FC: cycles_to_run -= 3; reg_PC = 0xC07B; return; // LFDF1:  JMP ContinueReset
        case 0xF6FF: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LFDF4:  INC $FFDF
        case 0xF702: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFDF7:  LDA MMC1Config
        case 0xF705: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFDFA:  JSR MMC1LoadConfig
        case 0xF708: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFDFD:  LDA ActiveNT0
        case 0xF70B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFE00:  JSR MMC1LoadNT0
        case 0xF70E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFE03:  LDA ActiveNT1
        case 0xF711: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LFE06:  JMP MMC1LoadNT1
        case 0xF714: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFE09:  STA MMC1Config
        case 0xF717: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFE0C:  STA MMCCfg
        case 0xF71A: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFE0F:  LSR
        case 0xF71B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFE10:  STA MMCCfg
        case 0xF71E: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFE13:  LSR
        case 0xF71F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFE14:  STA MMCCfg
        case 0xF722: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFE17:  LSR
        case 0xF723: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFE18:  STA MMCCfg
        case 0xF726: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFE1B:  LSR
        case 0xF727: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFE1C:  STA MMCCfg
        case 0xF72A: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFE1F:  RTS
        case 0xF72B: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LFE20:  LDY #$01
        case 0xF72D: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LFE22:  LDA BlockRAM,X
        case 0xF730: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0xC07E; return; } // LFE25:  BPL LoadBGDat
        case 0xF732: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LFE27:  TAY
        case 0xF733: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFE28:  LSR
        case 0xF734: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFE29:  LSR
        case 0xF735: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFE2A:  LSR
        case 0xF736: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFE2B:  LSR
        case 0xF737: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFE2C:  AND #$04
        case 0xF739: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFE2E:  ORA #$88
        case 0xF73B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFE30:  STA PPUControl0
        case 0xF73E: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LFE33:  TYA
        case 0xF73F: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LFE34:  INX
        case 0xF740: cycles_to_run -= 2; reg_Y = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_Y); reg_PC += 3; return; // LFE35:  LDY BlockRAM,X
        case 0xF743: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LFE38:  INX
        case 0xF744: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFE39:  AND #$3F
        case 0xF746: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFE3B:  STA PPUAddress
        case 0xF749: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LFE3E:  LDA BlockRAM,X
        case 0xF74C: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LFE41:  INX
        case 0xF74D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFE42:  STA PPUAddress
        case 0xF750: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LFE48:  INX
        case 0xF751: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFE49:  STA PPUIOReg
        case 0xF754: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LFE4C:  DEY
        case 0xF755: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LFE4D:  BNE -
        case 0xF757: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LFE4F:  DEC PPUEntCount
        case 0xF75A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LFE51:  BNE UpdateBG
        case 0xF75C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LFE53:  BEQ ProcessVBlank2
        case 0xF75E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFE55:  JSR SetPPUValues
        case 0xF761: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFE58:  LDA #$02
        case 0xF763: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFE5A:  STA SPRDMAReg
        case 0xF766: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LFE5D:  JMP DoFrameUpdates
        case 0xF769: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFE60:  LDA #$02
        case 0xF76B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFE62:  STA SPRDMAReg
        case 0xF76E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LFE65:  BNE ProcessVBlank2
        case 0xF770: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFE67:  PHA
        case 0xF771: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LFE68:  TXA
        case 0xF772: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFE69:  PHA
        case 0xF773: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LFE6A:  TYA
        case 0xF774: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LFE6B:  PHA
        case 0xF775: cycles_to_run -= 2; // Unsupported tsx reg_PC += 1; return; // LFE6C:  TSX
        case 0xF776: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LFE6D:  LDA Stack-10,X
        case 0xF779: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LFE70:  CMP #>WaitForNMI
        case 0xF77B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LFE72:  BNE NotVBlankReady
        case 0xF77D: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LFE74:  LDA Stack-11,X
        case 0xF780: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LFE77:  CMP #<WaitForNMI+3
        case 0xF782: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LFE79:  BCC NotVBlankReady
        case 0xF784: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LFE7B:  CMP #<WaitForNMI+9
        case 0xF786: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LFE7D:  BCS NotVBlankReady
        case 0xF788: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFE7F:  LDA PPUStatus
        case 0xF78B: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LFE82:  INC FrameCounter
        case 0xF78E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFE84:  LDA PPUEntCount
        case 0xF791: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LFE86:  BEQ SetSprtRAM
        case 0xF793: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LFE88:  CMP #$08
        case 0xF795: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LFE8A:  BCS ChkBGUpdates
        case 0xF797: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFE8C:  LDA #$02
        case 0xF799: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFE8E:  STA SPRDMAReg
        case 0xF79C: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LFE91:  LDX #$00
        case 0xF79E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFE93:  LDA UpdateBGTiles
        case 0xF7A1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0xC07E; return; } // LFE96:  BMI UpdateBG
        case 0xF7A3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFE9B:  STA PPUAddress
        case 0xF7A6: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LFE9E:  LDA BlockRAM+1,X
        case 0xF7A9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFEA1:  STA PPUAddress
        case 0xF7AC: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LFEA4:  LDA BlockRAM+2,X
        case 0xF7AF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFEA7:  STA PPUIOReg
        case 0xF7B2: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LFEAA:  INX
        case 0xF7B3: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LFEAB:  INX
        case 0xF7B4: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LFEAC:  INX
        case 0xF7B5: cycles_to_run -= 2; update_flags_cmp(reg_X, bus_read(0x0000)); reg_PC += 3; return; // LFEAD:  CPX PPUBufCount
        case 0xF7B8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LFEAF:  BNE -
        case 0xF7BA: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFEB1:  LDA #$3F
        case 0xF7BC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFEB3:  STA PPUAddress
        case 0xF7BF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFEB6:  LDA #$00
        case 0xF7C1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFEB8:  STA NMIStatus
        case 0xF7C4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFEBA:  STA PPUEntCount
        case 0xF7C7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFEBC:  STA PPUBufCount
        case 0xF7CA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFEBE:  STA UpdateBGTiles
        case 0xF7CD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFEC1:  STA PPUAddress
        case 0xF7D0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFEC4:  LDA #$0F
        case 0xF7D2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFEC6:  STA PPUIOReg
        case 0xF7D5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFEC9:  LDA ActiveNmTbl
        case 0xF7D8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LFECB:  BNE +
        case 0xF7DA: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFECD:  LDA #%10001000
        case 0xF7DC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LFECF:  BNE ++
        case 0xF7DE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFED6:  LDA ScrollX
        case 0xF7E1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFED8:  STA PPUScroll
        case 0xF7E4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFEDB:  LDA ScrollY
        case 0xF7E7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFEDD:  STA PPUScroll
        case 0xF7EA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFEE0:  JSR DoMMC1
        case 0xF7ED: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFEE3:  LDA SndEngineStat
        case 0xF7F0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LFEE6:  BNE DoFrameUpdates2
        case 0xF7F2: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFEE8:  LDA #PRG_BANK_1
        case 0xF7F4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFEEA:  JSR MMC1LoadPRG
        case 0xF7F7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LFEED:  JSR UpdateSound
        case 0xF7FA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFEF0:  LDA ActiveBank
        case 0xF7FD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0xC07E; return; } // LFEF3:  JSR SetPRGBankAndSwitch
        case 0xF800: cycles_to_run -= 2; // Unsupported tsx reg_PC += 1; return; // LFEF6:  TSX
        case 0xF801: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LFEF7:  LDA Stack-$A,X
        case 0xF804: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFEFA:  STA NMIPtrUB
        case 0xF807: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LFEFC:  CMP #>MMC1LoadPRG
        case 0xF809: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LFEFE:  BNE ChkValidInst
        case 0xF80B: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LFF00:  LDA Stack-$B,X
        case 0xF80E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LFF03:  CMP #<MMC1LoadPRG
        case 0xF810: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0xC07E; return; } // LFF05:  BCC ChkValidInst
        case 0xF812: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LFF07:  CMP #<MMC1LoadNT1+$14
        case 0xF814: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0xC07E; return; } // LFF09:  BCS ChkValidInst
        case 0xF816: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFF0B:  LDA #<MMC1LoadNT1+$14
        case 0xF818: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LFF0D:  STA Stack-$B,X
        case 0xF81B: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LFF10:  LDA Stack-$B,X
        case 0xF81E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFF13:  STA NMIPtrLB
        case 0xF821: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LFF15:  LDY #$00
        case 0xF823: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LFF17:  LDA (NMIPtr),Y
        case 0xF825: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFF19:  AND #$0F
        case 0xF827: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LFF1B:  CMP #$07
        case 0xF829: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0xC07E; return; } // LFF1D:  BEQ PrepForIRQFuncs
        case 0xF82B: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFF1F:  PLA
        case 0xF82C: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LFF20:  TAY
        case 0xF82D: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFF21:  PLA
        case 0xF82E: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LFF22:  TAX
        case 0xF82F: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFF23:  PLA
        case 0xF830: cycles_to_run -= 6; reg_P = cpu_pop(); reg_PC = (cpu_pop() | (cpu_pop() << 8)); return; // LFF24:  RTI
        case 0xF831: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFF25:  PLA
        case 0xF832: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LFF26:  TAY
        case 0xF833: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFF27:  PLA
        case 0xF834: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LFF28:  TAX
        case 0xF835: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LFF29:  PLA
        case 0xF836: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LFF2A:  JMP IRQ
        case 0xF839: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFF2D:  LDA #$3F
        case 0xF83B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFF2F:  STA PPUAddress
        case 0xF83E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFF32:  LDA #$00
        case 0xF840: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFF34:  STA PPUAddress
        case 0xF843: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFF37:  LDA #$0F
        case 0xF845: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFF39:  STA PPUIOReg
        case 0xF848: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFF3C:  LDA ActiveNmTbl
        case 0xF84B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LFF3E:  BNE SetNT1
        case 0xF84D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFF40:  LDA #%10001000
        case 0xF84F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0xC07E; return; } // LFF42:  BNE SetScrollRegs
        case 0xF851: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFF44:  LDA #%10001001
        case 0xF853: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFF46:  STA PPUControl0
        case 0xF856: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFF49:  LDA ScrollX
        case 0xF859: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFF4B:  STA PPUScroll
        case 0xF85C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LFF4E:  LDA ScrollY
        case 0xF85F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFF50:  STA PPUScroll
        case 0xF862: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFF53:  RTS
        case 0xF863: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LFF74:  LDA #$01
        case 0xF865: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFF76:  STA NMIStatus
        case 0xF868: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LFF7A:  BNE -
        case 0xF86A: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFF7C:  RTS
        case 0xF86B: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LFF8E:  JMP DoReset
        case 0xF86E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFF91:  STA ActiveBank
        case 0xF871: cycles_to_run -= 2; // NOP reg_PC += 1; return; // LFF94:  NOP
        case 0xF872: cycles_to_run -= 2; // NOP reg_PC += 1; return; // LFF95:  NOP
        case 0xF873: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFF96:  STA MMCPRG
        case 0xF876: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFF99:  LSR
        case 0xF877: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFF9A:  STA MMCPRG
        case 0xF87A: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFF9D:  LSR
        case 0xF87B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFF9E:  STA MMCPRG
        case 0xF87E: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFFA1:  LSR
        case 0xF87F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFFA2:  STA MMCPRG
        case 0xF882: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFFA5:  LSR
        case 0xF883: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFFA6:  STA MMCPRG
        case 0xF886: cycles_to_run -= 2; // NOP reg_PC += 1; return; // LFFA9:  NOP
        case 0xF887: cycles_to_run -= 2; // NOP reg_PC += 1; return; // LFFAA:  NOP
        case 0xF888: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFFAB:  RTS
        case 0xF889: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFFAC:  STA MMCCHR0
        case 0xF88C: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFFAF:  LSR
        case 0xF88D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFFB0:  STA MMCCHR0
        case 0xF890: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFFB3:  LSR
        case 0xF891: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFFB4:  STA MMCCHR0
        case 0xF894: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFFB7:  LSR
        case 0xF895: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFFB8:  STA MMCCHR0
        case 0xF898: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFFBB:  LSR
        case 0xF899: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFFBC:  STA MMCCHR0
        case 0xF89C: cycles_to_run -= 2; // NOP reg_PC += 1; return; // LFFBF:  NOP
        case 0xF89D: cycles_to_run -= 2; // NOP reg_PC += 1; return; // LFFC0:  NOP
        case 0xF89E: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFFC1:  RTS
        case 0xF89F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFFC2:  STA MMCCHR1
        case 0xF8A2: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFFC5:  LSR
        case 0xF8A3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFFC6:  STA MMCCHR1
        case 0xF8A6: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFFC9:  LSR
        case 0xF8A7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFFCA:  STA MMCCHR1
        case 0xF8AA: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFFCD:  LSR
        case 0xF8AB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFFCE:  STA MMCCHR1
        case 0xF8AE: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LFFD1:  LSR
        case 0xF8AF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LFFD2:  STA MMCCHR1
        case 0xF8B2: cycles_to_run -= 2; // NOP reg_PC += 1; return; // LFFD5:  NOP
        case 0xF8B3: cycles_to_run -= 2; // NOP reg_PC += 1; return; // LFFD6:  NOP
        case 0xF8B4: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LFFD7:  RTS
        case 0xF8B5: cycles_to_run -= 2; reg_P |= FLAG_I; reg_PC += 1; return; // LFFD8:  SEI
        case 0xF8B6: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LFFD9:  INC MMCReset2
        case 0xF8B9: cycles_to_run -= 3; reg_PC = 0xC07E; return; // LFFDC:  JMP DoReset
        case 0xFFFA: reg_PC = cpu_read_pointer(0xFFFA); return;
        case 0xFFFC: reg_PC = cpu_read_pointer(0xFFFC); return;
        case 0xFFFE: reg_PC = cpu_read_pointer(0xFFFE); return;
        default: reg_PC++; return;
    }
}
}
