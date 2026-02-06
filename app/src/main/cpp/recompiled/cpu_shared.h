#ifndef CPU_SHARED_H
#define CPU_SHARED_H

#include <stdint.h>
#include <android/log.h>

// This header is included by every recompiled BankXX.cpp file.
// It must match the definitions in native-lib.cpp exactly.

extern "C" {
    // --- CPU Registers ---
    extern uint8_t reg_A, reg_X, reg_Y, reg_P, reg_S;

    // --- Memory Bus ---
    extern uint8_t bus_read(uint16_t addr);
    extern void bus_write(uint16_t addr, uint8_t val);

    // --- Stack Operations ---
    extern void push_stack(uint8_t val);
    extern uint8_t pop_stack();

    // --- Flag Helpers ---
    extern void update_nz(uint8_t val);
    extern void update_flags_cmp(uint8_t reg, uint8_t val);

    // --- Math & Logic Helpers ---
    extern void cpu_adc(uint8_t val);
    extern void cpu_sbc(uint8_t val);
    extern void cpu_bit(uint8_t val);

    // --- Addressing Helpers ---
    // Essential for Dragon Warrior's pointer-heavy logic
    extern uint16_t read_pointer(uint16_t addr);
    extern uint16_t read_pointer_x(uint16_t addr);

    // --- Recompiled Entry Points ---
    void power_on_reset();
    void nmi_handler();
}

#endif // CPU_SHARED_H
