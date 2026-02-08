#include <jni.h>
#include <android/bitmap.h>
#include <android/log.h>
#include <cstring>
#include <cstdint>
#include <atomic>
#include <mutex>

#define LOG_TAG "DWA_Native"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// External declarations
extern void ppu_init();
extern void ppu_reset();
extern void ppu_render_frame(uint32_t* pixels);
extern void ppu_step();
extern uint8_t ppu_read_register(uint16_t addr);
extern void ppu_write_register(uint16_t addr, uint8_t value);

extern void mapper_init(uint8_t* prg_rom, size_t prg_size, uint8_t* chr_rom, size_t chr_size);
extern void mapper_reset();
extern uint8_t mapper_read(uint16_t addr);
extern void mapper_write(uint16_t addr, uint8_t value);
extern uint8_t mapper_get_current_bank();  // NEW: Get current PRG bank number

extern void controller_set_button(uint8_t button, bool pressed);
extern uint8_t controller_read();
extern void controller_write(uint8_t value);

// Recompiled banks - forward declarations
namespace Bank00 { void execute_at(uint16_t pc); }
namespace Bank01 { void execute_at(uint16_t pc); }
namespace Bank02 { void execute_at(uint16_t pc); }
namespace Bank03 { void execute_at(uint16_t pc); }

// CPU State - shared globally with recompiled code
uint8_t reg_A = 0;
uint8_t reg_X = 0;
uint8_t reg_Y = 0;
uint8_t reg_P = 0x24;  // IRQ disabled on startup
uint8_t reg_SP = 0xFD;
uint16_t reg_PC = 0;
int cycles_to_run = 0;

// RAM
static uint8_t ram[0x800];
static uint8_t sram[0x2000];  // Battery-backed save RAM

// ROM storage
static uint8_t* prg_rom = nullptr;
static uint8_t* chr_rom = nullptr;
static size_t prg_size = 0;
static size_t chr_size = 0;

// Emulation state
static std::atomic<bool> emulator_running{false};
static std::mutex emu_mutex;
static bool rom_loaded = false;

// Frame buffer (256x240 NES resolution)
static uint32_t frame_buffer[256 * 240];

// CPU Bus Read
uint8_t bus_read(uint16_t addr) {
    if (addr < 0x2000) {
        return ram[addr & 0x7FF];  // RAM mirrored
    } else if (addr < 0x4000) {
        return ppu_read_register(0x2000 | (addr & 0x7));  // PPU registers mirrored
    } else if (addr == 0x4016) {
        return controller_read();
    } else if (addr >= 0x6000 && addr < 0x8000) {
        return sram[addr - 0x6000];  // SRAM
    } else if (addr >= 0x8000) {
        return mapper_read(addr);  // ROM through mapper
    }
    return 0;
}

// CPU Bus Write
void bus_write(uint16_t addr, uint8_t value) {
    if (addr < 0x2000) {
        ram[addr & 0x7FF] = value;
    } else if (addr < 0x4000) {
        ppu_write_register(0x2000 | (addr & 0x7), value);
    } else if (addr == 0x4014) {
        // OAM DMA
        uint16_t dma_addr = value << 8;
        for (int i = 0; i < 256; i++) {
            uint8_t byte = bus_read(dma_addr + i);
            ppu_write_register(0x2004, byte);
        }
        cycles_to_run -= 513;  // DMA takes 513 cycles
    } else if (addr == 0x4016) {
        controller_write(value);
    } else if (addr >= 0x6000 && addr < 0x8000) {
        sram[addr - 0x6000] = value;
    } else if (addr >= 0x8000) {
        mapper_write(addr, value);
    }
}

// CPU Helper functions (used by recompiled code)
extern "C" {
    void cpu_push(uint8_t value) {
        bus_write(0x100 | reg_SP, value);
        reg_SP--;
    }

    uint8_t cpu_pop() {
        reg_SP++;
        return bus_read(0x100 | reg_SP);
    }

    uint16_t cpu_read_pointer(uint16_t addr) {
        uint8_t lo = bus_read(addr);
        uint8_t hi = bus_read(addr + 1);
        return lo | (hi << 8);
    }

    void update_nz(uint8_t value) {
        if (value == 0) reg_P |= 0x02;   // Zero flag
        else reg_P &= ~0x02;
        if (value & 0x80) reg_P |= 0x80; // Negative flag
        else reg_P &= ~0x80;
    }

    void update_flags_cmp(uint8_t reg_val, uint8_t value) {
        uint16_t result = reg_val - value;
        if (reg_val >= value) reg_P |= 0x01;  // Carry
        else reg_P &= ~0x01;
        update_nz(result & 0xFF);
    }

    void cpu_adc(uint8_t value) {
        uint16_t result = reg_A + value + (reg_P & 0x01 ? 1 : 0);
        if (((reg_A ^ result) & (value ^ result) & 0x80) != 0) reg_P |= 0x40;  // Overflow
        else reg_P &= ~0x40;
        if (result > 0xFF) reg_P |= 0x01;  // Carry
        else reg_P &= ~0x01;
        reg_A = result & 0xFF;
        update_nz(reg_A);
    }

    void cpu_sbc(uint8_t value) {
        cpu_adc(~value);
    }

    void cpu_bit(uint8_t value) {
        if ((reg_A & value) == 0) reg_P |= 0x02;  // Zero
        else reg_P &= ~0x02;
        if (value & 0x80) reg_P |= 0x80;  // Negative
        else reg_P &= ~0x80;
        if (value & 0x40) reg_P |= 0x40;  // Overflow
        else reg_P &= ~0x40;
    }

    uint8_t cpu_asl(uint8_t value) {
        if (value & 0x80) reg_P |= 0x01;
        else reg_P &= ~0x01;
        value <<= 1;
        update_nz(value);
        return value;
    }

    uint8_t cpu_lsr(uint8_t value) {
        if (value & 0x01) reg_P |= 0x01;
        else reg_P &= ~0x01;
        value >>= 1;
        update_nz(value);
        return value;
    }

    uint8_t cpu_rol(uint8_t value) {
        uint8_t old_carry = (reg_P & 0x01) ? 1 : 0;
        if (value & 0x80) reg_P |= 0x01;
        else reg_P &= ~0x01;
        value = (value << 1) | old_carry;
        update_nz(value);
        return value;
    }

    uint8_t cpu_ror(uint8_t value) {
        uint8_t old_carry = (reg_P & 0x01) ? 0x80 : 0;
        if (value & 0x01) reg_P |= 0x01;
        else reg_P &= ~0x01;
        value = (value >> 1) | old_carry;
        update_nz(value);
        return value;
    }

    uint16_t addr_abs_x(uint16_t addr, bool* page_crossed) {
        uint16_t result = addr + reg_X;
        if (page_crossed) *page_crossed = ((addr & 0xFF00) != (result & 0xFF00));
        return result;
    }

    uint16_t addr_abs_y(uint16_t addr, bool* page_crossed) {
        uint16_t result = addr + reg_Y;
        if (page_crossed) *page_crossed = ((addr & 0xFF00) != (result & 0xFF00));
        return result;
    }

    uint16_t read_pointer_indexed_x(uint16_t zp_addr) {
        uint8_t addr = (zp_addr + reg_X) & 0xFF;
        uint8_t lo = bus_read(addr);
        uint8_t hi = bus_read((addr + 1) & 0xFF);
        return lo | (hi << 8);
    }

    uint16_t read_pointer_indexed_y(uint16_t zp_addr, bool* page_crossed) {
        uint8_t lo = bus_read(zp_addr & 0xFF);
        uint8_t hi = bus_read((zp_addr + 1) & 0xFF);
        uint16_t base = lo | (hi << 8);
        uint16_t result = base + reg_Y;
        if (page_crossed) *page_crossed = ((base & 0xFF00) != (result & 0xFF00));
        return result;
    }
}

// Dispatcher - routes execution to correct bank
static void execute_recompiled(uint16_t pc) {
    if (pc >= 0xC000) {
        // Fixed bank 3
        Bank03::execute_at(pc);
    } else if (pc >= 0x8000) {
        // Switchable bank - ask mapper which one
        uint8_t bank_num = mapper_get_current_bank();
        switch (bank_num) {
            case 0: Bank00::execute_at(pc); break;
            case 1: Bank01::execute_at(pc); break;
            case 2: Bank02::execute_at(pc); break;
            default: Bank00::execute_at(pc); break;
        }
    } else {
        // Invalid PC
        LOGE("Invalid PC: $%04X", pc);
        reg_PC++;
    }
}

// Reset CPU to power-on state
static void cpu_reset() {
    reg_A = 0;
    reg_X = 0;
    reg_Y = 0;
    reg_P = 0x24;  // IRQ disabled
    reg_SP = 0xFD;
    
    // Read reset vector from $FFFC
    reg_PC = cpu_read_pointer(0xFFFC);
    LOGI("CPU Reset: PC set to $%04X", reg_PC);
    
    cycles_to_run = 0;
    memset(ram, 0, sizeof(ram));
}

// Execute CPU for one frame (29780 cycles @ 1.79 MHz)
static void cpu_run_frame() {
    cycles_to_run = 29780;
    int safety_counter = 0;
    
    while (cycles_to_run > 0) {
        // Safety check to prevent infinite loops
        if (++safety_counter > 100000) {
            LOGE("CPU safety limit reached at PC=$%04X", reg_PC);
            break;
        }
        
        uint16_t prev_pc = reg_PC;
        execute_recompiled(reg_PC);
        
        // If PC didn't change, something is wrong
        if (reg_PC == prev_pc) {
            LOGE("PC stuck at $%04X", reg_PC);
            reg_PC++;
        }
        
        // Step PPU (3 PPU cycles per CPU cycle)
        for (int i = 0; i < 3; i++) {
            ppu_step();
        }
    }
}

// JNI: Load ROM
extern "C" JNIEXPORT jboolean JNICALL
Java_com_canc_dwa_MainActivity_loadROM(JNIEnv* env, jobject, jbyteArray rom_data) {
    std::lock_guard<std::mutex> lock(emu_mutex);
    
    LOGI("Loading ROM...");
    
    jsize rom_length = env->GetArrayLength(rom_data);
    if (rom_length < 16) {
        LOGE("ROM too small: %d bytes", rom_length);
        return JNI_FALSE;
    }
    
    uint8_t* rom_bytes = (uint8_t*)env->GetByteArrayElements(rom_data, nullptr);
    
    // Verify iNES header
    if (rom_bytes[0] != 'N' || rom_bytes[1] != 'E' || rom_bytes[2] != 'S' || rom_bytes[3] != 0x1A) {
        LOGE("Invalid NES header: %02X %02X %02X %02X", 
             rom_bytes[0], rom_bytes[1], rom_bytes[2], rom_bytes[3]);
        env->ReleaseByteArrayElements(rom_data, (jbyte*)rom_bytes, JNI_ABORT);
        return JNI_FALSE;
    }
    
    // Parse header
    uint8_t prg_banks = rom_bytes[4];  // 16KB banks
    uint8_t chr_banks = rom_bytes[5];  // 8KB banks
    uint8_t mapper_num = (rom_bytes[6] >> 4) | (rom_bytes[7] & 0xF0);
    
    LOGI("PRG Banks: %d, CHR Banks: %d, Mapper: %d", prg_banks, chr_banks, mapper_num);
    
    if (mapper_num != 1) {
        LOGE("Only MMC1 (mapper 1) is supported, got mapper %d", mapper_num);
        env->ReleaseByteArrayElements(rom_data, (jbyte*)rom_bytes, JNI_ABORT);
        return JNI_FALSE;
    }
    
    // Allocate and copy PRG-ROM
    prg_size = prg_banks * 16384;
    if (prg_rom) delete[] prg_rom;
    prg_rom = new uint8_t[prg_size];
    memcpy(prg_rom, rom_bytes + 16, prg_size);
    LOGI("Loaded PRG-ROM: %zu bytes", prg_size);
    
    // Allocate and copy CHR-ROM (or RAM if chr_banks == 0)
    chr_size = chr_banks > 0 ? chr_banks * 8192 : 8192;
    if (chr_rom) delete[] chr_rom;
    chr_rom = new uint8_t[chr_size];
    if (chr_banks > 0) {
        memcpy(chr_rom, rom_bytes + 16 + prg_size, chr_size);
        LOGI("Loaded CHR-ROM: %zu bytes", chr_size);
    } else {
        memset(chr_rom, 0, chr_size);  // CHR-RAM
        LOGI("Initialized CHR-RAM: %zu bytes", chr_size);
    }
    
    env->ReleaseByteArrayElements(rom_data, (jbyte*)rom_bytes, JNI_ABORT);
    
    // Initialize subsystems
    LOGI("Initializing PPU...");
    ppu_init();
    
    LOGI("Initializing Mapper...");
    mapper_init(prg_rom, prg_size, chr_rom, chr_size);
    
    // Reset to power-on state
    LOGI("Resetting PPU...");
    ppu_reset();
    
    LOGI("Resetting Mapper...");
    mapper_reset();
    
    LOGI("Resetting CPU...");
    cpu_reset();
    
    rom_loaded = true;
    emulator_running = true;
    
    LOGI("=== ROM LOADED SUCCESSFULLY ===");
    LOGI("Reset vector points to: $%04X", reg_PC);
    return JNI_TRUE;
}

// JNI: Run one frame
extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_runFrame(JNIEnv* env, jobject, jobject bitmap) {
    if (!rom_loaded || !emulator_running) return;
    
    std::lock_guard<std::mutex> lock(emu_mutex);
    
    // Clear frame buffer
    memset(frame_buffer, 0, sizeof(frame_buffer));
    
    // Run CPU for one frame
    cpu_run_frame();
    
    // Render PPU output to frame buffer
    ppu_render_frame(frame_buffer);
    
    // Copy frame buffer to Android bitmap
    AndroidBitmapInfo info;
    void* pixels;
    if (AndroidBitmap_getInfo(env, bitmap, &info) == ANDROID_BITMAP_RESULT_SUCCESS) {
        if (AndroidBitmap_lockPixels(env, bitmap, &pixels) == ANDROID_BITMAP_RESULT_SUCCESS) {
            if (info.width == 256 && info.height == 240) {
                memcpy(pixels, frame_buffer, sizeof(frame_buffer));
            } else {
                // Nearest-neighbor scaling
                uint32_t* dest = (uint32_t*)pixels;
                for (uint32_t y = 0; y < info.height; y++) {
                    for (uint32_t x = 0; x < info.width; x++) {
                        uint32_t src_x = (x * 256) / info.width;
                        uint32_t src_y = (y * 240) / info.height;
                        dest[y * info.width + x] = frame_buffer[src_y * 256 + src_x];
                    }
                }
            }
            AndroidBitmap_unlockPixels(env, bitmap);
        }
    }
}

// JNI: Input handling
extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_setButton(JNIEnv*, jobject, jint button, jboolean pressed) {
    controller_set_button((uint8_t)button, pressed);
}

// JNI: Cleanup
extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_cleanup(JNIEnv*, jobject) {
    std::lock_guard<std::mutex> lock(emu_mutex);
    emulator_running = false;
    rom_loaded = false;
    
    if (prg_rom) {
        delete[] prg_rom;
        prg_rom = nullptr;
    }
    if (chr_rom) {
        delete[] chr_rom;
        chr_rom = nullptr;
    }
}