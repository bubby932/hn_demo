#pragma once

#include <stdint.h>

static inline void outbyte(uint16_t port, uint8_t value) {
    __asm__("outb %%al, %%dx" ::"a"(value), "d"(port));
}

static inline uint8_t inbyte(uint16_t port) {
    uint8_t value;
    __asm__ volatile ("inb %%dx, %%al":"=a" (value):"d" (port));
    return value;
}

static inline void io_wait() {
    outbyte(0x80, 0);
}

static inline void io_wait_long() {
    for (int i = 0; i < 300 * 300 * 100; i++)
        io_wait();
}

static inline void invlpg(void *m) {
    __asm__ volatile("invlpg (%0)" : : "b"(m) : "memory");
}

uint64_t rdtsc() {
    uint32_t low, high;
    __asm__ volatile("rdtsc": "=a"(low), "=d"(high));
    return ((uint64_t)high << 32) | low;
}
