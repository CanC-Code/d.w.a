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
#include "recompiled/cpu_shared.h"

#define LOG_TAG "DWA_NATIVE"

// --- Recompiled Bank Dispatcher ---
namespace Dispatcher {
    void execute();
}

// --- Global Hardware State ---
uint8_t cpu_ram[0x0800] = {0};
uint8_t ppu_vram[2048] = {0};
uint8_t palette_ram[32] = {0};
uint8_t oam_ram[256] = {0};
uint32_t screen_buffer[256 * 240] = {0};
uint8_t controller_state = 0;
uint8_t controller_shift = 0;
uint8_t last_strobe = 0;
uint8_t ppu_data_buffer = 0;
std::mutex buffer_mutex;
MapperMMC1 mapper;

uint32_t nes_palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00,
    0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFEFB0FF, 0xFF64B0FF, 0xFF9290FF, 0xFFC676FF, 0xFFF36AFF, 0xFFFE6ECC, 0xFFFE8170, 0xFFEA9E22,
    0xFFBCBE00, 0xFF88D100, 0xFF5CE430, 0xFF45E082, 0xFF48CDDE, 0xFF4F4F4F, 0xFF000000, 0xFF000000
};

// --- Shared 6502 CPU Implementation ---
extern "C" {
    uint16_t reg_PC = 0;
    uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;

    void execute_instruction() {
        Dispatcher::execute();
    }

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
        if (~(reg_A ^ val) & (reg_A ^ sum) & 0x80) reg_P |= FLAG_V; else reg_P &= ~FLAG_V;
        if (sum > 0xFF) reg_P |= FLAG_C; else reg_P &= ~FLAG_C;
        reg_A = (uint8_t)sum;
        update_nz(reg_A);
    }

    void cpu_sbc(uint8_t val) { cpu_adc(~val); }

    void cpu_bit(uint8_t val) {
        reg_P &= ~(FLAG_Z | FLAG_V | FLAG_N);
        if ((val & reg_A) == 0) reg_P |= FLAG_Z;
        reg_P |= (val & 0xC0);
    }

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

    uint16_t read_pointer(uint16_t addr) {
        return bus_read(addr) | (bus_read(addr + 1) << 8);
    }

    uint16_t read_pointer_indexed_y(uint16_t zp_addr) {
        uint16_t lo = bus_read(zp_addr);
        uint16_t hi = bus_read((zp_addr + 1) & 0xFF);
        return (lo | (hi << 8)) + reg_Y;
    }

    void push_stack(uint8_t val) { cpu_ram[0x0100 | (reg_S & 0xFF)] = val; reg_S--; }
    uint8_t pop_stack() { reg_S++; return cpu_ram[0x0100 | (reg_S & 0xFF)]; }
}

// --- PPU & Bus Logic ---
uint8_t ppu_status = 0, ppu_ctrl = 0, ppu_mask = 0, ppu_addr_latch = 0, oam_addr = 0;
uint16_t ppu_addr_reg = 0;
bool is_running = false;
bool is_paused = false;

uint16_t ntable_mirror(uint16_t addr) {
    addr = (addr - 0x2000) % 0x1000;
    return addr % 0x0800; // Vertical Mirroring
}

uint8_t read_palette(uint16_t addr) {
    uint8_t p_idx = addr & 0x1F;
    if (p_idx >= 0x10 && (p_idx % 4 == 0)) p_idx -= 0x10;
    return palette_ram[p_idx];
}

extern "C" uint8_t bus_read(uint16_t addr) {
    if (addr < 0x2000) return cpu_ram[addr % 0x0800];
    if (addr >= 0x2000 && addr <= 0x3FFF) {
        uint16_t reg = addr % 8;
        if (reg == 2) {
            uint8_t s = ppu_status;
            ppu_status &= ~0x80; 
            ppu_addr_latch = 0;
            return s;
        }
        if (reg == 7) {
            uint8_t data = ppu_data_buffer;
            uint16_t p_addr = ppu_addr_reg & 0x3FFF;
            if (p_addr < 0x2000) ppu_data_buffer = mapper.read_chr(p_addr);
            else if (p_addr < 0x3F00) ppu_data_buffer = ppu_vram[ntable_mirror(p_addr)];
            else data = read_palette(p_addr);
            ppu_addr_reg += (ppu_ctrl & 0x04) ? 32 : 1;
            return data;
        }
    }
    if (addr == 0x4016) {
        uint8_t ret = (controller_shift & 0x80) >> 7;
        controller_shift <<= 1;
        return 0x40 | ret;
    }
    if (addr >= 0x6000) return mapper.read_prg(addr);
    return 0;
}

extern "C" void bus_write(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) cpu_ram[addr % 0x0800] = val;
    else if (addr >= 0x2000 && addr <= 0x3FFF) {
        uint16_t reg = addr % 8;
        if (reg == 0) ppu_ctrl = val;
        else if (reg == 1) ppu_mask = val;
        else if (reg == 3) oam_addr = val;
        else if (reg == 4) oam_ram[oam_addr++] = val;
        else if (reg == 6) {
            if (ppu_addr_latch == 0) { ppu_addr_reg = (ppu_addr_reg & 0x00FF) | (val << 8); ppu_addr_latch = 1; }
            else { ppu_addr_reg = (ppu_addr_reg & 0xFF00) | val; ppu_addr_latch = 0; }
        }
        else if (reg == 7) {
            uint16_t p_addr = ppu_addr_reg & 0x3FFF;
            if (p_addr >= 0x2000 && p_addr < 0x3F00) ppu_vram[ntable_mirror(p_addr)] = val;
            else if (p_addr >= 0x3F00) {
                uint8_t p_idx = p_addr & 0x1F;
                if (p_idx >= 0x10 && (p_idx % 4 == 0)) p_idx -= 0x10;
                palette_ram[p_idx] = val;
            }
            ppu_addr_reg += (ppu_ctrl & 0x04) ? 32 : 1;
        }
    }
    else if (addr == 0x4014) {
        uint16_t base = val << 8;
        for (int i = 0; i < 256; i++) oam_ram[i] = bus_read(base + i);
    }
    else if (addr == 0x4016) {
        if ((val & 0x01) == 0 && (last_strobe & 0x01) == 1) controller_shift = controller_state;
        last_strobe = val;
    }
    else if (addr >= 0x6000) mapper.write(addr, val);
}

// --- Rendering ---
void draw_frame() {
    std::lock_guard<std::mutex> lock(buffer_mutex);
    uint16_t nt_base = (ppu_ctrl & 0x03) * 1024;
    uint16_t bg_pat_base = (ppu_ctrl & 0x10) ? 0x1000 : 0x0000;

    for (int ty = 0; ty < 30; ty++) {
        for (int tx = 0; tx < 32; tx++) {
            uint8_t tile = ppu_vram[ntable_mirror(0x2000 + nt_base + ty * 32 + tx)];
            for (int y = 0; y < 8; y++) {
                uint8_t p1 = mapper.read_chr(bg_pat_base + tile * 16 + y);
                uint8_t p2 = mapper.read_chr(bg_pat_base + tile * 16 + y + 8);
                for (int x = 0; x < 8; x++) {
                    uint8_t pix = ((p1 >> (7 - x)) & 0x01) | (((p2 >> (7 - x)) & 0x01) << 1);
                    uint8_t palette_val = read_palette(0x3F00 + pix);
                    screen_buffer[(ty * 8 + y) * 256 + (tx * 8 + x)] = nes_palette[palette_val & 0x3F];
                }
            }
        }
    }
}

// --- CPU Entry ---
extern "C" void power_on_reset() {
    mapper.reset();
    // MMC1 Specific: Initial state usually maps the last bank to $C000
    // Force a known valid state for Dragon Warrior
    mapper.control = 0x1C; 
    
    uint8_t lo = bus_read(0xFFFC);
    uint8_t hi = bus_read(0xFFFD);
    reg_PC = (hi << 8) | lo;

    if (reg_PC < 0x8000) reg_PC = 0xFF8E; 
    reg_S = 0xFD;
    reg_P = 0x24;
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Reset! PC: %04X", reg_PC);
}

extern "C" void nmi_handler() {
    ppu_status |= 0x80; 
    push_stack(reg_PC >> 8);
    push_stack(reg_PC & 0xFF);
    push_stack(reg_P);
    
    uint16_t nmi_vector = bus_read(0xFFFA) | (bus_read(0xFFFB) << 8);
    reg_PC = nmi_vector;
}

void engine_loop() {
    power_on_reset();
    is_running = true;
    while (is_running) {
        if (is_paused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
            continue;
        }
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < 29780; i++) {
            execute_instruction();
            if (!is_running) break;
        }
        
        // Trigger NMI if enabled by software
        if (ppu_ctrl & 0x80) {
            nmi_handler();
        } else {
            ppu_status |= 0x80; // Still set VBlank flag for manual polling
        }
        
        draw_frame();
        std::this_thread::sleep_until(start + std::chrono::microseconds(16666));
    }
}

// --- JNI Implementation ---

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv *env, jobject thiz, jstring romPath, jstring outDir) {
    const char *path = env->GetStringUTFChars(romPath, nullptr);
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        env->ReleaseStringUTFChars(romPath, path);
        return env->NewStringUTF("Error: Could not open file");
    }

    file.seekg(16, std::ios::beg); 
    for (int i = 0; i < 4; i++) file.read((char*)mapper.prg_rom[i], 16384);
    for (int i = 0; i < 2; i++) file.read((char*)mapper.chr_rom[i], 4096);

    file.close();
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
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv *env, jobject thiz, jint buttonBit, jboolean isPressed) {
    if (isPressed) controller_state |= (uint8_t)buttonBit;
    else controller_state &= ~((uint8_t)buttonBit);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativePauseEngine(JNIEnv *env, jobject thiz) {
    is_paused = true;
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeResumeEngine(JNIEnv *env, jobject thiz) {
    is_paused = false;
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv *env, jobject thiz, jobject bitmap) {
    AndroidBitmapInfo info;
    void* pixels;
    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0) return;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;
    {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        memcpy(pixels, screen_buffer, 256 * 240 * 4);
    }
    AndroidBitmap_unlockPixels(env, bitmap);
}
