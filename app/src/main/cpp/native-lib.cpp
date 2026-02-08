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
int frame_count = 0;

uint32_t nes_palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00,
    0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFFFFFFF, 0xFF64B0FF, 0xFF9290FF, 0xFFC676FF, 0xFFF36AFF, 0xFFFE6ECC, 0xFFFE8170, 0xFFEA9E22,
    0xFFBCBE00, 0xFF88D100, 0xFF5CE430, 0xFF45E082, 0xFF48CDDE, 0xFF4F4F4F, 0xFF000000, 0xFF000000,
    0xFFFFFFFF, 0xFFC0DFFF, 0xFFD3D2FF, 0xFFE8C8FF, 0xFFFBC2FF, 0xFFFEC4EA, 0xFFFECCC5, 0xFFF7D8A5,
    0xFFE4E594, 0xFFCFEF96, 0xFFBDF4AB, 0xFFB3F3CC, 0xFFB5EBF2, 0xFFB8B8B8, 0xFF000000, 0xFF000000
};

// ============================================================================
// CORE LOGIC
// ============================================================================

extern "C" {
    void execute_instruction() {
        if (nmi_pending) { nmi_handler(); nmi_pending = false; }
        else if (irq_pending && !(reg_P & FLAG_I)) { irq_handler(); irq_pending = false; }
        Dispatcher::execute();
    }

    void add_cycles(uint32_t c) { total_cycles += c; cycles_to_run -= c; }
    void update_nz(uint8_t v) { reg_P = (reg_P & 0x7D) | (v==0?2:0) | (v&0x80); }
    
    void update_flags_cmp(uint8_t r, uint8_t v) {
        reg_P &= 0x7C;
        if (r >= v) reg_P |= 1;
        if (r == v) reg_P |= 2;
        if ((uint8_t)(r-v) & 0x80) reg_P |= 0x80;
    }

    void cpu_adc(uint8_t v) {
        uint16_t c = (reg_P & 1), s = reg_A + v + c;
        if (~(reg_A ^ v) & (reg_A ^ s) & 0x80) reg_P |= 0x40; else reg_P &= 0xBF;
        if (s > 0xFF) reg_P |= 1; else reg_P &= 0xFE;
        reg_A = (uint8_t)s; update_nz(reg_A);
    }
    void cpu_sbc(uint8_t v) { cpu_adc(~v); }

    uint16_t read_pointer_indexed_y(uint16_t zp, bool* pc) {
        uint16_t b = bus_read(zp) | (bus_read((zp+1)&0xFF) << 8), a = b + reg_Y;
        if (pc) *pc = (a & 0xFF00) != (b & 0xFF00);
        return a;
    }

    void push_stack(uint8_t v) { cpu_ram[0x0100 | (reg_S--)] = v; }
    uint8_t pop_stack() { return cpu_ram[0x0100 | (++reg_S)]; }
}

// ============================================================================
// BUS & HARDWARE
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
        if (r == 0) ppu_ctrl = v; else if (r == 1) ppu_mask = v;
        else if (r == 6) { if (!ppu_addr_latch) { ppu_addr_reg = (ppu_addr_reg & 0xFF) | ((v&0x3F)<<8); ppu_addr_latch=1; } else { ppu_addr_reg = (ppu_addr_reg&0xFF00)|v; ppu_addr_latch=0; } }
        else if (r == 7) { uint16_t pa = ppu_addr_reg & 0x3FFF; if (pa >= 0x2000 && pa < 0x3F00) ppu_vram[pa&0x07FF]=v; else if (pa>=0x3F00) palette_ram[pa&0x1F]=v; ppu_addr_reg += (ppu_ctrl&4)?32:1; }
    }
    else if (a == 0x4014) { add_cycles(513); uint16_t b = v << 8; for(int i=0; i<256; i++) oam_ram[i] = bus_read(b+i); }
    else if (a == 0x4016) { if (!(v&1)) controller_shift = joypad_state; }
    else if (a >= 0x6000) mapper.write(a, v);
}

// ============================================================================
// EMULATION CONTROL
// ============================================================================

extern "C" void nmi_handler() {
    push_stack(reg_PC >> 8); push_stack(reg_PC & 0xFF); push_stack(reg_P);
    reg_PC = mapper.read_prg(0xFFFA) | (mapper.read_prg(0xFFFB) << 8);
    reg_P |= 0x04; add_cycles(7);
}

extern "C" void irq_handler() {
    push_stack(reg_PC >> 8); push_stack(reg_PC & 0xFF); push_stack(reg_P);
    reg_PC = mapper.read_prg(0xFFFE) | (mapper.read_prg(0xFFFF) << 8);
    reg_P |= 0x04; add_cycles(7);
}

extern "C" void power_on_reset() {
    mapper.reset();
    uint16_t rv = mapper.read_prg(0xFFFC) | (mapper.read_prg(0xFFFD) << 8);
    reg_PC = (rv < 0x8000) ? 0xC000 : rv; // Safety fallback
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
        nmi_pending = true; ppu_status |= 0x80; frame_count++;
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
