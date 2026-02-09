#include "cpu_shared.h"
namespace Bank01 {
void execute_at(uint16_t pc) {
    switch (pc) {
        case 0x8000: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // L8028:  PHA
        case 0x8001: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // L8029:  TXA
        case 0x8002: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // L802A:  PHA
        case 0x8003: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // L802B:  TYA
        case 0x8004: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // L802C:  PHA
        case 0x8005: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // L802D:  LDX #MCTL_NOIS_SW
        case 0x8007: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // L802F:  LDY #MCTL_SQ2_HW
        case 0x8009: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // L8031:  LDA SFXActive
        case 0x800C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // L8033:  BEQ +
        case 0x800E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // L8035:  LDA NoteOffset
        case 0x8011: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // L8037:  PHA
        case 0x8012: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L8038:  LDA #$00
        case 0x8014: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L803A:  STA NoteOffset
        case 0x8017: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // L803C:  JSR GetNextNote
        case 0x801A: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // L803F:  TAX
        case 0x801B: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // L8040:  PLA
        case 0x801C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L8041:  STA NoteOffset
        case 0x801F: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // L8043:  TXA
        case 0x8020: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // L8044:  BNE +
        case 0x8022: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L8046:  LDA #%00000101
        case 0x8024: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L8048:  STA APUCommonCntrl0
        case 0x8027: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L804B:  LDA #%00001111
        case 0x8029: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L804D:  STA APUCommonCntrl0
        case 0x802C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // L8050:  LDA SQ2Config
        case 0x802F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L8052:  STA SQ2Cntrl0
        case 0x8032: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L8055:  LDA #%00001000
        case 0x8034: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L8057:  STA SQ2Cntrl1
        case 0x8037: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L805A:  LDA #%00110000
        case 0x8039: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L805C:  STA NoiseCntrl0
        case 0x803C: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // L8061:  CLC
        case 0x803D: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // L8062:  ADC Tempo
        case 0x8040: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L8064:  STA TempoCntr
        case 0x8043: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // L8066:  BCC SoundUpdateEnd
        case 0x8045: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // L8068:  SBC #$96
        case 0x8047: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L806A:  STA TempoCntr
        case 0x804A: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // L806C:  LDX #MCTL_TRI_SW
        case 0x804C: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // L806E:  LDY #MCTL_TRI_HW
        case 0x804E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // L8070:  JSR GetNextNote
        case 0x8051: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // L8073:  LDX #MCTL_SQ2_SW
        case 0x8053: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // L8075:  LDY #MCTL_SQ2_HW
        case 0x8055: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // L8077:  LDA SFXActive
        case 0x8058: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // L8079:  BEQ +
        case 0x805A: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // L807B:  LDY #MCTL_DMC_HW
        case 0x805C: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // L8080:  LDX #MCTL_SQ1_SW
        case 0x805E: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // L8082:  LDY #MCTL_SQ1_HW
        case 0x8060: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // L8084:  JSR GetNextNote
        case 0x8063: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // L8087:  LDY #$00
        case 0x8065: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // L8089:  LDA (SQ1IndexLB),Y
        case 0x8067: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L808B:  STA MusicTrigger
        case 0x806A: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // L808E:  PLA
        case 0x806B: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // L808F:  TAY
        case 0x806C: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // L8090:  PLA
        case 0x806D: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // L8091:  TAX
        case 0x806E: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // L8092:  PLA
        case 0x806F: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // L8093:  RTS
        case 0x8070: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // L8094:  LDA SQ1ReturnLB,X
        case 0x8073: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // L8096:  STA SQ1IndexLB,X
        case 0x8076: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // L8098:  LDA SQ1ReturnUB,X
        case 0x8079: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // L809A:  STA SQ1IndexUB,X
        case 0x807C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // L809C:  BNE ProcessAudioByte
        case 0x807E: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // L809E:  CLC
        case 0x807F: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // L809F:  ADC NoteOffset
        case 0x8082: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // L80A1:  ASL
        case 0x8083: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // L80A2:  STX MusicTemp
        case 0x8086: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // L80A4:  TAX
        case 0x8087: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x802A, nullptr)); update_nz(reg_A); reg_PC += 3; return; // L80A5:  LDA MusicalNotesTbl,X
        case 0x808A: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // L80A8:  STA SQ1Cntrl2,Y
        case 0x808D: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // L80AB:  LDA MusicalNotesTbl+1,X
        case 0x8090: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // L80AE:  STA SQ1Cntrl3,Y
        case 0x8093: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // L80B1:  LDX MusicTemp
        case 0x8096: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // L80B3:  CPX #MCTL_NOIS_SW
        case 0x8098: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // L80B5:  BEQ ProcessAudioByte
        case 0x809A: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // L80B7:  LDA ChannelQuiet,X
        case 0x809D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // L80B9:  BEQ ProcessAudioByte
        case 0x809F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // L80BB:  BNE UpdateChnlUsage
        case 0x80A1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // L80BD:  JSR GetAudioData
        case 0x80A4: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // L80C0:  STA ChannelQuiet,X
        case 0x80A7: cycles_to_run -= 3; reg_PC = 0x802A; return; // L80C2:  JMP ProcessAudioByte
        case 0x80AA: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L80C5:  LDA #$00
        case 0x80AC: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // L80C7:  STA ChannelQuiet,X
        case 0x80AF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // L80C9:  BEQ ProcessAudioByte
        case 0x80B1: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // L80CB:  LDA ChannelLength,X
        case 0x80B4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // L80CD:  BEQ UpdateReturn
        case 0x80B6: cycles_to_run -= 5; { uint16_t a = addr_abs_x(0x0000, nullptr); uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // L80CF:  DEC ChannelLength,X
        case 0x80B9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // L80D1:  BNE UpdateReturn
        case 0x80BB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // L80D3:  JSR GetAudioData
        case 0x80BE: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // L80D6:  CMP #MCTL_JUMP
        case 0x80C0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // L80D8:  BEQ MusicJump
        case 0x80C2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // L80DA:  BCS ChangeTempo
        case 0x80C4: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // L80DC:  CMP #MCTL_NO_OP
        case 0x80C6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // L80DE:  BEQ ProcessAudioByte
        case 0x80C8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // L80E0:  BCS MusicReturn
        case 0x80CA: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // L80E2:  CMP #MCTL_CNTRL1
        case 0x80CC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // L80E4:  BEQ ChnlCntrl1
        case 0x80CE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // L80E6:  BCS ChnlCntrl0
        case 0x80D0: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // L80E8:  CMP #MCTL_NOISE_VOL
        case 0x80D2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // L80EA:  BEQ NoiseVolume
        case 0x80D4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // L80EC:  BCS GetNoteOffset
        case 0x80D6: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // L80EE:  CMP #MCTL_END_SPACE
        case 0x80D8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // L80F0:  BEQ EndChnlQuietTime
        case 0x80DA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // L80F2:  BCS ChnlQuietTime
        case 0x80DC: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // L80F4:  CMP #MCTL_NOISE_CFG
        case 0x80DE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // L80F6:  BCS LoadNoise
        case 0x80E0: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // L80F8:  CMP #MCTL_NOTE
        case 0x80E2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // L80FA:  BCS LoadMusicNote
        case 0x80E4: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // L80FC:  STA ChannelLength,X
        case 0x80E7: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // L80FE:  RTS
        case 0x80E8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // L80FF:  JSR GetAudioData
        case 0x80EB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L8102:  STA Tempo
        case 0x80EE: cycles_to_run -= 3; reg_PC = 0x802A; return; // L8104:  JMP ProcessAudioByte
        case 0x80F1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // L8107:  JSR GetAudioData
        case 0x80F4: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // L810A:  PHA
        case 0x80F5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // L810B:  JSR GetAudioData
        case 0x80F8: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // L810E:  PHA
        case 0x80F9: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // L810F:  LDA SQ1IndexLB,X
        case 0x80FC: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // L8111:  STA SQ1ReturnLB,X
        case 0x80FF: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // L8113:  LDA SQ1IndexUB,X
        case 0x8102: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // L8115:  STA SQ1ReturnUB,X
        case 0x8105: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // L8117:  PLA
        case 0x8106: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // L8118:  STA SQ1IndexUB,X
        case 0x8109: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // L811A:  PLA
        case 0x810A: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // L811B:  STA SQ1IndexLB,X
        case 0x810D: cycles_to_run -= 3; reg_PC = 0x802A; return; // L811D:  JMP ProcessAudioByte
        case 0x8110: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // L8120:  JSR GetAudioData
        case 0x8113: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // L8123:  CPX #$02
        case 0x8115: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // L8125:  BNE +
        case 0x8117: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L8127:  STA SQ2Config
        case 0x811A: cycles_to_run -= 3; reg_PC = 0x802A; return; // L812C:  JMP ProcessAudioByte
        case 0x811D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // L812F:  JSR GetAudioData
        case 0x8120: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L8132:  STA NoiseCntrl0
        case 0x8123: cycles_to_run -= 3; reg_PC = 0x802A; return; // L8135:  JMP ProcessAudioByte
        case 0x8126: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L8138:  AND #$0F
        case 0x8128: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L813A:  STA NoiseCntrl2
        case 0x812B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L813D:  LDA #%00001000
        case 0x812D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L813F:  STA NoiseCntrl3
        case 0x8130: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // L8142:  BNE ProcessAudioByte
        case 0x8132: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // L8144:  JSR GetAudioData
        case 0x8135: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L8147:  STA NoteOffset
        case 0x8138: cycles_to_run -= 3; reg_PC = 0x802A; return; // L8149:  JMP ProcessAudioByte
        case 0x813B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // L814C:  JSR GetAudioData
        case 0x813E: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // L814F:  STA SQ1Cntrl1,Y
        case 0x8141: cycles_to_run -= 3; reg_PC = 0x802A; return; // L8152:  JMP ProcessAudioByte
        case 0x8144: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_x(0x0000)); update_nz(reg_A); reg_PC += 2; return; // L8155:  LDA (SQ1IndexLB,X)
        case 0x8146: cycles_to_run -= 5; { uint16_t a = addr_abs_x(0x0000, nullptr); uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // L8157:  INC SQ1IndexLB,X
        case 0x8149: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // L8159:  BNE +
        case 0x814B: cycles_to_run -= 5; { uint16_t a = addr_abs_x(0x0000, nullptr); uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // L815B:  INC SQ1IndexUB,X
        case 0x814E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // L815E:  JSR WaitForNMI
        case 0x8151: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L8161:  LDA #MCTL_NO_OP
        case 0x8153: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // L8163:  LDX #MCTL_SQ1_SW
        case 0x8155: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(read_pointer_indexed_x(0x0000))); reg_PC += 2; return; // L8165:  CMP (SQ1IndexLB,X)
        case 0x8157: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // L8167:  BEQ +
        case 0x8159: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // L8169:  LDX #MCTL_NOIS_SW
        case 0x815B: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(read_pointer_indexed_x(0x0000))); reg_PC += 2; return; // L816B:  CMP (SQ1IndexLB,X)
        case 0x815D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // L816D:  BEQ +
        case 0x815F: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // L816F:  LDX #MCTL_TRI_SW
        case 0x8161: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(read_pointer_indexed_x(0x0000))); reg_PC += 2; return; // L8171:  CMP (SQ1IndexLB,X)
        case 0x8163: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // L8173:  BNE WaitForMusicEnd
        case 0x8165: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // L8178:  JSR WaitForNMI
        case 0x8168: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L817B:  LDA #$00
        case 0x816A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L817D:  STA DMCCntrl0
        case 0x816D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L8180:  STA APUCommonCntrl1
        case 0x8170: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L8183:  STA APUCommonCntrl0
        case 0x8173: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L8186:  STA SQ1Length
        case 0x8176: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L8188:  STA SQ2Length
        case 0x8179: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L818A:  STA TRILength
        case 0x817C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L818C:  STA SFXActive
        case 0x817F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L818E:  LDA #%00001111
        case 0x8181: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L8190:  STA APUCommonCntrl0
        case 0x8184: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L8193:  LDA #$FF
        case 0x8186: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L8195:  STA Tempo
        case 0x8189: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L8197:  LDA #$08
        case 0x818B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L8199:  STA SQ1Cntrl1
        case 0x818E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L819C:  STA SQ2Cntrl1
        case 0x8191: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // L819F:  RTS
        case 0x8192: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // L81A0:  LDX #$FF
        case 0x8194: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // L81A2:  STX SndEngineStat
        case 0x8197: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // L81A5:  TAX
        case 0x8198: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0x0000; return; } // L81A6:  BMI DoSFX
        case 0x819A: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // L81A8:  ASL
        case 0x819B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L81A9:  STA MusicTemp
        case 0x819E: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // L81AB:  ASL
        case 0x819F: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // L81AC:  ADC MusicTemp
        case 0x81A2: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // L81AE:  ADC #$04
        case 0x81A4: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // L81B0:  TAY
        case 0x81A5: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // L81B1:  LDX #$04
        case 0x81A7: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // L81B3:  LDA MscStrtIndxTbl+1,Y
        case 0x81AA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // L81B6:  BNE +
        case 0x81AC: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // L81B8:  LDA MscStrtIndxTbl+1,X
        case 0x81AF: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // L81BB:  STA SQ1IndexUB,X
        case 0x81B2: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x802A, nullptr)); update_nz(reg_A); reg_PC += 3; return; // L81BD:  LDA MscStrtIndxTbl,X
        case 0x81B5: cycles_to_run -= 3; reg_PC = 0x0000; return; // L81C0:  JMP ++
        case 0x81B8: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x802A, nullptr)); update_nz(reg_A); reg_PC += 3; return; // L81C5:  LDA MscStrtIndxTbl,Y
        case 0x81BB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L81CA:  LDA #$01
        case 0x81BD: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // L81CC:  STA ChannelLength,X
        case 0x81C0: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // L81CE:  DEY
        case 0x81C1: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // L81CF:  DEY
        case 0x81C2: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // L81D0:  DEX
        case 0x81C3: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // L81D1:  DEX
        case 0x81C4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x802A; return; } // L81D2:  BPL ChnlInitLoop
        case 0x81C6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L81D4:  LDA #$00
        case 0x81C8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L81D6:  STA NoteOffset
        case 0x81CB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L81D8:  STA SQ1Quiet
        case 0x81CE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L81DA:  STA SQ2Quiet
        case 0x81D1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L81DC:  STA TRIQuiet
        case 0x81D4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L81DE:  STA SndEngineStat
        case 0x81D7: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // L81E1:  RTS
        case 0x81D8: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // L81E2:  ASL
        case 0x81D9: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // L81E3:  TAX
        case 0x81DA: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L81E4:  LDA #$01
        case 0x81DC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L81E6:  STA SFXActive
        case 0x81DF: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // L81E8:  LDA SFXStrtIndxTbl,X
        case 0x81E2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L81EB:  STA NoisIndexLB
        case 0x81E5: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // L81ED:  LDA SFXStrtIndxTbl+1,X
        case 0x81E8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L81F0:  STA NoisIndexUB
        case 0x81EB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L81F2:  LDA #$08
        case 0x81ED: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L81F4:  STA SQ2Cntrl1
        case 0x81F0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L81F7:  LDA #$30
        case 0x81F2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L81F9:  STA SQ2Cntrl0
        case 0x81F5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L81FC:  STA NoiseCntrl0
        case 0x81F8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L81FF:  LDA #$00
        case 0x81FA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L8201:  STA SndEngineStat
        case 0x81FD: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // L8204:  RTS
        case 0x81FE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L9354:  LDA #%00000000
        case 0x8200: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L9356:  STA PPUControl1
        case 0x8203: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // L9359:  JSR ClearPPU
        case 0x8206: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L935C:  LDA #%00011000
        case 0x8208: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L935E:  STA PPUControl1
        case 0x820B: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // L9361:  RTS
        case 0x820C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L9362:  LDA #MSC_NOSOUND
        case 0x820E: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // L9364:  BRK
        case 0x820F: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // L9367:  BRK
        case 0x8210: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L936A:  LDA #$00
        case 0x8212: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L936C:  STA ExpLB
        case 0x8215: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L936E:  STA ScrollX
        case 0x8218: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L9370:  STA ScrollY
        case 0x821B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L9372:  STA ActiveNmTbl
        case 0x821E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L9374:  STA NPCUpdateCntr
        case 0x8221: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // L9376:  LDX #$3B
        case 0x8223: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // L937A:  DEX
        case 0x8224: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // L937B:  BPL -
        case 0x8226: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L937D:  LDA #EN_DRAGONLORD2
        case 0x8228: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L937F:  STA EnNumber
        case 0x822B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // L9381:  JSR ClearSpriteRAM
        case 0x822E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // L9384:  JSR WaitForNMI
        case 0x8231: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // L9387:  JSR EndGameClearPPU
        case 0x8234: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L938A:  LDA #$FF
        case 0x8236: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L938C:  STA HitPoints
        case 0x8239: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // L938E:  BRK
        case 0x823A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // L9391:  JSR Dowindow
        case 0x823D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // L9395:  JSR DoDialogHiBlock
        case 0x8240: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // L9399:  RTS
        case 0x8241: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // L939A:  JSR WaitForNMI
        case 0x8244: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L939D:  LDA #MSC_END
        case 0x8246: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // L939F:  BRK
        case 0x8247: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // L93A2:  BRK
        case 0x8248: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // L93A5:  BRK
        case 0x8249: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // L93A8:  JSR ClearSpriteRAM
        case 0x824C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L93AB:  LDA #%00000000
        case 0x824E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L93AD:  STA PPUControl1
        case 0x8251: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // L93B0:  JSR Bank0ToCHR0
        case 0x8254: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L93B3:  LDA #$00
        case 0x8256: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L93B5:  STA ExpLB
        case 0x8259: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L93B7:  STA ScrollX
        case 0x825C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L93B9:  STA ScrollY
        case 0x825F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L93BB:  STA ActiveNmTbl
        case 0x8262: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L93BD:  LDA #EN_DRAGONLORD2
        case 0x8264: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L93BF:  STA EnNumber
        case 0x8267: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // L93C1:  JSR WaitForNMI
        case 0x826A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // L93C4:  JSR EndGameClearPPU
        case 0x826D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L93C7:  LDA #$23
        case 0x826F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L93C9:  STA PPUAddrUB
        case 0x8272: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L93CB:  LDA #$C8
        case 0x8274: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L93CD:  STA PPUAddrLB
        case 0x8277: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L93CF:  LDA #$55
        case 0x8279: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L93D1:  STA PPUDataByte
        case 0x827C: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // L93D3:  LDY #$08
        case 0x827E: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // L93D8:  DEY
        case 0x827F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // L93D9:  BNE -
        case 0x8281: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // L93DB:  LDA #$AA
        case 0x8283: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L93DD:  STA PPUDataByte
        case 0x8286: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // L93DF:  LDY #$20
        case 0x8288: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // L93E4:  DEY
        case 0x8289: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // L93E5:  BNE -
        case 0x828B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // L93E7:  JSR WaitForNMI
        case 0x828E: cycles_to_run -= 2; reg_A = bus_read(0x802A); update_nz(reg_A); reg_PC += 3; return; // L93EA:  LDA EndCreditDatPtr
        case 0x8291: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L93ED:  STA DatPntr1LB
        case 0x8294: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // L93EF:  LDA EndCreditDatPtr+1
        case 0x8297: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L93F2:  STA DatPntrlUB
        case 0x829A: cycles_to_run -= 3; reg_PC = 0x802A; return; // L93F4:  JMP RollCredits
        case 0x829D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // L93F7:  JSR EndGameClearPPU
        case 0x82A0: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // L93FA:  LDY #$00
        case 0x82A2: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // L93FC:  LDA (DatPntr1),Y
        case 0x82A4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L93FE:  STA PPUAddrLB
        case 0x82A7: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // L9400:  INY
        case 0x82A8: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // L9401:  LDA (DatPntr1),Y
        case 0x82AA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L9403:  STA PPUAddrUB
        case 0x82AD: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // L9405:  LDY #$02
        case 0x82AF: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // L9407:  LDA (DatPntr1),Y
        case 0x82B1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L9409:  STA PPUDataByte
        case 0x82B4: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // L940B:  CMP #END_RPT
        case 0x82B6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // L940D:  BNE DoNonRepeatedValue
        case 0x82B8: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // L940F:  INY
        case 0x82B9: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // L9410:  LDA (DatPntr1),Y
        case 0x82BB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L9412:  STA GenByte3C
        case 0x82BE: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // L9414:  INY
        case 0x82BF: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // L9415:  LDA (DatPntr1),Y
        case 0x82C1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L9417:  STA PPUDataByte
        case 0x82C4: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // L941C:  DEC GenByte3C
        case 0x82C7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // L941E:  BNE -
        case 0x82C9: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // L9420:  INY
        case 0x82CA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // L9421:  BNE GetNextEndByte
        case 0x82CC: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // L9423:  CMP #END_TXT_END
        case 0x82CE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // L9425:  BEQ FinishEndDataBlock
        case 0x82D0: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // L9427:  CMP #END_RPT_END
        case 0x82D2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // L9429:  BEQ FinishEndDataBlock
        case 0x82D4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // L942B:  JSR AddPPUBufEntry
        case 0x82D7: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // L942E:  INY
        case 0x82D8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // L942F:  BNE GetNextEndByte
        case 0x82DA: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // L9431:  INY
        case 0x82DB: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // L9432:  TYA
        case 0x82DC: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // L9433:  CLC
        case 0x82DD: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // L9434:  ADC DatPntr1LB
        case 0x82E0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L9436:  STA DatPntr1LB
        case 0x82E3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // L9438:  BCC +
        case 0x82E5: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // L943A:  INC DatPntrlUB
        case 0x82E8: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // L943E:  CMP #END_TXT_END
        case 0x82EA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // L9440:  BEQ RollCredits
        case 0x82EC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // L9442:  JSR WaitForNMI
        case 0x82EF: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // L9445:  BRK
        case 0x82F0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // L9448:  LDA EndCreditCount
        case 0x82F3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // L944A:  BNE CheckCredits1
        case 0x82F5: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // L944C:  LDY #$08
        case 0x82F7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // L944E:  BNE WaitForMusTmng
        case 0x82F9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // L9450:  CMP #$01
        case 0x82FB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // L9452:  BNE CheckCredits2
        case 0x82FD: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // L9454:  LDY #$02
        case 0x82FF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // L9456:  BNE WaitForMusTmng
        case 0x8301: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // L9458:  CMP #$02
        case 0x8303: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // L945A:  BEQ CheckCreditEnd
        case 0x8305: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // L945C:  CMP #$03
        case 0x8307: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // L945E:  BEQ CheckCreditEnd
        case 0x8309: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // L9460:  CMP #$04
        case 0x830B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // L9462:  BEQ CheckCreditEnd
        case 0x830D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // L9464:  CMP #$0D
        case 0x830F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // L9466:  BEQ MusicTiming2
        case 0x8311: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // L9468:  BCC MusicTiming2
        case 0x8313: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // L946A:  CMP #$12
        case 0x8315: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // L946C:  BCC MusicTiming3
        case 0x8317: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // L946E:  LDY #$A0
        case 0x8319: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // L9473:  DEY
        case 0x831A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // L9474:  BNE -
        case 0x831C: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // L9476:  RTS
        case 0x831D: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // L9477:  LDY #$03
        case 0x831F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // L9479:  BNE WaitForMusTmng
        case 0x8321: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // L947B:  LDY #$02
        case 0x8323: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // L9480:  DEY
        case 0x8324: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // L9481:  BNE -
        case 0x8326: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // L9483:  INC EndCreditCount
        case 0x8329: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // L9485:  BRK
        case 0x832A: cycles_to_run -= 3; reg_PC = 0x802A; return; // L9488:  JMP DoClearPPU
        case 0x832D: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // L994F:  PHA
        case 0x832E: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // L9950:  TXA
        case 0x832F: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // L9951:  PHA
        case 0x8330: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // L9952:  LDX #$7B
        case 0x8332: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // L9957:  STA BlockRAM+$20,X
        case 0x8335: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // L995A:  DEX
        case 0x8336: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // L995B:  BPL -
        case 0x8338: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // L995D:  PLA
        case 0x8339: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // L995E:  TAX
        case 0x833A: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // L995F:  PLA
        case 0x833B: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // L9960:  RTS
        case 0x833C: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // L9961:  PHA
        case 0x833D: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // L9962:  TYA
        case 0x833E: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // L9963:  PHA
        case 0x833F: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // L9964:  LDY #$0F
        case 0x8341: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // L9966:  LDA EnDatPtrLB
        case 0x8344: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // L9968:  CLC
        case 0x8345: cycles_to_run -= 2; cpu_adc(bus_read(0x802A)); reg_PC += 3; return; // L9969:  ADC EnStatTblPtr
        case 0x8348: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L996C:  STA GenPtr3CLB
        case 0x834B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // L996E:  LDA EnDatPtrUB
        case 0x834E: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // L9970:  ADC EnStatTblPtr+1
        case 0x8351: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L9973:  STA GenPtr3CUB
        case 0x8354: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // L9977:  STA EnBaseAtt,Y
        case 0x8357: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // L997A:  DEY
        case 0x8358: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // L997B:  BPL -
        case 0x835A: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // L997D:  PLA
        case 0x835B: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // L997E:  TAY
        case 0x835C: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // L997F:  PLA
        case 0x835D: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // L9980:  RTS
        case 0x835E: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // L9981:  PHA
        case 0x835F: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // L9982:  TYA
        case 0x8360: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // L9983:  PHA
        case 0x8361: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // L9984:  LDY #$00
        case 0x8363: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // L9986:  LDA CopyCounterLB
        case 0x8366: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // L9988:  ORA CopyCounterUB
        case 0x8369: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // L998A:  BEQ CopyROMDone
        case 0x836B: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // L998C:  LDA (ROMSrcPtr),Y
        case 0x836D: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // L998E:  STA (RAMTrgtPtr),Y
        case 0x836F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // L9990:  LDA CopyCounterLB
        case 0x8372: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // L9992:  SEC
        case 0x8373: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // L9993:  SBC #$01
        case 0x8375: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L9995:  STA CopyCounterLB
        case 0x8378: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // L9997:  LDA CopyCounterUB
        case 0x837B: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // L9999:  SBC #$00
        case 0x837D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L999B:  STA CopyCounterUB
        case 0x8380: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // L999D:  ORA CopyCounterLB
        case 0x8383: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // L999F:  BEQ CopyROMDone
        case 0x8385: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // L99A1:  INC ROMSrcPtrLB
        case 0x8388: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // L99A3:  BNE +
        case 0x838A: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // L99A5:  INC ROMSrcPtrUB
        case 0x838D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // L99A9:  BNE +
        case 0x838F: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // L99AB:  INC RAMTrgtPtrUB
        case 0x8392: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // L99B0:  PLA
        case 0x8393: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // L99B1:  TAY
        case 0x8394: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // L99B2:  PLA
        case 0x8395: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // L99B3:  RTS
        case 0x8396: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // L99B4:  TYA
        case 0x8397: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // L99B5:  PHA
        case 0x8398: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // L99B6:  LDA BaseStatsTbl-2
        case 0x839B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L99B9:  STA PlayerDatPtrLB
        case 0x839E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // L99BB:  LDA BaseStatsTbl-1
        case 0x83A1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L99BE:  STA PlayerDatPtrUB
        case 0x83A4: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // L99C0:  LDY LevelDatPtr
        case 0x83A7: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // L99C2:  LDA (PlayerDatPtr),Y
        case 0x83A9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L99C4:  STA DisplayedStr
        case 0x83AC: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // L99C6:  INY
        case 0x83AD: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // L99C7:  LDA (PlayerDatPtr),Y
        case 0x83AF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L99C9:  STA DisplayedAgi
        case 0x83B2: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // L99CB:  INY
        case 0x83B3: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // L99CC:  LDA (PlayerDatPtr),Y
        case 0x83B5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L99CE:  STA DisplayedMaxHP
        case 0x83B8: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // L99D0:  INY
        case 0x83B9: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // L99D1:  LDA (PlayerDatPtr),Y
        case 0x83BB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L99D3:  STA DisplayedMaxMP
        case 0x83BE: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // L99D5:  INY
        case 0x83BF: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // L99D6:  LDA (PlayerDatPtr),Y
        case 0x83C1: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // L99D8:  ORA ModsnSpells
        case 0x83C4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L99DA:  STA ModsnSpells
        case 0x83C7: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // L99DC:  INY
        case 0x83C8: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // L99DD:  LDA (PlayerDatPtr),Y
        case 0x83CA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // L99DF:  STA SpellFlags
        case 0x83CD: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // L99E1:  PLA
        case 0x83CE: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // L99E2:  TAY
        case 0x83CF: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // L99E3:  RTS
        case 0x83D0: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA181:  PLA
        case 0x83D1: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA182:  CLC
        case 0x83D2: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LA183:  ADC #$01
        case 0x83D4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA185:  STA GenPtr3ELB
        case 0x83D7: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA187:  PLA
        case 0x83D8: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LA188:  ADC #$00
        case 0x83DA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA18A:  STA GenPtr3EUB
        case 0x83DD: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA18C:  PHA
        case 0x83DE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA18D:  LDA GenPtr3ELB
        case 0x83E1: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA18F:  PHA
        case 0x83E2: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LA190:  LDY #$00
        case 0x83E4: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA192:  LDA (GenPtr3E),Y
        case 0x83E6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA194:  JSR DoWindowPrep
        case 0x83E9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA197:  JSR WindowSequence
        case 0x83EC: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA19A:  RTS
        case 0x83ED: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA19B:  STA WindowType
        case 0x83F0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA19E:  LDA WndBuildPhase
        case 0x83F3: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA1A1:  ORA #$80
        case 0x83F5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA1A3:  STA WndBuildPhase
        case 0x83F8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA1A6:  JSR WndConstruct
        case 0x83FB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA1A9:  JSR WndCalcBufAddr
        case 0x83FE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA1AC:  LDA #$40
        case 0x8400: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA1AE:  STA WndBuildPhase
        case 0x8403: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA1B1:  JSR GetWndDatPtr
        case 0x8406: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA1B4:  JSR GetWndConfig
        case 0x8409: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA1B7:  JSR WindowEngine
        case 0x840C: cycles_to_run -= 3; cpu_bit(bus_read(0x0000)); reg_PC += 3; return; // LA1BA:  BIT WndBuildPhase
        case 0x840F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0x802A; return; } // LA1BD:  BMI WndConstructDone
        case 0x8411: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA1BF:  LDA WindowType
        case 0x8414: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA1C2:  CMP #WND_SPELL1
        case 0x8416: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LA1C4:  BCC WndConstructDone
        case 0x8418: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA1C6:  CMP #WND_ALPHBT
        case 0x841A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LA1C8:  BCS WndConstructDone
        case 0x841C: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LA1CA:  BRK
        case 0x841D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA1CD:  LDA WndSelResults
        case 0x8420: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA1CF:  RTS
        case 0x8421: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA1D0:  LDA #$00
        case 0x8423: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA1D2:  JSR GetDescPtr
        case 0x8426: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA1D5:  LDA WindowType
        case 0x8429: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA1D8:  ASL
        case 0x842A: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LA1D9:  TAY
        case 0x842B: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA1DA:  LDA (DescPtr),Y
        case 0x842D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA1DC:  STA WndDatPtrLB
        case 0x8430: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA1DE:  INY
        case 0x8431: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA1DF:  LDA (DescPtr),Y
        case 0x8433: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA1E1:  STA WndDatPtrUB
        case 0x8436: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA1E3:  RTS
        case 0x8437: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LA1E4:  LDY #$00
        case 0x8439: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA1E6:  LDA (WndDatPtr),Y
        case 0x843B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA1E8:  STA WndOptions
        case 0x843E: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA1EB:  INY
        case 0x843F: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA1EC:  LDA (WndDatPtr),Y
        case 0x8441: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA1EE:  STA WndHeightblks
        case 0x8444: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA1F1:  ASL
        case 0x8445: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA1F2:  STA WndHeight
        case 0x8448: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA1F5:  INY
        case 0x8449: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA1F6:  LDA (WndDatPtr),Y
        case 0x844B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA1F8:  STA WndWidth
        case 0x844E: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA1FB:  INY
        case 0x844F: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA1FC:  LDA (WndDatPtr),Y
        case 0x8451: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA1FE:  STA WndPosition
        case 0x8454: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA201:  PHA
        case 0x8455: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA202:  AND #$0F
        case 0x8457: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA204:  ASL
        case 0x8458: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA205:  STA WndColPos
        case 0x845B: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA207:  PLA
        case 0x845C: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA208:  AND #$F0
        case 0x845E: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA20A:  LSR
        case 0x845F: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA20B:  LSR
        case 0x8460: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA20C:  LSR
        case 0x8461: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA20D:  STA WndRowPos
        case 0x8464: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA20F:  INY
        case 0x8465: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA210:  LDA WndOptions
        case 0x8468: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LA213:  BPL +
        case 0x846A: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA215:  LDA (WndDatPtr),Y
        case 0x846C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA217:  STA WndColumns
        case 0x846F: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA21A:  INY
        case 0x8470: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA21B:  LDA (WndDatPtr),Y
        case 0x8472: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA21D:  STA WndCursorHome
        case 0x8475: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA220:  INY
        case 0x8476: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_V)) { reg_PC = 0x0000; return; } // LA224:  BVC +
        case 0x8478: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA226:  LDA (WndDatPtr),Y
        case 0x847A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA228:  STA WndUnused1
        case 0x847D: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA22B:  INY
        case 0x847E: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA22F:  RTS
        case 0x847F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA230:  JSR InitWindowEngine
        case 0x8482: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA233:  JSR WndUpdateWrkTile
        case 0x8485: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA236:  JSR GetNxtWndByte
        case 0x8488: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA239:  JSR JumpToWndFunc
        case 0x848B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA23C:  JSR WndShowLine
        case 0x848E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA23F:  JSR WndChkFullHeight
        case 0x8491: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LA242:  BCC BuildWindowLoop
        case 0x8493: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA244:  JSR DoBlinkingCursor
        case 0x8496: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA247:  RTS
        case 0x8497: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA248:  JSR ClearWndLineBuf
        case 0x849A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA24B:  LDA #$FF
        case 0x849C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA24D:  STA WndUnused64FB
        case 0x849F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA250:  LDA #$00
        case 0x84A1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA252:  STA WndXPos
        case 0x84A4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA255:  STA WndYPos
        case 0x84A7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA258:  STA WndThisDesc
        case 0x84AA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA25B:  STA WndDescHalf
        case 0x84AD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA25E:  STA WndBuildRow
        case 0x84B0: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LA261:  LDX #$0F
        case 0x84B2: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LA266:  DEX
        case 0x84B3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LA267:  BPL -
        case 0x84B5: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA269:  RTS
        case 0x84B6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA26A:  LDA #TL_BLANK_TILE1
        case 0x84B8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA26C:  STA WorkTile
        case 0x84BB: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LA26F:  LDX WndXPos
        case 0x84BE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA272:  BEQ CheckWndRow
        case 0x84C0: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LA274:  INX
        case 0x84C1: cycles_to_run -= 2; update_flags_cmp(reg_X, bus_read(0x0000)); reg_PC += 3; return; // LA275:  CPX WndWidth
        case 0x84C4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA278:  BNE CheckWndBottom
        case 0x84C6: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LA27A:  LDX WndYPos
        case 0x84C9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA27D:  BEQ WndUpRightCrnr
        case 0x84CB: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LA27F:  INX
        case 0x84CC: cycles_to_run -= 2; update_flags_cmp(reg_X, bus_read(0x0000)); reg_PC += 3; return; // LA280:  CPX WndHeight
        case 0x84CF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA283:  BEQ WndBotRightCrnr
        case 0x84D1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA285:  LDA #TL_RIGHT
        case 0x84D3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA287:  BNE UpdateWndWrkTile
        case 0x84D5: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA289:  LDA #TL_UPPER_RIGHT
        case 0x84D7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA28B:  BNE UpdateWndWrkTile
        case 0x84D9: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA28D:  LDA #TL_BOT_RIGHT
        case 0x84DB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA28F:  BNE UpdateWndWrkTile
        case 0x84DD: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LA291:  LDX WndYPos
        case 0x84E0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA294:  BEQ WndUpLeftCrnr
        case 0x84E2: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LA296:  INX
        case 0x84E3: cycles_to_run -= 2; update_flags_cmp(reg_X, bus_read(0x0000)); reg_PC += 3; return; // LA297:  CPX WndHeight
        case 0x84E6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA29A:  BEQ WndBotLeftCrnr
        case 0x84E8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA29C:  LDA #TL_LEFT
        case 0x84EA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA29E:  BNE UpdateWndWrkTile
        case 0x84EC: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA2A0:  LDA #TL_UPPER_LEFT
        case 0x84EE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA2A2:  BNE UpdateWndWrkTile
        case 0x84F0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA2A4:  LDA #TL_BOT_LEFT
        case 0x84F2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA2A6:  BNE UpdateWndWrkTile
        case 0x84F4: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LA2A8:  LDX WndYPos
        case 0x84F7: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LA2AB:  INX
        case 0x84F8: cycles_to_run -= 2; update_flags_cmp(reg_X, bus_read(0x0000)); reg_PC += 3; return; // LA2AC:  CPX WndHeight
        case 0x84FB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LA2AF:  BNE +
        case 0x84FD: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA2B1:  LDA #TL_BOTTOM
        case 0x84FF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA2B3:  STA WorkTile
        case 0x8502: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA2B7:  LDA WorkTile
        case 0x8505: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA2BA:  CMP #TL_BLANK_TILE1
        case 0x8507: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA2BC:  BNE WorkTileNotBlank
        case 0x8509: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA2BE:  LDA WndOptions
        case 0x850C: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA2C1:  AND #$20
        case 0x850E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA2C3:  BNE GetNextWndByte
        case 0x8510: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA2C5:  LDA WndYPos
        case 0x8513: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA2C8:  LSR
        case 0x8514: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LA2C9:  BCC GetNextWndByte
        case 0x8516: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA2CB:  LDA WndBuildRow
        case 0x8519: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA2CE:  CMP #$01
        case 0x851B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA2D0:  BNE ClearWndCntrlByte
        case 0x851D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA2D2:  LDA #$00
        case 0x851F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA2D4:  STA WndXPos
        case 0x8522: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LA2D7:  LDX WndYPos
        case 0x8525: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LA2DA:  INX
        case 0x8526: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LA2DB:  STX WndHeight
        case 0x8529: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA2DE:  PLA
        case 0x852A: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA2DF:  PLA
        case 0x852B: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA2E0:  JMP BuildWindowLoop
        case 0x852E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA2E3:  LDA #$00
        case 0x8530: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA2E5:  BEQ SeparateCntrlByte
        case 0x8532: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LA2E7:  LDY WndDatIndex
        case 0x8535: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA2EA:  INC WndDatIndex
        case 0x8538: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA2ED:  LDA (WndDatPtr),Y
        case 0x853A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x802A; return; } // LA2EF:  BPL GotCharDat
        case 0x853C: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA2F1:  AND #$7F
        case 0x853E: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA2F3:  PHA
        case 0x853F: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA2F4:  AND #$07
        case 0x8541: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA2F6:  STA WndParam
        case 0x8544: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA2F9:  PLA
        case 0x8545: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA2FA:  LSR
        case 0x8546: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA2FB:  LSR
        case 0x8547: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA2FC:  LSR
        case 0x8548: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA2FD:  STA WndCcontrol
        case 0x854B: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA300:  RTS
        case 0x854C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA301:  STA WorkTile
        case 0x854F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA304:  LDA #$10
        case 0x8551: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA306:  STA WndCcontrol
        case 0x8554: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA309:  RTS
        case 0x8555: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA30A:  LDA WndCcontrol
        case 0x8558: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA30D:  ASL
        case 0x8559: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LA30E:  TAX
        case 0x855A: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x802A, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA30F:  LDA WndCntrlPtrTbl,X
        case 0x855D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA312:  STA WndFcnLB
        case 0x8560: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA314:  LDA WndCntrlPtrTbl+1,X
        case 0x8563: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA317:  STA WndFcnUB
        case 0x8566: cycles_to_run -= 3; reg_PC = 0x0000; return; // LA319:  JMP (WndFcnPtr)
        case 0x8569: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA31C:  LDA #TL_BLANK_TILE1
        case 0x856B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA31E:  STA WorkTile
        case 0x856E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA321:  JSR SetCountLength
        case 0x8571: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_V) { reg_PC = 0x0000; return; } // LA327:  BVS +
        case 0x8573: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA329:  JSR BuildWndLine
        case 0x8576: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA32C:  JMP NextBlankTile
        case 0x8579: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA332:  DEC WndCounter
        case 0x857C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LA335:  BNE --
        case 0x857E: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA337:  RTS
        case 0x857F: cycles_to_run -= 3; cpu_bit(bus_read(0x0000)); reg_PC += 3; return; // LA338:  BIT WndOptions
        case 0x8582: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_V)) { reg_PC = 0x802A; return; } // LA33B:  BVC DoHorzTiles
        case 0x8584: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA33D:  LDA #TL_BLANK_TILE1
        case 0x8586: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA33F:  STA WorkTile
        case 0x8589: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA342:  JSR BuildWndLine
        case 0x858C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA345:  LDA #TL_TOP2
        case 0x858E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA347:  STA WorkTile
        case 0x8591: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA34A:  JSR BuildWndLine
        case 0x8594: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA34D:  LDA #TL_TOP1
        case 0x8596: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA34F:  STA WorkTile
        case 0x8599: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA352:  JSR SetCountLength
        case 0x859C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA355:  JSR BuildWndLine
        case 0x859F: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA358:  DEC WndCounter
        case 0x85A2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA35B:  BNE HorzTilesLoop
        case 0x85A4: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA35D:  RTS
        case 0x85A5: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA35E:  LDA #$03
        case 0x85A7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA360:  STA SubBufLength
        case 0x85AA: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LA363:  LDX #HitPoints
        case 0x85AC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA365:  LDA WndParam
        case 0x85AF: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA368:  AND #$04
        case 0x85B1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LA36A:  BEQ +
        case 0x85B3: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LA36C:  LDX #MagicPoints
        case 0x85B5: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA370:  JMP WndBinToBCD
        case 0x85B8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA373:  LDA #$05
        case 0x85BA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA375:  STA SubBufLength
        case 0x85BD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA378:  JSR GoldToBCD
        case 0x85C0: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA37B:  JMP WndTempToLineBuf
        case 0x85C3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA37E:  LDA WndParam
        case 0x85C6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA381:  BNE WndGetSavedGame
        case 0x85C8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA383:  LDA #$02
        case 0x85CA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA385:  STA SubBufLength
        case 0x85CD: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LA388:  LDX #DisplayedLevel
        case 0x85CF: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LA38A:  LDY #$01
        case 0x85D1: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA38C:  JMP WndBinToBCD
        case 0x85D4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LA38F:  JSR WndLoadGameDat
        case 0x85D7: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA392:  JMP WndCovertLvl
        case 0x85DA: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA395:  LDA #$05
        case 0x85DC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA397:  STA SubBufLength
        case 0x85DF: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LA39A:  LDX #ExpLB
        case 0x85E1: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LA39C:  LDY #$02
        case 0x85E3: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA39E:  JMP WndBinToBCD
        case 0x85E6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA3A1:  LDA WndParam
        case 0x85E9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA3A4:  CMP #$01
        case 0x85EB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA3A6:  BEQ WndGetfullName
        case 0x85ED: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA3A8:  CMP #$04
        case 0x85EF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA3AA:  BEQ WndFullSaved
        case 0x85F1: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA3AC:  CMP #$05
        case 0x85F3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LA3AE:  BCS WndLwr4Saved
        case 0x85F5: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA3B0:  LDA #$04
        case 0x85F7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA3B2:  STA SubBufLength
        case 0x85FA: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LA3B5:  LDX #$00
        case 0x85FC: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LA3B7:  LDY SubBufLength
        case 0x85FF: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA3BA:  LDA DispName0,X
        case 0x8602: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LA3BC:  STA TempBuffer-1,Y
        case 0x8605: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LA3BF:  INX
        case 0x8606: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LA3C0:  DEY
        case 0x8607: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA3C1:  BNE WndGetLwrName
        case 0x8609: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA3C3:  JMP WndTempToLineBuf
        case 0x860C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA3C6:  JSR WndPrepGetLwr
        case 0x860F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA3C9:  LDA #$04
        case 0x8611: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA3CB:  STA SubBufLength
        case 0x8614: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LA3CE:  LDX #$00
        case 0x8616: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LA3D0:  LDY SubBufLength
        case 0x8619: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA3D3:  LDA DispName4,X
        case 0x861C: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LA3D6:  STA TempBuffer-1,Y
        case 0x861F: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LA3D9:  INX
        case 0x8620: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LA3DA:  DEY
        case 0x8621: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA3DB:  BNE WndGetUprName
        case 0x8623: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA3DD:  JMP WndTempToLineBuf
        case 0x8626: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA3E0:  LDA #$04
        case 0x8628: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA3E2:  STA SubBufLength
        case 0x862B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA3E5:  LDA WndParam
        case 0x862E: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LA3E8:  SEC
        case 0x862F: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LA3E9:  SBC #$05
        case 0x8631: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA3EB:  STA SaveSelected
        case 0x8634: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LA3EE:  JSR WndLoadGameDat
        case 0x8637: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA3F1:  JMP WndPrepGetLwr
        case 0x863A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA3F4:  LDA #$08
        case 0x863C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA3F6:  STA SubBufLength
        case 0x863F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LA3F9:  JSR WndLoadGameDat
        case 0x8642: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA3FC:  JMP WndGetfullName
        case 0x8645: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA3FF:  LDA #$09
        case 0x8647: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA401:  STA SubBufLength
        case 0x864A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA404:  LDA WndParam
        case 0x864D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA407:  CMP #$03
        case 0x864F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LA409:  BCS WndDoInvItem
        case 0x8651: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA40B:  LDA WndParam
        case 0x8654: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LA40E:  ADC #$08
        case 0x8656: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LA410:  TAX
        case 0x8657: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA411:  LDA DescBuf,X
        case 0x865A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA413:  JSR WpnArmrConv
        case 0x865D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA416:  JSR LookupDescriptions
        case 0x8660: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA419:  JSR WndTempToLineBuf
        case 0x8663: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA41C:  JMP SecondDescHalf
        case 0x8666: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA41F:  JSR WndGetDescByte
        case 0x8669: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA422:  JSR DoInvConv
        case 0x866C: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA425:  PHA
        case 0x866D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA426:  LDA WndParam
        case 0x8670: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA429:  CMP #$03
        case 0x8672: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA42B:  BNE WndDescNum
        case 0x8674: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA42D:  PLA
        case 0x8675: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA42E:  PHA
        case 0x8676: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA42F:  CMP #DSC_HERB
        case 0x8678: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA431:  BEQ WndDecDescLength
        case 0x867A: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA433:  CMP #DSC_KEY
        case 0x867C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA435:  BNE WndDescNum
        case 0x867E: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA437:  DEC SubBufLength
        case 0x8681: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA43A:  PLA
        case 0x8682: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA43B:  JSR LookupDescriptions
        case 0x8685: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA43E:  JSR WndTempToLineBuf
        case 0x8688: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA441:  LDA WndDescHalf
        case 0x868B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA444:  BNE WndDesc2ndHalf
        case 0x868D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA446:  LDA WndParam
        case 0x8690: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA449:  CMP #$03
        case 0x8692: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA44B:  BNE WndDesc2ndHalf
        case 0x8694: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA44D:  LDA WndDescIndex
        case 0x8697: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA450:  CMP #DSC_HERB
        case 0x8699: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA452:  BEQ WndNumHerbs
        case 0x869B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA454:  CMP #DSC_KEY
        case 0x869D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA456:  BEQ WndNumKeys
        case 0x869F: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA458:  JMP SecondDescHalf
        case 0x86A2: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA45B:  LDA InventoryHerbs
        case 0x86A5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA45D:  BNE WndPrepBCD
        case 0x86A7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA45F:  LDA InventoryKeys
        case 0x86AA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA461:  STA BCDByte0
        case 0x86AD: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA463:  LDA #$00
        case 0x86AF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA465:  STA BCDByte1
        case 0x86B2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA467:  STA BCDByte2
        case 0x86B5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA469:  JSR ClearTempBuffer
        case 0x86B8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA46C:  LDA #$01
        case 0x86BA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA46E:  STA SubBufLength
        case 0x86BD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA471:  JSR BinWordToBCD_
        case 0x86C0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA474:  JSR WndTempToLineBuf
        case 0x86C3: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA477:  JMP SecondDescHalf
        case 0x86C6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA47A:  LDA #$09
        case 0x86C8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA47C:  STA SubBufLength
        case 0x86CB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA47F:  JSR WndGetDescByte
        case 0x86CE: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LA482:  SEC
        case 0x86CF: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LA483:  SBC #$01
        case 0x86D1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA485:  JSR WndGetSpellDesc
        case 0x86D4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA488:  JSR WndTempToLineBuf
        case 0x86D7: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA48B:  INC WndThisDesc
        case 0x86DA: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA48E:  RTS
        case 0x86DB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA48F:  JSR ClearTempBuffer
        case 0x86DE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA492:  LDA #$05
        case 0x86E0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA494:  STA SubBufLength
        case 0x86E3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA497:  LDA #$06
        case 0x86E5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA499:  JSR GetDescPtr
        case 0x86E8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA49C:  LDA WndDescIndex
        case 0x86EB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA49F:  BEQ WndCstToLineBuf
        case 0x86ED: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA4A1:  ASL
        case 0x86EE: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LA4A2:  TAY
        case 0x86EF: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA4A3:  LDA (DescPtr),Y
        case 0x86F1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA4A5:  STA BCDByte0
        case 0x86F4: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA4A7:  INY
        case 0x86F5: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA4A8:  LDA (DescPtr),Y
        case 0x86F7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA4AA:  STA BCDByte1
        case 0x86FA: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA4AC:  LDA #$00
        case 0x86FC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA4AE:  STA BCDByte2
        case 0x86FF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA4B0:  JSR BinWordToBCD_
        case 0x8702: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA4B3:  JMP WndTempToLineBuf
        case 0x8705: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA4B6:  LDA #$00
        case 0x8707: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA4B8:  STA WndThisDesc
        case 0x870A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA4BB:  LDA #$00
        case 0x870C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA4BD:  STA WndDescHalf
        case 0x870F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA4C0:  JSR CalcNumItems
        case 0x8712: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA4C3:  STA WndBuildRow
        case 0x8715: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA4C6:  LDA WndDatIndex
        case 0x8718: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA4C9:  STA WndRepeatIndex
        case 0x871B: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA4CC:  RTS
        case 0x871C: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LA4CD:  LDX #$01
        case 0x871E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA4D1:  CMP #ITM_END
        case 0x8720: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA4D3:  BEQ NumItemsEnd
        case 0x8722: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LA4D5:  INX
        case 0x8723: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LA4D6:  BNE -
        case 0x8725: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LA4D8:  DEX
        case 0x8726: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA4D9:  LDA DescBuf
        case 0x8729: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA4DB:  CMP #$01
        case 0x872B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA4DD:  BEQ ReturnNumItems
        case 0x872D: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LA4DF:  INX
        case 0x872E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA4E0:  CMP #$02
        case 0x8730: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA4E2:  BEQ ReturnNumItems
        case 0x8732: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LA4E4:  INX
        case 0x8733: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LA4E5:  TXA
        case 0x8734: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA4E6:  RTS
        case 0x8735: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA4E7:  LDA WndParam
        case 0x8738: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA4EA:  CMP #$02
        case 0x873A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA4EC:  BEQ WndBuildVarDone
        case 0x873C: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA4EE:  AND #$03
        case 0x873E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA4F0:  BNE WndBuildEnd
        case 0x8740: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA4F2:  LDA WndBuildRow
        case 0x8743: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA4F5:  BEQ WndBuildVarDone
        case 0x8745: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA4F7:  DEC WndBuildRow
        case 0x8748: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA4FA:  BEQ WndBuildVarDone
        case 0x874A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA4FC:  LDA WndRepeatIndex
        case 0x874D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA4FF:  STA WndDatIndex
        case 0x8750: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA502:  RTS
        case 0x8751: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA503:  LDA #$00
        case 0x8753: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA505:  STA WndXPos
        case 0x8756: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA508:  STA WndParam
        case 0x8759: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA50B:  LDA WndYPos
        case 0x875C: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA50E:  AND #$01
        case 0x875E: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA510:  EOR #$01
        case 0x8760: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA512:  CLC
        case 0x8761: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LA513:  ADC #$01
        case 0x8763: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA515:  ADC WndYPos
        case 0x8766: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA518:  STA WndHeight
        case 0x8769: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA51B:  LSR
        case 0x876A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA51C:  STA WndHeightblks
        case 0x876D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA51F:  LDA WndYPos
        case 0x8770: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA522:  AND #$01
        case 0x8772: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA524:  BNE WndEndBuild
        case 0x8774: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA526:  LDA #TL_LEFT
        case 0x8776: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA528:  STA WorkTile
        case 0x8779: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA52B:  JSR BuildWndLine
        case 0x877C: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA52E:  JMP WndBlankTiles
        case 0x877F: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA531:  RTS
        case 0x8780: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LA532:  LDX WndParam
        case 0x8783: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x802A, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA535:  LDA AttribVarTbl,X
        case 0x8786: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LA538:  TAX
        case 0x8787: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA539:  LDA #$03
        case 0x8789: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA53B:  STA SubBufLength
        case 0x878C: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LA53E:  LDY #$01
        case 0x878E: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA540:  JMP WndBinToBCD
        case 0x8791: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA543:  JMP BuildWndLine
        case 0x8794: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA546:  LDA WndYPos
        case 0x8797: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA549:  AND #$01
        case 0x8799: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA54B:  BEQ BldLoadWrkTile
        case 0x879B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA54D:  LDA WndWidth
        case 0x879E: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA550:  CLC
        case 0x879F: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA551:  ADC WndXPos
        case 0x87A2: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LA554:  TAX
        case 0x87A3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA555:  LDA WorkTile
        case 0x87A6: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LA558:  STA WndLineBuf,X
        case 0x87A9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA55B:  JSR WndStorePPUDat
        case 0x87AC: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA55E:  CMP #TL_LEFT
        case 0x87AE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LA560:  BCS WndNextXPos
        case 0x87B0: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA562:  LDA WndLineBuf-1,X
        case 0x87B3: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA565:  CMP #TL_TOP1
        case 0x87B5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LA567:  BNE WndNextXPos
        case 0x87B7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA569:  LDA WndXPos
        case 0x87BA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LA56C:  BEQ WndNextXPos
        case 0x87BC: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA56E:  LDA #TL_TOP2
        case 0x87BE: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LA570:  STA WndLineBuf-1,X
        case 0x87C1: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA573:  INC WndXPos
        case 0x87C4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA576:  LDA WndXPos
        case 0x87C7: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LA579:  CMP WndWidth
        case 0x87CA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LA57C:  BCC +
        case 0x87CC: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LA57E:  LDX #$01
        case 0x87CE: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LA580:  STX WndCounter
        case 0x87D1: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LA583:  DEX
        case 0x87D2: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LA584:  STX WndXPos
        case 0x87D5: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA587:  INC WndYPos
        case 0x87D8: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA58B:  PHA
        case 0x87D9: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LA58C:  TXA
        case 0x87DA: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA58D:  PHA
        case 0x87DB: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LA58E:  TYA
        case 0x87DC: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA58F:  PHA
        case 0x87DD: cycles_to_run -= 3; cpu_bit(bus_read(0x0000)); reg_PC += 3; return; // LA590:  BIT WndBuildPhase
        case 0x87E0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_V) { reg_PC = 0x802A; return; } // LA593:  BVS WndStorePPUDatEnd
        case 0x87E2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA595:  JSR PrepPPUAdrCalc
        case 0x87E5: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA598:  LDA #$20
        case 0x87E7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA59A:  STA PPURowBytesLB
        case 0x87EA: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA59C:  LDA #$00
        case 0x87EC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA59E:  STA PPURowBytesUB
        case 0x87EF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA5A0:  LDA WndYPos
        case 0x87F2: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LA5A3:  LDX #PPURowBytesLB
        case 0x87F4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA5A5:  JSR IndexedMult
        case 0x87F7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA5A8:  LDA PPURowBytesLB
        case 0x87FA: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA5AA:  CLC
        case 0x87FB: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA5AB:  ADC WndXPos
        case 0x87FE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA5AE:  STA PPURowBytesLB
        case 0x8801: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LA5B0:  BCC WndAddOffsetToAddr
        case 0x8803: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA5B2:  INC PPURowBytesUB
        case 0x8806: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA5B4:  CLC
        case 0x8807: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA5B5:  LDA PPURowBytesLB
        case 0x880A: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA5B7:  ADC PPUAddrLB
        case 0x880D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA5B9:  STA PPUAddrLB
        case 0x8810: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA5BB:  LDA PPURowBytesUB
        case 0x8813: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA5BD:  ADC PPUAddrUB
        case 0x8816: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA5BF:  STA PPUAddrUB
        case 0x8819: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LA5C1:  LDY #$00
        case 0x881B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA5C3:  LDA WorkTile
        case 0x881E: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LA5C6:  STA (PPUBufPtr),Y
        case 0x8820: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA5C8:  PLA
        case 0x8821: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LA5C9:  TAY
        case 0x8822: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA5CA:  PLA
        case 0x8823: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LA5CB:  TAX
        case 0x8824: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA5CC:  PLA
        case 0x8825: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA5CD:  RTS
        case 0x8826: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA5CE:  LDA WndYPos
        case 0x8829: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA5D1:  AND #$01
        case 0x882B: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA5D3:  ORA WndXPos
        case 0x882E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA5D6:  BNE WndExitShowLine
        case 0x8830: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA5D8:  LDA WndBuildPhase
        case 0x8833: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0x802A; return; } // LA5DB:  BMI WndExitShowLine
        case 0x8835: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA5DD:  LDA WndWidth
        case 0x8838: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA5E0:  LSR
        case 0x8839: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA5E1:  ORA #$10
        case 0x883B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA5E3:  STA WndWidthTemp
        case 0x883E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA5E6:  LDA WndPosition
        case 0x8841: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA5E9:  STA _WndPosition
        case 0x8844: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA5EC:  CLC
        case 0x8845: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LA5ED:  ADC #$10
        case 0x8847: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA5EF:  STA WndPosition
        case 0x884A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA5F2:  JSR WndShowHide
        case 0x884D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA5F5:  JSR ClearWndLineBuf
        case 0x8850: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA5F8:  RTS
        case 0x8851: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA5F9:  LDA WndYPos
        case 0x8854: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LA5FC:  CMP WndHeight
        case 0x8857: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA5FF:  RTS
        case 0x8858: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA600:  LDA WndParam
        case 0x885B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LA603:  BNE +
        case 0x885D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA605:  LDA #$FF
        case 0x885F: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA60A:  CLC
        case 0x8860: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA60B:  LDA WndWidth
        case 0x8863: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LA60E:  SBC WndXPos
        case 0x8866: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LA611:  BCC +
        case 0x8868: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LA613:  CMP SubBufLength
        case 0x886B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LA616:  BCS +
        case 0x886D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA618:  STA SubBufLength
        case 0x8870: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA61C:  JSR _BinWordToBCD
        case 0x8873: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA61F:  JMP WndTempToLineBuf
        case 0x8876: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA622:  JSR GetBinBytesBCD
        case 0x8879: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA625:  JSR ConvertToBCD
        case 0x887C: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA628:  JMP ClearBCDLeadZeros
        case 0x887F: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA62E:  LDA TempBuffer-1,X
        case 0x8882: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA631:  STA WorkTile
        case 0x8885: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA634:  JSR BuildWndLine
        case 0x8888: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA637:  DEC SubBufLength
        case 0x888B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LA63A:  BNE -
        case 0x888D: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA63C:  RTS
        case 0x888E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA63D:  LDA WndOptions
        case 0x8891: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LA640:  BPL +
        case 0x8893: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA642:  JSR WndDoSelect
        case 0x8896: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA646:  LDA #TL_BLANK_TILE1
        case 0x8898: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LA648:  LDX #$3B
        case 0x889A: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LA64D:  DEX
        case 0x889B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LA64E:  BPL -
        case 0x889D: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA650:  RTS
        case 0x889E: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LA651:  LDX WndThisDesc
        case 0x88A1: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA654:  LDA DescBuf+1,X
        case 0x88A4: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA656:  RTS
        case 0x88A5: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA657:  PHA
        case 0x88A6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA658:  LDA WndParam
        case 0x88A9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA65B:  CMP #$03
        case 0x88AB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA65D:  BEQ PlyrInvConv
        case 0x88AD: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA65F:  CMP #$04
        case 0x88AF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA661:  BEQ ShopInvConv
        case 0x88B1: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA663:  PLA
        case 0x88B2: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA664:  RTS
        case 0x88B3: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA665:  PLA
        case 0x88B4: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LA666:  TAX
        case 0x88B5: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA667:  LDA PlyrInvConvTbl-2,X
        case 0x88B8: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA66A:  RTS
        case 0x88B9: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA66B:  PLA
        case 0x88BA: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA66C:  CMP #$13
        case 0x88BC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LA66E:  BCS ToolInvConv
        case 0x88BE: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LA670:  TAX
        case 0x88BF: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA671:  LDA WpnShopConvTbl-2,X
        case 0x88C2: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA674:  RTS
        case 0x88C3: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LA675:  SEC
        case 0x88C4: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LA676:  SBC #$13
        case 0x88C6: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA678:  CMP #$05
        case 0x88C8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA67A:  BEQ DgnSclConv
        case 0x88CA: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA67C:  LSR
        case 0x88CB: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LA67D:  TAX
        case 0x88CC: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x802A, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA67E:  LDA ItmShopConvTbl,X
        case 0x88CF: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA681:  RTS
        case 0x88D0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA682:  LDA #DSC_DRGN_SCL
        case 0x88D2: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA684:  RTS
        case 0x88D3: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LA685:  TAX
        case 0x88D4: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA686:  LDA WpnArmrConvTbL-9,X
        case 0x88D7: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA689:  RTS
        case 0x88D8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA6EB:  STA IndMultByte
        case 0x88DB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA6EE:  LDA #$00
        case 0x88DD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA6F0:  STA IndMultNum1
        case 0x88E0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA6F3:  STA IndMultNum2
        case 0x88E3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LA6F9:  BCC +
        case 0x88E5: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA6FB:  LDA GenPtr00LB,X
        case 0x88E8: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA6FD:  CLC
        case 0x88E9: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA6FE:  ADC IndMultNum1
        case 0x88EC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA701:  STA IndMultNum1
        case 0x88EF: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA704:  LDA GenPtr00UB,X
        case 0x88F2: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA706:  ADC IndMultNum2
        case 0x88F5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA709:  STA IndMultNum2
        case 0x88F8: cycles_to_run -= 2; { uint16_t a = addr_abs_x(0x0000, nullptr); bus_write(a, cpu_rol(bus_read(a))); } reg_PC += 3; return; // LA70E:  ROL GenPtr00UB,X
        case 0x88FB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA710:  LDA IndMultByte
        case 0x88FE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LA713:  BNE --
        case 0x8900: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA715:  LDA IndMultNum1
        case 0x8903: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LA718:  STA GenPtr00LB,X
        case 0x8906: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA71A:  LDA IndMultNum2
        case 0x8909: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LA71D:  STA GenPtr00UB,X
        case 0x890C: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA71F:  RTS
        case 0x890D: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LA720:  TXA
        case 0x890E: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA721:  PHA
        case 0x890F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA722:  LDA #$00
        case 0x8911: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA724:  STA BCDResult
        case 0x8914: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LA726:  LDX #$18
        case 0x8916: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_rol(bus_read(a))); } reg_PC += 3; return; // LA72A:  ROL BCDByte1
        case 0x8919: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_rol(bus_read(a))); } reg_PC += 3; return; // LA72C:  ROL BCDByte2
        case 0x891C: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_rol(bus_read(a))); } reg_PC += 3; return; // LA72E:  ROL BCDResult
        case 0x891F: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LA730:  SEC
        case 0x8920: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA731:  LDA BCDResult
        case 0x8923: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LA733:  SBC #$0A
        case 0x8925: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LA735:  BCC +
        case 0x8927: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA737:  STA BCDResult
        case 0x892A: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA739:  INC BCDByte0
        case 0x892D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LA73C:  BNE --
        case 0x892F: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA73E:  PLA
        case 0x8930: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LA73F:  TAX
        case 0x8931: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA740:  RTS
        case 0x8932: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA741:  LDA #$00
        case 0x8934: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA743:  STA BCDByte2
        case 0x8937: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA745:  STA BCDByte1
        case 0x893A: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA747:  LDA GenWrd00LB,X
        case 0x893D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA749:  STA BCDByte0
        case 0x8940: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LA74B:  DEY
        case 0x8941: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LA74C:  BEQ +
        case 0x8943: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA74E:  LDA GenWrd00UB,X
        case 0x8946: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA750:  STA BCDByte1
        case 0x8949: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LA753:  LDY #$00
        case 0x894B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA758:  LDA BCDResult
        case 0x894E: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LA75A:  STA TempBuffer,Y
        case 0x8951: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA75D:  INY
        case 0x8952: cycles_to_run -= 2; update_flags_cmp(reg_Y, bus_read(0x0000)); reg_PC += 3; return; // LA75E:  CPY SubBufLength
        case 0x8955: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LA761:  BNE -
        case 0x8957: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA763:  RTS
        case 0x8958: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LA764:  LDX SubBufLength
        case 0x895B: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LA767:  DEX
        case 0x895C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LA76B:  BNE +
        case 0x895E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA76D:  LDA #TL_BLANK_TILE1
        case 0x8960: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LA76F:  STA TempBuffer,X
        case 0x8963: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LA772:  DEX
        case 0x8964: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LA773:  BNE -
        case 0x8966: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA776:  PHA
        case 0x8967: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LA777:  TXA
        case 0x8968: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA778:  PHA
        case 0x8969: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LA779:  LDX #$0C
        case 0x896B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA77B:  LDA #TL_BLANK_TILE1
        case 0x896D: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LA780:  DEX
        case 0x896E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LA781:  BPL -
        case 0x8970: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA783:  PLA
        case 0x8971: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LA784:  TAX
        case 0x8972: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA785:  PLA
        case 0x8973: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA786:  RTS
        case 0x8974: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA787:  JSR ClearAndSetBufLen
        case 0x8977: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LA78A:  CPX #$FF
        case 0x8979: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LA78C:  BEQ ++
        case 0x897B: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA78E:  LDA DescBuf,X
        case 0x897E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA790:  STA WndDescIndex
        case 0x8981: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA793:  JSR ClearAndSetBufLen
        case 0x8984: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA796:  LDA WndDescHalf
        case 0x8987: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA799:  AND #$01
        case 0x8989: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LA79B:  BEQ +
        case 0x898B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA79D:  LDA #$01
        case 0x898D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA7A0:  LDA WndDescIndex
        case 0x8990: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA7A3:  AND #$3F
        case 0x8992: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA7A5:  STA WndDescIndex
        case 0x8995: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LA7A8:  BEQ +
        case 0x8997: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA7AA:  JSR PrepIndexes
        case 0x899A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA7AE:  JSR ClearTempBuffer
        case 0x899D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA7B1:  LDA WndDescHalf
        case 0x89A0: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA7B4:  LSR
        case 0x89A1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LA7B5:  BCC +
        case 0x89A3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA7B7:  LDA #$08
        case 0x89A5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA7B9:  STA SubBufLength
        case 0x89A8: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA7BD:  PHA
        case 0x89A9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA7BE:  CMP #$20
        case 0x89AB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LA7C0:  BCC +
        case 0x89AD: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA7C2:  PLA
        case 0x89AE: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LA7C3:  SBC #$1F
        case 0x89B0: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA7C5:  PHA
        case 0x89B1: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LA7C6:  TYA
        case 0x89B2: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA7C7:  CLC
        case 0x89B3: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LA7C8:  ADC #$02
        case 0x89B5: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LA7CA:  TAY
        case 0x89B6: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA7CC:  INY
        case 0x89B7: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LA7CD:  TYA
        case 0x89B8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA7CE:  JSR GetDescPtr
        case 0x89BB: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA7D1:  PLA
        case 0x89BC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LA7D2:  BEQ --
        case 0x89BE: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA7D4:  JMP WndBuildTempBuf
        case 0x89C1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA7D7:  LDA WndDescHalf
        case 0x89C4: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA7DA:  EOR #$01
        case 0x89C6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LA7DC:  BNE +
        case 0x89C8: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA7DE:  INC WndThisDesc
        case 0x89CB: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA7E4:  RTS
        case 0x89CC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA7E5:  STA WorkTile
        case 0x89CF: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA7E8:  JMP BuildWndLine
        case 0x89D2: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA7EB:  PHA
        case 0x89D3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA7EC:  JSR ClearTempBuffer
        case 0x89D6: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA7EF:  PLA
        case 0x89D7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA7F0:  STA DescEntry
        case 0x89DA: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA7F2:  CMP #$FF
        case 0x89DC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LA7F4:  BEQ +
        case 0x89DE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA7F6:  LDA #$01
        case 0x89E0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA7F8:  JSR GetDescPtr
        case 0x89E3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA7FB:  LDA DescEntry
        case 0x89E6: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA7FD:  JMP WndBuildTempBuf
        case 0x89E9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA801:  STA DescEntry
        case 0x89EC: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LA803:  LDY #$07
        case 0x89EE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA805:  LDA WndDescHalf
        case 0x89F1: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA808:  LSR
        case 0x89F2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LA809:  BCC +
        case 0x89F4: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA80B:  INY
        case 0x89F5: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA80E:  PHA
        case 0x89F6: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA80F:  CMP #$33
        case 0x89F8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LA811:  BCC +
        case 0x89FA: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA813:  PLA
        case 0x89FB: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LA814:  SBC #$32
        case 0x89FD: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA816:  PHA
        case 0x89FE: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA817:  INY
        case 0x89FF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA819:  JSR GetDescPtr
        case 0x8A02: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA81C:  JSR ClearTempBuffer
        case 0x8A05: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA81F:  PLA
        case 0x8A06: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA820:  JMP WndBuildTempBuf
        case 0x8A09: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA823:  ASL
        case 0x8A0A: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LA824:  TAX
        case 0x8A0B: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x802A, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA825:  LDA DescPtrTbl,X
        case 0x8A0E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA828:  STA DescPtrLB
        case 0x8A11: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA82A:  LDA DescPtrTbl+1,X
        case 0x8A14: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA82D:  STA DescPtrUB
        case 0x8A17: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA82F:  RTS
        case 0x8A18: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LA842:  TAX
        case 0x8A19: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LA843:  LDY #$00
        case 0x8A1B: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LA845:  DEX
        case 0x8A1C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA846:  BEQ BaseDescFound
        case 0x8A1E: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA848:  LDA (DescPtr),Y
        case 0x8A20: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA84A:  CMP #$FF
        case 0x8A22: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA84C:  BEQ NextDescription
        case 0x8A24: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA84E:  INY
        case 0x8A25: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA84F:  BNE DescSrchInnerLoop
        case 0x8A27: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA851:  INC DescPtrUB
        case 0x8A2A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA853:  BNE DescSrchInnerLoop
        case 0x8A2C: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA855:  INY
        case 0x8A2D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA856:  BNE DescSrchOuterLoop
        case 0x8A2F: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA858:  INC DescPtrUB
        case 0x8A32: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA85A:  BNE DescSrchOuterLoop
        case 0x8A34: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA85D:  CLC
        case 0x8A35: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA85E:  ADC DescPtrLB
        case 0x8A38: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA860:  STA DescPtrLB
        case 0x8A3B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LA862:  BCC +
        case 0x8A3D: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA864:  INC DescPtrUB
        case 0x8A40: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LA868:  LDX SubBufLength
        case 0x8A43: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA86B:  LDA (DescPtr),Y
        case 0x8A45: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA86D:  CMP #$FF
        case 0x8A47: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LA86F:  BEQ +
        case 0x8A49: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LA871:  STA TempBuffer-1,X
        case 0x8A4C: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA874:  INY
        case 0x8A4D: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LA875:  DEX
        case 0x8A4E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA876:  BNE LoadDescLoop
        case 0x8A50: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA879:  JSR PrepPPUAdrCalc
        case 0x8A53: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA87C:  LDA WndHeight
        case 0x8A56: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA87F:  STA RowsRemaining
        case 0x8A59: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LA881:  LDY #$00
        case 0x8A5B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA883:  LDA WndWidth
        case 0x8A5E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA886:  STA _ColsRemaining
        case 0x8A61: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LA888:  LDA (PPUBufPtr),Y
        case 0x8A63: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA88A:  CMP #TXT_END2
        case 0x8A65: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA88C:  BNE CntrlNextCol
        case 0x8A67: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA88E:  LDA #TXT_NOP
        case 0x8A69: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LA890:  STA (PPUBufPtr),Y
        case 0x8A6B: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LA892:  INY
        case 0x8A6C: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA893:  DEC _ColsRemaining
        case 0x8A6F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA895:  BNE CntrlCharSwapCol
        case 0x8A71: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA897:  CLC
        case 0x8A72: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA898:  LDA PPUAddrLB
        case 0x8A75: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LA89A:  ADC #$20
        case 0x8A77: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA89C:  STA PPUAddrLB
        case 0x8A7A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LA89E:  BCC CntrlNextRow
        case 0x8A7C: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA8A0:  INC PPUAddrUB
        case 0x8A7F: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LA8A2:  DEC RowsRemaining
        case 0x8A82: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA8A4:  BNE CntrlCharSwapRow
        case 0x8A84: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LA8A6:  BRK
        case 0x8A85: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LA8A9:  JSR WaitForNMI
        case 0x8A88: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA8AC:  RTS
        case 0x8A89: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA8AD:  LDA WndColPos
        case 0x8A8C: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA8AF:  LSR
        case 0x8A8D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA8B0:  STA XPosFromLeft
        case 0x8A90: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA8B2:  LDA WndRowPos
        case 0x8A93: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA8B4:  LSR
        case 0x8A94: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA8B5:  STA YPosFromTop
        case 0x8A97: cycles_to_run -= 3; reg_PC = 0x0000; return; // LA8B7:  JMP CalcPPUBufAddr
        case 0x8A9A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA8BA:  LDA #$05
        case 0x8A9C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA8BC:  STA SubBufLength
        case 0x8A9F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA8BF:  LDA GoldLB
        case 0x8AA2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA8C1:  STA BCDByte0
        case 0x8AA5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA8C3:  LDA GoldUB
        case 0x8AA8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA8C5:  STA BCDByte1
        case 0x8AAB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA8C7:  LDA #$00
        case 0x8AAD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA8C9:  STA BCDByte2
        case 0x8AB0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA8CB:  JSR ConvertToBCD
        case 0x8AB3: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA8CE:  JMP ClearBCDLeadZeros
        case 0x8AB6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA8D1:  LDA WndBuildPhase
        case 0x8AB9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0x802A; return; } // LA8D4:  BMI WndDoSelectExit
        case 0x8ABB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA8D6:  JSR WndInitSelect
        case 0x8ABE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA8D9:  LDA #IN_RIGHT
        case 0x8AC0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA8DB:  STA WndBtnRetrig
        case 0x8AC3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA8DE:  STA JoypadBtns
        case 0x8AC6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA8E0:  JSR WndDoSelectLoop
        case 0x8AC9: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA8E3:  RTS
        case 0x8ACA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA8E4:  JSR WndGetButtons
        case 0x8ACD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA8E7:  JSR WndProcessInput
        case 0x8AD0: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA8EA:  JMP WndDoSelectLoop
        case 0x8AD3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LA8ED:  JSR WaitForNMI
        case 0x8AD6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LA8F0:  JSR UpdateCursorGFX
        case 0x8AD9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA8F3:  LDA JoypadBtns
        case 0x8ADC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA8F5:  BEQ SetRetrigger
        case 0x8ADE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA8F7:  LDA FrameCounter
        case 0x8AE1: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA8F9:  AND #$0F
        case 0x8AE3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA8FB:  BNE NoRetrigger
        case 0x8AE5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA8FD:  STA WndBtnRetrig
        case 0x8AE8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LA900:  JSR GetJoypadStatus
        case 0x8AEB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA903:  LDA WndBtnRetrig
        case 0x8AEE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA906:  BNE WndGetButtons
        case 0x8AF0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA908:  LDA WndBtnRetrig
        case 0x8AF3: cycles_to_run -= 2; reg_A &= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA90B:  AND JoypadBtns
        case 0x8AF6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA90D:  STA WndBtnRetrig
        case 0x8AF9: cycles_to_run -= 2; reg_A ^= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA910:  EOR JoypadBtns
        case 0x8AFC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA912:  STA WndBtnPresses
        case 0x8AFF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA915:  BEQ WndGetButtons
        case 0x8B01: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA917:  RTS
        case 0x8B02: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA918:  LDA #$00
        case 0x8B04: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA91A:  STA WndCol
        case 0x8B07: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA91C:  STA WndRow
        case 0x8B0A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA91E:  STA WndSelResults
        case 0x8B0D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA920:  STA WndCursorXPos
        case 0x8B10: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA923:  STA WndCursorYPos
        case 0x8B13: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA926:  STA WndBtnRetrig
        case 0x8B16: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA929:  LDA WndColumns
        case 0x8B19: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA92C:  LSR
        case 0x8B1A: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA92D:  LSR
        case 0x8B1B: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA92E:  LSR
        case 0x8B1C: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA92F:  LSR
        case 0x8B1D: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LA930:  TAX
        case 0x8B1E: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x802A, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LA931:  LDA NumColTbl,X
        case 0x8B21: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA934:  STA WndSelNumCols
        case 0x8B24: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA937:  LDA WindowType
        case 0x8B27: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA93A:  CMP #WND_MSG_SPEED
        case 0x8B29: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LA93C:  BNE WndSetCrsrHome
        case 0x8B2B: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LA93E:  LDX MessageSpeed
        case 0x8B2E: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LA940:  STX WndRow
        case 0x8B31: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LA942:  TXA
        case 0x8B32: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LA943:  ASL
        case 0x8B33: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA944:  STA WndCursorYPos
        case 0x8B36: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA947:  LDA WndCursorHome
        case 0x8B39: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LA94A:  PHA
        case 0x8B3A: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA94B:  AND #$0F
        case 0x8B3C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA94D:  STA WndUnused64F4
        case 0x8B3F: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA950:  CLC
        case 0x8B40: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA951:  ADC WndCursorXPos
        case 0x8B43: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA954:  STA WndCursorXPos
        case 0x8B46: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA957:  PLA
        case 0x8B47: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA958:  AND #$F0
        case 0x8B49: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA95A:  LSR
        case 0x8B4A: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA95B:  LSR
        case 0x8B4B: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA95C:  LSR
        case 0x8B4C: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA95D:  LSR
        case 0x8B4D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA95E:  STA WndCursorYHome
        case 0x8B50: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA961:  ADC WndCursorYPos
        case 0x8B53: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA964:  STA WndCursorYPos
        case 0x8B56: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA967:  LDA #$05
        case 0x8B58: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA969:  STA FrameCounter
        case 0x8B5B: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA96B:  RTS
        case 0x8B5C: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LA96C:  LDX #TL_BLANK_TILE1
        case 0x8B5E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA96E:  LDA FrameCounter
        case 0x8B61: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA970:  AND #$1F
        case 0x8B63: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA972:  CMP #$10
        case 0x8B65: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LA974:  BCS SetCursorTile
        case 0x8B67: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LA976:  LDX #TL_RIGHT_ARROW
        case 0x8B69: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LA978:  STX PPUDataByte
        case 0x8B6C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA97A:  LDA WndColPos
        case 0x8B6F: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA97C:  CLC
        case 0x8B70: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA97D:  ADC WndCursorXPos
        case 0x8B73: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA980:  STA ScrnTxtXCoord
        case 0x8B76: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA983:  LDA WndRowPos
        case 0x8B79: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LA985:  CLC
        case 0x8B7A: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LA986:  ADC WndCursorYPos
        case 0x8B7D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA989:  STA ScrnTxtYCoord
        case 0x8B80: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA98C:  JSR WndCalcPPUAddr
        case 0x8B83: cycles_to_run -= 3; reg_PC = 0x0000; return; // LA98F:  JMP AddPPUBufEntry
        case 0x8B86: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA992:  LDA WndBtnPresses
        case 0x8B89: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA995:  LSR
        case 0x8B8A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LA996:  BCS WndAPressed
        case 0x8B8C: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA998:  LSR
        case 0x8B8D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LA999:  BCS WndBPressed
        case 0x8B8F: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA99B:  LSR
        case 0x8B90: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA99C:  LSR
        case 0x8B91: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA99D:  LSR
        case 0x8B92: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LA99E:  BCS WndUpPressed
        case 0x8B94: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA9A0:  LSR
        case 0x8B95: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LA9A1:  BCS WndDownPressed
        case 0x8B97: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA9A3:  LSR
        case 0x8B98: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LA9A4:  BCS WndLeftPressed
        case 0x8B9A: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LA9A6:  LSR
        case 0x8B9B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LA9A7:  BCC WndEndUpPressed
        case 0x8B9D: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA9A9:  JMP WndRightPressed
        case 0x8BA0: cycles_to_run -= 3; reg_PC = 0x802A; return; // LA9AC:  JMP WndDoLeftPressed
        case 0x8BA3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA9AF:  LDA #IN_A
        case 0x8BA5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA9B1:  STA WndBtnRetrig
        case 0x8BA8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA9B4:  JSR WndUpdateCrsrPos
        case 0x8BAB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA9B7:  LDA #SFX_MENU_BTN
        case 0x8BAD: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LA9B9:  BRK
        case 0x8BAE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA9BC:  LDA WndCol
        case 0x8BB1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA9BE:  STA _WndCol
        case 0x8BB4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA9C0:  LDA WndRow
        case 0x8BB7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA9C2:  STA _WndRow
        case 0x8BBA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA9C4:  JSR WndCalcSelResult
        case 0x8BBD: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA9C7:  PLA
        case 0x8BBE: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA9C8:  PLA
        case 0x8BBF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA9C9:  LDA WndSelResults
        case 0x8BC2: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA9CB:  RTS
        case 0x8BC3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA9CC:  LDA #IN_B
        case 0x8BC5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA9CE:  STA WndBtnRetrig
        case 0x8BC8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA9D1:  JSR WndUpdateCrsrPos
        case 0x8BCB: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA9D4:  PLA
        case 0x8BCC: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LA9D5:  PLA
        case 0x8BCD: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA9D6:  LDA #WND_ABORT
        case 0x8BCF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA9D8:  STA WndSelResults
        case 0x8BD2: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA9DA:  RTS
        case 0x8BD3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA9DB:  LDA #IN_UP
        case 0x8BD5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA9DD:  STA WndBtnRetrig
        case 0x8BD8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA9E0:  LDA WndRow
        case 0x8BDB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA9E2:  BEQ WndEndUpPressed
        case 0x8BDD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA9E4:  JSR WndClearCursor
        case 0x8BE0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LA9E7:  LDA WindowType
        case 0x8BE3: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LA9EA:  CMP #WND_SPELL1
        case 0x8BE5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LA9EC:  BEQ WndSpell1Up
        case 0x8BE7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA9EE:  JSR WndMoveCursorUp
        case 0x8BEA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LA9F1:  JSR WndUpdateCrsrPos
        case 0x8BED: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LA9F4:  RTS
        case 0x8BEE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA9F5:  LDA #$03
        case 0x8BF0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA9F7:  STA WndCursorXPos
        case 0x8BF3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA9FA:  LDA #$02
        case 0x8BF5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LA9FC:  STA WndCursorYPos
        case 0x8BF8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LA9FF:  LDA #$00
        case 0x8BFA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA01:  STA WndRow
        case 0x8BFD: cycles_to_run -= 3; reg_PC = 0x802A; return; // LAA03:  JMP WndUpdateCrsrPos
        case 0x8C00: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAA06:  LDA #IN_DOWN
        case 0x8C02: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA08:  STA WndBtnRetrig
        case 0x8C05: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA0B:  LDA WindowType
        case 0x8C08: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAA0E:  CMP #WND_SPELL1
        case 0x8C0A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAA10:  BEQ WndSpell1Down
        case 0x8C0C: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAA12:  CMP #WND_MSG_SPEED
        case 0x8C0E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAA14:  BNE WndDownCont1
        case 0x8C10: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA16:  LDA WndRow
        case 0x8C13: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAA18:  CMP #$02
        case 0x8C15: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAA1A:  BEQ WndDownDone
        case 0x8C17: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LAA1C:  SEC
        case 0x8C18: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA1D:  LDA WndHeight
        case 0x8C1B: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LAA20:  SBC #$03
        case 0x8C1D: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAA22:  LSR
        case 0x8C1E: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LAA23:  CMP WndRow
        case 0x8C21: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAA25:  BEQ WndDownDone
        case 0x8C23: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAA27:  JSR WndClearCursor
        case 0x8C26: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA2A:  LDA WindowType
        case 0x8C29: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAA2D:  CMP #WND_ALPHBT
        case 0x8C2B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAA2F:  BNE WndDownCont2
        case 0x8C2D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAA31:  JSR WndSpclMoveCrsr
        case 0x8C30: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA34:  LDA WndCursorYPos
        case 0x8C33: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAA37:  BNE WndDownCont3
        case 0x8C35: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA39:  LDA WndCursorYHome
        case 0x8C38: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA3C:  STA WndCursorYPos
        case 0x8C3B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAA3F:  BNE WndDownUpdate
        case 0x8C3D: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAA41:  CLC
        case 0x8C3E: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LAA42:  ADC #$02
        case 0x8C40: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA44:  STA WndCursorYPos
        case 0x8C43: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAA47:  INC WndRow
        case 0x8C46: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAA49:  JSR WndUpdateCrsrPos
        case 0x8C49: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAA4C:  RTS
        case 0x8C4A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA4D:  LDA WndRow
        case 0x8C4D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAA4F:  CMP #$02
        case 0x8C4F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAA51:  BEQ WndDownDone
        case 0x8C51: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAA53:  JSR WndClearCursor
        case 0x8C54: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAA56:  LDA #$02
        case 0x8C56: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA58:  STA WndRow
        case 0x8C59: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAA5A:  LDA #$03
        case 0x8C5B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA5C:  STA WndCursorXPos
        case 0x8C5E: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAA5F:  LDA #$06
        case 0x8C60: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA61:  STA WndCursorYPos
        case 0x8C63: cycles_to_run -= 3; reg_PC = 0x802A; return; // LAA64:  JMP WndUpdateCrsrPos
        case 0x8C66: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAA67:  LDA #IN_LEFT
        case 0x8C68: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA69:  STA WndBtnRetrig
        case 0x8C6B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA6C:  LDA WindowType
        case 0x8C6E: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAA6F:  CMP #WND_SPELL1
        case 0x8C70: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAA71:  BEQ WndSpell1Left
        case 0x8C72: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA73:  LDA WndCol
        case 0x8C75: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAA75:  BEQ WndLeftDone
        case 0x8C77: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA77:  LDA WindowType
        case 0x8C7A: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAA7A:  CMP #WND_ALPHBT
        case 0x8C7C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAA7C:  BNE WndLeftUpdate
        case 0x8C7E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA7E:  LDA WndRow
        case 0x8C81: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAA80:  CMP #$05
        case 0x8C83: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAA82:  BNE WndLeftUpdate
        case 0x8C85: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA84:  LDA WndCol
        case 0x8C88: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAA86:  CMP #$09
        case 0x8C8A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAA88:  BNE WndLeftUpdate
        case 0x8C8C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAA8A:  LDA #$06
        case 0x8C8E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA8C:  STA WndCol
        case 0x8C91: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAA8E:  JSR WndClearCursor
        case 0x8C94: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAA91:  LDA #$0D
        case 0x8C96: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAA93:  BNE WndLeftUpdtFinish
        case 0x8C98: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAA95:  JSR WndClearCursor
        case 0x8C9B: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAA98:  DEC WndCol
        case 0x8C9E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAA9A:  LDA WndColumns
        case 0x8CA1: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAA9D:  AND #$0F
        case 0x8CA3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAA9F:  STA WndColLB
        case 0x8CA6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAAA1:  LDA WndCursorXPos
        case 0x8CA9: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LAAA4:  SEC
        case 0x8CAA: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LAAA5:  SBC WndColLB
        case 0x8CAD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAAA7:  STA WndCursorXPos
        case 0x8CB0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAAAA:  JSR WndUpdateCrsrPos
        case 0x8CB3: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAAAD:  RTS
        case 0x8CB4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAAAE:  LDA WndRow
        case 0x8CB7: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAAB0:  CMP #$03
        case 0x8CB9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAAB2:  BEQ WndLeftDone
        case 0x8CBB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAAB4:  JSR WndClearCursor
        case 0x8CBE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAAB7:  LDA #$03
        case 0x8CC0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAAB9:  STA WndRow
        case 0x8CC3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAABB:  LDA #$01
        case 0x8CC5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAABD:  STA WndCursorXPos
        case 0x8CC8: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAAC0:  LDA #$04
        case 0x8CCA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAAC2:  STA WndCursorYPos
        case 0x8CCD: cycles_to_run -= 3; reg_PC = 0x802A; return; // LAAC5:  JMP WndUpdateCrsrPos
        case 0x8CD0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAAC8:  LDA #IN_RIGHT
        case 0x8CD2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAACA:  STA WndBtnRetrig
        case 0x8CD5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAACD:  LDA WindowType
        case 0x8CD8: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAAD0:  CMP #WND_SPELL1
        case 0x8CDA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAAD2:  BEQ WndSpell1Right
        case 0x8CDC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAAD4:  LDA WndColumns
        case 0x8CDF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAAD7:  BEQ WndEndRghtPressed
        case 0x8CE1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAAD9:  LDA WindowType
        case 0x8CE4: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAADC:  CMP #WND_ALPHBT
        case 0x8CE6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAADE:  BNE WndRightCont1
        case 0x8CE8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAAE0:  LDA WndRow
        case 0x8CEB: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAAE2:  CMP #$05
        case 0x8CED: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAAE4:  BNE WndRightCont1
        case 0x8CEF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAAE6:  LDA WndCol
        case 0x8CF2: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAAE8:  CMP #$06
        case 0x8CF4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LAAEA:  BCC WndRightCont1
        case 0x8CF6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAAEC:  BNE WndEndRghtPressed
        case 0x8CF8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAAEE:  JSR WndClearCursor
        case 0x8CFB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAAF1:  LDA #$09
        case 0x8CFD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAAF3:  STA WndCol
        case 0x8D00: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAAF5:  LDA #$13
        case 0x8D02: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAAF7:  BNE WndRightUpdtFinish
        case 0x8D04: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LAAF9:  LDX WndSelNumCols
        case 0x8D07: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LAAFC:  DEX
        case 0x8D08: cycles_to_run -= 2; update_flags_cmp(reg_X, bus_read(0x0000)); reg_PC += 3; return; // LAAFD:  CPX WndCol
        case 0x8D0B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAAFF:  BEQ WndEndRghtPressed
        case 0x8D0D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAB01:  JSR WndClearCursor
        case 0x8D10: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAB04:  INC WndCol
        case 0x8D13: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAB06:  LDA WndColumns
        case 0x8D16: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAB09:  AND #$0F
        case 0x8D18: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAB0B:  CLC
        case 0x8D19: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LAB0C:  ADC WndCursorXPos
        case 0x8D1C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB0F:  STA WndCursorXPos
        case 0x8D1F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAB12:  JSR WndUpdateCrsrPos
        case 0x8D22: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAB15:  RTS
        case 0x8D23: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAB16:  LDA WndRow
        case 0x8D26: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAB18:  CMP #$01
        case 0x8D28: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAB1A:  BEQ WndEndRghtPressed
        case 0x8D2A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAB1C:  JSR WndClearCursor
        case 0x8D2D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAB1F:  LDA #$01
        case 0x8D2F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB21:  STA WndRow
        case 0x8D32: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAB23:  LDA #$07
        case 0x8D34: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB25:  STA WndCursorXPos
        case 0x8D37: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAB28:  LDA #$04
        case 0x8D39: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB2A:  STA WndCursorYPos
        case 0x8D3C: cycles_to_run -= 3; reg_PC = 0x802A; return; // LAB2D:  JMP WndUpdateCrsrPos
        case 0x8D3F: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LAB30:  LDX #TL_BLANK_TILE1
        case 0x8D41: cycles_to_run -= 3; reg_PC = 0x802A; return; // LAB32:  JMP SetCursorTile
        case 0x8D44: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAB35:  LDA #$05
        case 0x8D46: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB37:  STA FrameCounter
        case 0x8D49: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LAB39:  JSR ArrowCursorGFX
        case 0x8D4C: cycles_to_run -= 3; reg_PC = 0x0000; return; // LAB3C:  JMP WaitForNMI
        case 0x8D4F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAB3F:  LDA WndRow
        case 0x8D52: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAB41:  CMP #$04
        case 0x8D54: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAB43:  BNE WndEndUpdateCrsr
        case 0x8D56: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAB45:  LDA WndCol
        case 0x8D59: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAB47:  CMP #$07
        case 0x8D5B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAB49:  BEQ WndSetCrsrBack
        case 0x8D5D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAB4B:  CMP #$08
        case 0x8D5F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LAB4D:  BCC WndEndUpdateCrsr
        case 0x8D61: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAB4F:  LDA #$09
        case 0x8D63: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB51:  STA WndCol
        case 0x8D66: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAB53:  LDA #$13
        case 0x8D68: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB55:  STA WndCursorXPos
        case 0x8D6B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAB58:  BNE WndEndUpdateCrsr
        case 0x8D6D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAB5A:  LDA #$06
        case 0x8D6F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB5C:  STA WndCol
        case 0x8D72: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAB5E:  LDA #$0D
        case 0x8D74: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB60:  STA WndCursorXPos
        case 0x8D77: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAB63:  RTS
        case 0x8D78: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAB64:  LDA WindowType
        case 0x8D7B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAB67:  CMP #WND_ALPHBT
        case 0x8D7D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAB69:  BEQ WndCalcAlphaResult
        case 0x8D7F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAB6B:  LDA _WndCol
        case 0x8D82: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB6D:  STA WndColLB
        case 0x8D85: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAB6F:  LDA #$00
        case 0x8D87: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB71:  STA WndColUB
        case 0x8D8A: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LAB73:  SEC
        case 0x8D8B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAB74:  LDA WndHeight
        case 0x8D8E: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LAB77:  SBC #$03
        case 0x8D90: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAB79:  LSR
        case 0x8D91: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LAB7A:  TAX
        case 0x8D92: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LAB7B:  INX
        case 0x8D93: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LAB7C:  TXA
        case 0x8D94: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LAB7D:  LDX #WndColLB
        case 0x8D96: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAB7F:  JSR IndexedMult
        case 0x8D99: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAB82:  LDA WndColLB
        case 0x8D9C: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAB84:  CLC
        case 0x8D9D: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LAB85:  ADC _WndRow
        case 0x8DA0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB87:  STA WndSelResults
        case 0x8DA3: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAB89:  RTS
        case 0x8DA4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAB8A:  LDA _WndRow
        case 0x8DA7: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LAB8C:  LDX WndColumns
        case 0x8DAA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAB8F:  BEQ WndSetAlphaResult
        case 0x8DAC: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAB91:  AND #$0F
        case 0x8DAE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB93:  STA WndColLB
        case 0x8DB1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAB95:  LDA #$00
        case 0x8DB3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAB97:  STA WndColUB
        case 0x8DB6: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LAB99:  LDX #WndColLB
        case 0x8DB8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAB9B:  LDA WndSelNumCols
        case 0x8DBB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAB9E:  JSR IndexedMult
        case 0x8DBE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LABA1:  LDA WndColLB
        case 0x8DC1: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LABA3:  CLC
        case 0x8DC2: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LABA4:  ADC _WndCol
        case 0x8DC5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LABA6:  STA WndSelResults
        case 0x8DC8: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LABA8:  RTS
        case 0x8DC9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LABA9:  LDA WndCol
        case 0x8DCC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LABAB:  STA _WndCol
        case 0x8DCF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LABAD:  LDA WndRow
        case 0x8DD2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LABAF:  STA _WndRow
        case 0x8DD5: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LABB1:  RTS
        case 0x8DD6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LABB2:  LDA WndCursorYPos
        case 0x8DD9: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LABB5:  SEC
        case 0x8DDA: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LABB6:  SBC #$02
        case 0x8DDC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LABB8:  STA WndCursorYPos
        case 0x8DDF: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LABBB:  DEC WndRow
        case 0x8DE2: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LABBD:  RTS
        case 0x8DE3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LABC0:  LDA #$00
        case 0x8DE5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LABC2:  BNE WndShowHide+2
        case 0x8DE7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LABC4:  LDA #$00
        case 0x8DE9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LABC6:  JSR WndDoRow
        case 0x8DEC: cycles_to_run -= 3; reg_PC = 0x802A; return; // LABC9:  JMP WndUpdateTiles
        case 0x8DEF: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LABCC:  PHA
        case 0x8DF0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LABD0:  BEQ WndDoRowReady
        case 0x8DF2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LABD2:  JSR WndUpdateTiles
        case 0x8DF5: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LABD5:  LDA #$00
        case 0x8DF7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LABD7:  STA WndUnused64AB
        case 0x8DFA: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LABDA:  PLA
        case 0x8DFB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LABDB:  JSR WndStartRow
        case 0x8DFE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LABDE:  LDA #$00
        case 0x8E00: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LABE0:  STA WndLineBufIndex
        case 0x8E03: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LABE3:  STA WndAtrbBufIndex
        case 0x8E06: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LABE6:  LDA WndWidthTemp
        case 0x8E09: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LABE9:  PHA
        case 0x8E0A: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LABEA:  AND #$F0
        case 0x8E0C: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LABEC:  LSR
        case 0x8E0D: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LABED:  LSR
        case 0x8E0E: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LABEE:  LSR
        case 0x8E0F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LABEF:  STA WndBlkTileRow
        case 0x8E12: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LABF2:  PLA
        case 0x8E13: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LABF3:  AND #$0F
        case 0x8E15: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LABF5:  ASL
        case 0x8E16: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LABF6:  STA _WndWidth
        case 0x8E19: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LABF9:  STA WndUnused64AE
        case 0x8E1C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LABFF:  LDA PPUAddrUB
        case 0x8E1F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC01:  STA WndPPUAddrUB
        case 0x8E22: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC04:  LDA PPUAddrLB
        case 0x8E25: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC06:  STA WndPPUAddrLB
        case 0x8E28: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAC09:  AND #$1F
        case 0x8E2A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC0B:  STA WndNTRowOffset
        case 0x8E2D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAC0E:  LDA #$20
        case 0x8E2F: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LAC10:  SEC
        case 0x8E30: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LAC11:  SBC WndNTRowOffset
        case 0x8E33: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC14:  STA WndThisNTRow
        case 0x8E36: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC17:  LDA _WndWidth
        case 0x8E39: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LAC1A:  SEC
        case 0x8E3A: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LAC1B:  SBC WndThisNTRow
        case 0x8E3D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC1E:  STA WndNextNTRow
        case 0x8E40: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAC21:  BEQ WndNoCrossNT
        case 0x8E42: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LAC23:  BCS WndCrossNT
        case 0x8E44: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC25:  LDA _WndWidth
        case 0x8E47: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC28:  STA WndThisNTRow
        case 0x8E4A: cycles_to_run -= 3; reg_PC = 0x802A; return; // LAC2B:  JMP WndSingleNT
        case 0x8E4D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAC2E:  JSR WndLoadRowBuf
        case 0x8E50: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC31:  LDA WndPPUAddrUB
        case 0x8E53: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAC34:  EOR #$04
        case 0x8E55: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC36:  STA WndPPUAddrUB
        case 0x8E58: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC39:  LDA WndPPUAddrLB
        case 0x8E5B: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAC3C:  AND #$1F
        case 0x8E5D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC3E:  STA WndNTRowOffset
        case 0x8E60: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC41:  LDA WndPPUAddrLB
        case 0x8E63: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LAC44:  SEC
        case 0x8E64: cycles_to_run -= 2; cpu_sbc(bus_read(0x0000)); reg_PC += 3; return; // LAC45:  SBC WndNTRowOffset
        case 0x8E67: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC48:  STA WndPPUAddrLB
        case 0x8E6A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC4B:  LDA WndNextNTRow
        case 0x8E6D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC4E:  STA WndThisNTRow
        case 0x8E70: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAC51:  JSR WndLoadRowBuf
        case 0x8E73: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC54:  LDA PPUAddrUB
        case 0x8E76: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAC56:  AND #$FB
        case 0x8E78: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAC58:  CMP #$23
        case 0x8E7A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LAC5A:  BCC WndIncPPURow
        case 0x8E7C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC5C:  LDA PPUAddrLB
        case 0x8E7F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAC5E:  CMP #$A0
        case 0x8E81: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LAC60:  BCC WndIncPPURow
        case 0x8E83: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAC62:  AND #$1F
        case 0x8E85: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC64:  STA PPUAddrLB
        case 0x8E88: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC66:  LDA PPUAddrUB
        case 0x8E8B: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAC68:  AND #$FC
        case 0x8E8D: cycles_to_run -= 3; reg_PC = 0x802A; return; // LAC6A:  JMP UpdateNTAddr
        case 0x8E90: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC6D:  LDA PPUAddrLB
        case 0x8E93: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAC6F:  CLC
        case 0x8E94: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LAC70:  ADC #$20
        case 0x8E96: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC72:  STA PPUAddrLB
        case 0x8E99: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC74:  LDA PPUAddrUB
        case 0x8E9C: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LAC76:  ADC #$00
        case 0x8E9E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAC78:  STA PPUAddrUB
        case 0x8EA1: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAC7A:  DEC WndBlkTileRow
        case 0x8EA4: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAC7D:  BNE WndRowLoop
        case 0x8EA6: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAC82:  RTS
        case 0x8EA7: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC83:  LDA WndPPUAddrUB
        case 0x8EAA: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAC86:  ORA #$80
        case 0x8EAC: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAC88:  STA BlockRAM,X
        case 0x8EAF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC8B:  LDA WndThisNTRow
        case 0x8EB2: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAC8E:  STA BlockRAM+1,X
        case 0x8EB5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC91:  LDA WndPPUAddrLB
        case 0x8EB8: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAC94:  STA BlockRAM+2,X
        case 0x8EBB: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LAC97:  INX
        case 0x8EBC: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LAC98:  INX
        case 0x8EBD: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LAC99:  INX
        case 0x8EBE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAC9A:  LDA WndThisNTRow
        case 0x8EC1: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LAC9D:  PHA
        case 0x8EC2: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LAC9E:  LDY WndLineBufIndex
        case 0x8EC5: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LACA1:  LDA WndLineBuf,Y
        case 0x8EC8: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LACA4:  STA BlockRAM,X
        case 0x8ECB: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LACA7:  INX
        case 0x8ECC: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LACA8:  INY
        case 0x8ECD: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LACA9:  DEC WndThisNTRow
        case 0x8ED0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LACAC:  BNE WndBufLoadLoop
        case 0x8ED2: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LACAE:  STY WndLineBufIndex
        case 0x8ED5: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LACB1:  PLA
        case 0x8ED6: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LACB2:  LSR
        case 0x8ED7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LACB3:  STA WndThisNTRow
        case 0x8EDA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LACB6:  LDA WndBlkTileRow
        case 0x8EDD: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LACB9:  AND #$01
        case 0x8EDF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LACBB:  BEQ WndLoadRowBufEnd
        case 0x8EE1: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LACBD:  LDY WndAtrbBufIndex
        case 0x8EE4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LACC0:  LDA WndPPUAddrUB
        case 0x8EE7: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LACC3:  STA _WndPPUAddrUB
        case 0x8EEA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LACC6:  LDA WndPPUAddrLB
        case 0x8EED: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LACC9:  STA _WndPPUAddrLB
        case 0x8EF0: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LACCC:  TXA
        case 0x8EF1: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LACCD:  PHA
        case 0x8EF2: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LACCE:  TYA
        case 0x8EF3: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LACCF:  PHA
        case 0x8EF4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LACD0:  LDA WndPPUAddrUB
        case 0x8EF7: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LACD3:  PHA
        case 0x8EF8: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LACD4:  LDA AttribTblBuf,Y
        case 0x8EFB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LACD7:  JSR WndCalcAttribAddr
        case 0x8EFE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LACDA:  STA WndAtribDat
        case 0x8F01: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LACDD:  PLA
        case 0x8F02: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LACDE:  STA WndPPUAddrUB
        case 0x8F05: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LACE1:  PLA
        case 0x8F06: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LACE2:  TAY
        case 0x8F07: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LACE3:  PLA
        case 0x8F08: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LACE4:  TAX
        case 0x8F09: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LACE5:  LDA WndAtribAdrUB
        case 0x8F0C: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LACE8:  STA BlockRAM,X
        case 0x8F0F: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LACEB:  INX
        case 0x8F10: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LACEC:  LDA WndAtribAdrLB
        case 0x8F13: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LACEF:  STA BlockRAM,X
        case 0x8F16: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LACF2:  INX
        case 0x8F17: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LACF3:  LDA WndAtribDat
        case 0x8F1A: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LACF6:  STA BlockRAM,X
        case 0x8F1D: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LACF9:  INX
        case 0x8F1E: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LACFA:  INY
        case 0x8F1F: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LACFB:  INC _WndPPUAddrLB
        case 0x8F22: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LACFE:  INC _WndPPUAddrLB
        case 0x8F25: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAD04:  DEC WndThisNTRow
        case 0x8F28: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAD07:  BNE WndLoadAttribLoop
        case 0x8F2A: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LAD09:  STY WndAtrbBufIndex
        case 0x8F2D: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAD0F:  RTS
        case 0x8F2E: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LAD10:  PHA
        case 0x8F2F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAD11:  JSR WndGetRowStartPos
        case 0x8F32: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LAD14:  PLA
        case 0x8F33: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAD15:  BNE WndNTSwap
        case 0x8F35: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAD17:  RTS
        case 0x8F36: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD18:  LDA PPUAddrUB
        case 0x8F39: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAD1A:  EOR #$04
        case 0x8F3B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD1C:  STA PPUAddrUB
        case 0x8F3E: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAD1E:  RTS
        case 0x8F3F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD1F:  LDA _WndPosition
        case 0x8F42: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LAD22:  ASL
        case 0x8F43: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAD23:  AND #$1E
        case 0x8F45: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD25:  STA ScrnTxtXCoord
        case 0x8F48: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD28:  LDA _WndPosition
        case 0x8F4B: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAD2B:  LSR
        case 0x8F4C: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAD2C:  LSR
        case 0x8F4D: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAD2D:  LSR
        case 0x8F4E: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAD2E:  AND #$1E
        case 0x8F50: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD30:  STA ScrnTxtYCoord
        case 0x8F53: cycles_to_run -= 3; reg_PC = 0x802A; return; // LAD33:  JMP WndCalcPPUAddr
        case 0x8F56: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD36:  STA WndAttribVal
        case 0x8F59: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAD39:  LDA #$1F
        case 0x8F5B: cycles_to_run -= 2; reg_A &= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD3B:  AND _WndPPUAddrLB
        case 0x8F5E: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAD3E:  LSR
        case 0x8F5F: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAD3F:  LSR
        case 0x8F60: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD40:  STA WndPPUAddrUB
        case 0x8F63: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAD43:  LDA #$80
        case 0x8F65: cycles_to_run -= 2; reg_A &= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD45:  AND _WndPPUAddrLB
        case 0x8F68: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAD48:  LSR
        case 0x8F69: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAD49:  LSR
        case 0x8F6A: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAD4A:  LSR
        case 0x8F6B: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAD4B:  LSR
        case 0x8F6C: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD4C:  ORA WndPPUAddrUB
        case 0x8F6F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD4F:  STA WndPPUAddrUB
        case 0x8F72: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAD52:  LDA #$03
        case 0x8F74: cycles_to_run -= 2; reg_A &= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD54:  AND _WndPPUAddrUB
        case 0x8F77: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LAD57:  ASL
        case 0x8F78: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LAD58:  ASL
        case 0x8F79: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LAD59:  ASL
        case 0x8F7A: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LAD5A:  ASL
        case 0x8F7B: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAD5B:  ORA #$C0
        case 0x8F7D: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD5D:  ORA WndPPUAddrUB
        case 0x8F80: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD60:  STA WndAtribAdrLB
        case 0x8F83: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LAD63:  LDX #AT_ATRBTBL0_UB
        case 0x8F85: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD65:  LDA _WndPPUAddrUB
        case 0x8F88: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAD68:  CMP #NT_NAMETBL1_UB
        case 0x8F8A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LAD6A:  BCC WndSetAtribUB
        case 0x8F8C: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LAD6C:  LDX #AT_ATRBTBL1_UB
        case 0x8F8E: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LAD6E:  STX WndAtribAdrUB
        case 0x8F91: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD71:  LDA _WndPPUAddrLB
        case 0x8F94: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAD74:  AND #$40
        case 0x8F96: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAD76:  LSR
        case 0x8F97: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAD77:  LSR
        case 0x8F98: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAD78:  LSR
        case 0x8F99: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LAD79:  LSR
        case 0x8F9A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD7A:  STA AtribBitsOfst
        case 0x8F9D: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD7D:  LDA _WndPPUAddrLB
        case 0x8FA0: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAD80:  AND #$02
        case 0x8FA2: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD82:  ORA AtribBitsOfst
        case 0x8FA5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD85:  STA AtribBitsOfst
        case 0x8FA8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD88:  LDA WndAtribAdrLB
        case 0x8FAB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD8B:  STA AttribPtrLB
        case 0x8FAE: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD8D:  LDA WndAtribAdrUB
        case 0x8FB1: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAD90:  AND #$07
        case 0x8FB3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD92:  STA AttribPtrUB
        case 0x8FB6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAD94:  LDA EnNumber
        case 0x8FB9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAD96:  CMP #EN_DRAGONLORD2
        case 0x8FBB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAD98:  BNE ModAtribByte
        case 0x8FBD: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAD9A:  LDA #$07
        case 0x8FBF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAD9C:  STA AttribPtrUB
        case 0x8FC2: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LAD9E:  LDY #$00
        case 0x8FC4: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LADA0:  LDA (AttribPtr),Y
        case 0x8FC6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LADA2:  STA AttribByte
        case 0x8FC9: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LADA5:  LDA #$03
        case 0x8FCB: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LADA7:  LDY AtribBitsOfst
        case 0x8FCE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LADAA:  BEQ AddNewAtribVal
        case 0x8FD0: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LADAC:  ASL
        case 0x8FD1: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_asl(bus_read(a))); } reg_PC += 3; return; // LADAD:  ASL WndAttribVal
        case 0x8FD4: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LADB0:  DEY
        case 0x8FD5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LADB1:  BNE AtribValShiftLoop
        case 0x8FD7: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LADB3:  EOR #$FF
        case 0x8FD9: cycles_to_run -= 2; reg_A &= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LADB5:  AND AttribByte
        case 0x8FDC: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LADB8:  ORA WndAttribVal
        case 0x8FDF: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LADBB:  LDY #$00
        case 0x8FE1: cycles_to_run -= 3; bus_write(read_pointer_indexed_y(0x0000, nullptr), reg_A); reg_PC += 2; return; // LADBD:  STA (AttribPtr),Y
        case 0x8FE3: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LADBF:  RTS
        case 0x8FE4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LADC0:  LDA ActiveNmTbl
        case 0x8FE7: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LADC2:  ASL
        case 0x8FE8: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LADC3:  ASL
        case 0x8FE9: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LADC4:  AND #$04
        case 0x8FEB: cycles_to_run -= 2; reg_A |= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LADC6:  ORA #$20
        case 0x8FED: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LADC8:  STA PPUAddrUB
        case 0x8FF0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LADCA:  LDA ScrnTxtXCoord
        case 0x8FF3: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LADCD:  ASL
        case 0x8FF4: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LADCE:  ASL
        case 0x8FF5: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LADCF:  ASL
        case 0x8FF6: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LADD0:  CLC
        case 0x8FF7: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LADD1:  ADC ScrollX
        case 0x8FFA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LADD3:  STA PPUAddrLB
        case 0x8FFD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LADD5:  BCC WndAddY
        case 0x8FFF: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LADD7:  LDA PPUAddrUB
        case 0x9002: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LADD9:  EOR #$04
        case 0x9004: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LADDB:  STA PPUAddrUB
        case 0x9007: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LADDD:  LDA ScrollY
        case 0x900A: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LADDF:  LSR
        case 0x900B: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LADE0:  LSR
        case 0x900C: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LADE1:  LSR
        case 0x900D: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LADE2:  CLC
        case 0x900E: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LADE3:  ADC ScrnTxtYCoord
        case 0x9011: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LADE6:  CMP #$1E
        case 0x9013: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LADE8:  BCC WndAddrCombine
        case 0x9015: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LADEA:  SBC #$1E
        case 0x9017: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LADEC:  LSR
        case 0x9018: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LADED:  ROR PPUAddrLB
        case 0x901B: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LADEF:  LSR
        case 0x901C: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LADF0:  ROR PPUAddrLB
        case 0x901F: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LADF2:  LSR
        case 0x9020: cycles_to_run -= 2; { uint16_t a = 0x0000; bus_write(a, cpu_ror(bus_read(a))); } reg_PC += 3; return; // LADF3:  ROR PPUAddrLB
        case 0x9023: cycles_to_run -= 2; reg_A |= bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LADF5:  ORA PPUAddrUB
        case 0x9026: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LADF7:  STA PPUAddrUB
        case 0x9029: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LADF9:  RTS
        case 0x902A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LADFA:  LDA #$80
        case 0x902C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LADFC:  STA UpdateBGTiles
        case 0x902F: cycles_to_run -= 3; reg_PC = 0x0000; return; // LADFF:  JMP WaitForNMI
        case 0x9032: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAE02:  JSR InitNameWindow
        case 0x9035: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAE05:  JSR WndShowUnderscore
        case 0x9038: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAE08:  JSR WndDoSelect
        case 0x903B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAE0B:  JSR WndProcessChar
        case 0x903E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAE0E:  JSR WndMaxNameLength
        case 0x9041: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LAE11:  BCS WndStorePlyrName
        case 0x9043: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAE13:  JSR _WndDoSelectLoop
        case 0x9046: cycles_to_run -= 3; reg_PC = 0x802A; return; // LAE16:  JMP ProcessNameLoop
        case 0x9049: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LAE19:  LDX #$00
        case 0x904B: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LAE1B:  LDA TempBuffer,X
        case 0x904E: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAE1E:  STA DispName0,X
        case 0x9051: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LAE20:  LDA TempBuffer+4,X
        case 0x9054: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAE23:  STA DispName4,X
        case 0x9057: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LAE26:  INX
        case 0x9058: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LAE27:  CPX #$04
        case 0x905A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAE29:  BNE StoreNameLoop
        case 0x905C: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAE2B:  RTS
        case 0x905D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAE2C:  LDA #$00
        case 0x905F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAE2E:  STA WndNameIndex
        case 0x9062: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAE31:  STA WndUnused6505
        case 0x9065: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAE34:  LDA #WND_NM_ENTRY
        case 0x9067: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAE36:  JSR ShowWindow
        case 0x906A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAE39:  LDA #WND_ALPHBT
        case 0x906C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAE3B:  JSR ShowWindow
        case 0x906F: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAE3E:  LDA #$12
        case 0x9071: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAE40:  STA WndColumns
        case 0x9074: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAE43:  LDA #$21
        case 0x9076: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAE45:  STA WndCursorHome
        case 0x9079: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAE48:  LDA #TL_BLANK_TILE2
        case 0x907B: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LAE4A:  LDX #$0C
        case 0x907D: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAE4C:  STA TempBuffer,X
        case 0x9080: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LAE4F:  DEX
        case 0x9081: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x802A; return; } // LAE50:  BPL ClearNameBufLoop
        case 0x9083: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAE52:  RTS
        case 0x9084: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAE53:  CMP #WND_ABORT
        case 0x9086: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAE55:  BEQ WndDoBackspace
        case 0x9088: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAE57:  CMP #$1A
        case 0x908A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LAE59:  BCC WndUprCaseConvert
        case 0x908C: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAE5B:  CMP #$21
        case 0x908E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LAE5D:  BCC WndSymbConvert1
        case 0x9090: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAE5F:  CMP #$3B
        case 0x9092: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LAE61:  BCC WndLwrCaseConvert
        case 0x9094: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAE63:  CMP #$3D
        case 0x9096: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LAE65:  BCC WndSymbConvert2
        case 0x9098: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAE67:  CMP #$3D
        case 0x909A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAE69:  BEQ WndDoBackspace
        case 0x909C: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAE6B:  LDA #$08
        case 0x909E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAE6D:  STA WndNameIndex
        case 0x90A1: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAE70:  RTS
        case 0x90A2: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAE71:  CLC
        case 0x90A3: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LAE72:  ADC #TXT_UPR_A
        case 0x90A5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAE74:  BNE WndUpdateName
        case 0x90A7: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LAE76:  SEC
        case 0x90A8: cycles_to_run -= 2; cpu_sbc((uint8_t)0x0000); reg_PC += 2; return; // LAE77:  SBC #$17
        case 0x90AA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAE79:  BNE WndUpdateName
        case 0x90AC: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LAE7B:  TAX
        case 0x90AD: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LAE7C:  LDA SymbolConvTbl-$1A,X
        case 0x90B0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAE7F:  BNE WndUpdateName
        case 0x90B2: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LAE81:  TAX
        case 0x90B3: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LAE82:  LDA SymbolConvTbl-$34,X
        case 0x90B6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAE85:  BNE WndUpdateName
        case 0x90B8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAE87:  LDA WndNameIndex
        case 0x90BB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAE8A:  BEQ WndProcessCharEnd1
        case 0x90BD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAE8C:  JSR WndHideUnderscore
        case 0x90C0: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAE8F:  DEC WndNameIndex
        case 0x90C3: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAE92:  JSR WndShowUnderscore
        case 0x90C6: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAE95:  RTS
        case 0x90C7: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LAE96:  PHA
        case 0x90C8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAE97:  JSR WndHideUnderscore
        case 0x90CB: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LAE9A:  PLA
        case 0x90CC: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LAE9B:  LDX WndNameIndex
        case 0x90CF: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LAE9E:  STA TempBuffer,X
        case 0x90D2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LAEA1:  JSR WndNameCharYPos
        case 0x90D5: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LAEA4:  INC WndNameIndex
        case 0x90D8: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAEA7:  LDA WndNameIndex
        case 0x90DB: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAEAA:  CMP #$08
        case 0x90DD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LAEAC:  BCS WndProcessCharEnd2
        case 0x90DF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAEAE:  JSR WndShowUnderscore
        case 0x90E2: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAEB1:  RTS
        case 0x90E3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAEB2:  LDA WndNameIndex
        case 0x90E6: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAEB5:  CMP #$08
        case 0x90E8: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAEB7:  RTS
        case 0x90E9: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAEB8:  LDA #TL_TOP1
        case 0x90EB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LAEBA:  BNE WndUndrscrYPos
        case 0x90ED: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAEBC:  LDA #TL_BLANK_TILE1
        case 0x90EF: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LAEBE:  LDX #$09
        case 0x90F1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LAEC0:  BNE WndShowNameChar
        case 0x90F3: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LAEC2:  LDX #$08
        case 0x90F5: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LAEC4:  STX ScrnTxtYCoord
        case 0x90F8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAEC7:  STA PPUDataByte
        case 0x90FB: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LAEC9:  LDA WndNameIndex
        case 0x90FE: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LAECC:  CLC
        case 0x90FF: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LAECD:  ADC #$0C
        case 0x9101: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAECF:  STA ScrnTxtXCoord
        case 0x9104: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAED2:  JSR WndCalcPPUAddr
        case 0x9107: cycles_to_run -= 3; reg_PC = 0x0000; return; // LAED5:  JMP AddPPUBufEntry
        case 0x910A: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LAEE1:  PHA
        case 0x910B: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LAEE2:  LDX #$40
        case 0x910D: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LAEE4:  STX WndBuildPhase
        case 0x9110: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LAEE7:  LDX #$03
        case 0x9112: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LAEEC:  BEQ +
        case 0x9114: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LAEEE:  DEX
        case 0x9115: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LAEEF:  BPL -
        case 0x9117: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0x0000; return; } // LAEF1:  BMI ++
        case 0x9119: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAEF5:  STA WndBuildPhase
        case 0x911C: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LAEF9:  PHA
        case 0x911D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAEFA:  CMP #WND_CMD_NONCMB
        case 0x911F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LAEFC:  BEQ DoBeepSFX
        case 0x9121: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAEFE:  CMP #WND_CMD_CMB
        case 0x9123: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LAF00:  BEQ DoBeepSFX
        case 0x9125: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAF02:  CMP #WND_YES_NO1
        case 0x9127: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LAF04:  BEQ DoConfirmSFX
        case 0x9129: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAF06:  CMP #WND_DIALOG
        case 0x912B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LAF08:  BNE +
        case 0x912D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF0A:  LDA #$00
        case 0x912F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF0C:  STA WndTxtXCoord
        case 0x9132: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF0E:  STA WndTxtYCoord
        case 0x9135: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LAF10:  JSR ClearDialogOutBuf
        case 0x9138: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAF14:  RTS
        case 0x9139: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF15:  LDA #SFX_MENU_BTN
        case 0x913B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LAF17:  BNE +
        case 0x913D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF19:  LDA #SFX_CONFIRM
        case 0x913F: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LAF1E:  PLA
        case 0x9140: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAF1F:  RTS
        case 0x9141: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAF24:  CMP #WND_ALPHBT
        case 0x9143: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAF26:  BEQ WndErsAlphabet
        case 0x9145: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LAF28:  CMP #$FF
        case 0x9147: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LAF2A:  BEQ WndErsOther
        case 0x9149: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LAF2C:  ASL
        case 0x914A: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LAF2D:  TAY
        case 0x914B: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x802A, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LAF2E:  LDA WndwDataPtrTbl,Y
        case 0x914E: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF31:  STA GenPtr3ELB
        case 0x9151: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LAF33:  LDA WndwDataPtrTbl+1,Y
        case 0x9154: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF36:  STA GenPtr3EUB
        case 0x9157: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LAF38:  LDY #$01
        case 0x9159: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LAF3A:  LDA (GenPtr3E),Y
        case 0x915B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF3C:  STA WndEraseHght
        case 0x915E: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAF3F:  INY
        case 0x915F: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LAF40:  LDA (GenPtr3E),Y
        case 0x9161: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF42:  STA WndEraseWdth
        case 0x9164: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LAF45:  INY
        case 0x9165: cycles_to_run -= 2; reg_A = bus_read(read_pointer_indexed_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 2; return; // LAF46:  LDA (GenPtr3E),Y
        case 0x9167: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF48:  STA WndErasePos
        case 0x916A: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAF4B:  RTS
        case 0x916B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF4C:  LDA #$07
        case 0x916D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF4E:  STA WndEraseHght
        case 0x9170: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF51:  LDA #$16
        case 0x9172: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF53:  STA WndEraseWdth
        case 0x9175: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF56:  LDA #$21
        case 0x9177: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF58:  STA WndErasePos
        case 0x917A: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAF5B:  RTS
        case 0x917B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF5C:  LDA #$0C
        case 0x917D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF5E:  STA WndEraseHght
        case 0x9180: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF61:  LDA #$1A
        case 0x9182: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF63:  STA WndEraseWdth
        case 0x9185: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LAF66:  LDA #$22
        case 0x9187: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LAF68:  STA WndErasePos
        case 0x918A: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LAF6B:  RTS
        case 0x918B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB51D:  JSR FindDialogEntry
        case 0x918E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB520:  JSR InitDialogVars
        case 0x9191: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB526:  JSR WordToScreen
        case 0x9194: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB529:  JSR CheckDialogEnd
        case 0x9197: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LB52C:  BCC -
        case 0x9199: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB52E:  JSR DialogToScreenBuf
        case 0x919C: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB531:  RTS
        case 0x919D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB532:  STA TextEntry
        case 0x91A0: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB534:  AND #NBL_UPPER
        case 0x91A2: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LB536:  LSR
        case 0x91A3: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LB537:  LSR
        case 0x91A4: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LB538:  LSR
        case 0x91A5: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LB539:  LSR
        case 0x91A6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB53A:  STA TextBlock
        case 0x91A9: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LB53C:  TXA
        case 0x91AA: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LB53D:  ASL
        case 0x91AB: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LB53E:  ASL
        case 0x91AC: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LB53F:  ASL
        case 0x91AD: cycles_to_run -= 2; reg_A = cpu_asl(reg_A); reg_PC += 1; return; // LB540:  ASL
        case 0x91AE: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LB541:  ADC TextBlock
        case 0x91B1: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB543:  CLC
        case 0x91B2: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB544:  ADC #$01
        case 0x91B4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB546:  STA BankPtrIndex
        case 0x91B7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB548:  LDA #PRG_BANK_2
        case 0x91B9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB54A:  STA NewPRGBank
        case 0x91BC: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB54C:  LDX #$9F
        case 0x91BE: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB54E:  JSR GetAndStrDatPtr
        case 0x91C1: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB551:  LDA TextEntry
        case 0x91C4: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB553:  AND #NBL_LOWER
        case 0x91C6: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB555:  STA TextEntry
        case 0x91C9: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LB557:  TAX
        case 0x91CA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB558:  BEQ ++++
        case 0x91CC: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LB55A:  LDY #$00
        case 0x91CE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB55E:  LDA #PRG_BANK_2
        case 0x91D0: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB560:  JSR GetBankDataByte
        case 0x91D3: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB563:  INC DialogPtrLB
        case 0x91D6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB565:  BNE +
        case 0x91D8: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB567:  INC DialogPtrUB
        case 0x91DB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB56B:  BEQ +
        case 0x91DD: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB56D:  CMP #TXT_END2
        case 0x91DF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB56F:  BNE --
        case 0x91E1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB573:  BNE ---
        case 0x91E3: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB576:  LDA #$00
        case 0x91E5: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB578:  STA TxtIndent
        case 0x91E8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB57B:  STA Dialog00
        case 0x91EB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB57E:  STA DialogEnd
        case 0x91EE: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB581:  STA WrkBufBytsDone
        case 0x91F1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB584:  LDA #$08
        case 0x91F3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB586:  STA TxtLineSpace
        case 0x91F6: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB589:  LDA WndTxtXCoord
        case 0x91F9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB58B:  STA Unused6510
        case 0x91FC: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB58E:  LDA WndTxtYCoord
        case 0x91FF: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB590:  STA Unused6511
        case 0x9202: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB593:  RTS
        case 0x9203: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB594:  LDA DialogEnd
        case 0x9206: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB597:  BNE +
        case 0x9208: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB599:  CLC
        case 0x9209: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB59A:  RTS
        case 0x920A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB59D:  LDA Unused6512
        case 0x920D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB5A0:  BNE +
        case 0x920F: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LB5A2:  STX Unused6512
        case 0x9212: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB5A8:  BNE +
        case 0x9214: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LB5AA:  STX Unused6513
        case 0x9217: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB5AE:  RTS
        case 0x9218: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB5AF:  JSR GetTxtWord
        case 0x921B: cycles_to_run -= 3; cpu_bit(bus_read(0x0000)); reg_PC += 3; return; // LB5B2:  BIT Dialog00
        case 0x921E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0x802A; return; } // LB5B5:  BMI CalcCoordEnd
        case 0x9220: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB5B7:  LDA WndTxtXCoord
        case 0x9223: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB5B9:  STA WndXPosAW
        case 0x9226: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB5BC:  LDA #$00
        case 0x9228: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB5BE:  STA WordBufIndex
        case 0x922B: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LB5C1:  LDX WordBufIndex
        case 0x922E: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB5C4:  LDA WordBuffer,X
        case 0x9231: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB5C7:  INC WordBufIndex
        case 0x9234: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB5CA:  CMP #TL_BLANK_TILE1
        case 0x9236: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB5CC:  BEQ WordBufBreakFound
        case 0x9238: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB5CE:  CMP #TXT_SUBEND
        case 0x923A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LB5D0:  BCS WordBufBreakFound
        case 0x923C: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB5D2:  INC WndXPosAW
        case 0x923F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB5D5:  JSR CheckBetweenWords
        case 0x9242: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LB5D8:  BCS SearchWordBuf
        case 0x9244: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LB5DA:  LDX WndXPosAW
        case 0x9247: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB5DD:  BEQ +
        case 0x9249: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB5DF:  DEC WndXPosAW
        case 0x924C: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB5E5:  RTS
        case 0x924D: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB5E6:  LDX #$00
        case 0x924F: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LB5E8:  STX WordBufLen
        case 0x9252: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB5EE:  LDA WordBuffer,X
        case 0x9255: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB5F1:  INC WordBufLen
        case 0x9258: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB5F4:  CMP #TXT_SUBEND
        case 0x925A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LB5F6:  BCS TxtCntrlChars
        case 0x925C: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LB5F8:  PHA
        case 0x925D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB5F9:  JSR TextToPPU
        case 0x9260: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB5FC:  PLA
        case 0x9261: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB5FD:  JSR CheckBetweenWords
        case 0x9264: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LB600:  BCS -
        case 0x9266: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB602:  RTS
        case 0x9267: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB603:  CMP #TXT_WAIT
        case 0x9269: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB605:  BEQ WaitFound
        case 0x926B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB607:  CMP #TXT_END1
        case 0x926D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB609:  BEQ DialogEndFound
        case 0x926F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB60B:  CMP #TXT_NEWL
        case 0x9271: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB60D:  BEQ NewLineFound
        case 0x9273: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB60F:  CMP #TXT_NOP
        case 0x9275: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB611:  BEQ NewLineFound
        case 0x9277: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB613:  LDA #TXT_END2
        case 0x9279: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB615:  STA DialogEnd
        case 0x927C: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB618:  RTS
        case 0x927D: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB619:  JMP DoNewline
        case 0x9280: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB61C:  JSR DoNewline
        case 0x9283: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB61F:  JSR DoWait
        case 0x9286: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB622:  LDA TxtIndent
        case 0x9289: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB625:  BNE +
        case 0x928B: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB627:  JSR MoveToNextLine
        case 0x928E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB62B:  JSR DoNewline
        case 0x9291: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB62E:  LDA #$00
        case 0x9293: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB630:  STA WndTxtXCoord
        case 0x9296: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB632:  JMP DoDialogEnd
        case 0x9299: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB635:  LDA #$00
        case 0x929B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB637:  STA WordBufLen
        case 0x929E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB63A:  JSR GetTextByte
        case 0x92A1: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB63D:  CMP #TXT_NOP
        case 0x92A3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LB63F:  BNE BuildWordBuf
        case 0x92A5: cycles_to_run -= 3; cpu_bit(bus_read(0x0000)); reg_PC += 3; return; // LB641:  BIT Dialog00
        case 0x92A8: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x802A; return; } // LB644:  BPL GetTxtByteLoop
        case 0x92AA: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB646:  CMP #TXT_OPN_QUOTE
        case 0x92AC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB648:  BEQ TxtSetIndent
        case 0x92AE: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB64A:  CMP #TXT_INDENT
        case 0x92B0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB64C:  BNE +
        case 0x92B2: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB64E:  LDX #$01
        case 0x92B4: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LB650:  STX TxtIndent
        case 0x92B7: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB656:  STA WordBuffer,X
        case 0x92BA: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB659:  INC WordBufLen
        case 0x92BD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB65C:  JSR CheckBetweenWords
        case 0x92C0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LB65F:  BCS GetTxtByteLoop
        case 0x92C2: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB661:  RTS
        case 0x92C3: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LB662:  LDX WrkBufBytsDone
        case 0x92C6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB665:  BEQ GetROMByte
        case 0x92C8: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB667:  LDA WorkBuffer,X
        case 0x92CB: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB66A:  INC WrkBufBytsDone
        case 0x92CE: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB66D:  CMP #TXT_SUBEND
        case 0x92D0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB66F:  BNE +
        case 0x92D2: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB671:  LDX #$00
        case 0x92D4: cycles_to_run -= 3; bus_write(0x0000, reg_X); reg_PC += 3; return; // LB673:  STX WrkBufBytsDone
        case 0x92D7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB676:  BEQ GetROMByte
        case 0x92D9: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB679:  LDA #PRG_BANK_2
        case 0x92DB: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB67B:  LDX #DialogPtr
        case 0x92DD: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LB67D:  LDY #$00
        case 0x92DF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB67F:  JSR GetBankDataByte
        case 0x92E2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB682:  JSR IncDialogPtr
        case 0x92E5: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB685:  CMP #TXT_PLRL
        case 0x92E7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB687:  BEQ JmpDoPLRL
        case 0x92E9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB689:  CMP #TXT_DESC
        case 0x92EB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB68B:  BEQ JmpDoDESC
        case 0x92ED: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB68D:  CMP #TXT_PNTS
        case 0x92EF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB68F:  BEQ JmpDoPNTS
        case 0x92F1: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB691:  CMP #TXT_AMTP
        case 0x92F3: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB693:  BEQ JmpDoAMTP
        case 0x92F5: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB695:  CMP #TXT_AMNT
        case 0x92F7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB697:  BEQ JmpDoAMNT
        case 0x92F9: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB699:  CMP #TXT_SPEL
        case 0x92FB: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB69B:  BEQ JmpDoSPEL
        case 0x92FD: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB69D:  CMP #TXT_NAME
        case 0x92FF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB69F:  BEQ JmpDoNAME
        case 0x9301: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB6A1:  CMP #TXT_ITEM
        case 0x9303: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB6A3:  BEQ JmpDoITEM
        case 0x9305: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB6A5:  CMP #TXT_COPY
        case 0x9307: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB6A7:  BEQ JmpDoCOPY
        case 0x9309: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB6A9:  CMP #TXT_ENMY
        case 0x930B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB6AB:  BEQ JmpDoENMY
        case 0x930D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB6AD:  CMP #TXT_ENM2
        case 0x930F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB6AF:  BEQ JmpDoENM2
        case 0x9311: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB6B1:  RTS
        case 0x9312: cycles_to_run -= 3; reg_PC = 0x0000; return; // LB6B2:  JMP DoCOPY
        case 0x9315: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB6B5:  JMP DoNAME
        case 0x9318: cycles_to_run -= 3; reg_PC = 0x0000; return; // LB6B8:  JMP DoENMY
        case 0x931B: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB6BB:  JMP DoSPEL
        case 0x931E: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB6BE:  JMP DoDESC
        case 0x9321: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB6C1:  JMP DoENM2
        case 0x9324: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB6C4:  JMP DoITEM
        case 0x9327: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB6C7:  JMP DoPNTS
        case 0x932A: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB6CA:  JMP DoAMTP
        case 0x932D: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB6CD:  JSR BinWordToBCD
        case 0x9330: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB6D0:  LDA #TXT_SUBEND
        case 0x9332: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB6D2:  STA WorkBuffer,Y
        case 0x9335: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB6D5:  LDX #$00
        case 0x9337: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB6D7:  JMP WorkBufDone
        case 0x933A: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB6DA:  LDA #$05
        case 0x933C: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB6DC:  STA SubBufLength
        case 0x933F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB6DF:  LDA GenWrd00LB
        case 0x9342: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB6E1:  STA BCDByte0
        case 0x9345: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB6E3:  LDA GenWrd00UB
        case 0x9348: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB6E5:  STA BCDByte1
        case 0x934B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB6E7:  LDA #$00
        case 0x934D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB6E9:  STA BCDByte2
        case 0x9350: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB6EB:  JSR ConvertToBCD
        case 0x9353: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB6EE:  JSR ClearBCDLeadZeros
        case 0x9356: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LB6F1:  LDY #$00
        case 0x9358: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB6F6:  STA WorkBuffer,Y
        case 0x935B: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB6F9:  INY
        case 0x935C: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LB6FA:  DEX
        case 0x935D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LB6FB:  BPL -
        case 0x935F: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB6FD:  RTS
        case 0x9360: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB6FE:  LDA #$01
        case 0x9362: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB700:  STA SubBufLength
        case 0x9365: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB703:  LDA GenWrd00UB
        case 0x9368: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB705:  BNE +
        case 0x936A: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LB707:  LDX GenWrd00LB
        case 0x936D: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LB709:  DEX
        case 0x936E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB70A:  BEQ EndPlrl
        case 0x9370: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB70E:  STA WorkBuffer
        case 0x9373: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LB711:  LDY #$01
        case 0x9375: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB713:  INC SubBufLength
        case 0x9378: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB716:  JMP WorkBufEndChar
        case 0x937B: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LB719:  LDY #$00
        case 0x937D: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB71B:  JMP WorkBufEndChar
        case 0x9380: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LB71E:  LDY #$00
        case 0x9382: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB720:  LDA #$05
        case 0x9384: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB722:  BNE +
        case 0x9386: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB724:  JSR BinWordToBCD
        case 0x9389: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB727:  LDA SubBufLength
        case 0x938C: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB72A:  CLC
        case 0x938D: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB72B:  ADC #$06
        case 0x938F: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB730:  LDX #$05
        case 0x9391: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB735:  STA WorkBuffer,Y
        case 0x9394: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB738:  INY
        case 0x9395: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LB739:  DEX
        case 0x9396: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LB73A:  BPL -
        case 0x9398: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB73C:  LDA GenWrd00UB
        case 0x939B: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB73E:  BNE +
        case 0x939D: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LB740:  LDX GenWrd00LB
        case 0x93A0: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LB742:  DEX
        case 0x93A1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB743:  BEQ ++
        case 0x93A3: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB747:  STA WorkBuffer,Y
        case 0x93A6: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB74A:  INY
        case 0x93A7: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB74B:  INC SubBufLength
        case 0x93AA: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB757:  JSR GetDescHalves
        case 0x93AD: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB75A:  JMP WorkBufEndChar
        case 0x93B0: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB75D:  LDA #$00
        case 0x93B2: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB75F:  STA WndDescHalf
        case 0x93B5: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB762:  JSR PrepGetDesc
        case 0x93B8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB765:  JSR UpdateDescBufLen
        case 0x93BB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB768:  LDA #TL_BLANK_TILE1
        case 0x93BD: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB76A:  STA WorkBuffer,Y
        case 0x93C0: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB76D:  INY
        case 0x93C1: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LB76E:  TYA
        case 0x93C2: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LB76F:  PHA
        case 0x93C3: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB770:  INC WndDescHalf
        case 0x93C6: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB773:  JSR PrepGetDesc
        case 0x93C9: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LB776:  STY DescLength
        case 0x93CC: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB779:  PLA
        case 0x93CD: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LB77A:  TAY
        case 0x93CE: cycles_to_run -= 3; reg_PC = 0x0000; return; // LB77B:  JMP XferTempToWork
        case 0x93D1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB77E:  LDA #$09
        case 0x93D3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB780:  STA SubBufLength
        case 0x93D6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB783:  LDA #$20
        case 0x93D8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB785:  STA WndOptions
        case 0x93DB: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB788:  LDA #$04
        case 0x93DD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB78A:  STA WndParam
        case 0x93E0: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB78D:  LDA DescBuf
        case 0x93E3: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB78F:  AND #$3F
        case 0x93E5: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB791:  JMP LookupDescriptions
        case 0x93E8: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB794:  JSR GetDescHalves
        case 0x93EB: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB797:  JSR CheckAToAn
        case 0x93EE: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB79A:  JMP WorkBufEndChar
        case 0x93F1: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB79D:  JSR WorkBufShift
        case 0x93F4: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB7A0:  LDA WorkBuffer
        case 0x93F7: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB7A3:  CMP #TXT_UPR_A
        case 0x93F9: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB7A5:  BEQ VowelFound
        case 0x93FB: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB7A7:  CMP #TXT_UPR_I
        case 0x93FD: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB7A9:  BEQ VowelFound
        case 0x93FF: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB7AB:  CMP #TXT_UPR_U
        case 0x9401: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB7AD:  BEQ VowelFound
        case 0x9403: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB7AF:  CMP #TXT_UPR_E
        case 0x9405: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB7B1:  BEQ VowelFound
        case 0x9407: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB7B3:  CMP #TXT_UPR_O
        case 0x9409: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LB7B5:  BNE VowelNotFound
        case 0x940B: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB7B7:  LDA #TL_BLANK_TILE1
        case 0x940D: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB7B9:  STA WorkBuffer
        case 0x9410: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB7BC:  RTS
        case 0x9411: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB7BD:  JSR WorkBufShift
        case 0x9414: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB7C0:  LDA #TXT_LWR_N
        case 0x9416: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB7C2:  STA WorkBuffer
        case 0x9419: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB7C5:  LDA #TL_BLANK_TILE1
        case 0x941B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB7C7:  STA WorkBuffer+1
        case 0x941E: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB7CA:  RTS
        case 0x941F: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB7CB:  LDX #$26
        case 0x9421: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB7D0:  STA WorkBuffer+1,X
        case 0x9424: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LB7D3:  DEX
        case 0x9425: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LB7D4:  BPL -
        case 0x9427: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB7D6:  INY
        case 0x9428: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB7D7:  RTS
        case 0x9429: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB7D8:  LDA #$09
        case 0x942B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB7DA:  STA SubBufLength
        case 0x942E: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB7DD:  LDA DescBuf
        case 0x9431: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB7DF:  JSR WndGetSpellDesc
        case 0x9434: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB7E2:  JSR UpdateDescBufLen
        case 0x9437: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB7E5:  JMP WorkBufEndChar
        case 0x943A: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB7E8:  LDX #$00
        case 0x943C: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB7EC:  STA WorkBuffer,X
        case 0x943F: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB7EF:  INX
        case 0x9440: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB7F0:  CMP #TXT_SUBEND
        case 0x9442: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB7F2:  BNE -
        case 0x9444: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB7F4:  LDX #$00
        case 0x9446: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB7F6:  JMP WorkBufDone
        case 0x9449: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB7F9:  JSR NameToNameBuf
        case 0x944C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB7FC:  JSR NameBufToWorkBuf
        case 0x944F: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB7FF:  LDX #$00
        case 0x9451: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB801:  JMP WorkBufDone
        case 0x9454: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB804:  LDA EnNumber
        case 0x9457: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB806:  JSR GetEnName
        case 0x945A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB809:  JSR NameBufToWorkBuf
        case 0x945D: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB80C:  JMP BufFinished
        case 0x9460: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB80F:  LDA EnNumber
        case 0x9463: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB811:  JSR GetEnName
        case 0x9466: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB814:  JSR NameBufToWorkBuf
        case 0x9469: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB817:  JSR CheckAToAn
        case 0x946C: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB81A:  JMP BufFinished
        case 0x946F: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB81D:  LDX #$00
        case 0x9471: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB822:  STA WorkBuffer,X
        case 0x9474: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB825:  INX
        case 0x9475: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB826:  CMP #TXT_SUBEND
        case 0x9477: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB828:  BNE -
        case 0x9479: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB82A:  RTS
        case 0x947A: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LB82B:  STY DescLength
        case 0x947D: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LB82E:  LDY #$00
        case 0x947F: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LB830:  LDX DescLength
        case 0x9482: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB833:  BEQ NoXfer
        case 0x9484: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB835:  LDA #$00
        case 0x9486: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB837:  STA ThisTempIndex
        case 0x9489: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LB839:  LDX SubBufLength
        case 0x948C: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB83F:  STA WorkBuffer,Y
        case 0x948F: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LB842:  DEX
        case 0x9490: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB843:  INY
        case 0x9491: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB844:  INC ThisTempIndex
        case 0x9494: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB846:  LDA ThisTempIndex
        case 0x9497: cycles_to_run -= 2; update_flags_cmp(reg_A, bus_read(0x0000)); reg_PC += 3; return; // LB848:  CMP DescLength
        case 0x949A: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB84B:  BNE -
        case 0x949C: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB84D:  RTS
        case 0x949D: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LB84E:  DEY
        case 0x949E: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB84F:  RTS
        case 0x949F: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB850:  LDX #$00
        case 0x94A1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB852:  LDA #TL_BLANK_TILE1
        case 0x94A3: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB857:  INX
        case 0x94A4: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LB858:  CPX #$B0
        case 0x94A6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LB85A:  BCC -
        case 0x94A8: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB85C:  RTS
        case 0x94A9: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB85D:  LDA #$08
        case 0x94AB: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB85F:  STA RowsRemaining
        case 0x94AE: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB861:  LDX #$00
        case 0x94B0: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LB863:  LDY #$00
        case 0x94B2: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB865:  LDA #$16
        case 0x94B4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB867:  STA ColsRemaining
        case 0x94B7: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB869:  LDA DialogOutBuf,Y
        case 0x94BA: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB86C:  STA WinBufRAM+$0265,X
        case 0x94BD: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB86F:  INX
        case 0x94BE: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB870:  INY
        case 0x94BF: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB871:  DEC ColsRemaining
        case 0x94C2: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LB873:  BNE CopyDialogByte
        case 0x94C4: cycles_to_run -= 2; reg_A = reg_X; update_nz(reg_A); reg_PC += 1; return; // LB875:  TXA
        case 0x94C5: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB876:  CLC
        case 0x94C6: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB877:  ADC #$0A
        case 0x94C8: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LB879:  TAX
        case 0x94C9: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)-1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB87A:  DEC RowsRemaining
        case 0x94CC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LB87C:  BNE NewDialogRow
        case 0x94CE: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB87E:  RTS
        case 0x94CF: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LB87F:  LDY #$00
        case 0x94D1: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB881:  LDX #$00
        case 0x94D3: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB885:  STA NameBuffer,Y
        case 0x94D6: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB888:  LDA DispName4,X
        case 0x94D9: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB88B:  STA NameBuffer+4,Y
        case 0x94DC: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB88E:  INX
        case 0x94DD: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB88F:  INY
        case 0x94DE: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LB890:  CPY #$04
        case 0x94E0: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB892:  BNE -
        case 0x94E2: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LB894:  LDY #$08
        case 0x94E4: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB896:  JSR FindNameEnd
        case 0x94E7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB899:  LDA #TXT_SUBEND
        case 0x94E9: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB89B:  STA NameBuffer,Y
        case 0x94EC: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB89E:  RTS
        case 0x94ED: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB89F:  CLC
        case 0x94EE: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LB8A0:  ADC #$01
        case 0x94F0: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LB8A2:  PHA
        case 0x94F1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB8A3:  LDA #$00
        case 0x94F3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB8A5:  STA WndDescHalf
        case 0x94F6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB8A8:  LDA #$0B
        case 0x94F8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB8AA:  STA SubBufLength
        case 0x94FB: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB8AD:  PLA
        case 0x94FC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB8AE:  JSR GetEnDescHalf
        case 0x94FF: cycles_to_run -= 2; reg_Y = (uint8_t)0x0000; update_nz(reg_Y); reg_PC += 2; return; // LB8B1:  LDY #$00
        case 0x9501: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB8B3:  JSR AddTempBufToNameBuf
        case 0x9504: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB8B6:  JSR FindNameEnd
        case 0x9507: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB8B9:  LDA #TL_BLANK_TILE1
        case 0x9509: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB8BB:  STA NameBuffer,Y
        case 0x950C: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB8BE:  INY
        case 0x950D: cycles_to_run -= 2; reg_A = reg_Y; update_nz(reg_A); reg_PC += 1; return; // LB8BF:  TYA
        case 0x950E: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LB8C0:  PHA
        case 0x950F: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB8C1:  INC WndDescHalf
        case 0x9512: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB8C4:  LDA #$09
        case 0x9514: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB8C6:  STA SubBufLength
        case 0x9517: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB8C9:  LDA DescEntry
        case 0x951A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB8CB:  JSR GetEnDescHalf
        case 0x951D: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB8CE:  PLA
        case 0x951E: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LB8CF:  TAY
        case 0x951F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB8D0:  JSR AddTempBufToNameBuf
        case 0x9522: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB8D3:  JSR FindNameEnd
        case 0x9525: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB8D6:  JMP EndNameBuf
        case 0x9528: cycles_to_run -= 2; reg_A = bus_read(addr_abs_y(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB8D9:  LDA NameBuffer-1,Y
        case 0x952B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB8DC:  CMP #TL_BLANK_TILE2
        case 0x952D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB8DE:  BEQ +
        case 0x952F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB8E0:  CMP #TL_BLANK_TILE1
        case 0x9531: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB8E2:  BNE ++
        case 0x9533: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_N) { reg_PC = 0x0000; return; } // LB8E5:  BMI +
        case 0x9535: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LB8E7:  BNE FindNameEnd
        case 0x9537: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LB8EA:  LDX SubBufLength
        case 0x953A: cycles_to_run -= 3; bus_write(addr_abs_y(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB8F0:  STA NameBuffer,Y
        case 0x953D: cycles_to_run -= 2; // Unsupported iny reg_PC += 1; return; // LB8F3:  INY
        case 0x953E: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LB8F4:  DEX
        case 0x953F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB8F5:  BNE -
        case 0x9541: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB8F7:  RTS
        case 0x9542: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB8F8:  RTS
        case 0x9543: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB8F9:  CMP #TXT_SUBEND
        case 0x9545: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LB8FB:  BCS NonWordChar
        case 0x9547: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB8FD:  CMP #TL_BLANK_TILE1
        case 0x9549: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB8FF:  BEQ NonWordChar
        case 0x954B: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB901:  CMP #TXT_PERIOD
        case 0x954D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB903:  BEQ NonWordChar
        case 0x954F: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB905:  CMP #TXT_COMMA
        case 0x9551: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB907:  BEQ NonWordChar
        case 0x9553: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB909:  CMP #TXT_APOS
        case 0x9555: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB90B:  BEQ NonWordChar
        case 0x9557: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB90D:  CMP #TXT_PRD_QUOTE
        case 0x9559: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB90F:  BEQ NonWordChar
        case 0x955B: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LB911:  SEC
        case 0x955C: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB912:  RTS
        case 0x955D: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB913:  CLC
        case 0x955E: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB914:  RTS
        case 0x955F: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB915:  LDA WndXPosAW
        case 0x9562: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB918:  CMP #$16
        case 0x9564: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LB91A:  BCS MoveToNextLine
        case 0x9566: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB91C:  RTS
        case 0x9567: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB91D:  LDA WndTxtXCoord
        case 0x956A: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB91F:  STA WndXPosAW
        case 0x956D: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB922:  BEQ NewlineEnd
        case 0x956F: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LB924:  LDX WndTxtYCoord
        case 0x9572: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB926:  INX
        case 0x9573: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LB927:  CPX #$08
        case 0x9575: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LB929:  BCS ScrollDialog
        case 0x9577: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB92B:  LDA TxtLineSpace
        case 0x957A: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LB92E:  LSR
        case 0x957B: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LB92F:  LSR
        case 0x957C: cycles_to_run -= 2; reg_A ^= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB930:  EOR #$03
        case 0x957E: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB932:  CLC
        case 0x957F: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LB933:  ADC WndTxtYCoord
        case 0x9582: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB935:  STA WndTxtYCoord
        case 0x9585: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB937:  LDA TxtIndent
        case 0x9588: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB93A:  STA WndXPosAW
        case 0x958B: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB93D:  STA WndTxtXCoord
        case 0x958E: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LB93F:  CLC
        case 0x958F: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB940:  RTS
        case 0x9590: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB941:  JSR Scroll1Line
        case 0x9593: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB944:  LDA TxtLineSpace
        case 0x9596: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB947:  CMP #$04
        case 0x9598: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LB949:  BNE ScrollUpdate
        case 0x959A: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB94B:  JSR Scroll1Line
        case 0x959D: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB94E:  LDA #$13
        case 0x959F: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB950:  STA DialogScrlY
        case 0x95A2: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB953:  LDA #$00
        case 0x95A4: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB955:  STA DialogScrlInd
        case 0x95A7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB958:  JSR WaitForNMI
        case 0x95AA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB95E:  LDA DialogScrlY
        case 0x95AD: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB961:  CMP #$1B
        case 0x95AF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x0000; return; } // LB963:  BCC -
        case 0x95B1: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LB965:  BCS LineDone
        case 0x95B3: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LB967:  LDX #$00
        case 0x95B5: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB969:  LDA DialogOutBuf+$16,X
        case 0x95B8: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB96C:  AND #$7F
        case 0x95BA: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB96E:  CMP #$76
        case 0x95BC: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LB970:  BCS NextScrollByte
        case 0x95BE: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LB972:  PHA
        case 0x95BF: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB973:  LDA DialogOutBuf,X
        case 0x95C2: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB976:  AND #$7F
        case 0x95C4: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB978:  CMP #$76
        case 0x95C6: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB97A:  PLA
        case 0x95C7: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LB97B:  BCS NextScrollByte
        case 0x95C9: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LB97D:  STA DialogOutBuf,X
        case 0x95CC: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB980:  INX
        case 0x95CD: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LB981:  CPX #$9A
        case 0x95CF: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LB983:  BNE ScrollDialogLoop
        case 0x95D1: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB985:  LDA #TL_BLANK_TILE1
        case 0x95D3: cycles_to_run -= 2; // Unsupported inx reg_PC += 1; return; // LB98A:  INX
        case 0x95D4: cycles_to_run -= 2; update_flags_cmp(reg_X, (uint8_t)0x0000); reg_PC += 2; return; // LB98B:  CPX #$B0
        case 0x95D6: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB98D:  BNE -
        case 0x95D8: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB98F:  RTS
        case 0x95D9: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB990:  JSR Display1ScrollLine
        case 0x95DC: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB993:  INC DialogScrlY
        case 0x95DF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB996:  JSR Display1ScrollLine
        case 0x95E2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB999:  JSR WaitForNMI
        case 0x95E5: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB99C:  INC DialogScrlY
        case 0x95E8: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB99F:  RTS
        case 0x95E9: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB9A0:  LDA DialogScrlY
        case 0x95EC: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB9A3:  STA ScrnTxtYCoord
        case 0x95EF: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LB9A6:  LDA #$05
        case 0x95F1: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB9A8:  STA ScrnTxtXCoord
        case 0x95F4: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LB9AB:  LDX DialogScrlInd
        case 0x95F7: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB9AE:  LDA DialogOutBuf,X
        case 0x95FA: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB9B1:  STA PPUDataByte
        case 0x95FD: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB9B3:  JSR WndCalcPPUAddr
        case 0x9600: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB9B6:  JSR AddPPUBufEntry
        case 0x9603: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB9B9:  INC DialogScrlInd
        case 0x9606: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LB9BC:  INC ScrnTxtXCoord
        case 0x9609: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB9BF:  LDA ScrnTxtXCoord
        case 0x960C: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB9C2:  CMP #$1B
        case 0x960E: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LB9C4:  BNE DisplayScrollLoop
        case 0x9610: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LB9C6:  RTS
        case 0x9611: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LB9C7:  PHA
        case 0x9612: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB9C8:  LDA WndTxtXCoord
        case 0x9615: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB9CA:  STA WndXPosAW
        case 0x9618: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LB9CD:  JSR CheckForNewLine
        case 0x961B: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LB9D0:  LDA WndTxtYCoord
        case 0x961E: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LB9D2:  JSR CalcWndYByteNum
        case 0x9621: cycles_to_run -= 2; cpu_adc(bus_read(0x0000)); reg_PC += 3; return; // LB9D5:  ADC WndTxtXCoord
        case 0x9624: cycles_to_run -= 2; reg_X = reg_A; update_nz(reg_X); reg_PC += 1; return; // LB9D7:  TAX
        case 0x9625: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB9D8:  PLA
        case 0x9626: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB9D9:  CMP #TL_BLANK_TILE1
        case 0x9628: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x0000; return; } // LB9DB:  BEQ CheckXCoordIndent
        case 0x962A: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LB9DD:  CMP #TXT_OPN_QUOTE
        case 0x962C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LB9DF:  BNE CheckNextBufByte
        case 0x962E: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LB9E1:  LDY WndTxtXCoord
        case 0x9631: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LB9E3:  CPY #$01
        case 0x9633: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LB9E5:  BNE CheckNextBufByte
        case 0x9635: cycles_to_run -= 2; // Unsupported dey reg_PC += 1; return; // LB9E7:  DEY
        case 0x9636: cycles_to_run -= 3; bus_write(0x0000, reg_Y); reg_PC += 3; return; // LB9E8:  STY WndTxtXCoord
        case 0x9639: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LB9EA:  DEX
        case 0x963A: cycles_to_run -= 3; reg_PC = 0x802A; return; // LB9EB:  JMP CheckNextBufByte
        case 0x963D: cycles_to_run -= 2; reg_Y = bus_read(0x0000); update_nz(reg_Y); reg_PC += 3; return; // LB9EE:  LDY WndTxtXCoord
        case 0x9640: cycles_to_run -= 2; update_flags_cmp(reg_Y, bus_read(0x0000)); reg_PC += 3; return; // LB9F0:  CPY TxtIndent
        case 0x9643: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LB9F3:  BEQ EndTextToPPU
        case 0x9645: cycles_to_run -= 3; cpu_push(reg_A); reg_PC += 1; return; // LB9F5:  PHA
        case 0x9646: cycles_to_run -= 2; reg_A = bus_read(addr_abs_x(0x0000, nullptr)); update_nz(reg_A); reg_PC += 3; return; // LB9F6:  LDA DialogOutBuf,X
        case 0x9649: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LB9F9:  STA PPUDataByte
        case 0x964C: cycles_to_run -= 2; reg_Y = reg_A; update_nz(reg_Y); reg_PC += 1; return; // LB9FB:  TAY
        case 0x964D: cycles_to_run -= 4; reg_A = cpu_pop(); update_nz(reg_A); reg_PC += 1; return; // LB9FC:  PLA
        case 0x964E: cycles_to_run -= 2; update_flags_cmp(reg_Y, (uint8_t)0x0000); reg_PC += 2; return; // LB9FD:  CPY #TL_BLANK_TILE1
        case 0x9650: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LB9FF:  BNE +
        case 0x9652: cycles_to_run -= 3; bus_write(addr_abs_x(0x0000, nullptr), reg_A); reg_PC += 3; return; // LBA01:  STA DialogOutBuf,X
        case 0x9655: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA04:  STA PPUDataByte
        case 0x9658: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LBA09:  BEQ CalcTextWndPos
        case 0x965A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBA0B:  LDA PPUDataByte
        case 0x965D: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBA0D:  CMP #TL_BLANK_TILE1
        case 0x965F: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x802A; return; } // LBA0F:  BCS CalcTextWndPos
        case 0x9661: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBA11:  LDA WndTxtXCoord
        case 0x9664: cycles_to_run -= 2; reg_A = cpu_lsr(reg_A); reg_PC += 1; return; // LBA13:  LSR
        case 0x9665: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LBA14:  BCC CalcTextWndPos
        case 0x9667: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBA16:  LDA #SFX_TEXT
        case 0x9669: cycles_to_run -= 7; // Unsupported brk reg_PC += 1; return; // LBA18:  BRK
        case 0x966A: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBA1B:  LDA WndTxtXCoord
        case 0x966D: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LBA1D:  CLC
        case 0x966E: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LBA1E:  ADC #$05
        case 0x9670: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA20:  STA ScrnTxtXCoord
        case 0x9673: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBA23:  LDA WndTxtYCoord
        case 0x9676: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LBA25:  CLC
        case 0x9677: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LBA26:  ADC #$13
        case 0x9679: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA28:  STA ScrnTxtYCoord
        case 0x967C: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LBA2B:  JSR WndCalcPPUAddr
        case 0x967F: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBA2E:  JSR AddPPUBufEntry
        case 0x9682: cycles_to_run -= 2; reg_X = bus_read(0x0000); update_nz(reg_X); reg_PC += 3; return; // LBA31:  LDX MessageSpeed
        case 0x9685: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LBA36:  DEX
        case 0x9686: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LBA37:  BPL -
        case 0x9688: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LBA39:  INC WndTxtXCoord
        case 0x968B: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LBA3B:  RTS
        case 0x968C: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBA3C:  LDA PPUDataByte
        case 0x968F: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LBA3E:  LDX #$04
        case 0x9691: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LBA43:  BEQ TextSetCarry
        case 0x9693: cycles_to_run -= 2; // Unsupported dex reg_PC += 1; return; // LBA45:  DEX
        case 0x9694: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_N)) { reg_PC = 0x0000; return; } // LBA46:  BPL -
        case 0x9696: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBA48:  CMP #$24
        case 0x9698: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LBA4A:  BCC TextClearCarry
        case 0x969A: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBA4C:  CMP #$56
        case 0x969C: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_C)) { reg_PC = 0x802A; return; } // LBA4E:  BCC TextSetCarry
        case 0x969E: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LBA50:  CLC
        case 0x969F: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LBA51:  RTS
        case 0x96A0: cycles_to_run -= 2; reg_P |= FLAG_C; reg_PC += 1; return; // LBA52:  SEC
        case 0x96A1: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LBA53:  RTS
        case 0x96A2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LBA59:  JSR TxtCheckInput
        case 0x96A5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x802A; return; } // LBA5C:  BNE TxtBtnPressed
        case 0x96A7: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBA5E:  LDA #$10
        case 0x96A9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA60:  STA FrameCounter
        case 0x96AC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LBA62:  JSR TxtWaitAnim
        case 0x96AF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBA65:  JSR WaitForNMI
        case 0x96B2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LBA68:  JSR TxtCheckInput
        case 0x96B5: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_Z) { reg_PC = 0x802A; return; } // LBA6B:  BEQ TxtWaitLoop
        case 0x96B7: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LBA6D:  JSR TxtClearArrow
        case 0x96BA: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBA70:  LDA TxtIndent
        case 0x96BD: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA73:  STA WndTxtXCoord
        case 0x96C0: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LBA75:  RTS
        case 0x96C1: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LBA76:  LDX #$43
        case 0x96C3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBA78:  LDA FrameCounter
        case 0x96C6: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBA7A:  AND #$1F
        case 0x96C8: cycles_to_run -= 2; update_flags_cmp(reg_A, (uint8_t)0x0000); reg_PC += 2; return; // LBA7C:  CMP #$10
        case 0x96CA: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (reg_P & FLAG_C) { reg_PC = 0x0000; return; } // LBA7E:  BCS +
        case 0x96CC: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LBA80:  LDX #TL_BLANK_TILE1
        case 0x96CE: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBA84:  LDA #$10
        case 0x96D0: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA86:  STA ScrnTxtXCoord
        case 0x96D3: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBA89:  LDA WndTxtYCoord
        case 0x96D6: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LBA8B:  CLC
        case 0x96D7: cycles_to_run -= 2; cpu_adc((uint8_t)0x0000); reg_PC += 2; return; // LBA8C:  ADC #$13
        case 0x96D9: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBA8E:  STA ScrnTxtYCoord
        case 0x96DC: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LBA91:  JSR WndCalcPPUAddr
        case 0x96DF: cycles_to_run -= 3; reg_PC = 0x0000; return; // LBA94:  JMP AddPPUBufEntry
        case 0x96E2: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x0000; return; } // LBA97:  JSR GetJoypadStatus
        case 0x96E5: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBA9A:  LDA JoypadBtns
        case 0x96E8: cycles_to_run -= 2; reg_A &= (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBA9C:  AND #IN_A_OR_B
        case 0x96EA: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LBA9E:  RTS
        case 0x96EB: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LBA9F:  INC DialogPtrLB
        case 0x96EE: cycles_to_run -= 2; if (cycles_to_run <= 0) return; if (!(reg_P & FLAG_Z)) { reg_PC = 0x0000; return; } // LBAA1:  BNE +
        case 0x96F0: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LBAA3:  INC DialogPtrUB
        case 0x96F3: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBAA6:  STA TxtRowNum
        case 0x96F6: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBAA8:  LDA #$00
        case 0x96F8: cycles_to_run -= 3; bus_write(0x0000, reg_A); reg_PC += 3; return; // LBAAA:  STA TxtRowStart
        case 0x96FB: cycles_to_run -= 2; reg_X = (uint8_t)0x0000; update_nz(reg_X); reg_PC += 2; return; // LBAAC:  LDX #TxtRowNum
        case 0x96FD: cycles_to_run -= 2; reg_A = (uint8_t)0x0000; update_nz(reg_A); reg_PC += 2; return; // LBAAE:  LDA #$16
        case 0x96FF: cycles_to_run -= 6; { uint16_t ret = reg_PC + 2; cpu_push(ret >> 8); cpu_push(ret & 0xFF); reg_PC = 0x802A; return; } // LBAB0:  JSR IndexedMult
        case 0x9702: cycles_to_run -= 2; reg_A = bus_read(0x0000); update_nz(reg_A); reg_PC += 3; return; // LBAB3:  LDA TxtRowNum
        case 0x9705: cycles_to_run -= 2; reg_P &= ~FLAG_C; reg_PC += 1; return; // LBAB5:  CLC
        case 0x9706: cycles_to_run -= 6; reg_PC = (cpu_pop() | (cpu_pop() << 8)) + 1; return; // LBAB6:  RTS
        case 0x9707: cycles_to_run -= 2; reg_P |= FLAG_I; reg_PC += 1; return; // LBFD8:  SEI
        case 0x9708: cycles_to_run -= 5; { uint16_t a = 0x0000; uint8_t v = bus_read(a)+1; bus_write(a,v); update_nz(v); } reg_PC += 3; return; // LBFD9:  INC MMCReset1
        case 0x970B: cycles_to_run -= 3; reg_PC = 0x0000; return; // LBFDC:  JMP _DoReset
        default: reg_PC++; return;
    }
}
}
