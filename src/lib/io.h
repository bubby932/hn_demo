#ifndef LIB_HACKNET_IO
#define LIB_HACKNET_IO

#include <stdint.h>

#define outbyte(port, value) \
    __asm__ ("outb %%al, %%dx"::"a" (value), "d" (port))

#define inbyte(port) ({ unsigned char _v; __asm__ volatile ("inb %%dx, %%al":"=a" (_v):"d" (port)); _v; })

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

#endif