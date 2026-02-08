#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <cstdint>
#include <atomic>

class Controller {
public:
    Controller();

    // Called by the Android UI thread (via JNI)
    void update_state(uint8_t state);

    // Called by the CPU thread (via bus_read/write)
    void write_latch(uint8_t val);
    uint8_t read_serial();

private:
    std::atomic<uint8_t> button_state; // Current physical buttons held
    uint8_t shift_register;            // Latched state being shifted out
    bool strobe;                       // High = continuous reloading
};

#endif
