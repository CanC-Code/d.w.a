#include "cpu_shared.h"
namespace Bank02 {
void execute_at(uint16_t pc) {
    switch (pc) {
        case 0x8000: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBCB0:  JSR WaitForNMI
        case 0x8003: cycles_to_run -= 2; reg_A = bus_read(0x8015); update_nz(reg_A); reg_PC += 3; return; // LBCB3:  LDA IntroPalPtr1
        case 0x8006: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBCB6:  STA PalPtrLB
        case 0x8009: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBCB8:  LDA IntroPalPtr1+1
        case 0x800C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBCBB:  STA PalPtrUB
        case 0x800F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBCBD:  LDA #$00
        case 0x8011: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBCBF:  STA PalModByte
        case 0x8014: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBCC1:  JSR PrepBGPalLoad
        case 0x8017: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x8015; return; } // LBCC4:  JSR SetBlackBackDrop
        case 0x801A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBCC7:  LDA #MSC_INTRO
        case 0x801C: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LBCC9:  BRK
        case 0x801D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBCCC:  JSR WaitForNMI
        case 0x8020: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBCD2:  LDA JoypadBtns
        case 0x8023: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBCD4:  AND #IN_SEL_STRT
        case 0x8025: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x8015; return; } // LBCD6:  BNE ShowIntroText
        case 0x8027: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBCD8:  LDA MusicTrigger
        case 0x802A: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBCDB:  CMP #$FC
        case 0x802C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x8015; return; } // LBCDD:  BEQ ShowIntroText
        case 0x802E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LBCDF:  BNE -
        case 0x8030: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBCE1:  JSR WaitForNMI
        case 0x8033: cycles_to_run -= 2; reg_A = bus_read(0x8015); update_nz(reg_A); reg_PC += 3; return; // LBCE4:  LDA IntroPalPtr2
        case 0x8036: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBCE7:  STA PalPtrLB
        case 0x8039: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBCE9:  LDA IntroPalPtr2+1
        case 0x803C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBCEC:  STA PalPtrUB
        case 0x803F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBCEE:  LDA #$00
        case 0x8041: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBCF0:  STA PalModByte
        case 0x8044: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBCF2:  JSR PrepBGPalLoad
        case 0x8047: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBCF5:  LDA #$37
        case 0x8049: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBCF7:  STA PPUDataByte
        case 0x804C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBCF9:  LDA #$17
        case 0x804E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBCFB:  STA PPUAddrLB
        case 0x8051: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBCFD:  LDA #$3F
        case 0x8053: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBCFF:  STA PPUAddrUB
        case 0x8056: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBD01:  JSR AddPPUBufEntry
        case 0x8059: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x8015; return; } // LBD04:  JSR SetBlackBackDrop
        case 0x805C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBD07:  JSR WaitForNMI
        case 0x805F: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LBD0A:  LDX #$00
        case 0x8061: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LBD0F:  STA SpriteRAM,X
        case 0x8064: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LBD12:  INX
        case 0x8065: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LBD13:  CPX #$24
        case 0x8067: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LBD15:  BNE -
        case 0x8069: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBD17:  LDA #$00
        case 0x806B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD19:  STA IntroCounter
        case 0x806E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBD1B:  LDA #$FF
        case 0x8070: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD1D:  STA IntroPointer
        case 0x8073: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBD1F:  LDA #$0B
        case 0x8075: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD21:  STA _CharXPos
        case 0x8078: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBD23:  LDA #$0C
        case 0x807A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD25:  STA CharXPixelsLB
        case 0x807D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBD27:  LDA #$10
        case 0x807F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD29:  STA _CharYPos
        case 0x8082: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBD2B:  LDA #$17
        case 0x8084: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD2D:  STA CharYPixelsLB
        case 0x8087: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBD2F:  LDA #$01
        case 0x8089: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD31:  STA MessageSpeed
        case 0x808C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBD33:  JSR WaitForNMI
        case 0x808F: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LBD36:  LDX #$00
        case 0x8091: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBD38:  LDA IntroPointer
        case 0x8094: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBD3A:  CMP #$FF
        case 0x8096: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LBD3C:  BEQ +++
        case 0x8098: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LBD3E:  LDY IntroPointer
        case 0x809B: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x8015, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LBD40:  LDA StarburstPtrTbl,Y
        case 0x809E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD43:  STA DatPntr1LB
        case 0x80A1: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LBD45:  LDA StarburstPtrTbl+1,Y
        case 0x80A4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD48:  STA DatPntrlUB
        case 0x80A7: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LBD4A:  LDY #$00
        case 0x80A9: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LBD4C:  LDA (DatPntr1),Y
        case 0x80AB: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBD4E:  CMP #$FF
        case 0x80AD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LBD50:  BEQ +
        case 0x80AF: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LBD52:  STA SpriteRAM+$40,X
        case 0x80B2: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LBD55:  INY
        case 0x80B3: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LBD56:  INX
        case 0x80B4: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LBD57:  LDA (DatPntr1),Y
        case 0x80B6: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LBD59:  STA SpriteRAM+$40,X
        case 0x80B9: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LBD5C:  INY
        case 0x80BA: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LBD5D:  INX
        case 0x80BB: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LBD5E:  LDA (DatPntr1),Y
        case 0x80BD: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBD60:  AND #$C0
        case 0x80BF: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBD62:  ORA #$01
        case 0x80C1: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LBD64:  STA SpriteRAM+$40,X
        case 0x80C4: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LBD67:  INX
        case 0x80C5: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LBD68:  LDA (DatPntr1),Y
        case 0x80C7: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBD6A:  AND #$3F
        case 0x80C9: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LBD6C:  CLC
        case 0x80CA: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LBD6D:  ADC #$B4
        case 0x80CC: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LBD6F:  STA SpriteRAM+$40,X
        case 0x80CF: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LBD72:  INY
        case 0x80D0: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LBD73:  INX
        case 0x80D1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x8015; return; } // LBD74:  BNE IntroSpLoadLoop
        case 0x80D3: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBD78:  CMP #$80
        case 0x80D5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LBD7A:  BCS +
        case 0x80D7: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LBD7C:  LSR
        case 0x80D8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LBD7D:  BCC ++
        case 0x80DA: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LBD81:  INC IntroPointer
        case 0x80DD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBD83:  LDA IntroPointer
        case 0x80E0: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBD85:  CMP #$18
        case 0x80E2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LBD87:  BNE +
        case 0x80E4: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBD89:  LDA #$FF
        case 0x80E6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD8B:  STA IntroPointer
        case 0x80E9: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LBD92:  INX
        case 0x80EA: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LBD93:  CPX #$C0
        case 0x80EC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LBD95:  BNE -
        case 0x80EE: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LBD97:  INC IntroCounter
        case 0x80F1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBD99:  LDA IntroCounter
        case 0x80F4: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBD9B:  CMP #$20
        case 0x80F6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LBD9D:  BEQ +
        case 0x80F8: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBD9F:  CMP #$A0
        case 0x80FA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LBDA1:  BEQ +
        case 0x80FC: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBDA3:  CMP #$C0
        case 0x80FE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LBDA5:  BNE ++
        case 0x8100: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBDA9:  STA IntroPointer
        case 0x8103: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LBDAD:  PHA
        case 0x8104: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBDAE:  JSR GetJoypadStatus
        case 0x8107: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LBDB1:  PLA
        case 0x8108: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LBDB2:  BNE +
        case 0x810A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBDB4:  LDA JoypadBtns
        case 0x810D: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBDB6:  AND #IN_START
        case 0x810F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LBDB8:  BEQ +
        case 0x8111: cycles_to_run -= 2; reg_A = bus_read(0x8015); update_nz(reg_A); reg_PC += 3; return; // LBDBA:  LDA IntroPalPtr3
        case 0x8114: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBDBD:  STA PalPtrLB
        case 0x8117: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBDBF:  LDA IntroPalPtr3+1
        case 0x811A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBDC2:  STA PalPtrUB
        case 0x811D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBDC4:  LDA #$00
        case 0x811F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBDC6:  STA PalModByte
        case 0x8122: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBDC8:  JSR PrepBGPalLoad
        case 0x8125: cycles_to_run -= 2; reg_A = bus_read(0x8015); update_nz(reg_A); reg_PC += 3; return; // LBDCB:  LDA IntroPalPtr3
        case 0x8128: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBDCE:  STA PalPtrLB
        case 0x812B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBDD0:  LDA IntroPalPtr3+1
        case 0x812E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBDD3:  STA PalPtrUB
        case 0x8131: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBDD5:  LDA #$00
        case 0x8133: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBDD7:  STA PalModByte
        case 0x8136: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBDD9:  JSR PrepSPPalLoad
        case 0x8139: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LBDDC:  RTS
        case 0x813A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBDE0:  LDA #$0F
        case 0x813C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBDE2:  STA PPUDataByte
        case 0x813F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBDE4:  LDA #$00
        case 0x8141: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBDE6:  STA PPUAddrLB
        case 0x8144: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBDE8:  LDA #$3F
        case 0x8146: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBDEA:  STA PPUAddrUB
        case 0x8149: cycles_to_run -= 3; reg_PC = 0x0000; return; // LBDEC:  JMP AddPPUBufEntry
        case 0x814C: cycles_to_run -= 2; reg_P |= FLAG_I; reg_PC += 1; return; // LBFD8:  SEI
        case 0x814D: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LBFD9:  INC MMCReset1
        case 0x8150: cycles_to_run -= 3; reg_PC = 0x0000; return; // LBFDC:  JMP _DoReset
        default: reg_PC++; return;
    }
}
}
