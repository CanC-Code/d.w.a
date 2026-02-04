#include <jni.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

// Structure to hold the iNES Header info found in Header.asm
struct NESHeader {
    char magic[4];    // Should be "NES\x1a"
    uint8_t prg_size; // Number of 16KB units
    uint8_t chr_size; // Number of 8KB units
    uint8_t flags6;   // Mapper, mirroring, battery, trainer
    uint8_t flags7;   // Mapper, VS/Playchoice, NES 2.0
    uint8_t padding[8];
};

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* cRomPath = env->GetStringUTFChars(romPath, nullptr);
    const char* cOutDir = env->GetStringUTFChars(outDir, nullptr);

    std::ifstream rom(cRomPath, std::ios::binary);
    if (!rom.is_open()) {
        env->ReleaseStringUTFChars(romPath, cRomPath);
        return env->NewStringUTF("Error: Could not open ROM file.");
    }

    NESHeader header;
    rom.read(reinterpret_cast<char*>(&header), 16);

    // 1. Validate iNES Header
    if (header.magic[0] != 'N' || header.magic[1] != 'E' || header.magic[2] != 'S') {
        return env->NewStringUTF("Error: Invalid NES file format.");
    }

    // 2. Extract PRG Banks (Code)
    // Dragon Warrior uses 4 banks of 16KB as seen in build_script
    std::string outputDir = std::string(cOutDir);
    for(int i = 0; i < 4; i++) {
        std::vector<char> buffer(16384);
        rom.read(buffer.data(), 16384);
        
        std::string fileName = outputDir + "/prg_bank_" + std::to_string(i) + ".bin";
        std::ofstream out(fileName, std::ios::binary);
        out.write(buffer.data(), 16384);
        out.close();
    }

    // 3. Extract CHR Data (Graphics)
    // Dragon Warrior (U) has 1 bank of 8KB CHR
    if (header.chr_size > 0) {
        std::vector<char> chrBuffer(8192);
        rom.read(chrBuffer.data(), 8192);
        
        std::string chrName = outputDir + "/chr_assets.bin";
        std::ofstream chrOut(chrName, std::ios::binary);
        chrOut.write(chrBuffer.data(), 8192);
        chrOut.close();
    }

    // 4. Clean up
    rom.close();
    env->ReleaseStringUTFChars(romPath, cRomPath);
    env->ReleaseStringUTFChars(outDir, cOutDir);

    return env->NewStringUTF("Success: PRG Banks and CHR Graphics extracted natively.");
}

// Native hook to read the specific RAM addresses from Dragon_Warrior_Defines.asm
extern "C" JNIEXPORT jint JNICALL
Java_com_canc_dwa_MainActivity_getRamValue(JNIEnv* env, jobject thiz, jint address) {
    // This will be implemented when we integrate the 6502 Core
    // For now, it's a placeholder for the native port bridge
    return 0; 
}
