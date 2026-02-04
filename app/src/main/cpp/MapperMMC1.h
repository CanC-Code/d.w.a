#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H

#include <cstdint>

class MapperMMC1 {
public:
    uint8_t prg_rom[4][16384]; [span_0](start_span)// 4 banks of 16KB[span_0](end_span)
    uint8_t chr_rom[4][4096];  [span_1](start_span)// Fixed: Increased to 4 banks of 4KB (16KB total)[span_1](end_span)
    
    [span_2](start_span)uint8_t shift_register = 0x10;[span_2](end_span)
    [span_3](start_span)uint8_t control = 0x0C;[span_3](end_span)
    [span_4](start_span)uint8_t chr_bank_0 = 0;[span_4](end_span)
    [span_5](start_span)uint8_t chr_bank_1 = 0;[span_5](end_span)
    [span_6](start_span)uint8_t prg_bank = 0;[span_6](end_span)
    [span_7](start_span)uint8_t mirroring = 0;[span_7](end_span)

    void write(uint16_t addr, uint8_t val) {
        [span_8](start_span)if (val & 0x80) {[span_8](end_span)
            [span_9](start_span)shift_register = 0x10;[span_9](end_span)
            [span_10](start_span)control |= 0x0C;[span_10](end_span)
        } else {
            [span_11](start_span)bool complete = (shift_register & 0x01);[span_11](end_span)
            [span_12](start_span)shift_register >>= 1;[span_12](end_span)
            [span_13](start_span)shift_register |= ((val & 0x01) << 4);[span_13](end_span)

            [span_14](start_span)if (complete) {[span_14](end_span)
                [span_15](start_span)uint8_t data = shift_register & 0x1F;[span_15](end_span)
                [span_16](start_span)if (addr >= 0x8000 && addr <= 0x9FFF) {[span_16](end_span)
                    [span_17](start_span)control = data;[span_17](end_span)
                    [span_18](start_span)mirroring = data & 0x03;[span_18](end_span)
                [span_19](start_span)} else if (addr >= 0xA000 && addr <= 0xBFFF) {[span_19](end_span)
                    [span_20](start_span)chr_bank_0 = data;[span_20](end_span)
                [span_21](start_span)} else if (addr >= 0xC000 && addr <= 0xDFFF) {[span_21](end_span)
                    [span_22](start_span)chr_bank_1 = data;[span_22](end_span)
                [span_23](start_span)} else if (addr >= 0xE000) {[span_23](end_span)
                    [span_24](start_span)prg_bank = data & 0x0F;[span_24](end_span)
                }
                [span_25](start_span)shift_register = 0x10;[span_25](end_span)
            }
        }
    }

    uint8_t read_prg(uint16_t addr) {
        [span_26](start_span)if (addr >= 0x8000 && addr < 0xC000) {[span_26](end_span)
            [span_27](start_span)return prg_rom[prg_bank % 4][addr - 0x8000];[span_27](end_span)
        [span_28](start_span)} else if (addr >= 0xC000) {[span_28](end_span)
            [span_29](start_span)return prg_rom[3][addr - 0xC000];[span_29](end_span)
        }
        [span_30](start_span)return 0;[span_30](end_span)
    }

    uint8_t read_chr(uint16_t addr) {
        [span_31](start_span)if (addr < 0x1000) return chr_rom[chr_bank_0 % 4][addr];[span_31](end_span)
        [span_32](start_span)return chr_rom[chr_bank_1 % 4][addr - 0x1000];[span_32](end_span)
    }
};

#endif
