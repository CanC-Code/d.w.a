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

uint8_t cpu_ram[0x0800] = {0}, ppu_vram[2048] = {0}, palette_ram[32] = {0};
uint32_t screen_buffer[256 * 240] = {0};
uint8_t controller_state = 0, ppu_data_buffer = 0;
std::mutex buffer_mutex;
MapperMMC1 mapper;

uint16_t reg_PC = 0;
uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;
uint8_t ppu_status = 0, ppu_ctrl = 0, ppu_addr_latch = 0;
uint16_t ppu_addr_reg = 0;
bool is_running = false;

uint32_t nes_palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00,
    0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFEFB0FF, 0xFF64B0FF, 0xFF9290FF, 0xFFC676FF, 0xFFF36AFF, 0xFFFE6ECC, 0xFFFE8170, 0xFFEA9E22,
    0xFFBCBE00, 0xFF88D100, 0xFF5CE430, 0xFF45E082, 0xFF45E082, 0xFF48CDDE, 0xFF4F4F4F, 0xFF000000
};

uint8_t bus_read(uint16_t addr) {
    if (addr < 0x2000) return cpu_ram[addr % 0x0800];
    if (addr == 0x2002) { 
        uint8_t s = ppu_status; 
        ppu_status &= ~0x80; 
        ppu_addr_latch = 0; 
        return s; 
    }
    if (addr == 0x2007) {
        uint8_t data = ppu_data_buffer;
        uint16_t p_addr = ppu_addr_reg & 0x3FFF;
        if (p_addr < 0x3F00) {
            if (p_addr >= 0x2000) ppu_data_buffer = ppu_vram[(p_addr - 0x2000) % 2048];
            else ppu_data_buffer = mapper.read_chr(p_addr);
        } else {
            data = palette_ram[p_addr & 0x1F];
            ppu_data_buffer = ppu_vram[(p_addr - 0x2000) % 2048];
        }
        ppu_addr_reg += (ppu_ctrl & 0x04) ? 32 : 1;
        return data;
    }
    if (addr >= 0x8000) return mapper.read_prg(addr);
    return 0;
}

void bus_write(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) cpu_ram[addr % 0x0800] = val;
    else if (addr == 0x2000) ppu_ctrl = val;
    else if (addr == 0x2006) {
        if (ppu_addr_latch == 0) { ppu_addr_reg = (ppu_addr_reg & 0x00FF) | (val << 8); ppu_addr_latch = 1; }
        else { ppu_addr_reg = (ppu_addr_reg & 0xFF00) | val; ppu_addr_latch = 0; }
    }
    else if (addr == 0x2007) {
        uint16_t p_addr = ppu_addr_reg & 0x3FFF;
        if (p_addr >= 0x2000 && p_addr < 0x3F00) ppu_vram[(p_addr - 0x2000) % 2048] = val;
        else if (p_addr >= 0x3F00) palette_ram[p_addr & 0x1F] = val;
        ppu_addr_reg += (ppu_ctrl & 0x04) ? 32 : 1;
    }
    else if (addr >= 0x8000) mapper.write(addr, val);
}

void update_nz(uint8_t val) { reg_P = (reg_P & 0x7D) | (val & 0x80) | ((val == 0) << 1); }
void push(uint8_t val) { bus_write(0x0100 + reg_S--, val); }
uint8_t pull() { return bus_read(0x0100 + ++reg_S); }

void handle_interrupt() {
    if (!(ppu_ctrl & 0x80)) return; 
    push((reg_PC >> 8) & 0xFF); push(reg_PC & 0xFF); push(reg_P);
    reg_P |= 0x04;
    reg_PC = (bus_read(0xFFFB) << 8) | bus_read(0xFFFA); 
}

void execute_instruction() {
    uint8_t op = bus_read(reg_PC++);
    switch (op) {
        case 0xA9: reg_A = bus_read(reg_PC++); update_nz(reg_A); break;
        case 0xA2: reg_X = bus_read(reg_PC++); update_nz(reg_X); break;
        case 0xA0: reg_Y = bus_read(reg_PC++); update_nz(reg_Y); break;
        case 0x85: bus_write(bus_read(reg_PC++), reg_A); break;
        case 0x86: bus_write(bus_read(reg_PC++), reg_X); break;
        case 0x84: bus_write(bus_read(reg_PC++), reg_Y); break;
        case 0xAD: { uint16_t l = bus_read(reg_PC++); uint16_t h = bus_read(reg_PC++); reg_A = bus_read((h<<8)|l); update_nz(reg_A); break; }
        case 0x8D: { uint16_t l = bus_read(reg_PC++); uint16_t h = bus_read(reg_PC++); bus_write((h<<8)|l, reg_A); break; }
        case 0x9A: reg_S = reg_X; break;
        case 0x90: { int8_t rel = (int8_t)bus_read(reg_PC++); if (!(reg_P & 0x01)) reg_PC += rel; break; } 
        case 0xB0: { int8_t rel = (int8_t)bus_read(reg_PC++); if (reg_P & 0x01) reg_PC += rel; break; }  
        case 0xF0: { int8_t rel = (int8_t)bus_read(reg_PC++); if (reg_P & 0x02) reg_PC += rel; break; }  
        case 0xD0: { int8_t rel = (int8_t)bus_read(reg_PC++); if (!(reg_P & 0x02)) reg_PC += rel; break; } 
        case 0x10: { int8_t rel = (int8_t)bus_read(reg_PC++); if (!(reg_P & 0x80)) reg_PC += rel; break; } 
        case 0x30: { int8_t rel = (int8_t)bus_read(reg_PC++); if (reg_P & 0x80) reg_PC += rel; break; }  
        case 0xC9: { uint8_t val = bus_read(reg_PC++); update_nz(reg_A - val); reg_P = (reg_P & 0xFE) | (reg_A >= val); break; } 
        case 0xE0: { uint8_t val = bus_read(reg_PC++); update_nz(reg_X - val); reg_P = (reg_P & 0xFE) | (reg_X >= val); break; } 
        case 0xC0: { uint8_t val = bus_read(reg_PC++); update_nz(reg_Y - val); reg_P = (reg_P & 0xFE) | (reg_Y >= val); break; } 
        case 0x4C: { uint16_t l = bus_read(reg_PC++); uint16_t h = bus_read(reg_PC++); reg_PC = (h << 8) | l; break; }
        case 0x20: { uint16_t l = bus_read(reg_PC++); uint16_t h = bus_read(reg_PC++); uint16_t r = reg_PC - 1; push(r >> 8); push(r & 0xFF); reg_PC = (h << 8) | l; break; }
        case 0x60: { uint16_t l = pull(); uint16_t h = pull(); reg_PC = ((h << 8) | l) + 1; break; }
        case 0x40: { reg_P = pull(); uint16_t l = pull(); uint16_t h = pull(); reg_PC = (h << 8) | l; break; }
        case 0x78: reg_P |= 0x04; break;
        case 0xD8: reg_P &= ~0x08; break;
        case 0xEA: break;
        default: break; 
    }
}

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
                    screen_buffer[(ty * 8 + y) * 256 + (tx * 8 + x)] = (pix == 0) ? 0xFF000000 : nes_palette[palette_ram[pix] & 0x3F];
                }
            }
        }
    }
}

void engine_loop() {
    while (is_running) {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < 29780; i++) execute_instruction();
        ppu_status |= 0x80;
        handle_interrupt(); 
        draw_frame();
        std::this_thread::sleep_until(start + std::chrono::milliseconds(16));
    }
}

extern "C" JNIEXPORT jstring JNICALL Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* cPath = env->GetStringUTFChars(romPath, 0); const char* cOut = env->GetStringUTFChars(outDir, 0);
    std::string outDirStr(cOut); std::ifstream nes(cPath, std::ios::binary);
    if (!nes) return env->NewStringUTF("File Error");
    nes.seekg(16);
    for (int i = 0; i < 4; i++) {
        std::vector<char> buf(16384); nes.read(buf.data(), 16384);
        std::ofstream out(outDirStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary); out.write(buf.data(), 16384);
    }
    std::vector<char> chr(16384); nes.read(chr.data(), 16384);
    std::ofstream c_out(outDirStr + "/chr_rom.bin", std::ios::binary); c_out.write(chr.data(), 16384);
    env->ReleaseStringUTFChars(romPath, cPath); env->ReleaseStringUTFChars(outDir, cOut);
    return env->NewStringUTF("Success");
}

extern "C" JNIEXPORT void JNICALL Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz, jstring filesDir) {
    if (is_running) return;
    const char* cPath = env->GetStringUTFChars(filesDir, 0); std::string pathStr(cPath);
    for(int i = 0; i < 4; i++) {
        std::ifstream in(pathStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        if (in) in.read((char*)mapper.prg_rom[i], 16384);
    }
    std::ifstream c_in(pathStr + "/chr_rom.bin", std::ios::binary); if (c_in) c_in.read((char*)mapper.chr_rom, 16384);
    reg_PC = (bus_read(0xFFFD) << 8) | bus_read(0xFFFC);
    is_running = true; std::thread(engine_loop).detach();
    env->ReleaseStringUTFChars(filesDir, cPath);
}

extern "C" JNIEXPORT void JNICALL Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv* env, jobject thiz, jobject bitmap) {
    void* pixels; if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;
    { std::lock_guard<std::mutex> lock(buffer_mutex); memcpy(pixels, screen_buffer, 256 * 240 * 4); }
    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C" JNIEXPORT void JNICALL Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* env, jobject thiz, jint bit, jboolean pressed) {
    if (pressed) controller_state |= bit; else controller_state &= ~bit;
}
