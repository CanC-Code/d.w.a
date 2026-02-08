#include <cstdint>
#include <atomic>

// Controller state (thread-safe)
static std::atomic<uint8_t> controller_state{0};
static uint8_t controller_shift = 0;
static bool controller_strobe = false;

// Button bit masks
#define BTN_A      0x01
#define BTN_B      0x02
#define BTN_SELECT 0x04
#define BTN_START  0x08
#define BTN_UP     0x10
#define BTN_DOWN   0x20
#define BTN_LEFT   0x40
#define BTN_RIGHT  0x80

// Set button state (called from Java)
void controller_set_button(uint8_t button, bool pressed) {
    uint8_t current = controller_state.load(std::memory_order_relaxed);
    if (pressed) {
        controller_state.store(current | button, std::memory_order_release);
    } else {
        controller_state.store(current & ~button, std::memory_order_release);
    }
}

// Write to $4016 (strobe)
void controller_write(uint8_t value) {
    bool new_strobe = value & 0x01;
    
    if (controller_strobe && !new_strobe) {
        // Strobe transition: latch current state
        controller_shift = controller_state.load(std::memory_order_acquire);
    }
    
    controller_strobe = new_strobe;
}

// Read from $4016 (return next bit)
uint8_t controller_read() {
    if (controller_strobe) {
        // While strobing, always return A button state
        return (controller_state.load(std::memory_order_acquire) & BTN_A) ? 0x41 : 0x40;
    }
    
    // Return current bit
    uint8_t result = (controller_shift & 0x01) ? 0x41 : 0x40;
    controller_shift >>= 1;
    controller_shift |= 0x80;  // Set bit 7 (no controller on upper bits)
    
    return result;
}