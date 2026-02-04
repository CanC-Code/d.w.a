#include <vector>
#include <cstdint>

class MapperMMC1 {
public:
    // Memory Storage
    uint8_t prg_rom[4][16384]; [span_3](start_span)// 4 banks of 16KB[span_3](end_span)
    uint8_t chr_rom[2][4096];  [span_4](start_span)// 2 banks of 4KB (standard MMC1 behavior)[span_4](end_span)
    
    // Internal Registers
    [span_5](start_span)[span_6](start_span)uint8_t shift_register = 0x10;[span_5](end_span)[span_6](end_span)
    uint8_t control = 0x0C;        [span_7](start_span)// Default: PRG 16KB swap at $8000, fixed $C000[span_7](end_span)
    uint8_t chr_bank_0 = 0;
    uint8_t chr_bank_1 = 0;
    uint8_t prg_bank = 0;          [span_8](start_span)[span_9](start_span)// current_bank[span_8](end_span)[span_9](end_span)

    // Mirroring mode for PPU
    // 0: One-Screen lower, 1: One-Screen upper, 2: Vertical, 3: Horizontal
    uint8_t mirroring = 0;

    void write(uint16_t addr, uint8_t val) {
        if (val & 0x80) { 
            [span_10](start_span)// Reset shift register[span_10](end_span)
            shift_register = 0x10;
            control |= 0x0C; 
        } else {
            [span_11](start_span)bool complete = (shift_register & 0x01);[span_11](end_span)
            [span_12](start_span)shift_register >>= 1;[span_12](end_span)
            [span_13](start_span)shift_register |= ((val & 0x01) << 4);[span_13](end_span)

            if (complete) {
                uint8_t data = shift_register & 0x1F;
                
                if (addr >= 0x8000 && addr <= 0x9FFF) {
                    control = data;
                    mirroring = data & 0x03;
                } else if (addr >= 0xA000 && addr <= 0xBFFF) {
                    chr_bank_0 = data;
                } else if (addr >= 0xC000 && addr <= 0xDFFF) {
                    chr_bank_1 = data;
                } else if (addr >= 0xE000) { 
                    [span_14](start_span)prg_bank = data & 0x0F;[span_14](end_span)
                }
                [span_15](start_span)shift_register = 0x10;[span_15](end_span)
            }
        }
    }

    uint8_t read_prg(uint16_t addr) {
        [span_16](start_span)// Mapping: $8000-$BFFF = switchable, $C000-$FFFF = fixed to last bank[span_16](end_span)
        if (addr >= 0x8000 && addr < 0xC000) {
            [span_17](start_span)return prg_rom[prg_bank % 4][addr - 0x8000];[span_17](end_span)
        } else if (addr >= 0xC000) {
            [span_18](start_span)return prg_rom[3][addr - 0xC000];[span_18](end_span)
        }
        return 0;
    }

    uint8_t read_chr(uint16_t addr) {
        // MMC1 supports 4KB CHR banking
        if (addr < 0x1000) {
            [span_19](start_span)return chr_rom[chr_bank_0 % 2][addr];[span_19](end_span)
        } else {
            [span_20](start_span)return chr_rom[chr_bank_1 % 2][addr - 0x1000];[span_20](end_span)
        }
    }
};
