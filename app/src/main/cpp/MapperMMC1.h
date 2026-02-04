#include <vector>
#include <cstdint>

class MapperMMC1 {
public:
    uint8_t prg_banks[4][16384]; // 4 banks of 16KB
    int current_bank = 0;
    uint8_t shift_register = 0x10;

    void write(uint16_t addr, uint8_t val) {
        if (val & 0x80) { // Reset shift register
            shift_register = 0x10;
        } else {
            bool complete = (shift_register & 0x01);
            shift_register >>= 1;
            shift_register |= ((val & 0x01) << 4);
            
            if (complete) {
                // When 5 bits are shifted in, update the bank
                if (addr >= 0xE000) { 
                    current_bank = shift_register & 0x03;
                }
                shift_register = 0x10;
            }
        }
    }

    uint8_t read(uint16_t addr) {
        if (addr >= 0x8000 && addr < 0xC000) {
            return prg_banks[current_bank][addr - 0x8000];
        } else if (addr >= 0xC000) {
            return prg_banks[3][addr - 0xC000]; // Bank 03 is fixed at $C000
        }
        return 0;
    }
};
