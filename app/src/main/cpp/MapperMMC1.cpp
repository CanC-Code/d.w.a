#include <cstdint>
#include <cstring>
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "DWA_Mapper", __VA_ARGS__)

// Mapper state
static uint8_t* prg_rom_ptr = nullptr;
static uint8_t* chr_rom_ptr = nullptr;
static size_t prg_rom_size = 0;
static size_t chr_rom_size = 0;

// MMC1 Registers
static uint8_t shift_register = 0x10;  // Bit 7 set means "empty"
static uint8_t control = 0x0C;         // Start with 16KB PRG mode
static uint8_t chr_bank_0 = 0;
static uint8_t chr_bank_1 = 0;
static uint8_t prg_bank = 0;

// Current bank pointers
static uint8_t* prg_bank_low = nullptr;   // $8000-$BFFF
static uint8_t* prg_bank_high = nullptr;  // $C000-$FFFF
static uint8_t* chr_bank_0_ptr = nullptr; // $0000-$0FFF
static uint8_t* chr_bank_1_ptr = nullptr; // $1000-$1FFF

// Update bank pointers based on control register
static void update_banks() {
    int num_prg_banks = prg_rom_size / 16384;
    
    // PRG bank mode
    int prg_mode = (control >> 2) & 0x03;
    
    if (prg_mode == 0 || prg_mode == 1) {
        // 32KB mode: ignore low bit of bank number
        int bank = (prg_bank & 0x0E) % num_prg_banks;
        prg_bank_low = prg_rom_ptr + (bank * 16384);
        prg_bank_high = prg_bank_low + 16384;
    } else if (prg_mode == 2) {
        // Fix first bank at $8000, switch $C000
        prg_bank_low = prg_rom_ptr;
        int bank = (prg_bank & 0x0F) % num_prg_banks;
        prg_bank_high = prg_rom_ptr + (bank * 16384);
    } else {
        // Fix last bank at $C000, switch $8000
        int bank = (prg_bank & 0x0F) % num_prg_banks;
        prg_bank_low = prg_rom_ptr + (bank * 16384);
        prg_bank_high = prg_rom_ptr + ((num_prg_banks - 1) * 16384);
    }
    
    // CHR bank mode
    if (chr_rom_size > 0) {
        int num_chr_banks = chr_rom_size / 4096;
        
        if (control & 0x10) {
            // 4KB mode
            chr_bank_0_ptr = chr_rom_ptr + ((chr_bank_0 % num_chr_banks) * 4096);
            chr_bank_1_ptr = chr_rom_ptr + ((chr_bank_1 % num_chr_banks) * 4096);
        } else {
            // 8KB mode: ignore low bit
            int bank = (chr_bank_0 & 0x1E) % (num_chr_banks * 2);
            chr_bank_0_ptr = chr_rom_ptr + (bank * 4096);
            chr_bank_1_ptr = chr_bank_0_ptr + 4096;
        }
    }
}

void mapper_init(uint8_t* prg_rom, size_t prg_size, uint8_t* chr_rom, size_t chr_size) {
    prg_rom_ptr = prg_rom;
    chr_rom_ptr = chr_rom;
    prg_rom_size = prg_size;
    chr_rom_size = chr_size;
    
    LOGI("Mapper initialized: PRG=%zu bytes, CHR=%zu bytes", prg_size, chr_size);
}

void mapper_reset() {
    shift_register = 0x10;
    control = 0x0C;  // 16KB PRG mode, fix last bank
    chr_bank_0 = 0;
    chr_bank_1 = 0;
    prg_bank = 0;
    
    update_banks();
    LOGI("Mapper reset");
}

// Read from PRG-ROM ($8000-$FFFF)
uint8_t mapper_read(uint16_t addr) {
    if (addr >= 0x8000 && addr < 0xC000) {
        return prg_bank_low[addr - 0x8000];
    } else if (addr >= 0xC000) {
        return prg_bank_high[addr - 0xC000];
    }
    return 0;
}

// Write to mapper registers ($8000-$FFFF)
void mapper_write(uint16_t addr, uint8_t value) {
    if (addr < 0x8000) return;
    
    // Reset shift register on bit 7
    if (value & 0x80) {
        shift_register = 0x10;
        control |= 0x0C;  // Reset to 16KB mode on write
        update_banks();
        return;
    }
    
    // Shift in bit 0
    shift_register >>= 1;
    shift_register |= (value & 0x01) << 4;
    
    // After 5 writes, update register
    if (shift_register & 0x01) {
        uint8_t reg_value = shift_register >> 1;
        shift_register = 0x10;  // Reset
        
        // Determine which register based on address
        if (addr < 0xA000) {
            // Control register
            control = reg_value;
        } else if (addr < 0xC000) {
            // CHR bank 0
            chr_bank_0 = reg_value;
        } else if (addr < 0xE000) {
            // CHR bank 1
            chr_bank_1 = reg_value;
        } else {
            // PRG bank
            prg_bank = reg_value;
        }
        
        update_banks();
    }
}

// Read from CHR-ROM/RAM ($0000-$1FFF)
uint8_t mapper_read_chr(uint16_t addr) {
    if (addr < 0x1000) {
        return chr_bank_0_ptr[addr];
    } else {
        return chr_bank_1_ptr[addr - 0x1000];
    }
}

// Write to CHR-RAM (if CHR-ROM size is 0)
void mapper_write_chr(uint16_t addr, uint8_t value) {
    if (chr_rom_size == 0) {  // CHR-RAM
        if (addr < 0x1000) {
            chr_bank_0_ptr[addr] = value;
        } else {
            chr_bank_1_ptr[addr - 0x1000] = value;
        }
    }
}