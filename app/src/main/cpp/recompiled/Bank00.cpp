#include "cpu_shared.h"
namespace Bank00 {
void execute_at(uint16_t pc) {
    switch (pc) {
        case 0x8000: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA743:  LDA DrgnLrdPal
        case 0x8003: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA746:  CMP #EN_DRAGONLORD2
        case 0x8005: cycles_to_run -= 2; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LA748:  BEQ LoadEndPals
        case 0x8007: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA74A:  LDA #PAL_LOAD_BG
        case 0x8009: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA74C:  STA LoadBGPal
        case 0x800C: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LA74E:  LDA RegSPPalPtr
        case 0x800F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA751:  STA SprtPalPtrLB
        case 0x8012: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA753:  LDA RegSPPalPtr+1
        case 0x8015: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA756:  STA SprtPalPtrUB
        case 0x8018: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LA758:  LDA OverworldPalPtr
        case 0x801B: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA75B:  CLC
        case 0x801C: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA75C:  ADC MapType
        case 0x801F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA75E:  STA BGPalPtrLB
        case 0x8022: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA760:  LDA OverworldPalPtr+1
        case 0x8025: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LA763:  ADC #$00
        case 0x8027: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA765:  STA BGPalPtrUB
        case 0x802A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LA767:  JSR PalFadeOut
        case 0x802D: cycles_to_run -= 3; reg_PC = 0x804E; return; // LA76A:  JMP ClearWinBufRAM2
        case 0x8030: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA76D:  LDA #PAL_LOAD_BG
        case 0x8032: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA76F:  STA LoadBGPal
        case 0x8035: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LA771:  LDA EndBossPal1Ptr
        case 0x8038: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA774:  STA BGPalPtrLB
        case 0x803B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA776:  LDA EndBossPal1Ptr+1
        case 0x803E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA779:  STA BGPalPtrUB
        case 0x8041: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LA77B:  LDA EndBossPal2Ptr
        case 0x8044: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA77E:  STA SprtPalPtrLB
        case 0x8047: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA780:  LDA EndBossPal2Ptr+1
        case 0x804A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA783:  STA SprtPalPtrUB
        case 0x804D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LA785:  JSR PalFadeOut
        case 0x8050: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA788:  LDA #$00
        case 0x8052: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA78A:  STA GenPtr3CLB
        case 0x8055: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA78C:  LDA #$04
        case 0x8057: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA78E:  STA GenPtr3CUB
        case 0x805A: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LA790:  LDY #$00
        case 0x805C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA792:  LDA #$FF
        case 0x805E: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA796:  INY
        case 0x805F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LA797:  BNE -
        case 0x8061: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA799:  INC GenPtr3CUB
        case 0x8064: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA79B:  LDA GenPtr3CUB
        case 0x8067: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA79D:  CMP #$08
        case 0x8069: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LA79F:  BNE WinBufRAMLoop
        case 0x806B: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA7A1:  RTS
        case 0x806C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA7A2:  STA WndTypeCopy
        case 0x806F: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LA7A4:  BRK
        case 0x8070: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA7A7:  LDA WndEraseWdth
        case 0x8073: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA7AA:  LSR
        case 0x8074: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA7AB:  ORA #$10
        case 0x8076: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA7AD:  STA WndWidthTemp
        case 0x8079: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA7B0:  LDA WndEraseHght
        case 0x807C: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LA7B3:  SEC
        case 0x807D: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LA7B4:  SBC #$01
        case 0x807F: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA7B6:  ASL
        case 0x8080: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA7B7:  ASL
        case 0x8081: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA7B8:  ASL
        case 0x8082: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA7B9:  ASL
        case 0x8083: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA7BA:  ADC WndErasePos
        case 0x8086: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA7BD:  STA _WndPosition
        case 0x8089: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA7C0:  LDA WndErasePos
        case 0x808C: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA7C3:  AND #$0F
        case 0x808E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA7C5:  STA XPosFromLeft
        case 0x8091: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LA7C7:  SEC
        case 0x8092: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LA7C8:  SBC #$08
        case 0x8094: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA7CA:  ASL
        case 0x8095: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA7CB:  STA StartSignedXPos
        case 0x8098: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA7CD:  LDA WndEraseHght
        case 0x809B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA7D0:  STA BlockCounter
        case 0x809E: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LA7D2:  SEC
        case 0x809F: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LA7D3:  SBC #$01
        case 0x80A1: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA7D5:  PHA
        case 0x80A2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA7D6:  LDA WndErasePos
        case 0x80A5: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA7D9:  LSR
        case 0x80A6: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA7DA:  LSR
        case 0x80A7: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA7DB:  LSR
        case 0x80A8: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA7DC:  LSR
        case 0x80A9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA7DD:  STA WndLineBufIdx
        case 0x80AC: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA7DF:  PLA
        case 0x80AD: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA7E0:  CLC
        case 0x80AE: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA7E1:  ADC WndLineBufIdx
        case 0x80B1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA7E3:  STA WndLineBufIdx
        case 0x80B4: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LA7E5:  SEC
        case 0x80B5: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LA7E6:  SBC #$07
        case 0x80B7: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA7E8:  ASL
        case 0x80B8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA7E9:  STA YPosFromCenter
        case 0x80BB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA7EB:  LDA WndEraseWdth
        case 0x80BE: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA7EE:  LSR
        case 0x80BF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA7EF:  STA WndBlockWidth
        case 0x80C2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA7F1:  LDA WndLineBufIdx
        case 0x80C5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA7F3:  STA YPosFromTop
        case 0x80C8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LA7F5:  JSR CalcPPUBufAddr
        case 0x80CB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA7F8:  LDA PPUAddrLB
        case 0x80CE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA7FA:  STA BlockAddrLB
        case 0x80D1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA7FC:  LDA PPUAddrUB
        case 0x80D4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA7FE:  STA BlockAddrUB
        case 0x80D7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA800:  LDA WndTypeCopy
        case 0x80DA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LA802:  BEQ SetWndBack
        case 0x80DC: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA804:  CMP #WND_DIALOG
        case 0x80DE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LA806:  BEQ SetWndBack
        case 0x80E0: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA808:  CMP #WND_CMD_NONCMB
        case 0x80E2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LA80A:  BEQ SetWndBack
        case 0x80E4: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA80C:  CMP #WND_CMD_CMB
        case 0x80E6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LA80E:  BEQ SetWndBack
        case 0x80E8: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA810:  CMP #WND_ALPHBT
        case 0x80EA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LA812:  BEQ SetWndBack
        case 0x80EC: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA814:  LDA #WND_FOREGROUND
        case 0x80EE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LA816:  BEQ SetWndBackFore
        case 0x80F0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA818:  LDA #WND_BACKGROUND
        case 0x80F2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA81A:  STA WndForeBack
        case 0x80F5: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA81C:  LDA #$00
        case 0x80F7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA81E:  STA AttribBufIndex
        case 0x80FA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA820:  STA WndLineBufIdx
        case 0x80FD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA822:  LDA StartSignedXPos
        case 0x8100: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA824:  STA XPosFromCenter
        case 0x8103: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA826:  LDA WndBlockWidth
        case 0x8106: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA828:  STA WndColPos
        case 0x8109: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LA82A:  JSR ClearWndBuf
        case 0x810C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA82D:  LDA BlockAddrLB
        case 0x810F: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA82F:  CLC
        case 0x8110: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LA830:  ADC #$02
        case 0x8112: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA832:  STA BlockAddrLB
        case 0x8115: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LA834:  BCC +
        case 0x8117: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA836:  INC BlockAddrUB
        case 0x811A: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA83A:  INC WndLineBufIdx
        case 0x811D: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA83C:  INC AttribBufIndex
        case 0x8120: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA83E:  INC XPosFromCenter
        case 0x8123: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA840:  INC XPosFromCenter
        case 0x8126: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA842:  DEC WndColPos
        case 0x8129: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LA844:  BNE WndRemoveBlockLoop
        case 0x812B: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LA846:  BRK
        case 0x812C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA849:  LDA BlockAddrLB
        case 0x812F: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA84B:  CLC
        case 0x8130: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LA84C:  ADC #$C0
        case 0x8132: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA84E:  STA BlockAddrLB
        case 0x8135: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LA850:  BCS +
        case 0x8137: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA852:  DEC BlockAddrUB
        case 0x813A: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA856:  ASL
        case 0x813B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA857:  STA XPosFromLeft
        case 0x813E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA859:  LDA BlockAddrLB
        case 0x8141: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LA85B:  SEC
        case 0x8142: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LA85C:  SBC XPosFromLeft
        case 0x8145: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA85E:  STA BlockAddrLB
        case 0x8148: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LA860:  BCS +
        case 0x814A: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA862:  DEC BlockAddrUB
        case 0x814D: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LA867:  SEC
        case 0x814E: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LA868:  SBC #$10
        case 0x8150: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA86A:  STA _WndPosition
        case 0x8153: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA86D:  DEC YPosFromCenter
        case 0x8156: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA86F:  DEC YPosFromCenter
        case 0x8159: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA871:  DEC WndRowPos
        case 0x815C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LA873:  BNE WndRemoveRowLoop
        case 0x815E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA875:  LDA StopNPCMove
        case 0x8161: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LA877:  BEQ DoneRemoveWindow
        case 0x8163: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LA879:  JSR WaitForNMI
        case 0x8166: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LA87C:  JSR DoSprites
        case 0x8169: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA87F:  RTS
        case 0x816A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA880:  LDA WndForeBack
        case 0x816D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LA882:  BNE ClrWindowBlock
        case 0x816F: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LA884:  LDY #$00
        case 0x8171: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA886:  LDA (BlockAddr),Y
        case 0x8173: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA888:  CMP #$FF
        case 0x8175: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LA88A:  BEQ ClrWindowBlock
        case 0x8177: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA88C:  CMP #$FE
        case 0x8179: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LA88E:  BEQ ClrWindowBlock
        case 0x817B: cycles_to_run -= 3; reg_PC = 0x804E; return; // LA890:  JMP UncoverWindow
        case 0x817E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA893:  LDA #$00
        case 0x8180: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA895:  STA BlkRemoveFlgs
        case 0x8183: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA897:  STA AddAttribData
        case 0x8186: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LA899:  JSR ModWindowBlock
        case 0x8189: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LA89C:  LDY #$00
        case 0x818B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA89E:  LDA #$FF
        case 0x818D: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LA8A0:  STA (BlockAddr),Y
        case 0x818F: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA8A2:  INY
        case 0x8190: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LA8A3:  STA (BlockAddr),Y
        case 0x8192: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LA8A5:  LDY #$20
        case 0x8194: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LA8A7:  STA (BlockAddr),Y
        case 0x8196: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA8A9:  INY
        case 0x8197: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LA8AA:  STA (BlockAddr),Y
        case 0x8199: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA8AC:  RTS
        case 0x819A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA8AD:  LDA NTBlockY
        case 0x819D: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA8AF:  ASL
        case 0x819E: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA8B0:  ADC YPosFromCenter
        case 0x81A1: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA8B2:  CLC
        case 0x81A2: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LA8B3:  ADC #$1E
        case 0x81A4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA8B5:  STA DivNum1LB
        case 0x81A7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA8B7:  LDA #$1E
        case 0x81A9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA8B9:  STA DivNum2
        case 0x81AC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LA8BB:  JSR ByteDivide
        case 0x81AF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA8BE:  LDA DivRemainder
        case 0x81B2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA8C0:  STA YPosFromTop
        case 0x81B5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA8C2:  STA YFromTopTemp
        case 0x81B8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA8C4:  LDA NTBlockX
        case 0x81BB: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA8C6:  ASL
        case 0x81BC: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA8C7:  CLC
        case 0x81BD: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA8C8:  ADC XPosFromCenter
        case 0x81C0: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA8CA:  AND #$3F
        case 0x81C2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA8CC:  STA XPosFromLeft
        case 0x81C5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA8CE:  STA XFromLeftTemp
        case 0x81C8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LA8D0:  JSR DoAddrCalc
        case 0x81CB: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LA8D3:  LDX WndLineBufIdx
        case 0x81CE: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LA8D5:  LDY #$00
        case 0x81D0: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA8D7:  LDA (BlockAddr),Y
        case 0x81D2: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LA8D9:  STA WndLineBuf,X
        case 0x81D5: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA8DC:  INY
        case 0x81D6: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA8DD:  LDA (BlockAddr),Y
        case 0x81D8: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LA8DF:  STA WndLineBuf+1,X
        case 0x81DB: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LA8E2:  TXA
        case 0x81DC: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA8E3:  CLC
        case 0x81DD: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA8E4:  ADC WndEraseWdth
        case 0x81E0: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LA8E7:  TAX
        case 0x81E1: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LA8E8:  LDY #$20
        case 0x81E3: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA8EA:  LDA (BlockAddr),Y
        case 0x81E5: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LA8EC:  STA WndLineBuf,X
        case 0x81E8: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA8EF:  INY
        case 0x81E9: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA8F0:  LDA (BlockAddr),Y
        case 0x81EB: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LA8F2:  STA WndLineBuf+1,X
        case 0x81EE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA8F5:  LDA XFromLeftTemp
        case 0x81F1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA8F7:  STA XPosFromLeft
        case 0x81F4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA8F9:  LDA YFromTopTemp
        case 0x81F7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA8FB:  STA YPosFromTop
        case 0x81FA: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LA8FD:  LDY #$00
        case 0x81FC: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA8FF:  LDA (BlockAddr),Y
        case 0x81FE: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA901:  CMP #$C1
        case 0x8200: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LA903:  BCS +
        case 0x8202: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA905:  LDA #$00
        case 0x8204: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LA907:  BEQ StoreAttribByte
        case 0x8206: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LA90B:  BCS +
        case 0x8208: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA90D:  LDA #$01
        case 0x820A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LA90F:  BNE StoreAttribByte
        case 0x820C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LA913:  BCS +
        case 0x820E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA915:  LDA #$02
        case 0x8210: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LA917:  BNE StoreAttribByte
        case 0x8212: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LA91B:  LDX AttribBufIndex
        case 0x8215: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LA91D:  STA AttribTblBuf,X
        case 0x8218: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA920:  RTS
        case 0x8219: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA921:  LDA NTBlockY
        case 0x821C: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA923:  ASL
        case 0x821D: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA924:  CLC
        case 0x821E: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA925:  ADC YPosFromCenter
        case 0x8221: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA927:  CLC
        case 0x8222: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LA928:  ADC #$1E
        case 0x8224: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA92A:  STA DivNum1LB
        case 0x8227: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA92C:  LDA #$1E
        case 0x8229: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA92E:  STA DivNum2
        case 0x822C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LA930:  JSR ByteDivide
        case 0x822F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA933:  LDA DivRemainder
        case 0x8232: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA935:  STA YPosFromTop
        case 0x8235: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA937:  STA YFromTopTemp
        case 0x8238: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA939:  LDA NTBlockX
        case 0x823B: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA93B:  ASL
        case 0x823C: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA93C:  CLC
        case 0x823D: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA93D:  ADC XPosFromCenter
        case 0x8240: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA93F:  AND #$3F
        case 0x8242: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA941:  STA XPosFromLeft
        case 0x8245: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA943:  STA XFromLeftTemp
        case 0x8248: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LA945:  JSR DoAddrCalc
        case 0x824B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA948:  LDA XPosFromCenter
        case 0x824E: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA94A:  ASL
        case 0x824F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA94B:  LDA XPosFromCenter
        case 0x8252: cycles_to_run -= 2; reg_A = cpu_ror(reg_A); reg_PC += 1; return; // LA94D:  ROR
        case 0x8253: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA94E:  CLC
        case 0x8254: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA94F:  ADC CharXPos
        case 0x8257: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA951:  STA XTarget
        case 0x825A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA953:  LDA YPosFromCenter
        case 0x825D: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA955:  ASL
        case 0x825E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA956:  LDA YPosFromCenter
        case 0x8261: cycles_to_run -= 2; reg_A = cpu_ror(reg_A); reg_PC += 1; return; // LA958:  ROR
        case 0x8262: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA959:  CLC
        case 0x8263: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA95A:  ADC CharYPos
        case 0x8266: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA95C:  STA YTarget
        case 0x8269: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LA95E:  JSR GetBlockID
        case 0x826C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA961:  LDA MapType
        case 0x826F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA963:  CMP #MAP_DUNGEON
        case 0x8271: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LA965:  BNE ChkCoveredArea
        case 0x8273: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA967:  LDA XPosFromCenter
        case 0x8276: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LA969:  BPL +
        case 0x8278: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA96B:  EOR #$FF
        case 0x827A: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA96D:  CLC
        case 0x827B: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LA96E:  ADC #$01
        case 0x827D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA970:  STA GenByte3E
        case 0x8280: cycles_to_run -= 3; reg_PC = 0x0000; return; // LA972:  JMP ChkLightDiameterX
        case 0x8283: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA977:  STA GenByte3E
        case 0x8286: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA979:  LDA LightDiameter
        case 0x8289: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LA97B:  CMP GenByte3E
        case 0x828C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LA97D:  BCS ChkLightXEdge
        case 0x828E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA97F:  LDA #BLK_BLANK
        case 0x8290: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA981:  STA TargetResults
        case 0x8293: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA983:  LDA #$00
        case 0x8295: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA985:  STA BlkRemoveFlgs
        case 0x8298: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LA987:  BEQ CalcBlockIndex
        case 0x829A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LA989:  BNE ChkLightY
        case 0x829C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA98B:  LDA XPosFromCenter
        case 0x829F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LA98D:  BPL LightXRight
        case 0x82A1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA98F:  LDA #$05
        case 0x82A3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA991:  STA BlkRemoveFlgs
        case 0x82A6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LA993:  BNE ChkLightY
        case 0x82A8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA995:  LDA #$0A
        case 0x82AA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA997:  STA BlkRemoveFlgs
        case 0x82AD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA999:  LDA YPosFromCenter
        case 0x82B0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LA99B:  BPL LightYBottom
        case 0x82B2: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA99D:  EOR #$FF
        case 0x82B4: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA99F:  CLC
        case 0x82B5: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LA9A0:  ADC #$01
        case 0x82B7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA9A2:  STA GenByte3E
        case 0x82BA: cycles_to_run -= 3; reg_PC = 0x0000; return; // LA9A4:  JMP ChkLightDiameterY
        case 0x82BD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA9A7:  LDA YPosFromCenter
        case 0x82C0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA9A9:  STA GenByte3E
        case 0x82C3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA9AB:  LDA LightDiameter
        case 0x82C6: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LA9AD:  CMP GenByte3E
        case 0x82C9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LA9AF:  BCS ChkLightYEdge
        case 0x82CB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA9B1:  LDA #BLK_BLANK
        case 0x82CD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA9B3:  STA TargetResults
        case 0x82D0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA9B5:  LDA #$00
        case 0x82D2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA9B7:  STA BlkRemoveFlgs
        case 0x82D5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LA9B9:  BEQ CalcBlockIndex
        case 0x82D7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LA9BB:  BNE CalcBlockIndex
        case 0x82D9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA9BD:  LDA YPosFromCenter
        case 0x82DC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LA9BF:  BPL +
        case 0x82DE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA9C1:  LDA #$03
        case 0x82E0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA9C3:  STA BlkRemoveFlgs
        case 0x82E3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LA9C5:  BNE CalcBlockIndex
        case 0x82E5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA9C9:  STA BlkRemoveFlgs
        case 0x82E8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LA9CB:  BNE CalcBlockIndex
        case 0x82EA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LA9CD:  JSR HasCoverData
        case 0x82ED: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA9D0:  LDA CoverStatus
        case 0x82F0: cycles_to_run -= 2; reg_A ^= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA9D2:  EOR CoveredStsNext
        case 0x82F3: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA9D4:  AND #$08
        case 0x82F5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LA9D6:  BEQ CalcBlockIndex
        case 0x82F7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA9D8:  LDA CoverStatus
        case 0x82FA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LA9DA:  BNE +
        case 0x82FC: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA9DC:  LDA #BLK_SML_TILES
        case 0x82FE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA9DE:  STA TargetResults
        case 0x8301: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LA9E0:  BNE CalcBlockIndex
        case 0x8303: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA9E4:  STA TargetResults
        case 0x8306: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA9E6:  LDA TargetResults
        case 0x8309: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA9E8:  ASL
        case 0x830A: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA9E9:  ASL
        case 0x830B: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA9EA:  ADC TargetResults
        case 0x830E: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA9EC:  ADC GFXTilesPtr
        case 0x8311: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA9EF:  STA BlockDataPtrLB
        case 0x8314: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA9F1:  LDA GFXTilesPtr+1
        case 0x8317: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LA9F4:  ADC #$00
        case 0x8319: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA9F6:  STA BlockDataPtrUB
        case 0x831C: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LA9F8:  LDX WndLineBufIdx
        case 0x831F: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LA9FA:  LDY #$00
        case 0x8321: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA9FC:  LDA (BlockDataPtr),Y
        case 0x8323: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LA9FE:  STA WndLineBuf,X
        case 0x8326: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAA01:  INY
        case 0x8327: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LAA02:  LDA (BlockDataPtr),Y
        case 0x8329: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAA04:  STA WndLineBuf+1,X
        case 0x832C: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LAA07:  TXA
        case 0x832D: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAA08:  CLC
        case 0x832E: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LAA09:  ADC WndEraseWdth
        case 0x8331: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LAA0C:  TAX
        case 0x8332: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA0D:  LDA PPUAddrLB
        case 0x8335: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAA0F:  CLC
        case 0x8336: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LAA10:  ADC #$1E
        case 0x8338: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA12:  STA PPUAddrLB
        case 0x833B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LAA14:  BCC +
        case 0x833D: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAA16:  INC PPUAddrUB
        case 0x8340: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LAA19:  LDA (BlockDataPtr),Y
        case 0x8342: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAA1B:  STA WndLineBuf,X
        case 0x8345: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAA1E:  INY
        case 0x8346: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LAA1F:  LDA (BlockDataPtr),Y
        case 0x8348: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAA21:  STA WndLineBuf+1,X
        case 0x834B: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAA24:  INY
        case 0x834C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA25:  LDA XFromLeftTemp
        case 0x834F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA27:  STA XPosFromLeft
        case 0x8352: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA29:  LDA YFromTopTemp
        case 0x8355: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA2B:  STA YPosFromTop
        case 0x8358: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LAA2D:  LDA (BlockDataPtr),Y
        case 0x835A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA2F:  STA PPUDataByte
        case 0x835D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA31:  LDA AddAttribData
        case 0x8360: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAA33:  BNE ModWndExit
        case 0x8362: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LAA35:  LDX AttribBufIndex
        case 0x8365: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA37:  LDA PPUDataByte
        case 0x8368: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAA39:  STA AttribTblBuf,X
        case 0x836B: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAA3C:  RTS
        case 0x836C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LAA3D:  JSR LoadFadePals
        case 0x836F: cycles_to_run -= 3; reg_PC = 0x0000; return; // LAA40:  JMP PalFadeIn
        case 0x8372: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LAA43:  JSR LoadFadePals
        case 0x8375: cycles_to_run -= 3; reg_PC = 0x0000; return; // LAA46:  JMP PalFadeOut
        case 0x8378: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LAA49:  LDA BlackPalPtr
        case 0x837B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA4C:  STA SprtPalPtrLB
        case 0x837E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA4E:  LDA BlackPalPtr+1
        case 0x8381: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA51:  STA SprtPalPtrUB
        case 0x8384: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LAA53:  LDA FadePalPtr
        case 0x8387: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA56:  STA BGPalPtrLB
        case 0x838A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA58:  LDA FadePalPtr+1
        case 0x838D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA5B:  STA BGPalPtrUB
        case 0x8390: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAA5D:  LDA #PAL_LOAD_BG
        case 0x8392: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA5F:  STA LoadBGPal
        case 0x8395: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAA61:  RTS
        case 0x8396: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAA62:  LDA #PAL_LOAD_BG
        case 0x8398: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA64:  STA LoadBGPal
        case 0x839B: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LAA66:  LDA RegSPPalPtr
        case 0x839E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA69:  STA SprtPalPtrLB
        case 0x83A1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA6B:  LDA RegSPPalPtr+1
        case 0x83A4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA6E:  STA SprtPalPtrUB
        case 0x83A7: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LAA70:  LDA TownPalPtr
        case 0x83AA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA73:  STA BGPalPtrLB
        case 0x83AD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA75:  LDA TownPalPtr+1
        case 0x83B0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA78:  STA BGPalPtrUB
        case 0x83B3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LAA7A:  JSR PalFadeOut
        case 0x83B6: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAA7D:  RTS
        case 0x83B7: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LAA7E:  LDA RegSPPalPtr
        case 0x83BA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA81:  STA SprtPalPtrLB
        case 0x83BD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA83:  LDA RegSPPalPtr+1
        case 0x83C0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA86:  STA SprtPalPtrUB
        case 0x83C3: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LAA88:  LDA PreGamePalPtr
        case 0x83C6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA8B:  STA BGPalPtrLB
        case 0x83C9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA8D:  LDA PreGamePalPtr+1
        case 0x83CC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA90:  STA BGPalPtrUB
        case 0x83CF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAA92:  LDA #PAL_LOAD_BG
        case 0x83D1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA94:  STA LoadBGPal
        case 0x83D4: cycles_to_run -= 3; reg_PC = 0x0000; return; // LAA96:  JMP PalFadeIn
        case 0x83D7: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LAA99:  LDA BlackPalPtr
        case 0x83DA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA9C:  STA PalPtrLB
        case 0x83DD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA9E:  LDA BlackPalPtr+1
        case 0x83E0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAAA1:  STA PalPtrUB
        case 0x83E3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAAA3:  LDA #$00
        case 0x83E5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAAA5:  STA PalModByte
        case 0x83E8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LAAA7:  JSR PrepBGPalLoad
        case 0x83EB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LAAAA:  JSR WaitForNMI
        case 0x83EE: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LAAAD:  LDA BlackPalPtr
        case 0x83F1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAAB0:  STA PalPtrLB
        case 0x83F4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAAB2:  LDA BlackPalPtr+1
        case 0x83F7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAAB5:  STA PalPtrUB
        case 0x83FA: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAAB7:  LDA #$00
        case 0x83FC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAAB9:  STA PalModByte
        case 0x83FF: cycles_to_run -= 3; reg_PC = 0x0000; return; // LAABB:  JMP PrepSPPalLoad
        case 0x8402: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAABE:  LDA MapWidth
        case 0x8405: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAAC0:  CLC
        case 0x8406: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LAAC1:  ADC #$01
        case 0x8408: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAAC3:  LSR
        case 0x8409: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAAC4:  STA MultNum1LB
        case 0x840C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAAC6:  LDA #$00
        case 0x840E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAAC8:  STA MultNum1UB
        case 0x8411: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAACA:  STA MultNum2UB
        case 0x8414: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAACC:  LDA _TargetY
        case 0x8417: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAACE:  STA MultNum2LB
        case 0x841A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LAAD0:  JSR WordMultiply
        case 0x841D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAAD3:  LDA _TargetX
        case 0x8420: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAAD5:  LSR
        case 0x8421: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAAD6:  CLC
        case 0x8422: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LAAD7:  ADC MultRsltLB
        case 0x8425: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAAD9:  STA GenWord3ELB
        case 0x8428: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAADB:  LDA MultRsltUB
        case 0x842B: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LAADD:  ADC #$00
        case 0x842D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAADF:  STA GenWord3EUB
        case 0x8430: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAAE1:  LDA CoverDatLB
        case 0x8433: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAAE3:  ORA CoverDatUB
        case 0x8436: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAAE5:  BNE CalcCoveredBlock
        case 0x8438: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAAE7:  LDA #$00
        case 0x843A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAAE9:  STA CoveredStsNext
        case 0x843D: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAAEB:  RTS
        case 0x843E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAAEC:  LDA MapWidth
        case 0x8441: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LAAEE:  CMP _TargetX
        case 0x8444: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x804E; return; } // LAAF0:  BCC NoCover
        case 0x8446: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAAF2:  LDA MapHeight
        case 0x8449: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LAAF4:  CMP _TargetY
        case 0x844C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x804E; return; } // LAAF6:  BCC NoCover
        case 0x844E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAAF8:  LDA GenWord3ELB
        case 0x8451: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAAFA:  CLC
        case 0x8452: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LAAFB:  ADC CoverDatLB
        case 0x8455: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAAFD:  STA GenWord3ELB
        case 0x8458: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAAFF:  LDA GenWord3EUB
        case 0x845B: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LAB01:  ADC CoverDatUB
        case 0x845E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB03:  STA GenWord3EUB
        case 0x8461: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LAB05:  TYA
        case 0x8462: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LAB06:  PHA
        case 0x8463: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LAB07:  LDY #$00
        case 0x8465: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LAB09:  LDA (GenWord3E),Y
        case 0x8467: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB0B:  STA CoveredStsNext
        case 0x846A: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LAB0D:  PLA
        case 0x846B: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LAB0E:  TAY
        case 0x846C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAB0F:  LDA _TargetX
        case 0x846F: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAB11:  AND #$01
        case 0x8471: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAB13:  BNE FinishCoverCheck
        case 0x8473: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LAB15:  LSR CoveredStsNext
        case 0x8476: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LAB17:  LSR CoveredStsNext
        case 0x8479: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LAB19:  LSR CoveredStsNext
        case 0x847C: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LAB1B:  LSR CoveredStsNext
        case 0x847F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAB1D:  LDA CoveredStsNext
        case 0x8482: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAB1F:  AND #$08
        case 0x8484: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB21:  STA CoveredStsNext
        case 0x8487: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAB23:  RTS
        case 0x8488: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LAB24:  TAX
        case 0x8489: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LAB25:  PHA
        case 0x848A: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LAB26:  LDX #$00
        case 0x848C: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LAB28:  LDY XTarget
        case 0x848F: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LAB2A:  STY GenByte2C
        case 0x8492: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LAB2C:  CPY #$77
        case 0x8494: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LAB2E:  BEQ ChkWtrBlkRght
        case 0x8496: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAB30:  INY
        case 0x8497: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LAB31:  STY XTarget
        case 0x849A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LAB33:  JSR FindRowBlock
        case 0x849D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LAB36:  BEQ PrepBlockLeft
        case 0x849F: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LAB38:  TXA
        case 0x84A0: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAB39:  CLC
        case 0x84A1: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LAB3A:  ADC #$04
        case 0x84A3: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LAB3C:  TAX
        case 0x84A4: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LAB3D:  LDY GenByte2C
        case 0x84A7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LAB3F:  BEQ ChkWtrBlkLft
        case 0x84A9: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LAB41:  DEY
        case 0x84AA: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LAB42:  STY XTarget
        case 0x84AD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LAB44:  JSR FindRowBlock
        case 0x84B0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LAB47:  BEQ PrepBlockUp
        case 0x84B2: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LAB49:  INX
        case 0x84B3: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LAB4A:  INX
        case 0x84B4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAB4B:  LDA GenByte2C
        case 0x84B7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB4D:  STA XTarget
        case 0x84BA: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LAB4F:  LDY YTarget
        case 0x84BD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LAB51:  BEQ ChkWtrBlkUp
        case 0x84BF: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LAB53:  DEY
        case 0x84C0: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LAB54:  TYA
        case 0x84C1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LAB55:  JSR ChkWtrOrBrdg
        case 0x84C4: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LAB5A:  INX
        case 0x84C5: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LAB5D:  CPY #$77
        case 0x84C7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LAB5F:  BEQ +
        case 0x84C9: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAB61:  INY
        case 0x84CA: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LAB62:  TYA
        case 0x84CB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LAB63:  JSR ChkWtrOrBrdg
        case 0x84CE: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LAB68:  TXA
        case 0x84CF: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAB69:  CLC
        case 0x84D0: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LAB6A:  ADC #$08
        case 0x84D2: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LAB6C:  TAX
        case 0x84D3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB70:  STA TargetResults
        case 0x84D6: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LAB72:  PLA
        case 0x84D7: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LAB73:  TAX
        case 0x84D8: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LAB74:  PLA
        case 0x84D9: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LAB75:  TAY
        case 0x84DA: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAB76:  RTS
        case 0x84DB: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LAB77:  TYA
        case 0x84DC: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LAB78:  PHA
        case 0x84DD: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LAB79:  TXA
        case 0x84DE: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LAB7A:  PHA
        case 0x84DF: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LAB7B:  LDX #$00
        case 0x84E1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAB7D:  LDA MapNumber
        case 0x84E4: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAB7F:  CMP #MAP_OVERWORLD
        case 0x84E6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAB81:  BNE BoundsChkEnd
        case 0x84E8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAB83:  LDA YTarget
        case 0x84EB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0x0000; return; } // LAB85:  BMI ChkYBounds
        case 0x84ED: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAB87:  CMP #$78
        case 0x84EF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LAB89:  BCS ChkYBounds
        case 0x84F1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAB8B:  LDA XTarget
        case 0x84F4: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAB8D:  CMP #$FF
        case 0x84F6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LAB8F:  BEQ ChkRightBounds
        case 0x84F8: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAB91:  CMP #$78
        case 0x84FA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LAB93:  BNE ChkYBounds
        case 0x84FC: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAB95:  DEC XTarget
        case 0x84FF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAB97:  LDA YTarget
        case 0x8502: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LAB99:  JSR ChkWtrOrBrdg
        case 0x8505: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LAB9C:  BEQ +
        case 0x8507: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LAB9E:  LDX #$02
        case 0x8509: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LABA3:  INC XTarget
        case 0x850C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LABA5:  LDA YTarget
        case 0x850F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LABA7:  JSR ChkWtrOrBrdg
        case 0x8512: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LABAA:  BEQ +
        case 0x8514: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LABAC:  LDX #$04
        case 0x8516: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LABB1:  LDA XTarget
        case 0x8519: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0x804E; return; } // LABB3:  BMI BoundsChkEnd
        case 0x851B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LABB5:  CMP #$78
        case 0x851D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x804E; return; } // LABB7:  BCS BoundsChkEnd
        case 0x851F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LABB9:  LDA YTarget
        case 0x8522: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LABBB:  CMP #$FF
        case 0x8524: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LABBD:  BEQ ChkDownBounds
        case 0x8526: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LABBF:  CMP #$78
        case 0x8528: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LABC1:  BNE BoundsChkEnd
        case 0x852A: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LABC3:  DEC YTarget
        case 0x852D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LABC5:  LDA YTarget
        case 0x8530: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LABC7:  JSR ChkWtrOrBrdg
        case 0x8533: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LABCA:  BEQ +
        case 0x8535: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LABCC:  LDX #$01
        case 0x8537: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LABD1:  INC YTarget
        case 0x853A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LABD3:  LDA YTarget
        case 0x853D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LABD5:  JSR ChkWtrOrBrdg
        case 0x8540: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LABD8:  BEQ +
        case 0x8542: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LABDA:  LDX #$08
        case 0x8544: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LABDF:  LDA BoundryBlock
        case 0x8547: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LABE1:  STA TargetResults
        case 0x854A: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LABE3:  PLA
        case 0x854B: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LABE4:  TAX
        case 0x854C: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LABE5:  PLA
        case 0x854D: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LABE6:  TAY
        case 0x854E: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LABE7:  RTS
        case 0x854F: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LABE8:  ASL
        case 0x8550: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LABE9:  TAY
        case 0x8551: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x804E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LABEA:  LDA WrldMapPtrTbl,Y
        case 0x8554: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LABED:  STA WrldMapPtrLB
        case 0x8557: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LABEF:  LDA WrldMapPtrTbl+1,Y
        case 0x855A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LABF2:  STA WrldMapPtrUB
        case 0x855D: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LABF4:  LDY #$00
        case 0x855F: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LABF6:  STY WrldMapXPos
        case 0x8562: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LABF8:  LDA (WrldMapPtr),Y
        case 0x8564: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LABFA:  AND #$0F
        case 0x8566: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LABFC:  SEC
        case 0x8567: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LABFD:  ADC WrldMapXPos
        case 0x856A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LABFF:  STA WrldMapXPos
        case 0x856D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC01:  LDA XTarget
        case 0x8570: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LAC03:  CMP WrldMapXPos
        case 0x8573: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x804E; return; } // LAC05:  BCC MapBlkFound2
        case 0x8575: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAC07:  INY
        case 0x8576: cycles_to_run -= 3; reg_PC = 0x804E; return; // LAC08:  JMP FindMapBlkLoop2
        case 0x8579: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LAC0B:  LDA (WrldMapPtr),Y
        case 0x857B: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAC0D:  AND #$F0
        case 0x857D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAC0F:  CMP #$40
        case 0x857F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAC11:  BNE ChkBrdgBlk
        case 0x8581: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAC13:  RTS
        case 0x8582: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAC14:  CMP #$B0
        case 0x8584: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAC16:  RTS
        case 0x8585: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC17:  LDA XTarget
        case 0x8588: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC19:  STA _TargetX
        case 0x858B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC1B:  LDA YTarget
        case 0x858E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC1D:  STA _TargetY
        case 0x8591: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC1F:  LDA MapWidth
        case 0x8594: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LAC21:  CMP XTarget
        case 0x8597: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x804E; return; } // LAC23:  BCS BlkIDCheckEn
        case 0x8599: cycles_to_run -= 3; reg_PC = 0x804E; return; // LAC25:  JMP TrgtOutOfBounds
        case 0x859C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC28:  LDA EnNumber
        case 0x859F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAC2A:  CMP #EN_DRAGONLORD2
        case 0x85A1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LAC2C:  BNE BlkIDChkYCoord
        case 0x85A3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAC2E:  LDA #BLK_BLANK
        case 0x85A5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC30:  STA TargetResults
        case 0x85A8: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAC32:  RTS
        case 0x85A9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC33:  LDA MapHeight
        case 0x85AC: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LAC35:  CMP YTarget
        case 0x85AF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x804E; return; } // LAC37:  BCC JmpOutOfBounds
        case 0x85B1: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LAC39:  TYA
        case 0x85B2: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LAC3A:  PHA
        case 0x85B3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC3B:  LDA MapNumber
        case 0x85B6: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAC3D:  CMP #MAP_OVERWORLD
        case 0x85B8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAC3F:  BNE ChkOthrMaps
        case 0x85BA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC41:  LDA XTarget
        case 0x85BD: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAC43:  CMP #$40
        case 0x85BF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAC45:  BNE GetOvrWldTarget
        case 0x85C1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC47:  LDA YTarget
        case 0x85C4: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAC49:  CMP #$31
        case 0x85C6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAC4B:  BNE GetOvrWldTarget
        case 0x85C8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC4D:  LDA ModsnSpells
        case 0x85CB: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAC4F:  AND #F_RNBW_BRDG
        case 0x85CD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LAC51:  BEQ GetOvrWldTarget
        case 0x85CF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAC53:  LDA #BLK_BRIDGE
        case 0x85D1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC55:  STA TargetResults
        case 0x85D4: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LAC57:  PLA
        case 0x85D5: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LAC58:  TAY
        case 0x85D6: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAC59:  RTS
        case 0x85D7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC5A:  LDA YTarget
        case 0x85DA: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LAC5C:  ASL
        case 0x85DB: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LAC5D:  TAY
        case 0x85DC: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x804E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LAC5E:  LDA WrldMapPtrTbl,Y
        case 0x85DF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC61:  STA WrldMapPtrLB
        case 0x85E2: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LAC63:  LDA WrldMapPtrTbl+1,Y
        case 0x85E5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC66:  STA WrldMapPtrUB
        case 0x85E8: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LAC68:  LDY #$00
        case 0x85EA: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LAC6A:  STY WrldMapXPos
        case 0x85ED: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LAC6C:  LDA (WrldMapPtr),Y
        case 0x85EF: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAC6E:  AND #$0F
        case 0x85F1: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LAC70:  SEC
        case 0x85F2: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LAC71:  ADC WrldMapXPos
        case 0x85F5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC73:  STA WrldMapXPos
        case 0x85F8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC75:  LDA XTarget
        case 0x85FB: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LAC77:  CMP WrldMapXPos
        case 0x85FE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x804E; return; } // LAC79:  BCC MapBlkFound
        case 0x8600: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAC7B:  INY
        case 0x8601: cycles_to_run -= 3; reg_PC = 0x804E; return; // LAC7C:  JMP FindMapBlkLoop
        case 0x8604: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LAC7F:  LDA (WrldMapPtr),Y
        case 0x8606: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAC81:  LSR
        case 0x8607: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAC82:  LSR
        case 0x8608: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAC83:  LSR
        case 0x8609: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAC84:  LSR
        case 0x860A: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAC85:  CLC
        case 0x860B: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LAC86:  ADC MapType
        case 0x860E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAC88:  CMP #$04
        case 0x8610: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAC8A:  BNE ConvBlkID
        case 0x8612: cycles_to_run -= 3; reg_PC = 0x804E; return; // LAC8C:  JMP DoWtrConv
        case 0x8615: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LAC8F:  TAY
        case 0x8616: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x804E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LAC90:  LDA WrldBlkConvTbl,Y
        case 0x8619: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC93:  STA TargetResults
        case 0x861C: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LAC95:  PLA
        case 0x861D: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LAC96:  TAY
        case 0x861E: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAC97:  RTS
        case 0x861F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAC98:  LDA #$00
        case 0x8621: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC9A:  STA MultNum1UB
        case 0x8624: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC9C:  STA MultNum2UB
        case 0x8627: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC9E:  LDA MapWidth
        case 0x862A: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LACA0:  LSR
        case 0x862B: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LACA1:  ADC #$00
        case 0x862D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LACA3:  STA MultNum1LB
        case 0x8630: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LACA5:  JSR WordMultiply
        case 0x8633: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LACA8:  LDA _TargetX
        case 0x8636: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LACAA:  LSR
        case 0x8637: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LACAB:  CLC
        case 0x8638: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LACAC:  ADC MultRsltLB
        case 0x863B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LACAE:  STA MapBytePtrLB
        case 0x863E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LACB0:  STA GenPtr3ELB
        case 0x8641: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LACB2:  LDA MultRsltUB
        case 0x8644: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LACB4:  ADC #$00
        case 0x8646: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LACB6:  STA MapBytePtrUB
        case 0x8649: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LACB8:  STA GenPtr3EUB
        case 0x864C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LACBA:  LDA MapBytePtrLB
        case 0x864F: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LACBC:  CLC
        case 0x8650: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LACBD:  ADC MapDatPtrLB
        case 0x8653: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LACBF:  STA MapBytePtrLB
        case 0x8656: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LACC1:  LDA MapBytePtrUB
        case 0x8659: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LACC3:  ADC MapDatPtrUB
        case 0x865C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LACC5:  STA MapBytePtrUB
        case 0x865F: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LACC7:  LDY #$00
        case 0x8661: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LACC9:  LDA (MapBytePtr),Y
        case 0x8663: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LACCB:  STA TargetResults
        case 0x8666: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LACCD:  LDA _TargetX
        case 0x8669: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LACCF:  LSR
        case 0x866A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x804E; return; } // LACD0:  BCS ChkRemovedBlks
        case 0x866C: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LACD2:  LSR TargetResults
        case 0x866F: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LACD4:  LSR TargetResults
        case 0x8672: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LACD6:  LSR TargetResults
        case 0x8675: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LACD8:  LSR TargetResults
        case 0x8678: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LACDA:  LDA MapNumber
        case 0x867B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LACDC:  CMP #MAP_TANTCSTL_SL
        case 0x867D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x804E; return; } // LACDE:  BCC TownBlockMask
        case 0x867F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LACE0:  LDA #$07
        case 0x8681: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LACE2:  BNE GetBaseBlockID
        case 0x8683: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LACE4:  LDA #$0F
        case 0x8685: cycles_to_run -= 2; reg_A &= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LACE6:  AND XTarget
        case 0x8688: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LACE8:  CLC
        case 0x8689: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LACE9:  ADC MapType
        case 0x868C: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LACEB:  TAY
        case 0x868D: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x804E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LACEC:  LDA GenBlkConvTbl,Y
        case 0x8690: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LACEF:  STA TargetResults
        case 0x8693: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LACF1:  CMP #BLK_PRINCESS
        case 0x8695: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LACF3:  BNE ChkStairBlock
        case 0x8697: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LACF5:  LDA PlayerFlags
        case 0x869A: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LACF7:  AND #F_DONE_GWAELIN
        case 0x869C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LACF9:  BEQ ReturnBlockID
        case 0x869E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LACFB:  LDA #BLK_BRICK
        case 0x86A0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LACFD:  STA TargetResults
        case 0x86A3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LACFF:  BNE ReturnBlockID
        case 0x86A5: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAD01:  CMP #BLK_STAIR_DN
        case 0x86A7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAD03:  BNE ChkTrsrBlock
        case 0x86A9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD05:  LDA MapNumber
        case 0x86AC: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAD07:  CMP #MAP_DLCSTL_GF
        case 0x86AE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAD09:  BNE ReturnBlockID
        case 0x86B0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD0B:  LDA _TargetX
        case 0x86B3: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAD0D:  CMP #$0A
        case 0x86B5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAD0F:  BNE ReturnBlockID
        case 0x86B7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD11:  LDA _TargetY
        case 0x86BA: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAD13:  CMP #$01
        case 0x86BC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAD15:  BNE ReturnBlockID
        case 0x86BE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD17:  LDA ModsnSpells
        case 0x86C1: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAD19:  AND #F_PSG_FOUND
        case 0x86C3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAD1B:  BNE ReturnBlockID
        case 0x86C5: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAD1D:  LDA #BLK_FFIELD
        case 0x86C7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD1F:  STA TargetResults
        case 0x86CA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAD21:  BNE ReturnBlockID
        case 0x86CC: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAD23:  CMP #BLK_CHEST
        case 0x86CE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAD25:  BNE ChkDoorBlock
        case 0x86D0: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LAD27:  LDY #$00
        case 0x86D2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD29:  LDA _TargetX
        case 0x86D5: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(addr_abs_y(0x0000, nullptr))); reg_PC += 3; return; // LAD2B:  CMP TrsrXPos,Y
        case 0x86D8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAD2E:  BNE NextTrsr1
        case 0x86DA: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAD30:  INY
        case 0x86DB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD31:  LDA _TargetY
        case 0x86DE: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(addr_abs_y(0x0000, nullptr))); reg_PC += 3; return; // LAD33:  CMP TrsrXPos,Y
        case 0x86E1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAD36:  BNE NextTrsr2
        case 0x86E3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAD38:  LDA #BLK_BRICK
        case 0x86E5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD3A:  STA TargetResults
        case 0x86E8: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LAD3C:  PLA
        case 0x86E9: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LAD3D:  TAY
        case 0x86EA: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAD3E:  RTS
        case 0x86EB: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAD3F:  INY
        case 0x86EC: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAD40:  INY
        case 0x86ED: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LAD41:  CPY #$10
        case 0x86EF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAD43:  BNE ChkTreasureLoop
        case 0x86F1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD45:  LDA TargetResults
        case 0x86F4: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAD47:  CMP #BLK_DOOR
        case 0x86F6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAD49:  BNE ReturnBlockID
        case 0x86F8: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LAD4B:  LDY #$00
        case 0x86FA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD4D:  LDA _TargetX
        case 0x86FD: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(addr_abs_y(0x0000, nullptr))); reg_PC += 3; return; // LAD4F:  CMP DoorXPos,Y
        case 0x8700: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAD52:  BNE NextDoor1
        case 0x8702: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAD54:  INY
        case 0x8703: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD55:  LDA _TargetY
        case 0x8706: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(addr_abs_y(0x0000, nullptr))); reg_PC += 3; return; // LAD57:  CMP DoorXPos,Y
        case 0x8709: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAD5A:  BNE NextDoor2
        case 0x870B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LAD5C:  BEQ SetBrickID
        case 0x870D: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAD5E:  INY
        case 0x870E: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAD5F:  INY
        case 0x870F: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LAD60:  CPY #$10
        case 0x8711: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAD62:  BNE ChkDoorLoop
        case 0x8713: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LAD64:  BEQ ReturnBlockID
        case 0x8715: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD66:  LDA NTBlockY
        case 0x8718: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LAD68:  ASL
        case 0x8719: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAD69:  CLC
        case 0x871A: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LAD6A:  ADC YPosFromCenter
        case 0x871D: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAD6C:  CLC
        case 0x871E: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LAD6D:  ADC #$1E
        case 0x8720: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD6F:  STA DivNum1LB
        case 0x8723: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAD71:  LDA #$1E
        case 0x8725: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD73:  STA DivNum2
        case 0x8728: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LAD75:  JSR ByteDivide
        case 0x872B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD78:  LDA DivRemainder
        case 0x872E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD7A:  STA YPosFromTop
        case 0x8731: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD7C:  STA YFromTopTemp
        case 0x8734: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD7E:  LDA NTBlockX
        case 0x8737: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LAD80:  ASL
        case 0x8738: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAD81:  CLC
        case 0x8739: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LAD82:  ADC XPosFromCenter
        case 0x873C: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAD84:  AND #$3F
        case 0x873E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD86:  STA XPosFromLeft
        case 0x8741: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD88:  STA XFromLeftTemp
        case 0x8744: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LAD8A:  JSR DoAddrCalc
        case 0x8747: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD8D:  LDA XPosFromCenter
        case 0x874A: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LAD8F:  ASL
        case 0x874B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD90:  LDA XPosFromCenter
        case 0x874E: cycles_to_run -= 2; reg_A = cpu_ror(reg_A); reg_PC += 1; return; // LAD92:  ROR
        case 0x874F: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAD93:  CLC
        case 0x8750: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LAD94:  ADC CharXPos
        case 0x8753: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD96:  STA XTarget
        case 0x8756: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD98:  LDA YPosFromCenter
        case 0x8759: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LAD9A:  ASL
        case 0x875A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD9B:  LDA YPosFromCenter
        case 0x875D: cycles_to_run -= 2; reg_A = cpu_ror(reg_A); reg_PC += 1; return; // LAD9D:  ROR
        case 0x875E: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAD9E:  CLC
        case 0x875F: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LAD9F:  ADC CharYPos
        case 0x8762: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LADA1:  STA YTarget
        case 0x8765: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LADA3:  JSR GetBlockID
        case 0x8768: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LADA6:  LDA MapType
        case 0x876B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LADA8:  CMP #MAP_DUNGEON
        case 0x876D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LADAA:  BNE ChkCoveredArea2
        case 0x876F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LADAC:  LDA XPosFromCenter
        case 0x8772: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LADAE:  BPL +
        case 0x8774: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LADB0:  EOR #$FF
        case 0x8776: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LADB2:  CLC
        case 0x8777: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LADB3:  ADC #$01
        case 0x8779: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LADB5:  STA GenByte3E
        case 0x877C: cycles_to_run -= 3; reg_PC = 0x0000; return; // LADB7:  JMP ChkLightDiameterX2
        case 0x877F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LADBC:  STA GenByte3E
        case 0x8782: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LADBE:  LDA LightDiameter
        case 0x8785: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LADC0:  CMP GenByte3E
        case 0x8788: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LADC2:  BCS ChkLightXEdge2
        case 0x878A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LADC4:  LDA #BLK_BLANK
        case 0x878C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LADC6:  STA TargetResults
        case 0x878F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LADC8:  LDA #$00
        case 0x8791: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LADCA:  STA BlkRemoveFlgs
        case 0x8794: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LADCC:  BEQ CalcBlockIndex2
        case 0x8796: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LADCE:  BNE ChkLightY2
        case 0x8798: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LADD0:  LDA XPosFromCenter
        case 0x879B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LADD2:  BPL LightXRight2
        case 0x879D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LADD4:  LDA #$05
        case 0x879F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LADD6:  STA BlkRemoveFlgs
        case 0x87A2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LADD8:  BNE ChkLightY2
        case 0x87A4: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LADDA:  LDA #$0A
        case 0x87A6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LADDC:  STA BlkRemoveFlgs
        case 0x87A9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LADDE:  LDA YPosFromCenter
        case 0x87AC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LADE0:  BPL LightYBottom2
        case 0x87AE: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LADE2:  EOR #$FF
        case 0x87B0: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LADE4:  CLC
        case 0x87B1: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LADE5:  ADC #$01
        case 0x87B3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LADE7:  STA GenByte3E
        case 0x87B6: cycles_to_run -= 3; reg_PC = 0x0000; return; // LADE9:  JMP ChkLightDiameterY2
        case 0x87B9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LADEC:  LDA YPosFromCenter
        case 0x87BC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LADEE:  STA GenByte3E
        case 0x87BF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LADF0:  LDA LightDiameter
        case 0x87C2: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LADF2:  CMP GenByte3E
        case 0x87C5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LADF4:  BCS ChkLightYEdge2
        case 0x87C7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LADF6:  LDA #BLK_BLANK
        case 0x87C9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LADF8:  STA TargetResults
        case 0x87CC: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LADFA:  LDA #$00
        case 0x87CE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LADFC:  STA BlkRemoveFlgs
        case 0x87D1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LADFE:  BEQ CalcBlockIndex2
        case 0x87D3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAE00:  BNE CalcBlockIndex2
        case 0x87D5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAE02:  LDA YPosFromCenter
        case 0x87D8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LAE04:  BPL +
        case 0x87DA: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAE06:  LDA #$03
        case 0x87DC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAE08:  STA BlkRemoveFlgs
        case 0x87DF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAE0A:  BNE CalcBlockIndex2
        case 0x87E1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAE0E:  STA BlkRemoveFlgs
        case 0x87E4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAE10:  BNE CalcBlockIndex2
        case 0x87E6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LAE12:  JSR HasCoverData
        case 0x87E9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAE15:  LDA CoverStatus
        case 0x87EC: cycles_to_run -= 2; reg_A ^= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAE17:  EOR CoveredStsNext
        case 0x87EF: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAE19:  AND #$08
        case 0x87F1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LAE1B:  BEQ CalcBlockIndex2
        case 0x87F3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAE1D:  LDA CoverStatus
        case 0x87F6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAE1F:  BNE ModUnderCover
        case 0x87F8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAE21:  LDA #BLK_SML_TILES
        case 0x87FA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAE23:  STA GenByte3C
        case 0x87FD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAE25:  BNE CalcBlockIndex2
        case 0x87FF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAE27:  LDA #BLK_BLANK
        case 0x8801: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAE29:  STA GenByte3C
        case 0x8804: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAE2B:  LDA TargetResults
        case 0x8807: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LAE2D:  ASL
        case 0x8808: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LAE2E:  ASL
        case 0x8809: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LAE2F:  ADC TargetResults
        case 0x880C: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LAE31:  ADC GFXTilesPtr
        case 0x880F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAE34:  STA BlockDataPtrLB
        case 0x8812: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAE36:  LDA GFXTilesPtr+1
        case 0x8815: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LAE39:  ADC #$00
        case 0x8817: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAE3B:  STA BlockDataPtrUB
        case 0x881A: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LAE3D:  LDY #$00
        case 0x881C: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LAE3F:  LDA (BlockDataPtr),Y
        case 0x881E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAE41:  STA PPUDataByte
        case 0x8821: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LAE43:  JSR AddPPUBufEntry
        case 0x8824: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAE46:  LDA BlkRemoveFlgs
        case 0x8827: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAE48:  LSR
        case 0x8828: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x804E; return; } // LAE49:  BCC DoUpperRightTile
        case 0x882A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAE4B:  LDA MapType
        case 0x882D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAE4D:  CMP #MAP_DUNGEON
        case 0x882F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAE4F:  BNE RmvNonDngnTileUL
        case 0x8831: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LAE51:  LDX PPUBufCount
        case 0x8834: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LAE53:  DEX
        case 0x8835: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAE54:  LDA #TL_BLANK_TILE1
        case 0x8837: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAE56:  STA BlockRAM,X
        case 0x883A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAE59:  BNE DoUpperRightTile
        case 0x883C: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAE5B:  DEC PPUBufCount
        case 0x883F: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAE5D:  DEC PPUBufCount
        case 0x8842: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAE5F:  DEC PPUBufCount
        case 0x8845: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAE61:  DEC PPUEntCount
        case 0x8848: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAE63:  INY
        case 0x8849: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LAE64:  LDA (BlockDataPtr),Y
        case 0x884B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAE66:  STA PPUDataByte
        case 0x884E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LAE68:  JSR AddPPUBufEntry
        case 0x8851: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAE6B:  LDA BlkRemoveFlgs
        case 0x8854: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAE6D:  AND #BLK_UPPER_RIGHT
        case 0x8856: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LAE6F:  BEQ DoLowerLeftTile
        case 0x8858: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAE71:  LDA MapType
        case 0x885B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAE73:  CMP #MAP_DUNGEON
        case 0x885D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAE75:  BNE RmvNonDngnTileUR
        case 0x885F: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LAE77:  LDX PPUBufCount
        case 0x8862: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LAE79:  DEX
        case 0x8863: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAE7A:  LDA #TL_BLANK_TILE1
        case 0x8865: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAE7C:  STA BlockRAM,X
        case 0x8868: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAE7F:  BNE DoLowerLeftTile
        case 0x886A: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAE81:  DEC PPUBufCount
        case 0x886D: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAE83:  DEC PPUBufCount
        case 0x8870: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAE85:  DEC PPUBufCount
        case 0x8873: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAE87:  DEC PPUEntCount
        case 0x8876: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAE89:  INY
        case 0x8877: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAE8A:  LDA PPUAddrLB
        case 0x887A: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAE8C:  CLC
        case 0x887B: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LAE8D:  ADC #$1E
        case 0x887D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAE8F:  STA PPUAddrLB
        case 0x8880: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LAE91:  BCC +
        case 0x8882: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAE93:  INC PPUAddrUB
        case 0x8885: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAE97:  STA PPUDataByte
        case 0x8888: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LAE99:  JSR AddPPUBufEntry
        case 0x888B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAE9C:  LDA BlkRemoveFlgs
        case 0x888E: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAE9E:  AND #BLK_LOWER_LEFT
        case 0x8890: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LAEA0:  BEQ DoLowerRightTile
        case 0x8892: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAEA2:  LDA MapType
        case 0x8895: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAEA4:  CMP #MAP_DUNGEON
        case 0x8897: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAEA6:  BNE RmvNonDngnTileLL
        case 0x8899: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LAEA8:  LDX PPUBufCount
        case 0x889C: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LAEAA:  DEX
        case 0x889D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAEAB:  LDA #TL_BLANK_TILE1
        case 0x889F: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAEAD:  STA BlockRAM,X
        case 0x88A2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAEB0:  BNE DoLowerRightTile
        case 0x88A4: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAEB2:  DEC PPUBufCount
        case 0x88A7: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAEB4:  DEC PPUBufCount
        case 0x88AA: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAEB6:  DEC PPUBufCount
        case 0x88AD: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAEB8:  DEC PPUEntCount
        case 0x88B0: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAEBA:  INY
        case 0x88B1: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LAEBB:  LDA (BlockDataPtr),Y
        case 0x88B3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAEBD:  STA PPUDataByte
        case 0x88B6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LAEBF:  JSR AddPPUBufEntry
        case 0x88B9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAEC2:  LDA BlkRemoveFlgs
        case 0x88BC: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAEC4:  AND #BLK_LOWER_RIGHT
        case 0x88BE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LAEC6:  BEQ DoAttribByte
        case 0x88C0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAEC8:  LDA MapType
        case 0x88C3: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAECA:  CMP #MAP_DUNGEON
        case 0x88C5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAECC:  BNE RmvNonDngnTileLR
        case 0x88C7: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LAECE:  LDX PPUBufCount
        case 0x88CA: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LAED0:  DEX
        case 0x88CB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAED1:  LDA #TL_BLANK_TILE1
        case 0x88CD: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAED3:  STA BlockRAM,X
        case 0x88D0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAED6:  BNE DoAttribByte
        case 0x88D2: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAED8:  DEC PPUBufCount
        case 0x88D5: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAEDA:  DEC PPUBufCount
        case 0x88D8: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAEDC:  DEC PPUBufCount
        case 0x88DB: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAEDE:  DEC PPUEntCount
        case 0x88DE: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAEE0:  INY
        case 0x88DF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAEE1:  LDA XFromLeftTemp
        case 0x88E2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAEE3:  STA XPosFromLeft
        case 0x88E5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAEE5:  LDA YFromTopTemp
        case 0x88E8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAEE7:  STA YPosFromTop
        case 0x88EB: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LAEE9:  LDA (BlockDataPtr),Y
        case 0x88ED: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAEEB:  STA PPUDataByte
        case 0x88F0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LAEED:  JSR ModAttribBits
        case 0x88F3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAEF0:  LDA PPUHorzVert
        case 0x88F6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAEF2:  BNE ModBlockExit
        case 0x88F8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAEF4:  LDA PPUAddrUB
        case 0x88FB: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAEF6:  CLC
        case 0x88FC: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LAEF7:  ADC #$20
        case 0x88FE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAEF9:  STA PPUAddrUB
        case 0x8901: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LAEFB:  JSR AddPPUBufEntry
        case 0x8904: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAEFE:  RTS
        case 0x8905: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAEFF:  LDA CharXPos
        case 0x8908: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF01:  STA XTarget
        case 0x890B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAF03:  LDA CharYPos
        case 0x890E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF05:  STA YTarget
        case 0x8911: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LAF07:  JSR GetBlockID
        case 0x8914: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LAF0A:  JSR HasCoverData
        case 0x8917: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAF0D:  LDA CoveredStsNext
        case 0x891A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF0F:  STA CoverStatus
        case 0x891D: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAF11:  RTS
        case 0x891E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAF12:  LDA MapNumber
        case 0x8921: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAF14:  CMP #MAP_DLCSTL_SL1
        case 0x8923: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x804E; return; } // LAF16:  BCS ChkThRoomMap
        case 0x8925: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF18:  LDA #$01
        case 0x8927: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF1A:  STA LightDiameter
        case 0x892A: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAF1C:  LSR
        case 0x892B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF1D:  STA RadiantTimer
        case 0x892E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAF1F:  LDA MapNumber
        case 0x8931: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAF21:  CMP #MAP_THRONEROOM
        case 0x8933: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LAF23:  BEQ ChkOverworldMap
        case 0x8935: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAF25:  LDA PlayerFlags
        case 0x8938: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF27:  ORA #F_LEFT_THROOM
        case 0x893A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF29:  STA PlayerFlags
        case 0x893D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAF2B:  LDA MapNumber
        case 0x8940: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAF2D:  CMP #MAP_OVERWORLD
        case 0x8942: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAF2F:  BNE ChkLeftThRoom
        case 0x8944: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LAF31:  LDX #$00
        case 0x8946: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LAF33:  TXA
        case 0x8947: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAF34:  STA DoorXPos,X
        case 0x894A: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LAF37:  INX
        case 0x894B: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LAF38:  CPX #$20
        case 0x894D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAF3A:  BNE ClrOpenDoorsLoop
        case 0x894F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAF3C:  LDA PlayerFlags
        case 0x8952: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF3E:  AND #F_LEFT_THROOM
        case 0x8954: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LAF40:  BEQ SetNTAndScroll
        case 0x8956: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAF42:  LDA MapNumber
        case 0x8959: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAF44:  CMP #MAP_THRONEROOM
        case 0x895B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAF46:  BNE SetNTAndScroll
        case 0x895D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF48:  LDA #$04
        case 0x895F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF4A:  STA TrsrXPos+14
        case 0x8962: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF4D:  STA TrsrYPos+14
        case 0x8965: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF50:  STA TrsrYPos+12
        case 0x8968: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF53:  STA DoorXPos+14
        case 0x896B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF56:  LDA #$05
        case 0x896D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF58:  STA TrsrXPos+12
        case 0x8970: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF5B:  LDA #$06
        case 0x8972: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF5D:  STA TrsrXPos+10
        case 0x8975: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF60:  LDA #$01
        case 0x8977: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF62:  STA TrsrYPos+10
        case 0x897A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF65:  LDA #$07
        case 0x897C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF67:  STA DoorYPos+14
        case 0x897F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF6A:  LDA #$08
        case 0x8981: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF6C:  STA NTBlockX
        case 0x8984: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF6E:  LDA #$07
        case 0x8986: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF70:  STA NTBlockY
        case 0x8989: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF72:  LDA #$00
        case 0x898B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF74:  STA ScrollX
        case 0x898E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF76:  STA ScrollY
        case 0x8991: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF78:  STA ActiveNmTbl
        case 0x8994: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAF7A:  LDA MapNumber
        case 0x8997: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LAF7C:  ASL
        case 0x8998: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LAF7D:  ASL
        case 0x8999: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LAF7E:  ADC MapNumber
        case 0x899C: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LAF80:  TAY
        case 0x899D: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x804E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LAF81:  LDA MapDatTbl,Y
        case 0x89A0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF84:  STA MapDatPtrLB
        case 0x89A3: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAF86:  INY
        case 0x89A4: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x804E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LAF87:  LDA MapDatTbl,Y
        case 0x89A7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF8A:  STA MapDatPtrUB
        case 0x89AA: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAF8C:  INY
        case 0x89AB: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x804E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LAF8D:  LDA MapDatTbl,Y
        case 0x89AE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF90:  STA MapWidth
        case 0x89B1: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAF92:  INY
        case 0x89B2: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x804E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LAF93:  LDA MapDatTbl,Y
        case 0x89B5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF96:  STA MapHeight
        case 0x89B8: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAF98:  INY
        case 0x89B9: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x804E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LAF99:  LDA MapDatTbl,Y
        case 0x89BC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF9C:  STA BoundryBlock
        case 0x89BF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF9E:  LDA #$FF
        case 0x89C1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAFA0:  STA NPCUpdateCntr
        case 0x89C4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAFA2:  LDA StoryFlags
        case 0x89C7: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAFA4:  AND #F_DGNLRD_DEAD
        case 0x89C9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LAFA6:  BEQ ChkMapDungeon
        case 0x89CB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAFA8:  LDA MapNumber
        case 0x89CE: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAFAA:  CMP #MAP_TANTCSTL_GF
        case 0x89D0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAFAC:  BNE ChkMapDungeon
        case 0x89D2: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAFAE:  LDA #$0B
        case 0x89D4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAFB0:  BNE GetNPCDataPointer
        case 0x89D6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAFB2:  LDA MapNumber
        case 0x89D9: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LAFB4:  SEC
        case 0x89DA: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LAFB5:  SBC #MAP_TANTCSTL_GF
        case 0x89DC: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAFB7:  CMP #MAP_RAINBOW-3
        case 0x89DE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x804E; return; } // LAFB9:  BCS ChkGwaelinNPC
        case 0x89E0: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LAFBB:  ASL
        case 0x89E1: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LAFBC:  TAY
        case 0x89E2: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAFBD:  LDA #$00
        case 0x89E4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAFBF:  STA NPCUpdateCntr
        case 0x89E7: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x804E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LAFC1:  LDA NPCMobPtrTbl,Y
        case 0x89EA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAFC4:  STA NPCDatPtrLB
        case 0x89ED: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LAFC6:  LDA NPCMobPtrTbl+1,Y
        case 0x89F0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAFC9:  STA NPCDatPtrUB
        case 0x89F3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAFCB:  LDA #$00
        case 0x89F5: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LAFCD:  TAX
        case 0x89F6: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAFCE:  STA NPCXPos,X
        case 0x89F9: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LAFD0:  INX
        case 0x89FA: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LAFD1:  CPX #$3C
        case 0x89FC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAFD3:  BNE NPCClearLoop
        case 0x89FE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAFD5:  LDA MapNumber
        case 0x8A01: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAFD7:  CMP #MAP_DLCSTL_BF
        case 0x8A03: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAFD9:  BNE PrepMobNPCDatLoad
        case 0x8A05: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAFDB:  LDA StoryFlags
        case 0x8A08: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAFDD:  AND #F_DGNLRD_DEAD
        case 0x8A0A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LAFDF:  BNE ChkGwaelinNPC
        case 0x8A0C: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LAFE1:  LDY #$00
        case 0x8A0E: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LAFE3:  LDX #$00
        case 0x8A10: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LAFE5:  LDA (NPCDatPtr),Y
        case 0x8A12: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAFE7:  CMP #$FF
        case 0x8A14: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LAFE9:  BEQ PrepStatNPCDatLoad
        case 0x8A16: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAFEB:  STA NPCXPos,X
        case 0x8A19: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LAFED:  INX
        case 0x8A1A: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAFEE:  INY
        case 0x8A1B: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LAFEF:  LDA (NPCDatPtr),Y
        case 0x8A1D: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAFF1:  STA NPCXPos,X
        case 0x8A20: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LAFF3:  INX
        case 0x8A21: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAFF4:  INY
        case 0x8A22: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAFF5:  LDA #$00
        case 0x8A24: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAFF7:  STA NPCXPos,X
        case 0x8A27: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LAFF9:  INX
        case 0x8A28: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAFFA:  INY
        case 0x8A29: cycles_to_run -= 3; reg_PC = 0x804E; return; // LAFFB:  JMP LoadMobNPCDataLoop
        case 0x8A2C: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAFFE:  INY
        case 0x8A2D: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LAFFF:  LDX #$1E
        case 0x8A2F: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LB001:  LDA (NPCDatPtr),Y
        case 0x8A31: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB003:  CMP #$FF
        case 0x8A33: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB005:  BEQ ChkGwaelinNPC
        case 0x8A35: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB007:  STA NPCXPos,X
        case 0x8A38: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB009:  INX
        case 0x8A39: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB00A:  INY
        case 0x8A3A: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LB00B:  LDA (NPCDatPtr),Y
        case 0x8A3C: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB00D:  STA NPCXPos,X
        case 0x8A3F: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB00F:  INX
        case 0x8A40: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB010:  INY
        case 0x8A41: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB011:  LDA #$00
        case 0x8A43: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB013:  STA NPCXPos,X
        case 0x8A46: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB015:  INX
        case 0x8A47: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB016:  INY
        case 0x8A48: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB017:  JMP LoadStatNPCDataLoop
        case 0x8A4B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB01A:  LDA MapNumber
        case 0x8A4E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB01C:  CMP #MAP_THRONEROOM
        case 0x8A50: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB01E:  BNE ChkCoveredData
        case 0x8A52: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB020:  LDA PlayerFlags
        case 0x8A55: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB022:  AND #F_RTN_GWAELIN
        case 0x8A57: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB024:  BNE ChkCoveredData
        case 0x8A59: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB026:  LDA #$00
        case 0x8A5B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB028:  STA NPCXPos+$27
        case 0x8A5E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB02A:  STA NPCYPos+$27
        case 0x8A61: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB02C:  STA NPCMidPos+$27
        case 0x8A64: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB02E:  LDA MapNumber
        case 0x8A67: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB030:  CMP #MAP_DLCSTL_SL1
        case 0x8A69: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x804E; return; } // LB032:  BCC ChkWorldMap
        case 0x8A6B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB034:  LDA #MAP_DUNGEON
        case 0x8A6D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB036:  STA MapType
        case 0x8A70: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB038:  LDA #$00
        case 0x8A72: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB03A:  STA CoverDatLB
        case 0x8A75: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB03C:  STA CoverDatUB
        case 0x8A78: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB03E:  RTS
        case 0x8A79: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB03F:  LDA MapNumber
        case 0x8A7C: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB041:  CMP #MAP_OVERWORLD
        case 0x8A7E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB043:  BNE TownMap
        case 0x8A80: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB045:  LDA #MAP_OVRWLD
        case 0x8A82: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB047:  STA MapType
        case 0x8A85: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB049:  BEQ NoCoveredData
        case 0x8A87: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB04B:  LDA #MAP_TOWN
        case 0x8A89: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB04D:  STA MapType
        case 0x8A8C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB04F:  LDA MapNumber
        case 0x8A8F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB051:  CMP #MAP_BRECCONARY
        case 0x8A91: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB053:  BNE ChkMapGarinham
        case 0x8A93: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LB055:  LDA BrecCvrdDatPtr
        case 0x8A96: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB058:  STA CoverDatLB
        case 0x8A99: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB05A:  LDA BrecCvrdDatPtr+1
        case 0x8A9C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB05D:  STA CoverDatUB
        case 0x8A9F: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB05F:  RTS
        case 0x8AA0: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB060:  CMP #MAP_GARINHAM
        case 0x8AA2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB062:  BNE ChkMapCantlin
        case 0x8AA4: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LB064:  LDA GarinCvrdDatPtr
        case 0x8AA7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB067:  STA CoverDatLB
        case 0x8AAA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB069:  LDA GarinCvrdDatPtr+1
        case 0x8AAD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB06C:  STA CoverDatUB
        case 0x8AB0: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB06E:  RTS
        case 0x8AB1: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB06F:  CMP #MAP_CANTLIN
        case 0x8AB3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB071:  BNE ChkMapRimuldar
        case 0x8AB5: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LB073:  LDA CantCvrdDatPtr
        case 0x8AB8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB076:  STA CoverDatLB
        case 0x8ABB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB078:  LDA CantCvrdDatPtr+1
        case 0x8ABE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB07B:  STA CoverDatUB
        case 0x8AC1: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB07D:  RTS
        case 0x8AC2: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB07E:  CMP #MAP_RIMULDAR
        case 0x8AC4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB080:  BNE NoCoveredData
        case 0x8AC6: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LB082:  LDA RimCvrdDatPtr
        case 0x8AC9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB085:  STA CoverDatLB
        case 0x8ACC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB087:  LDA RimCvrdDatPtr+1
        case 0x8ACF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB08A:  STA CoverDatUB
        case 0x8AD2: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB08C:  RTS
        case 0x8AD3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB08D:  LDA #$00
        case 0x8AD5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB08F:  BEQ SetSoundAndFade
        case 0x8AD7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB091:  LDA #$00
        case 0x8AD9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB093:  STA GenByte25
        case 0x8ADC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB095:  BEQ ClearSprites
        case 0x8ADE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB097:  LDA #$01
        case 0x8AE0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB099:  STA GenByte25
        case 0x8AE3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB09B:  JSR ScreenFadeOut
        case 0x8AE6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB09E:  JSR ClearSpriteRAM
        case 0x8AE9: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB0A1:  LDA #%00000000
        case 0x8AEB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB0A3:  STA PPUControl1
        case 0x8AEE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB0A6:  STA DrgnLrdPal
        case 0x8AF1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB0A9:  JSR Bank1ToCHR0
        case 0x8AF4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB0AC:  JSR Bank2ToCHR1
        case 0x8AF7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB0AF:  LDA GenByte25
        case 0x8AFA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB0B1:  BEQ +
        case 0x8AFC: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB0B3:  LDA #SFX_STAIRS
        case 0x8AFE: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LB0B5:  BRK
        case 0x8AFF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB0BA:  STA PPUControl1
        case 0x8B02: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB0BD:  JSR InitMapData
        case 0x8B05: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB0C0:  JSR GetBlockNoTrans
        case 0x8B08: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB0C3:  LDA #$F2
        case 0x8B0A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB0C5:  STA YPosFromCenter
        case 0x8B0D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB0C7:  JSR WaitForNMI
        case 0x8B10: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB0CA:  LDA #$EE
        case 0x8B12: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB0CC:  STA XPosFromCenter
        case 0x8B15: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB0CE:  LDA #$00
        case 0x8B17: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB0D0:  STA BlkRemoveFlgs
        case 0x8B1A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB0D2:  STA PPUHorzVert
        case 0x8B1D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB0D4:  JSR ModMapBlock
        case 0x8B20: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB0D7:  INC XPosFromCenter
        case 0x8B23: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB0D9:  INC XPosFromCenter
        case 0x8B26: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB0DB:  BNE ChngMapLeftLoop
        case 0x8B28: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB0DD:  JSR WaitForNMI
        case 0x8B2B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB0E0:  LDA #$00
        case 0x8B2D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB0E2:  STA BlkRemoveFlgs
        case 0x8B30: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB0E4:  STA PPUHorzVert
        case 0x8B33: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB0E6:  JSR ModMapBlock
        case 0x8B36: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB0E9:  INC XPosFromCenter
        case 0x8B39: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB0EB:  INC XPosFromCenter
        case 0x8B3C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB0ED:  LDA XPosFromCenter
        case 0x8B3F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB0EF:  CMP #$12
        case 0x8B41: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB0F1:  BNE ChngMapRightLoop
        case 0x8B43: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB0F3:  INC YPosFromCenter
        case 0x8B46: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB0F5:  INC YPosFromCenter
        case 0x8B49: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB0F7:  LDA YPosFromCenter
        case 0x8B4C: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB0F9:  CMP #$10
        case 0x8B4E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB0FB:  BNE ChngMapRowLoop
        case 0x8B50: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB0FD:  JSR WaitForNMI
        case 0x8B53: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB100:  LDA #NPC_STOP
        case 0x8B55: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB102:  STA StopNPCMove
        case 0x8B58: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB104:  JSR DoSprites
        case 0x8B5B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB107:  LDA #NPC_MOVE
        case 0x8B5D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB109:  STA StopNPCMove
        case 0x8B60: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB10B:  JSR WaitForNMI
        case 0x8B63: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LB10E:  LDX MapNumber
        case 0x8B66: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x804E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB110:  LDA ResumeMusicTbl,X
        case 0x8B69: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LB113:  BRK
        case 0x8B6A: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LB116:  LDA RegSPPalPtr
        case 0x8B6D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB119:  STA SprtPalPtrLB
        case 0x8B70: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB11B:  LDA RegSPPalPtr+1
        case 0x8B73: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB11E:  STA SprtPalPtrUB
        case 0x8B76: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB120:  LDA #$FF
        case 0x8B78: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB122:  STA CoveredStsNext
        case 0x8B7B: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LB124:  LDA OverworldPalPtr
        case 0x8B7E: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB127:  CLC
        case 0x8B7F: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LB128:  ADC MapType
        case 0x8B82: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB12A:  STA BGPalPtrLB
        case 0x8B85: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB12C:  LDA OverworldPalPtr+1
        case 0x8B88: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB12F:  ADC #$00
        case 0x8B8A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB131:  STA BGPalPtrUB
        case 0x8B8D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB133:  JSR PalFadeIn
        case 0x8B90: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB136:  LDA #$02
        case 0x8B92: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB138:  STA PPUAddrLB
        case 0x8B95: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB13A:  LDA #$24
        case 0x8B97: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB13C:  STA PPUAddrUB
        case 0x8B9A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB13E:  LDA #TL_BLANK_TILE1
        case 0x8B9C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB140:  STA PPUDataByte
        case 0x8B9F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB142:  LDA #$0F
        case 0x8BA1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB144:  STA RowCounter
        case 0x8BA4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB146:  JSR WaitForNMI
        case 0x8BA7: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LB149:  LDY PPUBufCount
        case 0x8BAA: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB14B:  LDA #$80
        case 0x8BAC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB14D:  STA UpdateBGTiles
        case 0x8BAF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB150:  LDA PPUAddrUB
        case 0x8BB2: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB152:  ORA #$80
        case 0x8BB4: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB154:  STA BlockRAM,Y
        case 0x8BB7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB157:  LDA #$1C
        case 0x8BB9: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LB159:  TAX
        case 0x8BBA: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB15A:  STA BlockRAM+1,Y
        case 0x8BBD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB15D:  LDA PPUAddrLB
        case 0x8BC0: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB15F:  STA BlockRAM+2,Y
        case 0x8BC3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB162:  LDA PPUDataByte
        case 0x8BC6: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB167:  INY
        case 0x8BC7: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LB168:  DEX
        case 0x8BC8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB169:  BNE -
        case 0x8BCA: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB16B:  INC PPUEntCount
        case 0x8BCD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB16D:  LDA PPUAddrLB
        case 0x8BD0: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB16F:  CLC
        case 0x8BD1: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB170:  ADC #$20
        case 0x8BD3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB172:  STA PPUAddrLB
        case 0x8BD6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LB174:  BCC +
        case 0x8BD8: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB176:  INC PPUAddrUB
        case 0x8BDB: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB17A:  ORA #$80
        case 0x8BDD: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB17C:  STA BlockRAM+3,Y
        case 0x8BE0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB17F:  LDA #$1C
        case 0x8BE2: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LB181:  TAX
        case 0x8BE3: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB182:  STA BlockRAM+4,Y
        case 0x8BE6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB185:  LDA PPUAddrLB
        case 0x8BE9: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB187:  STA BlockRAM+5,Y
        case 0x8BEC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB18A:  LDA PPUDataByte
        case 0x8BEF: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB18F:  INY
        case 0x8BF0: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LB190:  DEX
        case 0x8BF1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB191:  BNE -
        case 0x8BF3: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB193:  INC PPUEntCount
        case 0x8BF6: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LB195:  TYA
        case 0x8BF7: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB196:  CLC
        case 0x8BF8: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB197:  ADC #$06
        case 0x8BFA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB199:  STA PPUBufCount
        case 0x8BFD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB19B:  LDA PPUAddrLB
        case 0x8C00: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB19D:  CLC
        case 0x8C01: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB19E:  ADC #$20
        case 0x8C03: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB1A0:  STA PPUAddrLB
        case 0x8C06: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LB1A2:  BCC +
        case 0x8C08: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB1A4:  INC PPUAddrUB
        case 0x8C0B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB1A8:  BNE NTClearLoop
        case 0x8C0D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB1AA:  JSR WaitForNMI
        case 0x8C10: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB1AD:  RTS
        case 0x8C11: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB1CC:  LDA _CharXPos
        case 0x8C14: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB1CE:  STA XTarget
        case 0x8C17: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB1D0:  LDA _CharYPos
        case 0x8C1A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB1D2:  STA YTarget
        case 0x8C1D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB1D4:  JSR GetBlockID
        case 0x8C20: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB1D7:  LDA TargetResults
        case 0x8C23: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB1D9:  CMP #BLK_LRG_TILE
        case 0x8C25: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x804E; return; } // LB1DB:  BCC CheckNPCCollision
        case 0x8C27: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB1DD:  LDA CharXPos
        case 0x8C2A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB1DF:  STA _CharXPos
        case 0x8C2D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB1E1:  LDA CharYPos
        case 0x8C30: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB1E3:  STA _CharYPos
        case 0x8C33: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB1E5:  PLA
        case 0x8C34: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB1E6:  PLA
        case 0x8C35: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB1E7:  PLA
        case 0x8C36: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB1E8:  PLA
        case 0x8C37: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB1E9:  LDA #SFX_WALL_BUMP
        case 0x8C39: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LB1EB:  BRK
        case 0x8C3A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB1EE:  LDA #$00
        case 0x8C3C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB1F0:  STA FrameCounter
        case 0x8C3F: cycles_to_run -= 3; reg_PC = 0x0000; return; // LB1F2:  JMP IdleUpdate
        case 0x8C42: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB1F5:  LDA NPCUpdateCntr
        case 0x8C45: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB1F7:  CMP #$FF
        case 0x8C47: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB1F9:  BNE InitNPCCheck
        case 0x8C49: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB1FB:  RTS
        case 0x8C4A: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB1FC:  LDX #$00
        case 0x8C4C: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB1FE:  LDA NPCXPos,X
        case 0x8C4F: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB200:  AND #$1F
        case 0x8C51: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LB202:  CMP _CharXPos
        case 0x8C54: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB204:  BNE NextNPC
        case 0x8C56: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB206:  LDA NPCYPos,X
        case 0x8C59: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB208:  AND #$1F
        case 0x8C5B: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LB20A:  CMP _CharYPos
        case 0x8C5E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB20C:  BNE NextNPC
        case 0x8C60: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB20E:  JMP DirectionBlocked
        case 0x8C63: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB211:  INX
        case 0x8C64: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB212:  INX
        case 0x8C65: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB213:  INX
        case 0x8C66: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LB214:  CPX #$3C
        case 0x8C68: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB216:  BNE NPCCheckLoop
        case 0x8C6A: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB218:  RTS
        case 0x8C6B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB219:  LDA MapWidth
        case 0x8C6E: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LB21B:  CMP CharXPos
        case 0x8C71: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x804E; return; } // LB21D:  BCC CheckMapExit
        case 0x8C73: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB21F:  LDA MapHeight
        case 0x8C76: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LB221:  CMP CharYPos
        case 0x8C79: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x804E; return; } // LB223:  BCC CheckMapExit
        case 0x8C7B: cycles_to_run -= 3; reg_PC = 0x0000; return; // LB225:  JMP CheckForTriggers
        case 0x8C7E: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB228:  LDX #$00
        case 0x8C80: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB22A:  LDA MapNumber
        case 0x8C83: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB22F:  BEQ NewMapFound
        case 0x8C85: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB231:  INX
        case 0x8C86: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB232:  INX
        case 0x8C87: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB233:  INX
        case 0x8C88: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LB234:  CPX #$93
        case 0x8C8A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB236:  BNE -
        case 0x8C8C: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB238:  RTS
        case 0x8C8D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB239:  LDA #DIR_DOWN
        case 0x8C8F: cycles_to_run -= 3; reg_PC = 0x0000; return; // LB23B:  JMP ChangeMaps
        case 0x8C92: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB23E:  LDA WinBufRAM+$84
        case 0x8C95: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB241:  CMP #$FF
        case 0x8C97: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB243:  BNE DoRemovePopUp
        case 0x8C99: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB245:  RTS
        case 0x8C9A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB246:  LDA FrameCounter
        case 0x8C9D: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LB248:  PHA
        case 0x8C9E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB249:  LDA #WND_POPUP
        case 0x8CA0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB24B:  JSR RemoveWindow
        case 0x8CA3: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB24E:  PLA
        case 0x8CA4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB24F:  STA FrameCounter
        case 0x8CA7: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB251:  RTS
        case 0x8CA8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB252:  JSR ChkRemovePopUp
        case 0x8CAB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB255:  LDA FrameCounter
        case 0x8CAE: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB257:  AND #$0F
        case 0x8CB0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB259:  BEQ RightSynced
        case 0x8CB2: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB25B:  PLA
        case 0x8CB3: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB25C:  PLA
        case 0x8CB4: cycles_to_run -= 3; reg_PC = 0x0000; return; // LB25D:  JMP IdleUpdate
        case 0x8CB7: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB260:  INC _CharXPos
        case 0x8CBA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB262:  JSR CheckCollision
        case 0x8CBD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB265:  LDA MapType
        case 0x8CC0: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB267:  CMP #MAP_DUNGEON
        case 0x8CC2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB269:  BNE UpdtRNonDungeon
        case 0x8CC4: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB26B:  INC CharXPos
        case 0x8CC7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB26D:  JSR UpdtHorzDungeon
        case 0x8CCA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB270:  LDA CharXPixelsLB
        case 0x8CCD: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB272:  CLC
        case 0x8CCE: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB273:  ADC #$08
        case 0x8CD0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB275:  STA CharXPixelsLB
        case 0x8CD3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LB277:  BCC +
        case 0x8CD5: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB279:  INC CharXPixelsUB
        case 0x8CD8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB27E:  LDA CharXPixelsLB
        case 0x8CDB: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB280:  CLC
        case 0x8CDC: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB281:  ADC #$08
        case 0x8CDE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB283:  STA CharXPixelsLB
        case 0x8CE1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LB285:  BCC +
        case 0x8CE3: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB287:  INC CharXPixelsUB
        case 0x8CE6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB28C:  LDA #$12
        case 0x8CE8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB28E:  STA XPosFromCenter
        case 0x8CEB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB290:  LDA #$F2
        case 0x8CED: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB292:  STA YPosFromCenter
        case 0x8CF0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB294:  JSR WaitForNMI
        case 0x8CF3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB297:  LDA #$00
        case 0x8CF5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB299:  STA BlkRemoveFlgs
        case 0x8CF8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB29B:  STA PPUHorzVert
        case 0x8CFB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB29D:  JSR ModMapBlock
        case 0x8CFE: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB2A0:  INC YPosFromCenter
        case 0x8D01: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB2A2:  INC YPosFromCenter
        case 0x8D04: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB2A4:  INC ScrollX
        case 0x8D07: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB2A6:  INC CharXPixelsLB
        case 0x8D0A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB2A8:  JSR DoSprites
        case 0x8D0D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB2AB:  LDA YPosFromCenter
        case 0x8D10: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB2AD:  CMP #$10
        case 0x8D12: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB2AF:  BNE RightColumnLoop
        case 0x8D14: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB2B1:  JSR WaitForNMI
        case 0x8D17: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB2B4:  INC ScrollX
        case 0x8D1A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB2B6:  BNE UpdtRNTBlock
        case 0x8D1C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB2B8:  LDA ActiveNmTbl
        case 0x8D1F: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB2BA:  EOR #$01
        case 0x8D21: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB2BC:  STA ActiveNmTbl
        case 0x8D24: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB2BE:  INC NTBlockX
        case 0x8D27: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB2C0:  LDA #$1F
        case 0x8D29: cycles_to_run -= 2; reg_A &= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB2C2:  AND NTBlockX
        case 0x8D2C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB2C4:  STA NTBlockX
        case 0x8D2F: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB2C6:  INC CharXPos
        case 0x8D32: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB2C8:  INC CharXPixelsLB
        case 0x8D35: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB2CA:  BNE +
        case 0x8D37: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB2CC:  INC CharXPixelsUB
        case 0x8D3A: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB2D1:  JMP DoCoveredArea
        case 0x8D3D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB2D4:  LDA NTBlockX
        case 0x8D40: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB2D6:  EOR #$10
        case 0x8D42: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB2D8:  AND #$1F
        case 0x8D44: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB2DA:  STA NTBlockX
        case 0x8D47: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB2DC:  LDA #$FA
        case 0x8D49: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB2DE:  STA YPosFromCenter
        case 0x8D4C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB2E0:  JSR WaitForNMI
        case 0x8D4F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB2E3:  LDA #$F9
        case 0x8D51: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB2E5:  STA XPosFromCenter
        case 0x8D54: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB2E7:  LDA #$00
        case 0x8D56: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB2E9:  STA BlkRemoveFlgs
        case 0x8D59: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB2EB:  STA PPUHorzVert
        case 0x8D5C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB2ED:  JSR ModMapBlock
        case 0x8D5F: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB2F0:  INC XPosFromCenter
        case 0x8D62: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB2F2:  INC XPosFromCenter
        case 0x8D65: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB2F4:  LDA XPosFromCenter
        case 0x8D68: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB2F6:  CMP #$09
        case 0x8D6A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB2F8:  BNE HorzDgnBlockLoop
        case 0x8D6C: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB2FA:  INC YPosFromCenter
        case 0x8D6F: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB2FC:  INC YPosFromCenter
        case 0x8D72: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB2FE:  LDA YPosFromCenter
        case 0x8D75: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB300:  CMP #$08
        case 0x8D77: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB302:  BNE HorzDgnRowLoop
        case 0x8D79: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB304:  JSR WaitForNMI
        case 0x8D7C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB307:  LDA ActiveNmTbl
        case 0x8D7F: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB309:  EOR #$01
        case 0x8D81: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB30B:  STA ActiveNmTbl
        case 0x8D84: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB30D:  RTS
        case 0x8D85: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB30E:  LDA NTBlockX
        case 0x8D88: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB310:  CLC
        case 0x8D89: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB311:  ADC #$10
        case 0x8D8B: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB313:  AND #$1F
        case 0x8D8D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB315:  STA NTBlockX
        case 0x8D90: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB317:  LDA #$FA
        case 0x8D92: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB319:  STA YPosFromCenter
        case 0x8D95: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB31B:  JSR DoSprites
        case 0x8D98: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB31E:  JSR WaitForNMI
        case 0x8D9B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB321:  LDA #$FA
        case 0x8D9D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB323:  STA XPosFromCenter
        case 0x8DA0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB325:  LDA #$00
        case 0x8DA2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB327:  STA BlkRemoveFlgs
        case 0x8DA5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB329:  STA PPUHorzVert
        case 0x8DA8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB32B:  JSR ModMapBlock
        case 0x8DAB: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB32E:  INC XPosFromCenter
        case 0x8DAE: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB330:  INC XPosFromCenter
        case 0x8DB1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB332:  LDA XPosFromCenter
        case 0x8DB4: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB334:  CMP #$08
        case 0x8DB6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB336:  BNE TickBlockLoop
        case 0x8DB8: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB338:  INC YPosFromCenter
        case 0x8DBB: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB33A:  INC YPosFromCenter
        case 0x8DBE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB33C:  LDA YPosFromCenter
        case 0x8DC1: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB33E:  CMP #$08
        case 0x8DC3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB340:  BNE TickColLoop
        case 0x8DC5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB342:  LDA ActiveNmTbl
        case 0x8DC8: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB344:  EOR #$01
        case 0x8DCA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB346:  STA ActiveNmTbl
        case 0x8DCD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB348:  JSR WaitForNMI
        case 0x8DD0: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB34B:  RTS
        case 0x8DD1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB34C:  JSR ChkRemovePopUp
        case 0x8DD4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB34F:  LDA FrameCounter
        case 0x8DD7: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB351:  AND #$0F
        case 0x8DD9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB353:  BEQ LeftSynced
        case 0x8DDB: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB355:  PLA
        case 0x8DDC: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB356:  PLA
        case 0x8DDD: cycles_to_run -= 3; reg_PC = 0x0000; return; // LB357:  JMP IdleUpdate
        case 0x8DE0: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB35A:  DEC _CharXPos
        case 0x8DE3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB35C:  JSR CheckCollision
        case 0x8DE6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB35F:  LDA MapType
        case 0x8DE9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB361:  CMP #MAP_DUNGEON
        case 0x8DEB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB363:  BNE UpdtLNonDungeon
        case 0x8DED: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB365:  JSR UpdtHorzDungeon
        case 0x8DF0: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB368:  DEC CharXPos
        case 0x8DF3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB36A:  LDA CharXPixelsLB
        case 0x8DF6: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LB36C:  SEC
        case 0x8DF7: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LB36D:  SBC #$08
        case 0x8DF9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB36F:  STA CharXPixelsLB
        case 0x8DFC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LB371:  BCS +
        case 0x8DFE: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB373:  DEC CharXPixelsUB
        case 0x8E01: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB378:  LDA CharXPixelsLB
        case 0x8E04: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LB37A:  SEC
        case 0x8E05: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LB37B:  SBC #$08
        case 0x8E07: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB37D:  STA CharXPixelsLB
        case 0x8E0A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LB37F:  BCS +
        case 0x8E0C: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB381:  DEC CharXPixelsUB
        case 0x8E0F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB386:  LDA #$EC
        case 0x8E11: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB388:  STA XPosFromCenter
        case 0x8E14: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB38A:  LDA #$F2
        case 0x8E16: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB38C:  STA YPosFromCenter
        case 0x8E19: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB38E:  JSR WaitForNMI
        case 0x8E1C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB391:  LDA #$00
        case 0x8E1E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB393:  STA BlkRemoveFlgs
        case 0x8E21: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB395:  STA PPUHorzVert
        case 0x8E24: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB397:  JSR ModMapBlock
        case 0x8E27: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB39A:  INC YPosFromCenter
        case 0x8E2A: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB39C:  INC YPosFromCenter
        case 0x8E2D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB39E:  LDA ScrollX
        case 0x8E30: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LB3A0:  SEC
        case 0x8E31: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LB3A1:  SBC #$01
        case 0x8E33: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB3A3:  STA ScrollX
        case 0x8E36: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x804E; return; } // LB3A5:  BCS UpdtLNTBlock
        case 0x8E38: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB3A7:  LDA ActiveNmTbl
        case 0x8E3B: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB3A9:  EOR #$01
        case 0x8E3D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB3AB:  STA ActiveNmTbl
        case 0x8E40: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB3AD:  LDA CharXPixelsLB
        case 0x8E43: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LB3AF:  SEC
        case 0x8E44: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LB3B0:  SBC #$01
        case 0x8E46: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB3B2:  STA CharXPixelsLB
        case 0x8E49: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LB3B4:  BCS +
        case 0x8E4B: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB3B6:  DEC CharXPixelsUB
        case 0x8E4E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB3BB:  LDA YPosFromCenter
        case 0x8E51: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB3BD:  CMP #$10
        case 0x8E53: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB3BF:  BNE UpdateLeftLoop
        case 0x8E55: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB3C1:  JSR WaitForNMI
        case 0x8E58: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB3C4:  DEC ScrollX
        case 0x8E5B: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB3C6:  DEC NTBlockX
        case 0x8E5E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB3C8:  LDA #$1F
        case 0x8E60: cycles_to_run -= 2; reg_A &= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB3CA:  AND NTBlockX
        case 0x8E63: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB3CC:  STA NTBlockX
        case 0x8E66: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB3CE:  DEC CharXPos
        case 0x8E69: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB3D0:  DEC CharXPixelsLB
        case 0x8E6C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB3D2:  JSR DoSprites
        case 0x8E6F: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB3D5:  JMP DoCoveredArea
        case 0x8E72: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB3D8:  JSR ChkRemovePopUp
        case 0x8E75: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB3DB:  LDA FrameCounter
        case 0x8E78: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB3DD:  AND #$0F
        case 0x8E7A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB3DF:  BEQ DownSynced
        case 0x8E7C: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB3E1:  PLA
        case 0x8E7D: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB3E2:  PLA
        case 0x8E7E: cycles_to_run -= 3; reg_PC = 0x0000; return; // LB3E3:  JMP IdleUpdate
        case 0x8E81: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB3E6:  INC _CharYPos
        case 0x8E84: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB3E8:  JSR CheckCollision
        case 0x8E87: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB3EB:  LDA MapType
        case 0x8E8A: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB3ED:  CMP #MAP_DUNGEON
        case 0x8E8C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB3EF:  BNE UpdtDNonDungeon
        case 0x8E8E: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB3F1:  INC CharYPos
        case 0x8E91: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB3F3:  JSR UpdtVertDungeon
        case 0x8E94: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB3F6:  LDA CharYPixelsLB
        case 0x8E97: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB3F8:  CLC
        case 0x8E98: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB3F9:  ADC #$08
        case 0x8E9A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB3FB:  STA CharYPixelsLB
        case 0x8E9D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LB3FD:  BCC +
        case 0x8E9F: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB3FF:  INC CharYPixelsUB
        case 0x8EA2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB404:  LDA CharYPixelsLB
        case 0x8EA5: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB406:  CLC
        case 0x8EA6: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB407:  ADC #$08
        case 0x8EA8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB409:  STA CharYPixelsLB
        case 0x8EAB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LB40B:  BCC +
        case 0x8EAD: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB40D:  INC CharYPixelsUB
        case 0x8EB0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB412:  JSR WaitForNMI
        case 0x8EB3: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB415:  INC ScrollY
        case 0x8EB6: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB417:  INC CharYPixelsLB
        case 0x8EB9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB419:  JSR DoSprites
        case 0x8EBC: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB41C:  LDA #$10
        case 0x8EBE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB41E:  STA YPosFromCenter
        case 0x8EC1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB420:  LDA #$EE
        case 0x8EC3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB422:  STA XPosFromCenter
        case 0x8EC6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB424:  LDA #$03
        case 0x8EC8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB426:  STA TileCounter
        case 0x8ECB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB428:  JSR WaitForNMI
        case 0x8ECE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB42B:  LDA #$0C
        case 0x8ED0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB42D:  STA BlkRemoveFlgs
        case 0x8ED3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB42F:  STA PPUHorzVert
        case 0x8ED6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB431:  JSR ModMapBlock
        case 0x8ED9: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB434:  INC XPosFromCenter
        case 0x8EDC: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB436:  INC XPosFromCenter
        case 0x8EDF: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB438:  DEC TileCounter
        case 0x8EE2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB43A:  BNE RowInnerLoop1
        case 0x8EE4: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB43C:  INC ScrollY
        case 0x8EE7: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB43E:  INC CharYPixelsLB
        case 0x8EEA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB440:  JSR DoSprites
        case 0x8EED: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB443:  LDA XPosFromCenter
        case 0x8EF0: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB445:  CMP #$12
        case 0x8EF2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB447:  BNE RowOuterLoop1
        case 0x8EF4: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB449:  LDA #$10
        case 0x8EF6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB44B:  STA YPosFromCenter
        case 0x8EF9: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB44D:  LDA #$EC
        case 0x8EFB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB44F:  STA XPosFromCenter
        case 0x8EFE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB451:  LDA #$05
        case 0x8F00: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB453:  STA TileCounter
        case 0x8F03: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB455:  JSR WaitForNMI
        case 0x8F06: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB458:  JSR ClearAttribByte
        case 0x8F09: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB45B:  LDA XPosFromCenter
        case 0x8F0C: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB45D:  CLC
        case 0x8F0D: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB45E:  ADC #$04
        case 0x8F0F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB460:  STA XPosFromCenter
        case 0x8F12: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB462:  DEC TileCounter
        case 0x8F15: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB464:  BNE AttribClearLoop1
        case 0x8F17: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB466:  INC ScrollY
        case 0x8F1A: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB468:  INC CharYPixelsLB
        case 0x8F1D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB46A:  JSR DoSprites
        case 0x8F20: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB46D:  LDA XPosFromCenter
        case 0x8F23: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB46F:  CMP #$14
        case 0x8F25: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB471:  BNE VertAttribLoop1
        case 0x8F27: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB473:  LDA #$10
        case 0x8F29: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB475:  STA YPosFromCenter
        case 0x8F2C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB477:  LDA #$EE
        case 0x8F2E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB479:  STA XPosFromCenter
        case 0x8F31: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB47B:  LDA #$03
        case 0x8F33: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB47D:  STA TileCounter
        case 0x8F36: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB47F:  JSR WaitForNMI
        case 0x8F39: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB482:  LDA #$03
        case 0x8F3B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB484:  STA BlkRemoveFlgs
        case 0x8F3E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB486:  STA PPUHorzVert
        case 0x8F41: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB488:  JSR ModMapBlock
        case 0x8F44: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB48B:  INC XPosFromCenter
        case 0x8F47: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB48D:  INC XPosFromCenter
        case 0x8F4A: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB48F:  DEC TileCounter
        case 0x8F4D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB491:  BNE VertBlockLoop1
        case 0x8F4F: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB493:  INC ScrollY
        case 0x8F52: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB495:  INC CharYPixelsLB
        case 0x8F55: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB497:  JSR DoSprites
        case 0x8F58: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB49A:  LDA XPosFromCenter
        case 0x8F5B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB49C:  CMP #$12
        case 0x8F5D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB49E:  BNE VertRowLoop1
        case 0x8F5F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB4A0:  JSR WaitForNMI
        case 0x8F62: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB4A3:  INC ScrollY
        case 0x8F65: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB4A5:  LDA ScrollY
        case 0x8F68: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB4A7:  CMP #$F0
        case 0x8F6A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB4A9:  BNE +
        case 0x8F6C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB4AB:  LDA #$00
        case 0x8F6E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB4AD:  STA ScrollY
        case 0x8F71: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB4B1:  LDA NTBlockY
        case 0x8F74: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB4B3:  CMP #$0F
        case 0x8F76: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB4B5:  BNE +
        case 0x8F78: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB4B7:  LDA #$00
        case 0x8F7A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB4B9:  STA NTBlockY
        case 0x8F7D: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB4BD:  INC CharYPixelsLB
        case 0x8F80: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB4BF:  BNE +
        case 0x8F82: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB4C1:  INC CharYPixelsUB
        case 0x8F85: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB4C6:  JMP DoCoveredArea
        case 0x8F88: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB4C9:  LDA NTBlockX
        case 0x8F8B: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB4CB:  CLC
        case 0x8F8C: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB4CC:  ADC #$10
        case 0x8F8E: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB4CE:  AND #$1F
        case 0x8F90: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB4D0:  STA NTBlockX
        case 0x8F93: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB4D2:  LDA #$FA
        case 0x8F95: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB4D4:  STA XPosFromCenter
        case 0x8F98: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB4D6:  JSR WaitForNMI
        case 0x8F9B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB4D9:  LDA #$F9
        case 0x8F9D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB4DB:  STA YPosFromCenter
        case 0x8FA0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB4DD:  LDA #$00
        case 0x8FA2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB4DF:  STA BlkRemoveFlgs
        case 0x8FA5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB4E1:  STA PPUHorzVert
        case 0x8FA8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB4E3:  JSR ModMapBlock
        case 0x8FAB: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB4E6:  INC YPosFromCenter
        case 0x8FAE: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB4E8:  INC YPosFromCenter
        case 0x8FB1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB4EA:  LDA YPosFromCenter
        case 0x8FB4: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB4EC:  CMP #$09
        case 0x8FB6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB4EE:  BNE VertDgnBlockLoop
        case 0x8FB8: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB4F0:  INC XPosFromCenter
        case 0x8FBB: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB4F2:  INC XPosFromCenter
        case 0x8FBE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB4F4:  LDA XPosFromCenter
        case 0x8FC1: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB4F6:  CMP #$08
        case 0x8FC3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB4F8:  BNE VertDgnRowLoop
        case 0x8FC5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB4FA:  JSR WaitForNMI
        case 0x8FC8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB4FD:  LDA ActiveNmTbl
        case 0x8FCB: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB4FF:  EOR #$01
        case 0x8FCD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB501:  STA ActiveNmTbl
        case 0x8FD0: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB503:  RTS
        case 0x8FD1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB504:  JSR ChkRemovePopUp
        case 0x8FD4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB507:  LDA FrameCounter
        case 0x8FD7: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB509:  AND #$0F
        case 0x8FD9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB50B:  BEQ UpSynced
        case 0x8FDB: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB50D:  PLA
        case 0x8FDC: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB50E:  PLA
        case 0x8FDD: cycles_to_run -= 3; reg_PC = 0x0000; return; // LB50F:  JMP IdleUpdate
        case 0x8FE0: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB512:  DEC _CharYPos
        case 0x8FE3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB514:  JSR CheckCollision
        case 0x8FE6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB517:  LDA MapType
        case 0x8FE9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB519:  CMP #MAP_DUNGEON
        case 0x8FEB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB51B:  BNE UpdtUNonDungeon
        case 0x8FED: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB51D:  JSR UpdtVertDungeon
        case 0x8FF0: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB520:  DEC CharYPos
        case 0x8FF3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB522:  LDA CharYPixelsLB
        case 0x8FF6: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LB524:  SEC
        case 0x8FF7: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LB525:  SBC #$08
        case 0x8FF9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB527:  STA CharYPixelsLB
        case 0x8FFC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LB529:  BCS +
        case 0x8FFE: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB52B:  DEC CharYPixelsUB
        case 0x9001: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB530:  LDA CharYPixelsLB
        case 0x9004: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LB532:  SEC
        case 0x9005: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LB533:  SBC #$08
        case 0x9007: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB535:  STA CharYPixelsLB
        case 0x900A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LB537:  BCS +
        case 0x900C: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB539:  DEC CharYPixelsUB
        case 0x900F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB53E:  JSR WaitForNMI
        case 0x9012: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB541:  DEC ScrollY
        case 0x9015: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB543:  LDA ScrollY
        case 0x9018: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB545:  CMP #$FF
        case 0x901A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB547:  BNE +
        case 0x901C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB549:  LDA #$EF
        case 0x901E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB54B:  STA ScrollY
        case 0x9021: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LB54F:  SEC
        case 0x9022: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LB550:  SBC #$01
        case 0x9024: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB552:  STA CharYPixelsLB
        case 0x9027: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LB554:  BCS +
        case 0x9029: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB556:  DEC CharYPixelsUB
        case 0x902C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB55B:  LDA #$F0
        case 0x902E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB55D:  STA YPosFromCenter
        case 0x9031: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB55F:  LDA #$EE
        case 0x9033: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB561:  STA XPosFromCenter
        case 0x9036: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB563:  LDA #$03
        case 0x9038: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB565:  STA TileCounter
        case 0x903B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB567:  JSR WaitForNMI
        case 0x903E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB56A:  LDA #$03
        case 0x9040: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB56C:  STA BlkRemoveFlgs
        case 0x9043: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB56E:  STA PPUHorzVert
        case 0x9046: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB570:  JSR ModMapBlock
        case 0x9049: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB573:  INC XPosFromCenter
        case 0x904C: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB575:  INC XPosFromCenter
        case 0x904F: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB577:  DEC TileCounter
        case 0x9052: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB579:  BNE VertBlockLoop2
        case 0x9054: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB57B:  DEC ScrollY
        case 0x9057: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB57D:  DEC CharYPixelsLB
        case 0x905A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB57F:  JSR DoSprites
        case 0x905D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB582:  LDA XPosFromCenter
        case 0x9060: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB584:  CMP #$12
        case 0x9062: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB586:  BNE VertRowLoop2
        case 0x9064: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB588:  LDA #$F0
        case 0x9066: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB58A:  STA YPosFromCenter
        case 0x9069: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB58C:  LDA #$EC
        case 0x906B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB58E:  STA XPosFromCenter
        case 0x906E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB590:  LDA #$05
        case 0x9070: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB592:  STA TileCounter
        case 0x9073: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB594:  JSR WaitForNMI
        case 0x9076: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB597:  JSR ClearAttribByte
        case 0x9079: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 2; return; // LB59A:  LDA $0F
        case 0x907B: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB59C:  CLC
        case 0x907C: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB59D:  ADC #$04
        case 0x907E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB59F:  STA XPosFromCenter
        case 0x9081: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB5A1:  DEC TileCounter
        case 0x9084: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB5A3:  BNE AttribClearLoop2
        case 0x9086: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB5A5:  DEC ScrollY
        case 0x9089: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB5A7:  DEC CharYPixelsLB
        case 0x908C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB5A9:  JSR DoSprites
        case 0x908F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB5AC:  LDA XPosFromCenter
        case 0x9092: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB5AE:  CMP #$14
        case 0x9094: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB5B0:  BNE VertAttribLoop2
        case 0x9096: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB5B2:  LDA #$F0
        case 0x9098: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB5B4:  STA YPosFromCenter
        case 0x909B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB5B6:  LDA #$EE
        case 0x909D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB5B8:  STA XPosFromCenter
        case 0x90A0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB5BA:  LDA #$03
        case 0x90A2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB5BC:  STA TileCounter
        case 0x90A5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB5BE:  JSR WaitForNMI
        case 0x90A8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB5C1:  LDA #$0C
        case 0x90AA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB5C3:  STA BlkRemoveFlgs
        case 0x90AD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB5C5:  STA PPUHorzVert
        case 0x90B0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB5C7:  JSR ModMapBlock
        case 0x90B3: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB5CA:  INC XPosFromCenter
        case 0x90B6: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB5CC:  INC XPosFromCenter
        case 0x90B9: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB5CE:  DEC TileCounter
        case 0x90BC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB5D0:  BNE VertBlockLoop3
        case 0x90BE: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB5D2:  DEC ScrollY
        case 0x90C1: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB5D4:  DEC CharYPixelsLB
        case 0x90C4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB5D6:  JSR DoSprites
        case 0x90C7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB5D9:  LDA XPosFromCenter
        case 0x90CA: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB5DB:  CMP #$12
        case 0x90CC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB5DD:  BNE VertRowLoop3
        case 0x90CE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB5DF:  JSR WaitForNMI
        case 0x90D1: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB5E2:  DEC ScrollY
        case 0x90D4: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB5E4:  DEC NTBlockY
        case 0x90D7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB5E6:  LDA NTBlockY
        case 0x90DA: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB5E8:  CMP #$FF
        case 0x90DC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB5EA:  BNE +
        case 0x90DE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB5EC:  LDA #$0E
        case 0x90E0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB5EE:  STA NTBlockY
        case 0x90E3: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB5F2:  DEC CharYPixelsLB
        case 0x90E6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB5F4:  JSR DoSprites
        case 0x90E9: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB5F7:  JMP DoCoveredArea
        case 0x90EC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB5FA:  LDA CharXPos
        case 0x90EF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB5FC:  STA _TargetX
        case 0x90F2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB5FE:  LDA CharYPos
        case 0x90F5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB600:  STA _TargetY
        case 0x90F8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB602:  JSR CheckCoveredArea
        case 0x90FB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB605:  LDA CoveredStsNext
        case 0x90FE: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LB607:  CMP CoverStatus
        case 0x9101: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB609:  BNE ToggleCoveredArea
        case 0x9103: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB60B:  RTS
        case 0x9104: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB60C:  STA CoverStatus
        case 0x9107: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB60E:  LDA CoverStatus
        case 0x910A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB610:  BEQ PrepToggleCover
        case 0x910C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB612:  JSR WaitForNMI
        case 0x910F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB615:  LDA #$F0
        case 0x9111: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB617:  STA SpriteRAM
        case 0x9114: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB61A:  STA SpriteRAM+4
        case 0x9117: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB61D:  STA SpriteRAM+8
        case 0x911A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB620:  STA SpriteRAM+12
        case 0x911D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB623:  LDA NTBlockX
        case 0x9120: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB625:  CLC
        case 0x9121: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB626:  ADC #$10
        case 0x9123: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB628:  AND #$1F
        case 0x9125: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB62A:  STA NTBlockX
        case 0x9128: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB62C:  LDA #$F2
        case 0x912A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB62E:  STA YPosFromCenter
        case 0x912D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB630:  JSR WaitForNMI
        case 0x9130: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB633:  LDA #$F0
        case 0x9132: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB635:  STA XPosFromCenter
        case 0x9135: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB637:  LDA #$00
        case 0x9137: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB639:  STA BlkRemoveFlgs
        case 0x913A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB63B:  STA PPUHorzVert
        case 0x913D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB63D:  JSR ModMapBlock
        case 0x9140: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB640:  INC XPosFromCenter
        case 0x9143: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB642:  INC XPosFromCenter
        case 0x9146: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB644:  LDA XPosFromCenter
        case 0x9149: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB646:  CMP #$00
        case 0x914B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB648:  BNE CoverLeftLoop
        case 0x914D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB64A:  JSR WaitForNMI
        case 0x9150: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB64D:  LDA #$00
        case 0x9152: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB64F:  STA BlkRemoveFlgs
        case 0x9155: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB651:  STA PPUHorzVert
        case 0x9158: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB653:  JSR ModMapBlock
        case 0x915B: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB656:  INC XPosFromCenter
        case 0x915E: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB658:  INC XPosFromCenter
        case 0x9161: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB65A:  LDA XPosFromCenter
        case 0x9164: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB65C:  CMP #$10
        case 0x9166: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB65E:  BNE CoverRightLoop
        case 0x9168: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB660:  INC YPosFromCenter
        case 0x916B: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB662:  INC YPosFromCenter
        case 0x916E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB664:  LDA YPosFromCenter
        case 0x9171: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB666:  CMP #$10
        case 0x9173: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB668:  BNE CoverOuterLoop
        case 0x9175: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB66A:  JSR WaitForNMI
        case 0x9178: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB66D:  LDA #$01
        case 0x917A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB66F:  STA FrameCounter
        case 0x917D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB671:  LDA #NPC_STOP
        case 0x917F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB673:  STA StopNPCMove
        case 0x9182: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB675:  JSR DoSprites
        case 0x9185: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB678:  LDA #NPC_MOVE
        case 0x9187: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB67A:  STA StopNPCMove
        case 0x918A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB67C:  LDA ActiveNmTbl
        case 0x918D: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB67E:  EOR #$01
        case 0x918F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB680:  STA ActiveNmTbl
        case 0x9192: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB682:  LDA #$EE
        case 0x9194: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB684:  STA XPosFromCenter
        case 0x9197: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB686:  JSR WaitForNMI
        case 0x919A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB689:  LDA #$F2
        case 0x919C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB68B:  STA YPosFromCenter
        case 0x919F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB68D:  LDA #$00
        case 0x91A1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB68F:  STA BlkRemoveFlgs
        case 0x91A4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB691:  STA PPUHorzVert
        case 0x91A7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB693:  JSR ModMapBlock
        case 0x91AA: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB696:  INC YPosFromCenter
        case 0x91AD: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB698:  INC YPosFromCenter
        case 0x91B0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB69A:  LDA YPosFromCenter
        case 0x91B3: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB69C:  CMP #$02
        case 0x91B5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB69E:  BNE CoverHiColumnLoop
        case 0x91B7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB6A0:  JSR WaitForNMI
        case 0x91BA: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB6A3:  LDA #$00
        case 0x91BC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB6A5:  STA BlkRemoveFlgs
        case 0x91BF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB6A7:  STA PPUHorzVert
        case 0x91C2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB6A9:  JSR ModMapBlock
        case 0x91C5: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB6AC:  INC YPosFromCenter
        case 0x91C8: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB6AE:  INC YPosFromCenter
        case 0x91CB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB6B0:  LDA YPosFromCenter
        case 0x91CE: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB6B2:  CMP #$10
        case 0x91D0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB6B4:  BNE CoverLoColumnLoop
        case 0x91D2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB6B6:  LDA XPosFromCenter
        case 0x91D5: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB6B8:  CLC
        case 0x91D6: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB6B9:  ADC #$22
        case 0x91D8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB6BB:  STA XPosFromCenter
        case 0x91DB: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB6BD:  CMP #$32
        case 0x91DD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB6BF:  BNE ColumnOuterLoop
        case 0x91DF: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB6C1:  RTS
        case 0x91E0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB6C2:  STA NPCCounter
        case 0x91E3: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LB6C4:  LDA CharSpriteTblPtr
        case 0x91E6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB6C7:  STA GenPtr22LB
        case 0x91E9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB6C9:  LDA CharSpriteTblPtr+1
        case 0x91EC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB6CC:  STA GenPtr22UB
        case 0x91EF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB6D0:  BEQ SprtFacingEnd
        case 0x91F1: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB6D2:  INC GenPtr22UB
        case 0x91F4: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB6D4:  DEC NPCCounter
        case 0x91F7: cycles_to_run -= 3; reg_PC = 0x0000; return; // LB6D6:  JMP -
        case 0x91FA: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB6D9:  RTS
        case 0x91FB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB6DA:  LDA EnNumber
        case 0x91FE: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB6DC:  CMP #EN_DRAGONLORD2
        case 0x9200: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB6DE:  BNE SprtChkFrameCntr
        case 0x9202: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB6E0:  RTS
        case 0x9203: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB6E1:  LDA FrameCounter
        case 0x9206: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB6E3:  AND #$0F
        case 0x9208: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB6E5:  BNE ChkGotGwaelin
        case 0x920A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB6E7:  LDA CharLeftRight
        case 0x920D: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB6E9:  CLC
        case 0x920E: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB6EA:  ADC #$08
        case 0x9210: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB6EC:  STA CharLeftRight
        case 0x9213: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB6EE:  LDA PlayerFlags
        case 0x9216: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB6F0:  AND #F_GOT_GWAELIN
        case 0x9218: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB6F2:  BEQ ChkPlayerWeapons
        case 0x921A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB6F4:  LDA #$C0
        case 0x921C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB6F6:  STA GenByte3C
        case 0x921F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB6F8:  BNE GetPlayerAnim
        case 0x9221: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB6FA:  LDA #$80
        case 0x9223: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB6FC:  STA GenByte3C
        case 0x9226: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB6FE:  LDA EqippedItems
        case 0x9229: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB700:  AND #WP_WEAPONS
        case 0x922B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB702:  BEQ ChkPlayerShields
        case 0x922D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB704:  LDA #$90
        case 0x922F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB706:  STA GenByte3C
        case 0x9232: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB708:  LDA EqippedItems
        case 0x9235: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB70A:  AND #SH_SHIELDS
        case 0x9237: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB70C:  BEQ GetPlayerAnim
        case 0x9239: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB70E:  LDA #$20
        case 0x923B: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB710:  ORA GenByte3C
        case 0x923E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB712:  STA GenByte3C
        case 0x9241: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB714:  LDA CharLeftRight
        case 0x9244: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB716:  AND #$08
        case 0x9246: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB718:  ORA GenByte3C
        case 0x9249: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LB71A:  TAY
        case 0x924A: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB71B:  LDX #$00
        case 0x924C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB71D:  LDA #$6F
        case 0x924E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB71F:  STA CharYScrPos
        case 0x9251: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB721:  LDA CharDirection
        case 0x9254: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB724:  JSR SprtFacingBaseAddr
        case 0x9257: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB727:  LDA #$80
        case 0x9259: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB729:  STA CharXScrPos
        case 0x925C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB72B:  LDA WinBufRAM+$1D0
        case 0x925F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB72E:  CMP #$FF
        case 0x9261: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB730:  BEQ +
        case 0x9263: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB732:  LDA #$F0
        case 0x9265: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB734:  BNE PlyrSetXCord
        case 0x9267: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB738:  STA SpriteRAM,X
        case 0x926A: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB73B:  INX
        case 0x926B: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LB73C:  LDA (GenPtr22),Y
        case 0x926D: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB73E:  STA SpriteRAM,X
        case 0x9270: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB741:  INX
        case 0x9271: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB742:  INY
        case 0x9272: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LB743:  LDA (GenPtr22),Y
        case 0x9274: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB745:  STA SpriteRAM,X
        case 0x9277: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB748:  INX
        case 0x9278: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB749:  INY
        case 0x9279: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB74A:  LDA CharXScrPos
        case 0x927C: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB74C:  STA SpriteRAM,X
        case 0x927F: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB74F:  INX
        case 0x9280: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB750:  LDA CharXScrPos
        case 0x9283: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB752:  CLC
        case 0x9284: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB753:  ADC #$08
        case 0x9286: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB755:  STA CharXScrPos
        case 0x9289: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB757:  CMP #$90
        case 0x928B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB759:  BNE GetPlayerTileLoop2
        case 0x928D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB75B:  LDA CharYScrPos
        case 0x9290: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB75D:  CLC
        case 0x9291: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB75E:  ADC #$08
        case 0x9293: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB760:  STA CharYScrPos
        case 0x9296: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB762:  CMP #$7F
        case 0x9298: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB764:  BNE GetPlayerTileLoop1
        case 0x929A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB766:  LDA NPCUpdateCntr
        case 0x929D: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB768:  AND #$F0
        case 0x929F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB76A:  BEQ UpdateNPCs1
        case 0x92A1: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB76C:  JMP UpdateNPCCounter
        case 0x92A4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB76F:  LDA NPCUpdateCntr
        case 0x92A7: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LB771:  ASL
        case 0x92A8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB772:  STA GenByte3C
        case 0x92AB: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LB774:  ASL
        case 0x92AC: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LB775:  ADC GenByte3C
        case 0x92AF: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LB777:  TAX
        case 0x92B0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB778:  LDA #$02
        case 0x92B2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB77A:  STA NPCLoopCounter
        case 0x92B5: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB77C:  LDA NPCXPos,X
        case 0x92B8: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB77E:  AND #$1F
        case 0x92BA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB780:  BNE +
        case 0x92BC: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB782:  LDA NPCYPos,X
        case 0x92BF: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB784:  AND #$1F
        case 0x92C1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB786:  BNE +
        case 0x92C3: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB788:  JMP EndNPCMoveLoop
        case 0x92C6: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB78D:  AND #$0F
        case 0x92C8: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB78F:  CMP #$01
        case 0x92CA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB791:  BEQ +
        case 0x92CC: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB793:  JMP NPCProcessCont
        case 0x92CF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB798:  BEQ SetNPCDir
        case 0x92D1: cycles_to_run -= 2; { uint16_t a = addr_abs_x(0x0000, nullptr); bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LB79A:  ASL NPCYPos,X
        case 0x92D4: cycles_to_run -= 2; { uint16_t a = addr_abs_x(0x0000, nullptr); bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LB79C:  LSR NPCYPos,X
        case 0x92D7: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB79E:  JMP EndNPCMoveLoop
        case 0x92DA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB7A1:  JSR UpdateRandNum
        case 0x92DD: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB7A4:  LDA NPCYPos,X
        case 0x92E0: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB7A6:  AND #$9F
        case 0x92E2: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB7A8:  STA NPCYPos,X
        case 0x92E5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB7AA:  LDA RandNumUB
        case 0x92E8: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB7AC:  AND #$60
        case 0x92EA: cycles_to_run -= 2; reg_A |= bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB7AE:  ORA NPCYPos,X
        case 0x92ED: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB7B0:  STA NPCYPos,X
        case 0x92F0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB7B2:  JSR GetNPCPosCopy
        case 0x92F3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB7B5:  JSR ChkNPCWndwBlock
        case 0x92F6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB7B8:  LDA NPCOnScreen
        case 0x92F9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB7BA:  BEQ +
        case 0x92FB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB7BC:  LDA WindowBlock
        case 0x92FE: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB7BE:  CMP #$FF
        case 0x9300: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB7C0:  BNE HaltNPCMoveCalcs
        case 0x9302: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB7C5:  LDA NPCYPos,X
        case 0x9305: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB7C7:  AND #$60
        case 0x9307: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB7C9:  BNE +
        case 0x9309: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB7CB:  DEC ThisNPCYPos
        case 0x930C: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB7CD:  JMP ChkNPCMapLimit
        case 0x930F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB7D2:  BNE +
        case 0x9311: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB7D4:  INC ThisNPCXPos
        case 0x9314: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB7D6:  JMP ChkNPCMapLimit
        case 0x9317: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB7DB:  BNE +
        case 0x9319: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB7DD:  INC ThisNPCYPos
        case 0x931C: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB7DF:  JMP ChkNPCMapLimit
        case 0x931F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB7E4:  LDA MapHeight
        case 0x9322: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LB7E6:  CMP ThisNPCYPos
        case 0x9325: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LB7E8:  BCS +
        case 0x9327: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB7EA:  JMP HaltNPCMoveCalcs
        case 0x932A: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LB7EF:  CMP ThisNPCXPos
        case 0x932D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x804E; return; } // LB7F1:  BCC HaltNPCMoveCalcs
        case 0x932F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB7F3:  JSR ChkNPCWndwBlock
        case 0x9332: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB7F6:  LDA NPCOnScreen
        case 0x9335: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB7F8:  BEQ +
        case 0x9337: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB7FA:  LDA WindowBlock
        case 0x933A: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB7FC:  CMP #$FF
        case 0x933C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB7FE:  BNE HaltNPCMoveCalcs
        case 0x933E: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LB802:  CMP CharXPos
        case 0x9341: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB804:  BNE +
        case 0x9343: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB806:  LDA ThisNPCYPos
        case 0x9346: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LB808:  CMP CharYPos
        case 0x9349: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB80A:  BEQ HaltNPCMoveCalcs
        case 0x934B: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LB80E:  CMP _CharXPos
        case 0x934E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB810:  BNE +
        case 0x9350: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB812:  LDA ThisNPCYPos
        case 0x9353: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LB814:  CMP _CharYPos
        case 0x9356: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB816:  BNE +
        case 0x9358: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB818:  JMP HaltNPCMoveCalcs
        case 0x935B: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB81D:  LDA _NPCXPos,Y
        case 0x935E: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB820:  AND #$1F
        case 0x9360: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LB822:  CMP ThisNPCXPos
        case 0x9363: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB824:  BNE +
        case 0x9365: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB826:  LDA _NPCYPos,Y
        case 0x9368: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB829:  AND #$1F
        case 0x936A: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LB82B:  CMP ThisNPCYPos
        case 0x936D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB82D:  BNE +
        case 0x936F: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB82F:  JMP HaltNPCMoveCalcs
        case 0x9372: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB833:  INY
        case 0x9373: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB834:  INY
        case 0x9374: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LB835:  CPY #$3C
        case 0x9376: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB837:  BNE NPCCollideLoop
        case 0x9378: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB839:  LDA GenByte3D
        case 0x937B: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LB83B:  PHA
        case 0x937C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB83C:  LDA ThisNPCXPos
        case 0x937F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB83E:  STA XTarget
        case 0x9382: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB840:  LDA ThisNPCYPos
        case 0x9385: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB842:  STA YTarget
        case 0x9388: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB844:  JSR GetBlockID
        case 0x938B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB847:  JSR HasCoverData
        case 0x938E: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB84A:  PLA
        case 0x938F: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LB84B:  CMP CoveredStsNext
        case 0x9392: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB84D:  BEQ +
        case 0x9394: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB84F:  JMP HaltNPCMoveCalcs
        case 0x9397: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB854:  CMP #BLK_FFIELD
        case 0x9399: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LB856:  BCC +
        case 0x939B: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB858:  JMP HaltNPCMoveCalcs
        case 0x939E: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB85D:  ORA #$80
        case 0x93A0: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB85F:  STA NPCYPos,X
        case 0x93A3: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB861:  LDA NPCYPos,X
        case 0x93A6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0x0000; return; } // LB863:  BMI +
        case 0x93A8: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB865:  JMP EndNPCMoveLoop
        case 0x93AB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB86A:  BEQ +
        case 0x93AD: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB86C:  JMP EndNPCMoveLoop
        case 0x93B0: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB871:  AND #$60
        case 0x93B2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB873:  BNE +
        case 0x93B4: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB875:  LDA NPCMidPos,X
        case 0x93B7: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB877:  AND #$0F
        case 0x93B9: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LB879:  SEC
        case 0x93BA: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LB87A:  SBC #$01
        case 0x93BC: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB87C:  AND #$0F
        case 0x93BE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB87E:  STA GenByte3C
        case 0x93C1: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB880:  LDA NPCMidPos,X
        case 0x93C4: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB882:  AND #$F0
        case 0x93C6: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB884:  ORA GenByte3C
        case 0x93C9: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB886:  STA NPCMidPos,X
        case 0x93CC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB888:  LDA GenByte3C
        case 0x93CF: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB88A:  CMP #$0F
        case 0x93D1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB88C:  BNE EndNPCMoveLoop
        case 0x93D3: cycles_to_run -= 5; { uint16_t a = addr_abs_x(0x0000, nullptr); uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB88E:  DEC NPCYPos,X
        case 0x93D6: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB890:  JMP EndNPCMoveLoop
        case 0x93D9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB895:  BNE +
        case 0x93DB: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB897:  LDA NPCMidPos,X
        case 0x93DE: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB899:  AND #$F0
        case 0x93E0: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB89B:  CLC
        case 0x93E1: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB89C:  ADC #$10
        case 0x93E3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB89E:  STA GenByte3C
        case 0x93E6: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB8A0:  LDA NPCMidPos,X
        case 0x93E9: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB8A2:  AND #$0F
        case 0x93EB: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB8A4:  ORA GenByte3C
        case 0x93EE: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB8A6:  STA NPCMidPos,X
        case 0x93F1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB8A8:  LDA GenByte3C
        case 0x93F4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB8AA:  BNE EndNPCMoveLoop
        case 0x93F6: cycles_to_run -= 5; { uint16_t a = addr_abs_x(0x0000, nullptr); uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB8AC:  INC NPCXPos,X
        case 0x93F9: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB8AE:  JMP EndNPCMoveLoop
        case 0x93FC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB8B3:  BNE +
        case 0x93FE: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB8B5:  LDA NPCMidPos,X
        case 0x9401: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB8B7:  AND #$0F
        case 0x9403: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB8B9:  CLC
        case 0x9404: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB8BA:  ADC #$01
        case 0x9406: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB8BC:  AND #$0F
        case 0x9408: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB8BE:  STA GenByte3C
        case 0x940B: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB8C0:  LDA NPCMidPos,X
        case 0x940E: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB8C2:  AND #$F0
        case 0x9410: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB8C4:  ORA GenByte3C
        case 0x9413: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB8C6:  STA NPCMidPos,X
        case 0x9416: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB8C8:  LDA GenByte3C
        case 0x9419: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB8CA:  BNE EndNPCMoveLoop
        case 0x941B: cycles_to_run -= 5; { uint16_t a = addr_abs_x(0x0000, nullptr); uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB8CC:  INC NPCYPos,X
        case 0x941E: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB8CE:  JMP EndNPCMoveLoop
        case 0x9421: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB8D3:  AND #$F0
        case 0x9423: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LB8D5:  SEC
        case 0x9424: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LB8D6:  SBC #$10
        case 0x9426: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB8D8:  STA GenByte3C
        case 0x9429: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB8DA:  LDA NPCMidPos,X
        case 0x942C: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB8DC:  AND #$0F
        case 0x942E: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB8DE:  ORA GenByte3C
        case 0x9431: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB8E0:  STA NPCMidPos,X
        case 0x9434: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB8E2:  LDA GenByte3C
        case 0x9437: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB8E4:  CMP #$F0
        case 0x9439: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB8E6:  BNE EndNPCMoveLoop
        case 0x943B: cycles_to_run -= 5; { uint16_t a = addr_abs_x(0x0000, nullptr); uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB8E8:  DEC NPCXPos,X
        case 0x943E: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB8EA:  INX
        case 0x943F: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB8EB:  INX
        case 0x9440: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB8EC:  INX
        case 0x9441: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB8ED:  DEC NPCLoopCounter
        case 0x9444: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB8EF:  BEQ UpdateNPCs2
        case 0x9446: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB8F1:  JMP NPCMoveLoop
        case 0x9449: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB8F4:  LDX #$00
        case 0x944B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB8F6:  LDA #$10
        case 0x944D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB8F8:  STA SpriteRAMIndex
        case 0x9450: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB8FA:  LDA NPCXPos,X
        case 0x9453: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB8FC:  AND #$1F
        case 0x9455: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB8FE:  BNE CalcNPCSprites
        case 0x9457: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB900:  LDA NPCYPos,X
        case 0x945A: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB902:  AND #$1F
        case 0x945C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB904:  BNE CalcNPCSprites
        case 0x945E: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB906:  JMP NextNPCSprites
        case 0x9461: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB909:  JSR NPCXScreenCord
        case 0x9464: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB90C:  LDA NPCXPixelsLB
        case 0x9467: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB90E:  CLC
        case 0x9468: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB90F:  ADC #$07
        case 0x946A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB911:  STA NPCXPixelsLB
        case 0x946D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB913:  LDA NPCXPixelsUB
        case 0x9470: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB915:  ADC #$00
        case 0x9472: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB917:  BEQ ChkNPCYLoc
        case 0x9474: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB919:  CMP #$01
        case 0x9476: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB91B:  BEQ +
        case 0x9478: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB91D:  JMP NextNPCSprites
        case 0x947B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB922:  CMP #$07
        case 0x947D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LB924:  BCC ChkNPCYLoc
        case 0x947F: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB926:  JMP NextNPCSprites
        case 0x9482: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB929:  JSR NPCYScreenCord
        case 0x9485: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB92C:  LDA NPCYPixelsLB
        case 0x9488: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB92E:  CLC
        case 0x9489: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB92F:  ADC #$11
        case 0x948B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB931:  STA NPCYPixelsLB
        case 0x948E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB933:  LDA NPCYPixelsUB
        case 0x9491: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB935:  ADC #$00
        case 0x9493: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB937:  BEQ +
        case 0x9495: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB939:  JMP NextNPCSprites
        case 0x9498: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB93F:  JSR ChkNPCWndwBlock
        case 0x949B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB942:  LDA NPCOnScreen
        case 0x949E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB944:  BEQ +
        case 0x94A0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB946:  LDA WindowBlock
        case 0x94A3: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB948:  CMP #$FF
        case 0x94A5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB94A:  BEQ +
        case 0x94A7: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB94C:  JMP NextNPCSprites
        case 0x94AA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB951:  STA GenByte3C
        case 0x94AD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB953:  LDA ThisNPCYPos
        case 0x94B0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB955:  STA GenByte3E
        case 0x94B3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB957:  JSR CheckCoveredArea
        case 0x94B6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB95A:  LDA CoveredStsNext
        case 0x94B9: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LB95C:  CMP CoverStatus
        case 0x94BC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB95E:  BEQ +
        case 0x94BE: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB960:  JMP NextNPCSprites
        case 0x94C1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB966:  STA GenByte3C
        case 0x94C4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB968:  JSR NPCXScreenCord
        case 0x94C7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB96B:  JSR NPCYScreenCord
        case 0x94CA: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LB96E:  LDY SpriteRAMIndex
        case 0x94CD: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LB970:  STX NPCROMIndex
        case 0x94D0: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LB972:  LDX GenByte3C
        case 0x94D3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB974:  LDA #$00
        case 0x94D5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB976:  STA CharYScrPos
        case 0x94D8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB978:  LDA #$00
        case 0x94DA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB97A:  STA NPCSpriteXOfst
        case 0x94DD: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB97C:  LDA NPCXPixelsLB
        case 0x94E0: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB97E:  CLC
        case 0x94E1: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LB97F:  ADC NPCSpriteXOfst
        case 0x94E4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB981:  STA ThisNPCXPos
        case 0x94E7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB983:  LDA NPCXPixelsUB
        case 0x94EA: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB985:  ADC #$00
        case 0x94EC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB987:  BNE NextNPCSprite
        case 0x94EE: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LB989:  TYA
        case 0x94EF: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LB98A:  STX NPCOffset
        case 0x94F2: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LB98C:  TAX
        case 0x94F3: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LB98D:  LDY NPCROMIndex
        case 0x94F6: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB98F:  LDA _NPCYPos,Y
        case 0x94F9: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB992:  AND #$60
        case 0x94FB: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LB994:  ASL
        case 0x94FC: cycles_to_run -= 2; reg_A = cpu_rol(reg_A); reg_PC += 1; return; // LB995:  ROL
        case 0x94FD: cycles_to_run -= 2; reg_A = cpu_rol(reg_A); reg_PC += 1; return; // LB996:  ROL
        case 0x94FE: cycles_to_run -= 2; reg_A = cpu_rol(reg_A); reg_PC += 1; return; // LB997:  ROL
        case 0x94FF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LB998:  JSR SprtFacingBaseAddr
        case 0x9502: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LB99B:  LDY NPCOffset
        case 0x9505: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB99D:  LDA ThisNPCXPos
        case 0x9508: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB99F:  STA SpriteRAM+3,X
        case 0x950B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB9A2:  LDA NPCYPixelsLB
        case 0x950E: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB9A4:  CLC
        case 0x950F: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LB9A5:  ADC NPCSpriteYOfst
        case 0x9512: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB9A7:  STA SpriteRAM,X
        case 0x9515: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LB9AA:  LDA (GenPtr22),Y
        case 0x9517: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB9AC:  STA SpriteRAM+1,X
        case 0x951A: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB9AF:  INY
        case 0x951B: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LB9B0:  LDA (GenPtr22),Y
        case 0x951D: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LB9B2:  DEY
        case 0x951E: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB9B3:  STA SpriteRAM+2,X
        case 0x9521: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LB9B6:  TYA
        case 0x9522: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LB9B7:  STX GenByte22
        case 0x9525: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LB9B9:  TAX
        case 0x9526: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LB9BA:  LDY GenByte22
        case 0x9529: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB9BC:  INY
        case 0x952A: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB9BD:  INY
        case 0x952B: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB9BE:  INY
        case 0x952C: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB9BF:  INY
        case 0x952D: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB9C0:  INX
        case 0x952E: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB9C1:  INX
        case 0x952F: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LB9C2:  TYA
        case 0x9530: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB9C3:  BEQ NPCLoopDone
        case 0x9532: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB9C5:  LDA NPCSpriteXOfst
        case 0x9535: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB9C7:  CLC
        case 0x9536: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB9C8:  ADC #$08
        case 0x9538: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB9CA:  STA NPCSpriteXOfst
        case 0x953B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB9CC:  CMP #$10
        case 0x953D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB9CE:  BNE NPCSpriteColLoop
        case 0x953F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB9D0:  LDA NPCSpriteYOfst
        case 0x9542: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB9D2:  CLC
        case 0x9543: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB9D3:  ADC #$08
        case 0x9545: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB9D5:  STA CharYScrPos
        case 0x9548: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB9D7:  CMP #$10
        case 0x954A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LB9D9:  BNE NPCSpriteRowLoop
        case 0x954C: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LB9DB:  LDX NPCROMIndex
        case 0x954F: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LB9DD:  STY SpriteRAMIndex
        case 0x9552: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB9DF:  INX
        case 0x9553: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB9E0:  INX
        case 0x9554: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB9E1:  INX
        case 0x9555: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LB9E2:  CPX #$3C
        case 0x9557: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB9E4:  BEQ NPCLoopDone
        case 0x9559: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB9E6:  JMP NPCSpritesLoop
        case 0x955C: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LB9E9:  LDY SpriteRAMIndex
        case 0x955F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB9EB:  LDA #$F0
        case 0x9561: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LB9ED:  CPY #$00
        case 0x9563: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LB9EF:  BEQ UpdateNPCCounter
        case 0x9565: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB9F1:  STA SpriteRAM,Y
        case 0x9568: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB9F4:  INY
        case 0x9569: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB9F5:  INY
        case 0x956A: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB9F6:  INY
        case 0x956B: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB9F7:  INY
        case 0x956C: cycles_to_run -= 3; reg_PC = 0x804E; return; // LB9F8:  JMP HideSpriteLoop
        case 0x956F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB9FB:  LDA FrameCounter
        case 0x9572: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB9FD:  AND #$0F
        case 0x9574: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB9FF:  BEQ +
        case 0x9576: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LBA01:  RTS
        case 0x9577: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBA04:  CMP #$FF
        case 0x9579: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LBA06:  BEQ SpritesEnd
        case 0x957B: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LBA08:  INC NPCUpdateCntr
        case 0x957E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBA0A:  LDA NPCUpdateCntr
        case 0x9581: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBA0C:  CMP #$05
        case 0x9583: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LBA0E:  BNE SpritesEnd
        case 0x9585: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBA10:  LDA #$00
        case 0x9587: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA12:  STA NPCUpdateCntr
        case 0x958A: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LBA14:  RTS
        case 0x958B: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LBA15:  LDA NPCXPos,X
        case 0x958E: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBA17:  AND #$1F
        case 0x9590: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA19:  STA ThisNPCXPos
        case 0x9593: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LBA1B:  LDA NPCYPos,X
        case 0x9596: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBA1D:  AND #$1F
        case 0x9598: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA1F:  STA ThisNPCYPos
        case 0x959B: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LBA21:  RTS
        case 0x959C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBA22:  LDA #$00
        case 0x959E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA24:  STA NPCOnScreen
        case 0x95A1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBA26:  LDA ThisNPCXPos
        case 0x95A4: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LBA28:  SEC
        case 0x95A5: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LBA29:  SBC CharXPos
        case 0x95A8: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LBA2B:  CLC
        case 0x95A9: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LBA2C:  ADC #$08
        case 0x95AB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA2E:  STA XPosFromLeft
        case 0x95AE: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBA30:  CMP #$10
        case 0x95B0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LBA32:  BCC +
        case 0x95B2: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LBA34:  RTS
        case 0x95B3: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LBA37:  SEC
        case 0x95B4: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LBA38:  SBC CharYPos
        case 0x95B7: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LBA3A:  CLC
        case 0x95B8: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LBA3B:  ADC #$07
        case 0x95BA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA3D:  STA YPosFromTop
        case 0x95BD: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBA3F:  CMP #$0F
        case 0x95BF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LBA41:  BCC +
        case 0x95C1: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LBA43:  RTS
        case 0x95C2: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LBA47:  LDY #$00
        case 0x95C4: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LBA49:  LDA (PPUBufPtr),Y
        case 0x95C6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA4B:  STA WindowBlock
        case 0x95C9: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBA4D:  LDA #$FF
        case 0x95CB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA4F:  STA NPCOnScreen
        case 0x95CE: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LBA51:  RTS
        case 0x95CF: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LBA52:  LDA NPCXPos,X
        case 0x95D2: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBA54:  AND #$1F
        case 0x95D4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA56:  STA NPCXPixelsUB
        case 0x95D7: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LBA58:  LDA NPCMidPos,X
        case 0x95DA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA5A:  STA NPCXPixelsLB
        case 0x95DD: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LBA5C:  LSR NPCXPixelsUB
        case 0x95E0: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LBA5E:  ROR NPCXPixelsLB
        case 0x95E3: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LBA60:  LSR NPCXPixelsUB
        case 0x95E6: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LBA62:  ROR NPCXPixelsLB
        case 0x95E9: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LBA64:  LSR NPCXPixelsUB
        case 0x95EC: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LBA66:  ROR NPCXPixelsLB
        case 0x95EF: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LBA68:  LSR NPCXPixelsUB
        case 0x95F2: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LBA6A:  ROR NPCXPixelsLB
        case 0x95F5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBA6C:  LDA NPCXPixelsLB
        case 0x95F8: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LBA6E:  SEC
        case 0x95F9: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LBA6F:  SBC CharXPixelsLB
        case 0x95FC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA71:  STA NPCXPixelsLB
        case 0x95FF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBA73:  LDA NPCXPixelsUB
        case 0x9602: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LBA75:  SBC CharXPixelsUB
        case 0x9605: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA77:  STA NPCXPixelsUB
        case 0x9608: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBA79:  LDA NPCXPixelsLB
        case 0x960B: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBA7B:  EOR #$80
        case 0x960D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA7D:  STA NPCXPixelsLB
        case 0x9610: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0x0000; return; } // LBA7F:  BMI +
        case 0x9612: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LBA81:  INC NPCXPixelsUB
        case 0x9615: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LBA84:  LDA NPCYPos,X
        case 0x9618: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBA86:  AND #$1F
        case 0x961A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA88:  STA NPCYPixelsUB
        case 0x961D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBA8A:  LDA #$00
        case 0x961F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA8C:  STA NPCYPixelsLB
        case 0x9622: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LBA8E:  LSR NPCYPixelsUB
        case 0x9625: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LBA90:  ROR NPCYPixelsLB
        case 0x9628: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LBA92:  LSR NPCYPixelsUB
        case 0x962B: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LBA94:  ROR NPCYPixelsLB
        case 0x962E: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LBA96:  LSR NPCYPixelsUB
        case 0x9631: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LBA98:  ROR NPCYPixelsLB
        case 0x9634: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_lsr(bus_read(a))); } reg_PC += 3; return; // LBA9A:  LSR NPCYPixelsUB
        case 0x9637: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LBA9C:  ROR NPCYPixelsLB
        case 0x963A: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LBA9E:  LDA NPCMidPos,X
        case 0x963D: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBAA0:  AND #$0F
        case 0x963F: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBAA2:  ORA NPCYPixelsLB
        case 0x9642: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBAA4:  STA NPCYPixelsLB
        case 0x9645: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LBAA6:  SEC
        case 0x9646: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LBAA7:  SBC CharYPixelsLB
        case 0x9649: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBAA9:  STA NPCYPixelsLB
        case 0x964C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBAAB:  LDA NPCYPixelsUB
        case 0x964F: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LBAAD:  SBC CharYPixelsUB
        case 0x9652: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBAAF:  STA NPCYPixelsUB
        case 0x9655: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBAB1:  LDA NPCYPixelsLB
        case 0x9658: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LBAB3:  CLC
        case 0x9659: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LBAB4:  ADC #$6F
        case 0x965B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBAB6:  STA NPCYPixelsLB
        case 0x965E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LBAB8:  BCC +
        case 0x9660: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LBABA:  INC NPCYPixelsUB
        case 0x9663: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBABD:  LDA #PAL_LOAD_BG
        case 0x9665: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBABF:  STA LoadBGPal
        case 0x9668: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBAC1:  JSR WaitForNMI
        case 0x966B: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LBAC4:  LDA TownPalPtr
        case 0x966E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBAC7:  STA BGPalPtrLB
        case 0x9671: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBAC9:  LDA TownPalPtr+1
        case 0x9674: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBACC:  STA BGPalPtrUB
        case 0x9677: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LBACE:  LDA BlackPalPtr
        case 0x967A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBAD1:  STA SprtPalPtrLB
        case 0x967D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBAD3:  LDA BlackPalPtr+1
        case 0x9680: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBAD6:  STA SprtPalPtrUB
        case 0x9683: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBAD8:  JSR PalFadeOut
        case 0x9686: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBADB:  LDA #NT_NAMETBL0_LB
        case 0x9688: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBADD:  STA PPUAddrLB
        case 0x968B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBADF:  LDA #NT_NAMETBL0_UB
        case 0x968D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBAE1:  STA PPUAddrUB
        case 0x9690: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBAE3:  LDA #$1E
        case 0x9692: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBAE5:  STA BufByteCntr
        case 0x9695: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBAE7:  LDA #TL_BLANK_TILE1
        case 0x9697: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBAE9:  STA PPUDataByte
        case 0x969A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LBAEB:  JSR LoadBufferRows
        case 0x969D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBAEE:  LDA #$00
        case 0x969F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBAF0:  STA PPUDataByte
        case 0x96A2: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBAF2:  LDA #$02
        case 0x96A4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBAF4:  STA BufByteCntr
        case 0x96A7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LBAF6:  JSR LoadBufferRows
        case 0x96AA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBAF9:  JSR WaitForNMI
        case 0x96AD: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBAFC:  LDA #$FF
        case 0x96AF: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LBAFE:  LDY #$00
        case 0x96B1: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LBB03:  STA WinBufRAM+$100,Y
        case 0x96B4: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LBB06:  STA WinBufRAM+$200,Y
        case 0x96B7: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LBB09:  STA WinBufRAM+$300,Y
        case 0x96BA: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LBB0C:  DEY
        case 0x96BB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LBB0D:  BNE -
        case 0x96BD: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBB0F:  LDA #%00000000
        case 0x96BF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBB11:  STA PPUControl1
        case 0x96C2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBB14:  JSR Bank0ToCHR0
        case 0x96C5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBB17:  JSR Bank0ToCHR1
        case 0x96C8: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LBB1A:  LDY #$00
        case 0x96CA: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x804E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LBB1C:  LDA EndBossBGTiles,Y
        case 0x96CD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBB1F:  STA PPUDataByte
        case 0x96D0: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LBB21:  INY
        case 0x96D1: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x804E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LBB22:  LDA EndBossBGTiles,Y
        case 0x96D4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBB25:  STA PPUAddrLB
        case 0x96D7: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LBB27:  INY
        case 0x96D8: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x804E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LBB28:  LDA EndBossBGTiles,Y
        case 0x96DB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBB2B:  STA PPUAddrUB
        case 0x96DE: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LBB2D:  INY
        case 0x96DF: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LBB2E:  CPY #$3C
        case 0x96E1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x804E; return; } // LBB30:  BCS EBTileBufLoad
        case 0x96E3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBB32:  LDA PPUAddrLB
        case 0x96E6: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LBB34:  SEC
        case 0x96E7: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LBB35:  SBC #$02
        case 0x96E9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBB37:  STA PPUAddrLB
        case 0x96EC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x804E; return; } // LBB39:  BCS EBTileBufLoad
        case 0x96EE: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LBB3B:  DEC PPUAddrUB
        case 0x96F1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LBB3D:  JSR LoadBufferByte
        case 0x96F4: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LBB40:  CPY #$45
        case 0x96F6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LBB42:  BNE EBTileLoadLoop
        case 0x96F8: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LBB44:  LDX #$00
        case 0x96FA: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x804E, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LBB46:  LDA EndBossSPTiles,X
        case 0x96FD: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBB49:  CMP #$FF
        case 0x96FF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LBB4B:  BNE EBSpriteSave
        case 0x9701: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LBB4D:  LDA EndBossSPTiles+1,X
        case 0x9704: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBB50:  CMP #$FF
        case 0x9706: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LBB52:  BEQ EBSpriteLoadDone
        case 0x9708: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LBB54:  STA SpriteRAM,X
        case 0x970B: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LBB57:  INX
        case 0x970C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LBB58:  BNE EBSpriteLoadLoop
        case 0x970E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBB5A:  JSR WaitForNMI
        case 0x9711: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBB5D:  LDA #%00011000
        case 0x9713: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBB5F:  STA PPUControl1
        case 0x9716: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LBB62:  LDA EndBossPal2Ptr
        case 0x9719: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBB65:  STA SprtPalPtrLB
        case 0x971C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBB67:  LDA EndBossPal2Ptr+1
        case 0x971F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBB6A:  STA SprtPalPtrUB
        case 0x9722: cycles_to_run -= 2; reg_A = bus_read(0x804E); update_nz(reg_A); reg_PC += 3; return; // LBB6C:  LDA EndBossPal1Ptr
        case 0x9725: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBB6F:  STA BGPalPtrLB
        case 0x9728: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBB71:  LDA EndBossPal1Ptr+1
        case 0x972B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBB74:  STA BGPalPtrUB
        case 0x972E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBB76:  LDA #$00
        case 0x9730: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBB78:  STA ScrollX
        case 0x9733: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBB7A:  STA ScrollY
        case 0x9736: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBB7C:  STA ActiveNmTbl
        case 0x9739: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBB7E:  LDA #$08
        case 0x973B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBB80:  STA NTBlockX
        case 0x973E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBB82:  LDA #$07
        case 0x9740: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBB84:  STA NTBlockY
        case 0x9743: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBB86:  LDA #SFX_FIRE
        case 0x9745: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LBB88:  BRK
        case 0x9746: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBB8B:  LDA #EN_DRAGONLORD2
        case 0x9748: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBB8D:  STA EnNumber
        case 0x974B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBB8F:  JSR WaitForNMI
        case 0x974E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBB92:  LDA #PAL_LOAD_BG
        case 0x9750: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBB94:  STA LoadBGPal
        case 0x9753: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBB96:  JSR PalFadeIn
        case 0x9756: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBB99:  JSR PalFadeIn
        case 0x9759: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBB9C:  JSR PalFadeIn
        case 0x975C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBB9F:  JSR PalFadeIn
        case 0x975F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBBA2:  JSR WaitForNMI
        case 0x9762: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LBBA5:  LDX #$28
        case 0x9764: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LBBAA:  DEX
        case 0x9765: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LBBAB:  BNE -
        case 0x9767: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LBBAD:  RTS
        case 0x9768: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBBAE:  JSR WaitForNMI
        case 0x976B: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LBBB1:  LDY #$20
        case 0x976D: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LBBB6:  DEY
        case 0x976E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LBBB7:  BNE -
        case 0x9770: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LBBB9:  DEC BufByteCntr
        case 0x9773: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LBBBB:  BNE LoadBufferRows
        case 0x9775: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LBBBD:  RTS
        case 0x9776: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBBBE:  LDA NTBlockY
        case 0x9779: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LBBC0:  ASL
        case 0x977A: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LBBC1:  CLC
        case 0x977B: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LBBC2:  ADC YPosFromCenter
        case 0x977E: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LBBC4:  CLC
        case 0x977F: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LBBC5:  ADC #$1E
        case 0x9781: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBBC7:  STA DivNum1LB
        case 0x9784: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBBC9:  LDA #$1E
        case 0x9786: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBBCB:  STA DivNum2
        case 0x9789: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBBCD:  JSR ByteDivide
        case 0x978C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 2; return; // LBBD0:  LDA $40
        case 0x978E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 2; return; // LBBD2:  STA $3E
        case 0x9790: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBBD4:  LDA NTBlockX
        case 0x9793: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LBBD6:  ASL
        case 0x9794: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LBBD7:  CLC
        case 0x9795: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 2; return; // LBBD8:  ADC $0F
        case 0x9797: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBBDA:  AND #$3F
        case 0x9799: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 2; return; // LBBDC:  STA $3C
        case 0x979B: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LBBDE:  RTS
        case 0x979C: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LBBDF:  TYA
        case 0x979D: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LBBE0:  PHA
        case 0x979E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBBE1:  LDA PPUAddrLB
        case 0x97A1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBBE3:  STA GenPtr22LB
        case 0x97A4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBBE5:  LDA PPUAddrUB
        case 0x97A7: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LBBE7:  SEC
        case 0x97A8: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LBBE8:  SBC #$1C
        case 0x97AA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBBEA:  STA GenPtr22UB
        case 0x97AD: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LBBEC:  LDY #$00
        case 0x97AF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBBEE:  LDA PPUDataByte
        case 0x97B2: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LBBF0:  STA (GenPtr22),Y
        case 0x97B4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBBF2:  JSR AddPPUBufEntry
        case 0x97B7: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LBBF5:  PLA
        case 0x97B8: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LBBF6:  TAY
        case 0x97B9: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LBBF7:  RTS
        case 0x97BA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBD5B:  JSR WaitForNMI
        case 0x97BD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x804E; return; } // LBD5E:  JSR LoadIntroPals
        case 0x97C0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBD61:  JSR ClearSpriteRAM
        case 0x97C3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBD64:  LDA #%00001000
        case 0x97C5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD66:  STA PPUControl0
        case 0x97C8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBD69:  LDA IntroGFXTblPtr+1
        case 0x97CB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD6C:  STA DatPntrlUB
        case 0x97CE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBD6E:  LDA IntroGFXTblPtr
        case 0x97D1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD71:  STA DatPntr1LB
        case 0x97D4: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBD73:  LDA #NT_NAMETBL0_UB
        case 0x97D6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD75:  STA PPUAddrUB
        case 0x97D9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD77:  STA PPUAddress
        case 0x97DC: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBD7A:  LDA #NT_NAMETBL0_LB
        case 0x97DE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD7C:  STA PPUAddrLB
        case 0x97E1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD7E:  STA PPUAddress
        case 0x97E4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBD81:  JSR IntroGFXPtrInc
        case 0x97E7: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBD84:  CMP #END_TXT_END
        case 0x97E9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x804E; return; } // LBD86:  BEQ ChkNTEnd
        case 0x97EB: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBD88:  CMP #END_RPT
        case 0x97ED: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x804E; return; } // LBD8A:  BNE IncToNextByte
        case 0x97EF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBD8C:  JSR IntroGFXPtrInc
        case 0x97F2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD8F:  STA RepeatCounter
        case 0x97F5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBD91:  JSR IntroGFXPtrInc
        case 0x97F8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD94:  STA PPUDataByte
        case 0x97FB: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LBD99:  DEC RepeatCounter
        case 0x97FE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LBD9B:  BNE -
        case 0x9800: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LBD9D:  BEQ IntroGFXLoop
        case 0x9802: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBD9F:  STA PPUDataByte
        case 0x9805: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBDA1:  JSR LoadGFXAndInc
        case 0x9808: cycles_to_run -= 3; reg_PC = 0x0000; return; // LBDA4:  JMP IntroGFXLoop
        case 0x980B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBDA7:  LDA PPUAddrUB
        case 0x980E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBDA9:  CMP #NT_NAMETBL1_UB
        case 0x9810: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LBDAB:  BNE IntroGFXLoop
        case 0x9812: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBDAD:  LDA #%10001000
        case 0x9814: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBDAF:  STA PPUControl0
        case 0x9817: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LBDB2:  RTS
        case 0x9818: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBDB3:  LDA PPUDataByte
        case 0x981B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBDB5:  STA PPUIOReg
        case 0x981E: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LBDB8:  INC PPUAddrLB
        case 0x9821: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LBDBA:  BNE +
        case 0x9823: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LBDBC:  INC PPUAddrUB
        case 0x9826: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LBDBF:  LDY #$00
        case 0x9828: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LBDC1:  LDA (DatPntr1),Y
        case 0x982A: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LBDC3:  INC DatPntr1LB
        case 0x982D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LBDC5:  BNE +
        case 0x982F: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LBDC7:  INC DatPntrlUB
        case 0x9832: cycles_to_run -= 2; reg_P |= FLAG_I; reg_PC += 1; return; // LBFD8:  SEI
        case 0x9833: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LBFD9:  INC MMCReset1
        case 0x9836: cycles_to_run -= 3; reg_PC = 0x0000; return; // LBFDC:  JMP _DoReset
        default: reg_PC++; return;
    }
}
}
