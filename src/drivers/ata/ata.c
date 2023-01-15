#ifndef HN_DRIVER_ATA_PIO
#define HN_DRIVER_ATA_PIO

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

void ata_lba_read(uint32_t address, uint8_t sectors, char *buffer) {
    __asm__ volatile(
        "call raw_ata_lba_read" :: "a"(address), "c"(sectors), "D"(buffer)
    );
}

void ata_lba_write(uint32_t address, uint8_t sectors, char *buffer) {
    __asm__ volatile(
        "call raw_ata_lba_write" :: "a"(address), "c"(sectors), "D"(buffer)
    );
}

#endif