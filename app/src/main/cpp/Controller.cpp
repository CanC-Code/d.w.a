#include "Controller.h"

Controller::Controller() : button_state(0), shift_register(0), strobe(false) {}

void Controller::update_state(uint8_t state) {
    button_state.store(state, std::memory_order_relaxed);
}

/**
 * When the CPU writes to $4016:
 * Bit 0 high: Strobe is active, shift register constantly reloads from button_state.
 * Bit 0 low: Strobe inactive, shift register is ready to be shifted out.
 */
void Controller::write_latch(uint8_t val) {
    strobe = (val & 0x01);
    if (strobe) {
        shift_register = button_state.load(std::memory_order_relaxed);
    }
}

/**
 * When the CPU reads from $4016:
 * Returns the current bit and shifts to the next button.
 */
uint8_t Controller::read_serial() {
    if (strobe) {
        // While strobe is high, NES always returns the 'A' button status
        return (button_state.load() & 0x01) | 0x40; 
    }

    uint8_t value = (shift_register & 0x01);
    shift_register >>= 1;
    // NES returns 1 in bits 5-7 (open bus), bit 0 is the button status
    return value | 0x40; 
}
