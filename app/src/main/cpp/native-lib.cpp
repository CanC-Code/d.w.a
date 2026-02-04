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

[span_33](start_span)uint8_t cpu_ram[0x0800] = {0};[span_33](end_span)
[span_34](start_span)uint8_t ppu_vram[2048] = {0};[span_34](end_span)
[span_35](start_span)uint8_t palette_ram[32] = {0};[span_35](end_span)
[span_36](start_span)uint32_t screen_buffer[256 * 240] = {0};[span_36](end_span)
uint8_t controller_state = 0;
[span_37](start_span)std::mutex buffer_mutex;[span_37](end_span)
[span_38](start_span)MapperMMC1 mapper;[span_38](end_span)

[span_39](start_span)uint16_t reg_PC = 0;[span_39](end_span)
[span_40](start_span)uint8_t reg_A = 0, reg_X = 0, reg_Y = 0, reg_S = 0xFD, reg_P = 0x24;[span_40](end_span)
[span_41](start_span)uint8_t ppu_status = 0, ppu_ctrl = 0, ppu_addr_latch = 0;[span_41](end_span)
[span_42](start_span)uint16_t ppu_addr_reg = 0;[span_42](end_span)
[span_43](start_span)bool is_running = false;[span_43](end_span)

uint32_t nes_palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00,
    0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000
[span_44](start_span)};[span_44](end_span)

uint8_t bus_read(uint16_t addr) {
    [span_45](start_span)if (addr < 0x2000) return cpu_ram[addr % 0x0800];[span_45](end_span)
    if (addr == 0x2002) { uint8_t s = ppu_status; ppu_status &= ~0x80; ppu_addr_latch = 0; return s; [span_46](start_span)}
    if (addr >= 0x8000) return mapper.read_prg(addr);[span_46](end_span)
    [span_47](start_span)return 0;[span_47](end_span)
}

void bus_write(uint16_t addr, uint8_t val) {
    [span_48](start_span)if (addr < 0x2000) cpu_ram[addr % 0x0800] = val;[span_48](end_span)
    [span_49](start_span)else if (addr == 0x2000) ppu_ctrl = val;[span_49](end_span)
    else if (addr == 0x2006) {
        if (ppu_addr_latch == 0) { ppu_addr_reg = (ppu_addr_reg & 0x00FF) | (val << 8); ppu_addr_latch = 1; [span_50](start_span)}
        else { ppu_addr_reg = (ppu_addr_reg & 0xFF00) | val; ppu_addr_latch = 0; }[span_50](end_span)
    }
    else if (addr == 0x2007) {
        [span_51](start_span)uint16_t p_addr = ppu_addr_reg & 0x3FFF;[span_51](end_span)
        [span_52](start_span)if (p_addr >= 0x2000 && p_addr < 0x3F00) ppu_vram[(p_addr - 0x2000) % 2048] = val;[span_52](end_span)
        [span_53](start_span)else if (p_addr >= 0x3F00) palette_ram[p_addr & 0x1F] = val;[span_53](end_span)
        ppu_addr_reg += (ppu_ctrl & 0x04) ? [span_54](start_span)32 : 1;[span_54](end_span)
    }
    [span_55](start_span)else if (addr >= 0x8000) mapper.write(addr, val);[span_55](end_span)
}

void update_nz(uint8_t val) {
    reg_P = (reg_P & 0x7D) | (val & 0x80) | ((val == 0) [span_56](start_span)<< 1);[span_56](end_span)
}

void execute_instruction() {
    [span_57](start_span)uint8_t opcode = bus_read(reg_PC++);[span_57](end_span)
    switch (opcode) {
        [span_58](start_span)case 0xA9: reg_A = bus_read(reg_PC++); update_nz(reg_A); break;[span_58](end_span)
        [span_59](start_span)case 0xA2: reg_X = bus_read(reg_PC++); update_nz(reg_X); break;[span_59](end_span)
        [span_60](start_span)case 0xA0: reg_Y = bus_read(reg_PC++); update_nz(reg_Y); break;[span_60](end_span)
        [span_61](start_span)case 0x85: bus_write(bus_read(reg_PC++), reg_A); break;[span_61](end_span)
        [span_62](start_span)case 0x86: bus_write(bus_read(reg_PC++), reg_X); break;[span_62](end_span)
        [span_63](start_span)case 0x9A: reg_S = reg_X; break;[span_63](end_span)
        case 0x4C: { 
            uint16_t l = bus_read(reg_PC++); uint16_t h = bus_read(reg_PC++);
            [span_64](start_span)reg_PC = (h << 8) | l; break;[span_64](end_span)
        }
        case 0x20: {
            uint16_t l = bus_read(reg_PC++); uint16_t h = bus_read(reg_PC++);
            uint16_t ret = reg_PC - 1;
            [span_65](start_span)bus_write(0x0100 + reg_S--, (ret >> 8) & 0xFF);[span_65](end_span)
            bus_write(0x0100 + reg_S--, ret & 0xFF);
            [span_66](start_span)reg_PC = (h << 8) | l; break;[span_66](end_span)
        }
        case 0x60: {
            uint16_t l = bus_read(0x0100 + ++reg_S); uint16_t h = bus_read(0x0100 + ++reg_S);
            [span_67](start_span)reg_PC = ((h << 8) | l) + 1; break;[span_67](end_span)
        }
        [span_68](start_span)case 0x78: reg_P |= 0x04; break;[span_68](end_span)
        [span_69](start_span)case 0xD8: reg_P &= ~0x08; break;[span_69](end_span)
        [span_70](start_span)case 0xEA: break;[span_70](end_span)
        [span_71](start_span)default: break;[span_71](end_span)
    }
}

void draw_frame() {
    std::lock_guard<std::mutex> lock(buffer_mutex);
    for (int ty = 0; ty < 30; ty++) {
        for (int tx = 0; tx < 32; tx++) {
            uint8_t tile_id = ppu_vram[ty * 32 + tx];
            for (int y = 0; y < 8; y++) {
                // NES tiles are 16 bytes: 8 bytes for bitplane 0, 8 for bitplane 1
                uint8_t p1 = mapper.read_chr(tile_id * 16 + y);
                uint8_t p2 = mapper.read_chr(tile_id * 16 + y + 8);
                for (int x = 0; x < 8; x++) {
                    uint8_t pix = ((p1 >> (7-x)) & 0x01) | (((p2 >> (7-x)) & 0x01) << 1);
                    uint32_t color = (pix == 0) ? 0xFF000000 : nes_palette[palette_ram[pix] & 0x1F];
                    screen_buffer[(ty * 8 + y) * 256 + (tx * 8 + x)] = color;
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
        draw_frame();
        std::this_thread::sleep_until(start + std::chrono::milliseconds(16));
    }
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    [span_72](start_span)const char* cPath = env->GetStringUTFChars(romPath, 0);[span_72](end_span)
    [span_73](start_span)const char* cOut = env->GetStringUTFChars(outDir, 0);[span_73](end_span)
    std::string outDirStr(cOut);
    std::ifstream nes(cPath, std::ios::binary);
    if (!nes) return env->NewStringUTF("File Error");

    nes.seekg(16); [span_74](start_span)// Skip iNES Header[span_74](end_span)
    for (int i = 0; i < 4; i++) {
        std::vector<char> buf(16384);
        [span_75](start_span)nes.read(buf.data(), 16384);[span_75](end_span)
        std::ofstream out(outDirStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        [span_76](start_span)out.write(buf.data(), 16384);[span_76](end_span)
    }
    
    // Dragon Warrior has 16KB of CHR-ROM
    std::vector<char> chr(16384);
    [span_77](start_span)nes.read(chr.data(), 16384);[span_77](end_span)
    std::ofstream c_out(outDirStr + "/chr_rom.bin", std::ios::binary);
    [span_78](start_span)c_out.write(chr.data(), 16384);[span_78](end_span)
    
    [span_79](start_span)env->ReleaseStringUTFChars(romPath, cPath);[span_79](end_span)
    [span_80](start_span)env->ReleaseStringUTFChars(outDir, cOut);[span_80](end_span)
    return env->NewStringUTF("Success");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz, jstring filesDir) {
    [span_81](start_span)if (is_running) return;[span_81](end_span)
    const char* cPath = env->GetStringUTFChars(filesDir, 0);
    std::string pathStr(cPath);
    for(int i = 0; i < 4; i++) {
        std::ifstream in(pathStr + "/prg_bank_" + std::to_string(i) + ".bin", std::ios::binary);
        if (in) in.read((char*)mapper.prg_rom[i], 16384);
    }
    std::ifstream c_in(pathStr + "/chr_rom.bin", std::ios::binary);
    if (c_in) c_in.read((char*)mapper.chr_rom, 16384);
    
    reg_PC = (bus_read(0xFFFD) << 8) | bus_read(0xFFFC);
    is_running = true;
    std::thread(engine_loop).detach();
    env->ReleaseStringUTFChars(filesDir, cPath);
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv* env, jobject thiz, jobject bitmap) {
    void* pixels;
    [span_82](start_span)if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;[span_82](end_span)
    {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        [span_83](start_span)memcpy(pixels, screen_buffer, 256 * 240 * 4);[span_83](end_span)
    }
    [span_84](start_span)AndroidBitmap_unlockPixels(env, bitmap);[span_84](end_span)
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* env, jobject thiz, jint bit, jboolean pressed) {
    [span_85](start_span)if (pressed) controller_state |= bit;[span_85](end_span)
    [span_86](start_span)else controller_state &= ~bit;[span_86](end_span)
}
