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

#define LOG_TAG "DWA_NATIVE"

// --- Global Hardware State ---
uint8_t cpu_ram[0x0800] = {0};
uint8_t ppu_vram[2048] = {0};
uint8_t palette_ram[32] = {0};
uint32_t screen_buffer[256 * 240] = {0};
uint8_t controller_state = 0;
uint8_t ppu_data_buffer = 0;
std::mutex buffer_mutex;
MapperMMC1 mapper;

// --- Shared 6502 CPU State ---
extern "C" {
    uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;

    uint8_t bus_read(uint16_t addr);
    void bus_write(uint16_t addr, uint8_t val);

    // --- STACK ---
    void push_stack(uint8_t val) {
        cpu_ram[0x0100 | reg_S] = val;
        reg_S--;
    }

    uint8_t pop_stack() {
        reg_S++;
        return cpu_ram[0x0100 | reg_S];
    }

    // --- FLAGS ---
    void update_nz(uint8_t val) { 
        reg_P &= ~0x82; 
        if (val == 0) reg_P |= 0x02;
        if (val & 0x80) reg_P |= 0x80;
    }

    void update_flags_cmp(uint8_t reg, uint8_t val) {
        reg_P &= ~0x83; 
        if (reg >= val) reg_P |= 0x01; 
        if (reg == val) reg_P |= 0x02; 
        uint8_t res = reg - val;
        if (res & 0x80) reg_P |= 0x80; 
    }

    // --- MATH & LOGIC ---
    void cpu_adc(uint8_t val) {
        uint16_t carry = (reg_P & 0x01);
        uint16_t sum = reg_A + val + carry;
        if (~(reg_A ^ val) & (reg_A ^ sum) & 0x80) reg_P |= 0x40; 
        else reg_P &= ~0x40;
        if (sum > 0xFF) reg_P |= 0x01; 
        else reg_P &= ~0x01;
        reg_A = (uint8_t)sum;
        update_nz(reg_A);
    }

    void cpu_sbc(uint8_t val) { cpu_adc(~val); }

    // Required for bitwise assembly instructions
    void cpu_bit(uint8_t val) {
        reg_P &= ~0xC2; // Clear N, V, Z
        if ((val & reg_A) == 0) reg_P |= 0x02;
        reg_P |= (val & 0xC0); // Copy bits 7 and 6 to N and V
    }

    // --- ADDRESSING HELPERS ---
    uint16_t read_pointer(uint16_t addr) {
        // Zero-page wrap-around is a 6502 quirk often used in Dragon Warrior
        uint8_t lo = bus_read(addr);
        uint8_t hi = bus_read((addr & 0xFF00) | ((addr + 1) & 0x00FF));
        return (hi << 8) | lo;
    }

    uint16_t read_pointer_x(uint16_t addr) {
        uint8_t ptr = (addr + reg_X) & 0xFF;
        return read_pointer(ptr);
    }

    // Recompiled Entry Points
    void power_on_reset();
    void nmi_handler();
}

// --- PPU & PALETTE ---
uint8_t ppu_status = 0, ppu_ctrl = 0, ppu_addr_latch = 0;
uint16_t ppu_addr_reg = 0;
bool is_running = false;

uint32_t nes_palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00,
    0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFEFB0FF, 0xFF64B0FF, 0xFF9290FF, 0xFFC676FF, 0xFFF36AFF, 0xFFFE6ECC, 0xFFFE8170, 0xFFEA9E22,
    0xFFBCBE00, 0xFF88D100, 0xFF5CE430, 0xFF45E082, 0xFF48CDDE, 0xFF4F4F4F, 0xFF000000, 0xFF000000
};

// --- BUS ---
extern "C" uint8_t bus_read(uint16_t addr) {
    if (addr < 0x2000) return cpu_ram[addr % 0x0800];
    if (addr >= 0x2000 && addr <= 0x3FFF) {
        if (addr % 8 == 2) { 
            uint8_t s = ppu_status; 
            ppu_status &= ~0x80; 
            ppu_addr_latch = 0; 
            return s; 
        }
        if (addr % 8 == 7) {
            uint8_t data = ppu_data_buffer;
            uint16_t p_addr = ppu_addr_reg & 0x3FFF;
            if (p_addr < 0x3F00) ppu_data_buffer = (p_addr >= 0x2000) ? ppu_vram[p_addr % 2048] : mapper.read_chr(p_addr);
            else { data = palette_ram[p_addr & 0x1F]; ppu_data_buffer = ppu_vram[p_addr % 2048]; }
            ppu_addr_reg += (ppu_ctrl & 0x04) ? 32 : 1;
            return data;
        }
    }
    if (addr == 0x4016) { 
        uint8_t ret = (controller_state & 0x80) >> 7;
        controller_state <<= 1;
        return ret;
    }
    if (addr >= 0x8000) return mapper.read_prg(addr);
    return 0;
}

extern "C" void bus_write(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) cpu_ram[addr % 0x0800] = val;
    else if (addr >= 0x2000 && addr <= 0x3FFF) {
        uint16_t reg = addr % 8;
        if (reg == 0) ppu_ctrl = val;
        else if (reg == 6) {
            if (ppu_addr_latch == 0) { ppu_addr_reg = (ppu_addr_reg & 0x00FF) | (val << 8); ppu_addr_latch = 1; }
            else { ppu_addr_reg = (ppu_addr_reg & 0xFF00) | val; ppu_addr_latch = 0; }
        }
        else if (reg == 7) {
            uint16_t p_addr = ppu_addr_reg & 0x3FFF;
            if (p_addr >= 0x2000 && p_addr < 0x3F00) ppu_vram[p_addr % 2048] = val;
            else if (p_addr >= 0x3F00) palette_ram[p_addr & 0x1F] = val;
            ppu_addr_reg += (ppu_ctrl & 0x04) ? 32 : 1;
        }
    }
    else if (addr >= 0x8000) mapper.write(addr, val);
}

// --- GRAPHICS ---
void draw_frame() {
    std::lock_guard<std::mutex> lock(buffer_mutex);
    uint16_t nt_base = (ppu_ctrl & 0x03) * 1024;
    uint16_t pat_base = (ppu_ctrl & 0x10) ? 0x1000 : 0x0000;
    for (int ty = 0; ty < 30; ty++) {
        for (int tx = 0; tx < 32; tx++) {
            uint8_t tile = ppu_vram[(nt_base + ty * 32 + tx) % 2048];
            for (int y = 0; y < 8; y++) {
                uint8_t p1 = mapper.read_chr(pat_base + tile * 16 + y);
                uint8_t p2 = mapper.read_chr(pat_base + tile * 16 + y + 8);
                for (int x = 0; x < 8; x++) {
                    uint8_t pix = ((p1 >> (7 - x)) & 0x01) | (((p2 >> (7 - x)) & 0x01) << 1);
                    screen_buffer[(ty * 8 + y) * 256 + (tx * 8 + x)] = (pix != 0) ? nes_palette[palette_ram[pix] & 0x3F] : 0xFF000000;
                }
            }
        }
    }
}

void engine_loop() {
    power_on_reset();
    while (is_running) {
        auto start = std::chrono::steady_clock::now();
        ppu_status |= 0x80; // Set VBlank
        if (ppu_ctrl & 0x80) nmi_handler();
        draw_frame();
        std::this_thread::sleep_until(start + std::chrono::milliseconds(16));
    }
}

// --- JNI ---
extern "C" JNIEXPORT void JNICALL Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz, jstring filesDir) {
    if (is_running) return;
    const char* cPath = env->GetStringUTFChars(filesDir, 0); 
    std::string pathStr(cPath);
    for(int i = 0; i < 4; i++) {
        std::ifstream in(pathStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        if (in) in.read((char*)mapper.prg_rom[i], 16384);
    }
    std::ifstream c_in(pathStr + "/chr_rom.bin", std::ios::binary); 
    if (c_in) c_in.read((char*)mapper.chr_rom, 16384);
    is_running = true; 
    std::thread(engine_loop).detach();
    env->ReleaseStringUTFChars(filesDir, cPath);
}

extern "C" JNIEXPORT void JNICALL Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv* env, jobject thiz, jobject bitmap) {
    void* pixels; 
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;
    { std::lock_guard<std::mutex> lock(buffer_mutex); memcpy(pixels, screen_buffer, 256 * 240 * 4); }
    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C" JNIEXPORT jstring JNICALL Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* cPath = env->GetStringUTFChars(romPath, 0); 
    const char* cOut = env->GetStringUTFChars(outDir, 0);
    std::ifstream nes(cPath, std::ios::binary);
    if (!nes) return env->NewStringUTF("File Error");
    nes.seekg(16);
    for (int i = 0; i < 4; i++) {
        std::vector<char> buf(16384); nes.read(buf.data(), 16384);
        std::ofstream out(std::string(cOut) + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        out.write(buf.data(), 16384);
    }
    std::vector<char> chr(16384); nes.read(chr.data(), 16384);
    std::ofstream c_out(std::string(cOut) + "/chr_rom.bin", std::ios::binary);
    c_out.write(chr.data(), 16384);
    env->ReleaseStringUTFChars(romPath, cPath); env->ReleaseStringUTFChars(outDir, cOut);
    return env->NewStringUTF("Success");
}

extern "C" JNIEXPORT void JNICALL Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* env, jobject thiz, jint bit, jboolean pressed) {
    if (pressed) controller_state |= bit; else controller_state &= ~bit;
}
