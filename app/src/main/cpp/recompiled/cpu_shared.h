#include <stdint.h>
#include <android/log.h>

extern "C" {
    // CPU Registers
    extern uint8_t reg_A, reg_X, reg_Y, reg_P, reg_S;
    
    // Memory and Stack
    extern uint8_t bus_read(uint16_t addr);
    extern void bus_write(uint16_t addr, uint8_t val);
    extern void push_stack(uint8_t val);
    extern uint8_t pop_stack();

    // Flag Helpers
    inline void update_nz(uint8_t val) {
        reg_P &= ~(0x80 | 0x02); // Clear N and Z
        if (val == 0) reg_P |= 0x02;
        if (val & 0x80) reg_P |= 0x80;
    }
    
    inline void update_flags_cmp(uint8_t reg, uint8_t val) {
        reg_P &= ~(0x80 | 0x02 | 0x01); // Clear N, Z, C
        if (reg >= val) reg_P |= 0x01; // Carry
        if (reg == val) reg_P |= 0x02; // Zero
        update_nz(reg - val);
    }
}
