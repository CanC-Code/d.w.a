#include <jni.h>
#include <string>
#include <vector>
#include <fstream>
#include <android/log.h>
#include <android/bitmap.h>
#include <aaudio/AAudio.h>
#include <thread>
#include <cstring>

#define LOG_TAG "DWA_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// --- NES Hardware Constants ---
#define ADDR_PRG_ROM_LOW  0x8000
#define ADDR_PRG_ROM_HIGH 0xC000

// --- Emulator State ---
uint8_t cpu_ram[0x0800];
uint8_t prg_rom[4][16384]; // 4 Banks of 16KB for DW1
uint32_t screen_buffer[256 * 240];
uint8_t controller_state = 0;
bool is_running = false;

// Registers
uint16_t reg_PC;
uint8_t reg_A, reg_X, reg_Y, reg_S, reg_P;

// Audio
AAudioStream *audio_stream;

// --- NES Palette ---
uint32_t nes_palette[64] = {
    0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
    0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFADADAD, 0xFF155FD9, 0xFF4142FF, 0xFF7C1AFF, 0xFFB513F9, 0xFFD210BE, 0xFFD42C44, 0xFFB84B00,
    0xFF816D00, 0xFF458800, 0xFF129800, 0xFF009923, 0xFF008985, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFFFFFFF, 0xFF64B0FF, 0xFF9290FF, 0xFFC676FF, 0xFFF35AFF, 0xFFFE66CC, 0xFFFE7373, 0xFFF38913,
    0xFFD3AB00, 0xFFA1D000, 0xFF7FDE00, 0xFF72E248, 0xFF72D1AF, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFFFFFFF, 0xFFC0DFFF, 0xFFD3D2FF, 0xFFE8C8FF, 0xFFFBC2FF, 0xFFFEC4EA, 0xFFFECCC5, 0xFFF7D8A5,
    0xFFE4E594, 0xFFCFEF96, 0xFFBDF4AB, 0xFFB3F3CC, 0xFFB5EBF2, 0xFF000000, 0xFF000000, 0xFF000000
};

// --- Memory Access ---
uint8_t read_byte(uint16_t addr) {
    if (addr < 0x2000) return cpu_ram[addr % 0x0800];
    if (addr >= 0x8000) {
        // Simple bank mapping for DW1 (Mapper 1 / MMC1 simplified)
        int bank = (addr < 0xC000) ? 0 : 3; // Fixed bank logic
        return prg_rom[bank][addr - (bank < 3 ? 0x8000 : 0xC000)];
    }
    return 0;
}

// --- Audio Callback ---
aaudio_data_callback_result_t audio_callback(AAudioStream *stream, void *userData, void *audioData, int32_t numFrames) {
    memset(audioData, 0, numFrames * sizeof(float));
    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

extern "C" {

// 1. ROM Extraction (Splits iNES into Banks)
JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* cRomPath = env->GetStringUTFChars(romPath, nullptr);
    const char* cOutDir = env->GetStringUTFChars(outDir, nullptr);
    
    std::ifstream rom(cRomPath, std::ios::binary);
    if (!rom) return env->NewStringUTF("Error: File not found");

    char header[16];
    rom.read(header, 16);
    
    // Verify iNES Header
    if (header[0] != 'N' || header[1] != 'E' || header[2] != 'S') return env->NewStringUTF("Error: Invalid NES File");

    std::string outDirStr(cOutDir);
    for (int i = 0; i < 4; i++) {
        std::vector<char> buffer(16384);
        rom.read(buffer.data(), 16384);
        std::ofstream bank(outDirStr + "/Bank0" + std::to_string(i) + ".bin", std::ios::binary);
        bank.write(buffer.data(), 16384);
        bank.close();
    }

    env->ReleaseStringUTFChars(romPath, cRomPath);
    env->ReleaseStringUTFChars(outDir, cOutDir);
    return env->NewStringUTF("Success: ROM Ready");
}

// 2. Engine Init
JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz, jstring filesDir) {
    const char* cPath = env->GetStringUTFChars(filesDir, nullptr);
    std::string pathStr(cPath);

    // Load Banks into Memory
    for(int i = 0; i < 4; i++) {
        std::ifstream in(pathStr + "/Bank0" + std::to_string(i) + ".bin", std::ios::binary);
        if (in.is_open()) in.read((char*)prg_rom[i], 16384);
    }

    // Initialize CPU (Reset Vector)
    reg_PC = (read_byte(0xFFFD) << 8) | read_byte(0xFFFC);
    reg_S = 0xFD;
    reg_P = 0x24; // Default flags
    
    memset(screen_buffer, 0, sizeof(screen_buffer));

    // Audio Start
    AAudioStreamBuilder *builder;
    AAudio_createStreamBuilder(&builder);
    AAudioStreamBuilder_setFormat(builder, AAUDIO_FORMAT_PCM_FLOAT);
    AAudioStreamBuilder_setDataCallback(builder, audio_callback, nullptr);
    AAudioStreamBuilder_openStream(builder, &audio_stream);
    AAudioStream_requestStart(audio_stream);
    AAudioStreamBuilder_delete(builder);

    is_running = true;
    env->ReleaseStringUTFChars(filesDir, cPath);
}

// 3. Render (Frame Update)
JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_nativeUpdateSurface(JNIEnv* env, jobject thiz, jobject bitmap) {
    void* pixels;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) return;
    
    // Placeholder rendering logic: Fill screen with a palette color
    // In a full emu, the PPU would fill screen_buffer throughout the frame
    memcpy(pixels, screen_buffer, 256 * 240 * 4);
    
    AndroidBitmap_unlockPixels(env, bitmap);
}

// 4. Input Injection
JNIEXPORT void JNICALL
Java_com_canc_dwa_MainActivity_injectInput(JNIEnv* env, jobject thiz, jint buttonBit, jboolean isPressed) {
    if (isPressed) controller_state |= buttonBit;
    else controller_state &= ~buttonBit;
}

} // extern "C"
