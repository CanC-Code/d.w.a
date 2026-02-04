#include <jni.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

extern "C" JNIEXPORT jstring JNICALL
Java_com_canc_dwa_MainActivity_nativeExtractRom(JNIEnv* env, jobject thiz, jstring romPath, jstring outDir) {
    const char* cRomPath = env->GetStringUTFChars(romPath, nullptr);
    const char* cOutDir = env->GetStringUTFChars(outDir, nullptr);

    std::ifstream rom(cRomPath, std::ios::binary);
    if (!rom.is_open()) return env->NewStringUTF("Error: Cannot open ROM");

    // Read iNES Header (16 bytes)
    char header[16];
    rom.read(header, 16);

    // According to your disassembly:
    // Bank00-03 are PRG (16KB each = 64KB)
    // There is also CHR data (8KB)
    
    std::string outputBase = std::string(cOutDir) + "/bank_";
    for(int i=0; i<4; i++) {
        char bankData[16384];
        rom.read(bankData, 16384);
        std::ofstream out(outputBase + std::to_string(i) + ".bin", std::ios::binary);
        out.write(bankData, 16384);
        out.close();
    }

    env->ReleaseStringUTFChars(romPath, cRomPath);
    env->ReleaseStringUTFChars(outDir, cOutDir);

    return env->NewStringUTF("Extraction Complete: 4 Banks Extracted to Internal Storage");
}
