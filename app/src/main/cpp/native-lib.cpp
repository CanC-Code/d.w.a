#include <jni.h>
#include <string>
#include <fstream>
#include <android/log.h>
#include <android/bitmap.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstring>
#include "MapperMMC1.h"
#include "recompiled/cpu_shared.h"

#define LOG_TAG "DWA_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace Dispatcher {
    void execute();
}

// ============================================================================
// GLOBAL EMULATION STATE
// ============================================================================
extern "C" {
    uint16_t reg_PC = 0;
    uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;
    int64_t total_cycles = 0;
    int32_t cycles_to_run = 0;
    bool is_running = false;
    bool nmi_pending = false;
    bool irq_pending = false;
    uint8_t joypad_state = 0;
}

uint8_t cpu_ram[0x0800] = {0};
uint8_t ppu_vram[0x0800] = {0};
uint8_t palette_ram[32] = {0};
uint8_t oam_ram[256] = {0};
uint32_t screen_buffer[256 * 240] = {0};
std::mutex buffer_mutex;

MapperMMC1 mapper;
uint8_t ppu_status = 0, ppu_ctrl = 0, ppu_mask = 0, oam_addr = 0;
uint16_t ppu_addr_reg = 0;
uint8_t ppu_addr_latch = 0, ppu_data_buffer = 0, controller_shift = 0;
bool is_paused = false;

// ============================================================================
// CORE CPU IMPLEMENTATION
// ============================================================================

extern "C" {
    void update_nz(uint8_t v) {
        reg_P &= ~(FLAG_N | FLAG_Z);
        if (v == 0) reg_P |= FLAG_Z;
        if (v & 0x80) reg_P |= FLAG_N;
    }

    void update_flags_cmp(uint8_t reg, uint8_t val) {
        reg_P &= ~(FLAG_C | FLAG_Z | FLAG_N);
        if (reg >= val) reg_P |= FLAG_C;
        if (reg == val) reg_P |= FLAG_Z;
        if ((uint8_t)(reg - val) & 0x80) reg_P |= FLAG_N;
    }

    void cpu_push(uint8_t v) { cpu_ram[0x0100 | (reg_S--)] = v; }
    uint8_t cpu_pop() { return cpu_ram[0x0100 | (++reg_S)]; }

    // --- New Hardware-Accurate Addressing Helpers ---
    
    uint16_t addr_abs_x(uint16_t base, bool* page_crossed) {
        uint16_t addr = base + reg_X;
        if (page_crossed && (addr & 0xFF00) != (base & 0xFF00)) {
            *page_crossed = true;
            add_cycles(1);
        }
        return addr;
    }

    uint16_t addr_abs_y(uint16_t base, bool* page_crossed) {
        uint16_t addr = base + reg_Y;
        if (page_crossed && (addr & 0xFF00) != (base & 0xFF00)) {
            *page_crossed = true;
            add_cycles(1);
        }
        return addr;
    }

    uint16_t cpu_read_pointer(uint16_t addr) {
        return bus_read(addr) | (bus_read(addr + 1) << 8);
    }

    uint16_t cpu_read_jmp_indirect(uint16_t addr) {
        uint8_t low = bus_read(addr);
        // Simulate 6502 Hardware Bug: Page Wrap
        uint16_t high_addr = (addr & 0xFF00) | ((addr + 1) & 0x00FF);
        uint8_t high = bus_read(high_addr);
        return (high << 8) | low;
    }

    uint16_t read_pointer_indexed_y(uint16_t zp, bool* page_crossed) {
        uint16_t base = bus_read(zp) | (bus_read((zp + 1) & 0xFF) << 8);
        uint16_t addr = base + reg_Y;
        if (page_crossed && (addr & 0xFF00) != (base & 0xFF00)) {
            *page_crossed = true;
            add_cycles(1);
        }
        return addr;
    }

    uint16_t read_pointer_indexed_x(uint16_t zp) {
        uint8_t addr_zp = (zp + reg_X) & 0xFF;
        return bus_read(addr_zp) | (bus_read((addr_zp + 1) & 0xFF) << 8);
    }

    // --- Arithmetic & Logic ---

    void cpu_adc(uint8_t v) {
        uint16_t c = (reg_P & FLAG_C) ? 1 : 0;
        uint16_t sum = reg_A + v + c;
        reg_P &= ~(FLAG_C | FLAG_V | FLAG_Z | FLAG_N);
        if (~(reg_A ^ v) & (reg_A ^ sum) & 0x80) reg_P |= FLAG_V;
        if (sum > 0xFF) reg_P |= FLAG_C;
        reg_A = (uint8_t)sum;
        update_nz(reg_A);
    }

    void cpu_sbc(uint8_t v) { cpu_adc(~v); }

    void cpu_bit(uint8_t v) {
        reg_P &= ~(FLAG_Z | FLAG_V | FLAG_N);
        if (!(reg_A & v)) reg_P |= FLAG_Z;
        if (v & 0x40) reg_P |= FLAG_V;
        if (v & 0x80) reg_P |= FLAG_N;
    }

    // Rotates/Shifts
    uint8_t cpu_asl(uint8_t v) {
        reg_P = (reg_P & ~FLAG_C) | ((v & 0x80) ? FLAG_C : 0);
        v <<= 1; update_nz(v); return v;
    }
    uint8_t cpu_lsr(uint8_t v) {
        reg_P = (reg_P & ~FLAG_C) | (v & FLAG_C);
        v >>= 1; update_nz(v); return v;
    }
    uint8_t cpu_rol(uint8_t v) {
        uint8_t c = (reg_P & FLAG_C) ? 1 : 0;
        reg_P = (reg_P & ~FLAG_C) | ((v & 0x80) ? FLAG_C : 0);
        v = (v << 1) | c; update_nz(v); return v;
    }
    uint8_t cpu_ror(uint8_t v) {
        uint8_t c = (reg_P & FLAG_C) ? 0x80 : 0;
        reg_P = (reg_P & ~FLAG_C) | (v & FLAG_C);
        v = (v >> 1) | c; update_nz(v); return v;
    }

    void execute_instruction() {
        if (nmi_pending) { nmi_handler(); nmi_pending = false; }
        else if (irq_pending && !(reg_P & FLAG_I)) { irq_handler(); irq_pending = false; }
        Dispatcher::execute();
    }
}

// ============================================================================
// BUS READ/WRITE
// ============================================================================

extern "C" uint8_t bus_read(uint16_t a) {
    if (a < 0x2000) return cpu_ram[a & 0x07FF];
    if (a < 0x4000) {
        uint16_t r = a & 7;
        if (r == 2) { uint8_t s = ppu_status; ppu_status &= 0x7F; ppu_addr_latch = 0; return s; }
        if (r == 7) { 
            uint8_t d = ppu_data_buffer; uint16_t pa = ppu_addr_reg & 0x3FFF;
            if (pa < 0x2000) ppu_data_buffer = mapper.read_chr(pa);
            else if (pa < 0x3F00) ppu_data_buffer = ppu_vram[pa & 0x07FF];
            else { d = palette_ram[pa & 0x1F]; ppu_data_buffer = d; }
            ppu_addr_reg += (ppu_ctrl & 4) ? 32 : 1; return d;
        }
    }
    if (a == 0x4016) { uint8_t r = (controller_shift & 0x80) >> 7; controller_shift <<= 1; return r; }
    return (a >= 0x6000) ? mapper.read_prg(a) : 0;
}

extern "C" void bus_write(uint16_t a, uint8_t v) {
    if (a < 0x2000) cpu_ram[a & 0x07FF] = v;
    else if (a < 0x4000) {
        uint16_t r = a & 7;
        if (r == 0) ppu_ctrl = v; 
        else if (r == 1) ppu_mask = v;
        else if (r == 3) oam_addr = v;
        else if (r == 6) { 
            if (!ppu_addr_latch) { ppu_addr_reg = (ppu_addr_reg & 0xFF) | ((v & 0x3F) << 8); ppu_addr_latch = 1; } 
            else { ppu_addr_reg = (ppu_addr_reg & 0xFF00) | v; ppu_addr_latch = 0; } 
        }
        else if (r == 7) { 
            uint16_t pa = ppu_addr_reg & 0x3FFF; 
            if (pa >= 0x2000 && pa < 0x3F00) ppu_vram[pa & 0x07FF] = v; 
            else if (pa >= 0x3F00) palette_ram[pa & 0x1F] = v; 
            ppu_addr_reg += (ppu_ctrl & 4) ? 32 : 1; 
        }
    }
    else if (a == 0x4014) { add_cycles(513); uint16_t b = v << 8; for(int i=0; i<256; i++) oam_ram[i] = bus_read(b+i); }
    else if (a == 0x4016) { if (!(v&1)) controller_shift = joypad_state; }
    else if (a >= 0x6000) mapper.write(a, v);
}

// ============================================================================
// HANDLERS & ENGINE
// ============================================================================

extern "C" void nmi_handler() {
    cpu_push(reg_PC >> 8); cpu_push(reg_PC & 0xFF); cpu_push(reg_P);
    reg_P |= FLAG_I;
    reg_PC = cpu_read_pointer(0xFFFA);
    add_cycles(7);
}

extern "C" void irq_handler() {
    cpu_push(reg_PC >> 8); cpu_push(reg_PC & 0xFF); cpu_push(reg_P);
    reg_P |= FLAG_I;
    reg_PC = cpu_read_pointer(0xFFFE);
    add_cycles(7);
}

extern "C" void power_on_reset() {
    mapper.reset();
    reg_PC = cpu_read_pointer(0xFFFC);
    if (reg_PC < 0x8000) reg_PC = 0xC000;
    reg_S = 0xFD; reg_P = 0x24; is_running = true;
    LOGI("Engine: Reset to $%04X", reg_PC);
}

void engine_loop() {
    power_on_reset();
    while (is_running) {
        if (is_paused) { std::this_thread::sleep_for(std::chrono::milliseconds(16)); continue; }
        auto frame_start = std::chrono::steady_clock::now();
        cycles_to_run += 29780;
        while (cycles_to_run > 0) execute_instruction();
        nmi_pending = true; ppu_status |= 0x80;
        std::this_thread::sleep_until(frame_start + std::chrono::microseconds(16666));
    }
}

// ============================================================================
// JNI INTERFACE
// ============================================================================

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char *path = env->GetStringUTFChars(romPath, nullptr);
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return env->NewStringUTF("Error: File not found");

    uint8_t header[16]; file.read((char*)header, 16);
    if (header[0]!='N' || header[1]!='E' || header[2]!='S') return env->NewStringUTF("Error: Invalid iNES");

    for (int i=0; i<header[4] && i<4; i++) file.read((char*)mapper.prg_rom[i], 16384);
    if (header[5]>0) file.read((char*)mapper.chr_rom, header[5]*8192);

    file.close(); env->ReleaseStringUTFChars(romPath, path);
    return env->NewStringUTF("Success");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* e, jobject t, jstring d) {
    if (!is_running) std::thread(engine_loop).detach();
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv* env, jobject thiz, jobject bitmap) {
    AndroidBitmapInfo info; void* pixels;
    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0) return;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;
    { std::lock_guard<std::mutex> lock(buffer_mutex); memcpy(pixels, screen_buffer, 256*240*4); }
    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* e, jobject t, jint b, jboolean p) {
    if (p) joypad_state |= (uint8_t)b; else joypad_state &= ~((uint8_t)b);
}
