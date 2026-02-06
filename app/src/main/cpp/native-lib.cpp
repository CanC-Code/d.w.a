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
#define LOG_CPU(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

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
uint8_t ppu_data_buffer = 0;
std::mutex buffer_mutex;
MapperMMC1 mapper;

uint32_t nes_palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00,
    0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFFFFFFF, 0xFF64B0FF, 0xFF9290FF, 0xFFC676FF, 0xFFF36AFF, 0xFFFE6ECC, 0xFFFE8170, 0xFFEA9E22,
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
        uint16_t carry = (reg_P & FLAG_C);
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
        uint8_t old_c = (reg_P & FLAG_C);
        reg_P = (reg_P & ~FLAG_C) | ((val >> 7) & 0x01);
        uint8_t res = (val << 1) | old_c;
        update_nz(res);
        return res;
    }

    uint8_t cpu_ror(uint8_t val) {
        uint8_t old_c = (reg_P & FLAG_C);
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
    return addr % 0x0800;
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
    if (addr >= 0x6000) return mapper.read_prg(addr);
    if (addr == 0x4016) {
        uint8_t ret = (controller_shift & 0x80) >> 7;
        controller_shift <<= 1;
        return ret;
    }
    return 0;
}

extern "C" void bus_write(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) cpu_ram[addr % 0x0800] = val;
    else if (addr >= 0x2000 && addr <= 0x3FFF) {
        uint16_t reg = addr % 8;
        if (reg == 0) {
            ppu_ctrl = val;
            LOG_CPU("PPU_CTRL = $%02X", val);
        }
        else if (reg == 1) {
            ppu_mask = val;
            LOG_CPU("PPU_MASK = $%02X", val);
        }
        else if (reg == 3) oam_addr = val;
        else if (reg == 4) oam_ram[oam_addr++] = val;
        else if (reg == 6) {
            if (ppu_addr_latch == 0) { 
                ppu_addr_reg = (ppu_addr_reg & 0x00FF) | (val << 8); 
                ppu_addr_latch = 1; 
            }
            else { 
                ppu_addr_reg = (ppu_addr_reg & 0xFF00) | val; 
                ppu_addr_latch = 0; 
            }
        }
        else if (reg == 7) {
            uint16_t p_addr = ppu_addr_reg & 0x3FFF;
            if (p_addr >= 0x2000 && p_addr < 0x3F00) {
                ppu_vram[ntable_mirror(p_addr)] = val;
            }
            else if (p_addr >= 0x3F00) {
                uint8_t p_idx = p_addr & 0x1F;
                if (p_idx >= 0x10 && (p_idx % 4 == 0)) p_idx -= 0x10;
                palette_ram[p_idx] = val;
                // Log palette writes
                static int palette_write_count = 0;
                if (palette_write_count < 32) {
                    LOG_CPU("Palette[$%02X] = $%02X", p_idx, val);
                    palette_write_count++;
                }
            }
            ppu_addr_reg += (ppu_ctrl & 0x04) ? 32 : 1;
        }
    }
    else if (addr == 0x4014) {
        uint16_t base = val << 8;
        for (int i = 0; i < 256; i++) oam_ram[i] = bus_read(base + i);
    }
    else if (addr == 0x4016) { 
        if ((val & 0x01) == 0) controller_shift = controller_state; 
    }
    else if (addr >= 0x6000) mapper.write(addr, val);
}

// --- Initialize screen to black ---
void clear_screen_to_black() {
    std::lock_guard<std::mutex> lock(buffer_mutex);
    for (int i = 0; i < 256 * 240; i++) {
        screen_buffer[i] = 0xFF000000;  // Black
    }
}

// --- Rendering ---
void draw_frame() {
    // Don't render if PPU is disabled (both background and sprites off)
    if ((ppu_mask & 0x18) == 0) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(buffer_mutex);
    uint16_t nt_base = (ppu_ctrl & 0x03) * 1024;
    uint16_t bg_pat_base = (ppu_ctrl & 0x10) ? 0x1000 : 0x0000;
    uint16_t attr_base = nt_base + 0x3C0;  // Attribute table starts at +960 bytes

    for (int ty = 0; ty < 30; ty++) {
        for (int tx = 0; tx < 32; tx++) {
            uint8_t tile = ppu_vram[ntable_mirror(0x2000 + nt_base + ty * 32 + tx)];
            
            // Get attribute byte for this 2x2 metatile group
            uint16_t attr_addr = attr_base + (ty / 4) * 8 + (tx / 4);
            uint8_t attr_byte = ppu_vram[ntable_mirror(0x2000 + attr_addr)];
            
            // Extract 2-bit palette for this specific tile
            uint8_t shift = ((ty & 2) << 1) | (tx & 2);
            uint8_t palette_select = (attr_byte >> shift) & 0x03;
            
            for (int y = 0; y < 8; y++) {
                uint8_t p1 = mapper.read_chr(bg_pat_base + tile * 16 + y);
                uint8_t p2 = mapper.read_chr(bg_pat_base + tile * 16 + y + 8);
                
                for (int x = 0; x < 8; x++) {
                    uint8_t pix = ((p1 >> (7 - x)) & 0x01) | (((p2 >> (7 - x)) & 0x01) << 1);
                    
                    // Build full palette index: background palette base + color
                    uint8_t palette_addr = (palette_select * 4) + pix;
                    uint8_t palette_val = read_palette(0x3F00 + palette_addr);
                    
                    screen_buffer[(ty * 8 + y) * 256 + (tx * 8 + x)] = nes_palette[palette_val & 0x3F];
                }
            }
        }
    }
}

// --- CPU Entry ---
extern "C" void power_on_reset() {
    mapper.reset();
    
    // Reset vector is at $FFFC-$FFFD in the fixed bank (bank 3)
    // MMC1 mode 3: $C000-$FFFF is fixed to last bank (16KB bank)
    uint8_t lo = mapper.prg_rom[3][0x3FFC];  // Offset 0x3FFC in 16KB bank
    uint8_t hi = mapper.prg_rom[3][0x3FFD];  // Offset 0x3FFD in 16KB bank
    reg_PC = (hi << 8) | lo;
    
    LOG_CPU("Power-On Reset! Reset Vector: $%04X", reg_PC);
    
    // Validate PC is in ROM space
    if (reg_PC < 0x8000) {
        LOG_CPU("ERROR: Reset vector points to RAM/IO ($%04X)! Using fallback.", reg_PC);
        reg_PC = 0xC000;  // Start of fixed bank as fallback
    }
    
    reg_S = 0xFD;
    reg_P = 0x24;
    reg_A = 0;
    reg_X = 0;
    reg_Y = 0;
    
    LOG_CPU("CPU State: PC=$%04X A=$%02X X=$%02X Y=$%02X P=$%02X S=$%02X",
            reg_PC, reg_A, reg_X, reg_Y, reg_P, reg_S);
}

extern "C" void nmi_handler() {
    push_stack(reg_PC >> 8);
    push_stack(reg_PC & 0xFF);
    push_stack(reg_P);
    reg_PC = (bus_read(0xFFFB) << 8) | bus_read(0xFFFA);
    ppu_status |= 0x80;
}

void engine_loop() {
    power_on_reset();
    is_running = true;
    int frame_count = 0;
    
    while (is_running) {
        if (is_paused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
            continue;
        }
        
        auto start = std::chrono::steady_clock::now();
        
        // Log first few frames for debugging
        if (frame_count < 10) {
            LOG_CPU("Frame %d: PC=$%04X A=$%02X X=$%02X Y=$%02X P=$%02X S=$%02X Bank=%d", 
                    frame_count, reg_PC, reg_A, reg_X, reg_Y, reg_P, reg_S, mapper.prg_bank);
        }
        
        // Execute CPU cycles (~29780 cycles per frame)
        for (int i = 0; i < 20000; i++) {
            execute_instruction();
            if (!is_running) break;
        }
        
        nmi_handler();
        draw_frame();
        
        frame_count++;
        std::this_thread::sleep_until(start + std::chrono::microseconds(16666));
    }
}

// --- JNI Lifecycle ---

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv *env, jobject thiz, jstring romPath, jstring outDir) {
    const char *path = env->GetStringUTFChars(romPath, nullptr);
    
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        env->ReleaseStringUTFChars(romPath, path);
        return env->NewStringUTF("Error: Could not open file");
    }

    // Read and validate iNES header
    uint8_t header[16];
    file.read((char*)header, 16);
    
    // Check "NES" magic bytes
    if (header[0] != 'N' || header[1] != 'E' || header[2] != 'S' || header[3] != 0x1A) {
        file.close();
        env->ReleaseStringUTFChars(romPath, path);
        return env->NewStringUTF("Error: Not a valid NES ROM");
    }
    
    // Validate mapper and ROM size
    uint8_t prg_banks = header[4];  // 16KB units
    uint8_t chr_banks = header[5];  // 8KB units
    uint8_t mapper_num = ((header[6] >> 4) | (header[7] & 0xF0));
    
    LOG_CPU("ROM Header: PRG=%dKB CHR=%dKB Mapper=%d", prg_banks*16, chr_banks*8, mapper_num);
    
    if (mapper_num != 1) {
        file.close();
        env->ReleaseStringUTFChars(romPath, path);
        return env->NewStringUTF("Error: Not an MMC1 ROM (Dragon Warrior requires MMC1/Mapper 1)");
    }
    
    if (prg_banks != 4) {
        LOG_CPU("Warning: Expected 4 PRG banks (64KB), found %d", prg_banks);
    }

    // Read PRG-ROM (4 banks of 16KB for Dragon Warrior)
    for (int i = 0; i < 4; i++) {
        file.read((char*)mapper.prg_rom[i], 16384);
        if (file.gcount() != 16384) {
            LOG_CPU("Error: Failed to read PRG bank %d (read %ld bytes)", i, file.gcount());
            file.close();
            env->ReleaseStringUTFChars(romPath, path);
            return env->NewStringUTF("Error: Incomplete PRG-ROM");
        }
    }
    
    // Read CHR-ROM (Dragon Warrior uses 8KB = 2 banks of 4KB)
    int chr_read_count = chr_banks > 0 ? (chr_banks * 2) : 2;  // Convert 8KB to 4KB banks
    for (int i = 0; i < chr_read_count && i < 8; i++) {
        file.read((char*)mapper.chr_rom[i], 4096);
        if (file.gcount() != 4096) {
            LOG_CPU("Warning: Partial CHR bank %d read (%ld bytes)", i, file.gcount());
        }
    }
    
    // Verify reset vector
    uint16_t reset_vector = (mapper.prg_rom[3][0x3FFD] << 8) | mapper.prg_rom[3][0x3FFC];
    LOG_CPU("Reset vector: $%04X", reset_vector);
    
    if (reset_vector < 0x8000 || reset_vector == 0xFFFF) {
        LOG_CPU("Warning: Reset vector $%04X may be invalid", reset_vector);
    }
    
    // Log first few bytes of CHR-ROM for verification
    LOG_CPU("CHR-ROM first bytes: %02X %02X %02X %02X", 
            mapper.chr_rom[0][0], mapper.chr_rom[0][1], 
            mapper.chr_rom[0][2], mapper.chr_rom[0][3]);
    
    file.close();
    env->ReleaseStringUTFChars(romPath, path);
    
    LOG_CPU("ROM Extracted and Mapped successfully.");
    return env->NewStringUTF("Success");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv *env, jobject thiz, jstring filesDir) {
    if (!is_running) {
        clear_screen_to_black();
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

// Debug function to check emulator state
extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeGetDebugInfo(JNIEnv *env, jobject thiz) {
    char info[1024];
    uint16_t reset_vec = (mapper.prg_rom[3][0x3FFD] << 8) | mapper.prg_rom[3][0x3FFC];
    
    snprintf(info, sizeof(info),
        "PC: $%04X\n"
        "A: $%02X X: $%02X Y: $%02X\n"
        "P: $%02X S: $%02X\n"
        "PPU_CTRL: $%02X\n"
        "PPU_MASK: $%02X\n"
        "PPU_STATUS: $%02X\n"
        "Reset Vector: $%04X\n"
        "PRG Bank: %d\n"
        "Palette[0]: $%02X\n"
        "Palette[1]: $%02X\n"
        "Palette[2]: $%02X\n"
        "Palette[3]: $%02X\n"
        "CHR[0]: $%02X\n"
        "Running: %d Paused: %d",
        reg_PC, reg_A, reg_X, reg_Y, reg_P, reg_S,
        ppu_ctrl, ppu_mask, ppu_status,
        reset_vec, mapper.prg_bank,
        palette_ram[0], palette_ram[1], palette_ram[2], palette_ram[3],
        mapper.chr_rom[0][0],
        is_running ? 1 : 0, is_paused ? 1 : 0
    );
    
    return env->NewStringUTF(info);
}
