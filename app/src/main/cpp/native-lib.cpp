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

// Forward declaration
namespace Dispatcher {
    void execute();
}

// ============================================================================
// GLOBAL HARDWARE STATE
// ============================================================================

// CPU Memory
uint8_t cpu_ram[0x0800] = {0};

// PPU Memory
uint8_t ppu_vram[0x0800] = {0};  // 2KB nametable RAM
uint8_t palette_ram[32] = {0};
uint8_t oam_ram[256] = {0};

// Screen buffer (256x240 pixels)
uint32_t screen_buffer[256 * 240] = {0};
std::mutex buffer_mutex;

// Controller state
uint8_t controller_state = 0;
uint8_t controller_shift = 0;

// PPU state
uint8_t ppu_ctrl = 0;
uint8_t ppu_mask = 0;
uint8_t ppu_status = 0;
uint8_t oam_addr = 0;
uint16_t ppu_addr_reg = 0;
uint8_t ppu_addr_latch = 0;
uint8_t ppu_scroll_x = 0;
uint8_t ppu_scroll_y = 0;
uint8_t ppu_data_buffer = 0;

// Mapper
MapperMMC1 mapper;

// Engine control
bool is_running = false;
bool is_paused = false;
int frame_count = 0;

// ============================================================================
// NES PALETTE (64 colors in ARGB format)
// ============================================================================

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
// 6502 CPU REGISTERS & STATE
// ============================================================================

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
        if (~(reg_A ^ val) & (reg_A ^ sum) & 0x80) reg_P |= FLAG_V; 
        else reg_P &= ~FLAG_V;
        if (sum > 0xFF) reg_P |= FLAG_C; 
        else reg_P &= ~FLAG_C;
        reg_A = (uint8_t)sum;
        update_nz(reg_A);
    }

    void cpu_sbc(uint8_t val) { 
        cpu_adc(~val); 
    }

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

    uint16_t read_pointer_indexed_x(uint16_t zp_addr) {
        uint8_t addr = (zp_addr + reg_X) & 0xFF;
        uint16_t lo = bus_read(addr);
        uint16_t hi = bus_read((addr + 1) & 0xFF);
        return lo | (hi << 8);
    }

    void push_stack(uint8_t val) { 
        cpu_ram[0x0100 | (reg_S & 0xFF)] = val; 
        reg_S--; 
    }
    
    uint8_t pop_stack() { 
        reg_S++; 
        return cpu_ram[0x0100 | (reg_S & 0xFF)]; 
    }
}

// ============================================================================
// PPU HELPER FUNCTIONS
// ============================================================================

uint16_t ntable_mirror(uint16_t addr) {
    // Horizontal mirroring for Dragon Warrior
    addr = (addr - 0x2000) & 0x0FFF;
    if (addr >= 0x0800) addr -= 0x0800;
    return addr & 0x07FF;
}

uint8_t read_palette(uint16_t addr) {
    uint8_t p_idx = addr & 0x1F;
    // Mirror $3F10/$3F14/$3F18/$3F1C to $3F00/$3F04/$3F08/$3F0C
    if (p_idx >= 0x10 && (p_idx % 4 == 0)) {
        p_idx -= 0x10;
    }
    return palette_ram[p_idx];
}

// ============================================================================
// MEMORY BUS (CPU $0000-$FFFF address space)
// ============================================================================

extern "C" uint8_t bus_read(uint16_t addr) {
    // $0000-$1FFF: 2KB RAM (mirrored 4 times)
    if (addr < 0x2000) {
        return cpu_ram[addr & 0x07FF];
    }
    
    // $2000-$3FFF: PPU registers (mirrored every 8 bytes)
    if (addr < 0x4000) {
        uint16_t reg = addr & 0x0007;
        
        if (reg == 2) { // PPU_STATUS ($2002)
            uint8_t status = ppu_status;
            ppu_status &= ~0x80;  // Clear VBlank flag
            ppu_addr_latch = 0;    // Reset address latch
            return status;
        }
        
        if (reg == 4) { // OAM_DATA ($2004)
            return oam_ram[oam_addr];
        }
        
        if (reg == 7) { // PPU_DATA ($2007)
            uint8_t data = ppu_data_buffer;
            uint16_t p_addr = ppu_addr_reg & 0x3FFF;
            
            if (p_addr < 0x2000) {
                // CHR-ROM/RAM read (buffered)
                ppu_data_buffer = mapper.read_chr(p_addr);
            } 
            else if (p_addr < 0x3F00) {
                // Nametable read (buffered)
                ppu_data_buffer = ppu_vram[ntable_mirror(p_addr)];
            } 
            else {
                // Palette read (NOT buffered)
                data = read_palette(p_addr);
                ppu_data_buffer = data;
            }
            
            ppu_addr_reg += (ppu_ctrl & 0x04) ? 32 : 1;
            return data;
        }
        
        return 0;
    }
    
    // $4000-$4017: APU and I/O
    if (addr == 0x4016) { // Controller 1
        uint8_t ret = (controller_shift & 0x80) >> 7;
        controller_shift <<= 1;
        return ret;
    }
    
    // $6000-$FFFF: Cartridge space (PRG-RAM and PRG-ROM)
    if (addr >= 0x6000) {
        return mapper.read_prg(addr);
    }
    
    return 0;
}

extern "C" void bus_write(uint16_t addr, uint8_t val) {
    // $0000-$1FFF: 2KB RAM
    if (addr < 0x2000) {
        cpu_ram[addr & 0x07FF] = val;
        return;
    }
    
    // $2000-$3FFF: PPU registers
    if (addr < 0x4000) {
        uint16_t reg = addr & 0x0007;
        
        if (reg == 0) { // PPU_CTRL ($2000)
            ppu_ctrl = val;
        }
        else if (reg == 1) { // PPU_MASK ($2001)
            ppu_mask = val;
        }
        else if (reg == 3) { // OAM_ADDR ($2003)
            oam_addr = val;
        }
        else if (reg == 4) { // OAM_DATA ($2004)
            oam_ram[oam_addr++] = val;
        }
        else if (reg == 5) { // PPU_SCROLL ($2005)
            if (ppu_addr_latch == 0) {
                ppu_scroll_x = val;
                ppu_addr_latch = 1;
            } else {
                ppu_scroll_y = val;
                ppu_addr_latch = 0;
            }
        }
        else if (reg == 6) { // PPU_ADDR ($2006)
            if (ppu_addr_latch == 0) {
                ppu_addr_reg = (ppu_addr_reg & 0x00FF) | ((val & 0x3F) << 8);
                ppu_addr_latch = 1;
            } else {
                ppu_addr_reg = (ppu_addr_reg & 0xFF00) | val;
                ppu_addr_latch = 0;
            }
        }
        else if (reg == 7) { // PPU_DATA ($2007)
            uint16_t p_addr = ppu_addr_reg & 0x3FFF;
            
            if (p_addr >= 0x2000 && p_addr < 0x3F00) {
                // Nametable write
                ppu_vram[ntable_mirror(p_addr)] = val;
            }
            else if (p_addr >= 0x3F00) {
                // Palette write
                uint8_t p_idx = p_addr & 0x1F;
                if (p_idx >= 0x10 && (p_idx % 4 == 0)) {
                    p_idx -= 0x10;
                }
                palette_ram[p_idx] = val;
            }
            
            ppu_addr_reg += (ppu_ctrl & 0x04) ? 32 : 1;
        }
        return;
    }
    
    // $4000-$4017: APU and I/O
    if (addr == 0x4014) { // OAM_DMA ($4014)
        uint16_t base = val << 8;
        for (int i = 0; i < 256; i++) {
            oam_ram[i] = bus_read(base + i);
        }
        return;
    }
    
    if (addr == 0x4016) { // Controller strobe
        if ((val & 0x01) == 0) {
            controller_shift = controller_state;
        }
        return;
    }
    
    // $6000-$FFFF: Cartridge space
    if (addr >= 0x6000) {
        mapper.write(addr, val);
    }
}

// ============================================================================
// PPU RENDERING
// ============================================================================

void clear_screen() {
    std::lock_guard<std::mutex> lock(buffer_mutex);
    memset(screen_buffer, 0, sizeof(screen_buffer));
}

void draw_frame() {
    // Don't render if PPU rendering is disabled
    if ((ppu_mask & 0x18) == 0) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(buffer_mutex);
    
    uint16_t nt_base = (ppu_ctrl & 0x03) * 0x0400;
    uint16_t bg_pat_base = (ppu_ctrl & 0x10) ? 0x1000 : 0x0000;
    uint16_t attr_base = nt_base + 0x03C0;
    
    // Render background (30 rows x 32 columns of 8x8 tiles)
    for (int ty = 0; ty < 30; ty++) {
        for (int tx = 0; tx < 32; tx++) {
            // Get tile index
            uint8_t tile = ppu_vram[ntable_mirror(0x2000 + nt_base + ty * 32 + tx)];
            
            // Get attribute byte (2x2 tile groups share attributes)
            uint16_t attr_addr = attr_base + (ty / 4) * 8 + (tx / 4);
            uint8_t attr_byte = ppu_vram[ntable_mirror(0x2000 + attr_addr)];
            
            // Extract 2-bit palette index for this tile
            uint8_t shift = ((ty & 2) << 1) | (tx & 2);
            uint8_t palette_select = (attr_byte >> shift) & 0x03;
            
            // Render 8x8 tile
            for (int y = 0; y < 8; y++) {
                uint8_t p1 = mapper.read_chr(bg_pat_base + tile * 16 + y);
                uint8_t p2 = mapper.read_chr(bg_pat_base + tile * 16 + y + 8);
                
                for (int x = 0; x < 8; x++) {
                    uint8_t bit1 = (p1 >> (7 - x)) & 0x01;
                    uint8_t bit2 = (p2 >> (7 - x)) & 0x01;
                    uint8_t color_index = bit1 | (bit2 << 1);
                    
                    // Build palette address
                    uint8_t palette_addr = (palette_select * 4) + color_index;
                    uint8_t color = read_palette(0x3F00 + palette_addr);
                    
                    int pixel_y = ty * 8 + y;
                    int pixel_x = tx * 8 + x;
                    screen_buffer[pixel_y * 256 + pixel_x] = nes_palette[color & 0x3F];
                }
            }
        }
    }
}

// ============================================================================
// CPU RESET & INTERRUPT HANDLERS
// ============================================================================

extern "C" void power_on_reset() {
    mapper.reset();
    
    // Read reset vector from $FFFC-$FFFD (in fixed bank)
    uint8_t lo = mapper.read_prg(0xFFFC);
    uint8_t hi = mapper.read_prg(0xFFFD);
    reg_PC = (hi << 8) | lo;
    
    LOGI("=== POWER-ON RESET ===");
    LOGI("Reset Vector: $%04X", reg_PC);
    
    // Verify valid address
    if (reg_PC < 0x8000 || reg_PC == 0xFFFF) {
        LOGE("Invalid reset vector $%04X! Using fallback.", reg_PC);
        reg_PC = 0xC000;
    }
    
    // Initialize CPU state
    reg_S = 0xFD;
    reg_P = 0x24;
    reg_A = 0;
    reg_X = 0;
    reg_Y = 0;
    
    // Clear PPU state
    ppu_ctrl = 0;
    ppu_mask = 0;
    ppu_status = 0x00;
    ppu_addr_reg = 0;
    ppu_addr_latch = 0;
    
    LOGI("CPU: PC=$%04X A=$%02X X=$%02X Y=$%02X P=$%02X S=$%02X",
         reg_PC, reg_A, reg_X, reg_Y, reg_P, reg_S);
}

extern "C" void nmi_handler() {
    // Push PC and status to stack
    push_stack(reg_PC >> 8);
    push_stack(reg_PC & 0xFF);
    push_stack(reg_P & ~0x10);  // Clear B flag
    
    // Read NMI vector from $FFFA-$FFFB
    uint8_t lo = mapper.read_prg(0xFFFA);
    uint8_t hi = mapper.read_prg(0xFFFB);
    reg_PC = (hi << 8) | lo;
    
    // Set VBlank flag
    ppu_status |= 0x80;
    
    // Disable interrupts
    reg_P |= FLAG_I;
}

// ============================================================================
// MAIN ENGINE LOOP
// ============================================================================

void engine_loop() {
    power_on_reset();
    clear_screen();
    is_running = true;
    frame_count = 0;
    
    LOGI("Engine started!");
    
    while (is_running) {
        if (is_paused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
            continue;
        }
        
        auto frame_start = std::chrono::steady_clock::now();
        
        // Debug first 5 frames
        if (frame_count < 5) {
            LOGI("Frame %d: PC=$%04X Bank=%d PPU_CTRL=$%02X PPU_MASK=$%02X",
                 frame_count, reg_PC, mapper.prg_bank, ppu_ctrl, ppu_mask);
        }
        
        // Execute ~29780 CPU cycles per frame (NTSC)
        for (int i = 0; i < 25000 && is_running; i++) {
            execute_instruction();
        }
        
        // Trigger NMI (VBlank)
        nmi_handler();
        
        // Render frame
        draw_frame();
        
        frame_count++;
        
        // Frame timing (60 FPS = ~16.67ms per frame)
        auto frame_end = frame_start + std::chrono::microseconds(16666);
        std::this_thread::sleep_until(frame_end);
    }
    
    LOGI("Engine stopped.");
}

// ============================================================================
// JNI FUNCTIONS
// ============================================================================

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(
    JNIEnv *env, jobject thiz, jstring romPath, jstring outDir) 
{
    const char *path = env->GetStringUTFChars(romPath, nullptr);
    
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        env->ReleaseStringUTFChars(romPath, path);
        LOGE("Failed to open ROM file");
        return env->NewStringUTF("Error: Could not open file");
    }
    
    // Read iNES header
    uint8_t header[16];
    file.read((char*)header, 16);
    
    // Validate NES magic bytes
    if (header[0] != 'N' || header[1] != 'E' || header[2] != 'S' || header[3] != 0x1A) {
        file.close();
        env->ReleaseStringUTFChars(romPath, path);
        LOGE("Not a valid NES ROM");
        return env->NewStringUTF("Error: Not a valid NES ROM");
    }
    
    uint8_t prg_banks = header[4];
    uint8_t chr_banks = header[5];
    uint8_t mapper_num = ((header[6] >> 4) | (header[7] & 0xF0));
    
    LOGI("ROM: PRG=%dKB CHR=%dKB Mapper=%d", prg_banks*16, chr_banks*8, mapper_num);
    
    if (mapper_num != 1) {
        file.close();
        env->ReleaseStringUTFChars(romPath, path);
        LOGE("ROM requires MMC1 (Mapper 1), found Mapper %d", mapper_num);
        return env->NewStringUTF("Error: Wrong mapper type (need MMC1)");
    }
    
    // Read PRG-ROM (16KB banks)
    for (int i = 0; i < prg_banks && i < 4; i++) {
        file.read((char*)mapper.prg_rom[i], 16384);
        if (file.gcount() != 16384) {
            LOGE("Failed to read PRG bank %d", i);
            file.close();
            env->ReleaseStringUTFChars(romPath, path);
            return env->NewStringUTF("Error: Incomplete PRG-ROM");
        }
    }
    
    // Read CHR-ROM (8KB banks)
    if (chr_banks > 0) {
        file.read((char*)mapper.chr_rom, chr_banks * 8192);
        if (file.gcount() != chr_banks * 8192) {
            LOGE("Failed to read CHR-ROM");
        }
    }
    
    file.close();
    env->ReleaseStringUTFChars(romPath, path);
    
    // Verify reset vector
    uint16_t reset_vec = (mapper.prg_rom[3][0x3FFD] << 8) | mapper.prg_rom[3][0x3FFC];
    LOGI("Reset Vector: $%04X", reset_vec);
    LOGI("ROM loaded successfully!");
    
    return env->NewStringUTF("Success");
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(
    JNIEnv *env, jobject thiz, jstring filesDir) 
{
    if (!is_running) {
        LOGI("Starting engine thread...");
        std::thread(engine_loop).detach();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(
    JNIEnv *env, jobject thiz, jobject bitmap) 
{
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

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(
    JNIEnv *env, jobject thiz, jint buttonBit, jboolean isPressed) 
{
    if (isPressed) {
        controller_state |= (uint8_t)buttonBit;
    } else {
        controller_state &= ~((uint8_t)buttonBit);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativePauseEngine(JNIEnv *env, jobject thiz) {
    is_paused = true;
}

extern "C" JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeResumeEngine(JNIEnv *env, jobject thiz) {
    is_paused = false;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeGetDebugInfo(JNIEnv *env, jobject thiz) {
    char info[1024];
    snprintf(info, sizeof(info),
        "Frame: %d\n"
        "PC: $%04X\n"
        "A: $%02X X: $%02X Y: $%02X\n"
        "P: $%02X S: $%02X\n"
        "PPU_CTRL: $%02X\n"
        "PPU_MASK: $%02X\n"
        "Bank: %d\n"
        "Pal[0-3]: $%02X $%02X $%02X $%02X\n"
        "Running: %s Paused: %s",
        frame_count, reg_PC, reg_A, reg_X, reg_Y, reg_P, reg_S,
        ppu_ctrl, ppu_mask, mapper.prg_bank,
        palette_ram[0], palette_ram[1], palette_ram[2], palette_ram[3],
        is_running ? "YES" : "NO",
        is_paused ? "YES" : "NO"
    );
    return env->NewStringUTF(info);
}
