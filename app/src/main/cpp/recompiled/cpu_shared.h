#ifndef CPU_SHARED_H
#define CPU_SHARED_H

#include <stdint.h>
#include <android/log.h>

// Log helper for debugging recompiled execution
#define LOG_CPU(...) __android_log_print(ANDROID_LOG_DEBUG, "DWA_RECOMPILED", __VA_ARGS__)

extern "C" {
    // --- CPU Registers ---
    // reg_PC is vital for tracking execution state across recompiled blocks
    extern uint16_t reg_PC;
    extern uint8_t reg_A, reg_X, reg_Y, reg_P, reg_S;

    // --- Memory Bus ---
    // These link directly to the bus logic in native-lib.cpp
    extern uint8_t bus_read(uint16_t addr);
    extern void bus_write(uint16_t addr, uint8_t val);

    // --- Stack Operations ---
    extern void push_stack(uint8_t val);
    extern uint8_t pop_stack();

    // --- Flag Helpers ---
    // Updates Zero and Negative flags based on val
    extern void update_nz(uint8_t val);
    // Handles CMP, CPX, CPY logic
    extern void update_flags_cmp(uint8_t reg, uint8_t val);

    // --- Math & Logic Helpers ---
    extern void cpu_adc(uint8_t val);
    extern void cpu_sbc(uint8_t val);
    extern void cpu_bit(uint8_t val);

    // --- Addressing Helpers ---
    // read_pointer: fetches 16-bit address from memory (handles page wrap if necessary)
    extern uint16_t read_pointer(uint16_t addr);
    // read_pointer_x: handles Indexed Indirect addressing -> (Indirect, X)
    extern uint16_t read_pointer_x(uint16_t addr);

    // --- Execution Control ---
    /**
     * This is the main dispatcher. 
     * In a static recompiled setup, this function usually contains a large 
     * switch-case or a jump table based on reg_PC to call the correct 
     * recompiled C++ function for the current instruction.
     */
    void execute_instruction();

    // Reset and Interrupt vectors
    void power_on_reset();
    void nmi_handler();
}

#endif // CPU_SHARED_H
