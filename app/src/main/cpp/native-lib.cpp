#include <jni.h>
#include <string>
#include <vector>
#include <fstream>
#include <android/log.h>
#include <android/bitmap.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstring>
#include "MapperMMC1.h"
#include "PPU.h"
#include "recompiled/cpu_shared.h"

#define LOG_TAG "DWA_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Memory addresses specific to Dragon Warrior's engine
#define DW_RAM_VBLANK_FLAG   0x002D
#define DW_RAM_FRAME_COUNTER 0x003C

namespace Dispatcher { 
    void execute(); 
    void request_nmi(); 
}

// --- Hardware State ---
uint8_t cpu_ram[0x0800] = {0};
MapperMMC1 mapper;
PPU ppu;
std::mutex buffer_mutex;

bool is_running = false;
bool is_paused = false;
uint8_t controller_state = 0;
uint8_t controller_shift = 0;
uint8_t last_strobe = 0;

uint32_t nes_palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00,
    0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFEFB0FF, 0xFF64B0FF, 0xFF9290FF, 0xFFC676FF, 0xFFF36AFF, 0xFFFE6ECC, 0xFFFE8170, 0xFFEA9E22,
    0xFFBCBE00, 0xFF88D100, 0xFF5CE430, 0xFF45E082, 0xFF48CDDE, 0xFF4F4F4F, 0xFF000000, 0xFF000000
};

// --- CPU Global State ---
extern "C" {
    uint16_t reg_PC = 0;
    uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;

    void execute_instruction() { Dispatcher::execute(); }

    void update_nz(uint8_t val) {
        reg_P &= ~(FLAG_N | FLAG_Z);
        if (val == 0) reg_P |= FLAG_Z;
        if (val & 0x80) reg_P |= FLAG_N;
    }

    void update_flags_cmp(uint8_t reg, uint8_t val) {
        reg_P &= ~(FLAG_N | FLAG_Z | FLAG_C);
        if (reg >= val) reg_P |= FLAG_C;
        if (reg == val) reg_P |= FLAG_Z;
        if ((uint8_t)(reg - val) & 0x80) reg_P |= FLAG_N;
    }

    void cpu_adc(uint8_t val) {
        uint16_t carry = (reg_P & FLAG_C) ? 1 : 0;
        uint16_t sum = reg_A + val + carry;
        // Overflow flag logic
        if (~(reg_A ^ val) & (reg_A ^ sum) & 0x80) reg_P |= FLAG_V; else reg_P &= ~FLAG_V;
        if (sum > 0xFF) reg_P |= FLAG_C; else reg_P &= ~FLAG_C;
        reg_A = (uint8_t)sum;
        update_nz(reg_A);
    }

    void cpu_sbc(uint8_t val) { cpu_adc(~val); }

    void cpu_bit(uint8_t val) {
        reg_P &= ~(FLAG_Z | FLAG_V | FLAG_N);
        if ((val & reg_A) == 0) reg_P |= FLAG_Z;
        reg_P |= (val & 0xC0); // Bits 6 and 7 of memory are transferred to V and N
    }

    // --- Shift Operations ---
    uint8_t cpu_asl(uint8_t val) {
        reg_P = (reg_P & ~FLAG_C) | ((val >> 7) & 0x01);
        uint8_t res = val << 1;
        update_nz(res);
        return res;
    }
    uint8_t cpu_lsr(uint8_t val) {
        reg_P = (reg_P & ~FLAG_C) | (val & 0x01);
        uint8_t res = val >> 1;
        update_nz(res);
        return res;
    }
    uint8_t cpu_rol(uint8_t val) {
        uint8_t old_c = (reg_P & FLAG_C) ? 1 : 0;
        reg_P = (reg_P & ~FLAG_C) | ((val >> 7) & 0x01);
        uint8_t res = (val << 1) | old_c;
        update_nz(res);
        return res;
    }
    uint8_t cpu_ror(uint8_t val) {
        uint8_t old_c = (reg_P & FLAG_C) ? 1 : 0;
        reg_P = (reg_P & ~FLAG_C) | (val & 0x01);
        uint8_t res = (val >> 1) | (old_c << 7);
        update_nz(res);
        return res;
    }

    // --- Bus Access ---
    uint8_t bus_read(uint16_t addr) {
        if (addr < 0x2000) return cpu_ram[addr % 0x0800];
        if (addr >= 0x2000 && addr <= 0x3FFF) return ppu.cpu_read(addr, mapper);
        if (addr == 0x4016) {
            uint8_t r = (controller_shift & 0x80) >> 7;
            controller_shift <<= 1;
            return 0x40 | r;
        }
        // PRG-ROM Access via Mapper
        if (addr >= 0x6000) return mapper.read_prg(addr);
        return 0;
    }

    void bus_write(uint16_t addr, uint8_t val) {
        if (addr < 0x2000) cpu_ram[addr % 0x0800] = val;
        else if (addr >= 0x2000 && addr <= 0x3FFF) ppu.cpu_write(addr, val, mapper);
        else if (addr == 0x4014) { // OAM DMA
            uint16_t b = val << 8;
            for(int i=0; i<256; i++) ppu.oam_ram[i] = bus_read(b + i);
        }
        else if (addr == 0x4016) {
            if ((val & 0x01) == 0 && (last_strobe & 0x01) == 1) controller_shift = controller_state;
            last_strobe = val;
        }
        else if (addr >= 0x6000) mapper.write(addr, val);
    }

    // --- Pointers & Stack ---
    uint16_t read_pointer(uint16_t addr) { return bus_read(addr) | (bus_read(addr + 1) << 8); }
    
    // Support for Zero-Page Wrap-around
    uint16_t read_pointer_indexed_x(uint16_t addr) {
        uint8_t zp = (uint8_t)(addr + reg_X);
        return bus_read(zp) | (bus_read((uint8_t)(zp + 1)) << 8);
    }
    uint16_t read_pointer_indexed_y(uint16_t zp_addr) {
        uint16_t lo = bus_read((uint8_t)zp_addr);
        uint16_t hi = bus_read((uint8_t)(zp_addr + 1));
        return (uint16_t)((lo | (hi << 8)) + reg_Y);
    }

    void push_stack(uint8_t val) { cpu_ram[0x0100 | (reg_S & 0xFF)] = val; reg_S--; }
    uint8_t pop_stack() { reg_S++; return cpu_ram[0x0100 | (reg_S & 0xFF)]; }
}

// --- Interrupt Handling ---
void nmi_handler() {
    // Dragon Warrior logic depends on these RAM flags to exit "WaitVblank" loops
    cpu_ram[DW_RAM_VBLANK_FLAG] = 1; 
    cpu_ram[DW_RAM_FRAME_COUNTER]++;
    Dispatcher::request_nmi();
}

// --- Main Engine Loop ---
void engine_loop() {
    LOGI("Engine Thread Started");
    memset(cpu_ram, 0, sizeof(cpu_ram));
    mapper.reset();
    ppu.reset();

    // Reset CPU registers
    reg_A = 0; reg_X = 0; reg_Y = 0;
    reg_S = 0xFD; reg_P = 0x24;

    // Fetch Reset Vector from ROM
    uint8_t lo = bus_read(0xFFFC);
    uint8_t hi = bus_read(0xFFFD);
    reg_PC = (hi << 8) | lo;
    LOGI("CPU Booting from Reset Vector: 0x%04X", reg_PC);

    is_running = true;
    uint32_t total_cycles = 0;

    while (is_running) {
        if (is_paused) { std::this_thread::sleep_for(std::chrono::milliseconds(16)); continue; }

        auto frame_start = std::chrono::steady_clock::now();

        // 29780 cycles per frame divided into slices for better PPU/CPU sync
        for (int slice = 0; slice < 10; slice++) {
            for (int i = 0; i < 2978; i++) {
                uint16_t prev_pc = reg_PC;
                execute_instruction();
                // Prevent hardware trap: if instruction didn't move PC, force it
                if (reg_PC == prev_pc) reg_PC++; 
                total_cycles++;
            }
            
            // Periodically check if we should trigger VBlank mid-frame to satisfy boot checks
            if (slice == 5) ppu.status |= 0x80; 
            
            if (total_cycles % 150000 == 0) {
                LOGI("Heartbeat - PC: 0x%04X, A: %02X, SP: %02X, Flags: %02X", reg_PC, reg_A, reg_S, reg_P);
            }
        }

        // Check for NMI request from PPU
        if (ppu.ctrl & 0x80) nmi_handler();
        
        // Finalize frame and clear VBlank for the next one
        ppu.status &= ~0x80;

        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            ppu.render_frame(mapper, nes_palette);
        }

        std::this_thread::sleep_until(frame_start + std::chrono::microseconds(16666));
    }
    LOGI("Engine Thread Stopped");
}

// --- JNI Interface ---

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv *env, jobject thiz, jstring romPath, jstring outDir) {
    const char *path = env->GetStringUTFChars(romPath, nullptr);
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        LOGE("Failed to open ROM at %s", path);
        env->ReleaseStringUTFChars(romPath, path);
        return env->NewStringUTF("Error: Could not open ROM");
    }

    // iNES Header skip
    file.seekg(16, std::ios::beg);
    // Load PRG/CHR Banks
    for (int i = 0; i < 4; i++) file.read((char*)mapper.prg_rom[i], 16384);
    for (int i = 0; i < 2; i++) file.read((char*)mapper.chr_rom[i], 4096);

    file.close();
    LOGI("ROM Loaded into MapperMMC1. Reset Vector is 0x%02X%02X", mapper.read_prg(0xFFFD), mapper.read_prg(0xFFFC));
    env->ReleaseStringUTFChars(romPath, path);
    return env->NewStringUTF("Success");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv *env, jobject thiz, jstring filesDir) {
    if (!is_running) {
        std::thread(engine_loop).detach();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv *env, jobject thiz, jobject bitmap) {
    void* pixels;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;
    {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        memcpy(pixels, ppu.screen_buffer, 256 * 240 * 4);
    }
    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C" JNIEXPORT void JNICALL 
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv *env, jobject thiz, jint bit, jboolean p) {
    if (p) controller_state |= (uint8_t)bit; else controller_state &= ~((uint8_t)bit);
}

extern "C" JNIEXPORT void JNICALL 
Java_com_canc_dwa_MainActivity_nativePauseEngine(JNIEnv *env, jobject thiz) { is_paused = true; }

extern "C" JNIEXPORT void JNICALL 
Java_com_canc_dwa_MainActivity_nativeResumeEngine(JNIEnv *env, jobject thiz) { is_paused = false; }
