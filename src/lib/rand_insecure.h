#ifndef LIB_HACKNET_RAND_INSECURE
#define LIB_HACKNET_RAND_INSECURE

#include <stdint.h>

#include "kutils.c"

uint32_t current;

uint32_t insec_rand_next() {
    current = current * 1103515245 + 12345;
    return (current / 65536) % 32768;
}

void insec_rand_seed() {
    debug_terminal_writestring("[RAND] Seeding random number generator...\n");
    current = (uint32_t)(rdtsc() >> 32);
    io_wait_long();
    current ^= (uint32_t)rdtsc();
    for (size_t i = 0; i < 50; i++)
        insec_rand_next();
    
    debug_terminal_writestring("[RAND] Random number generator seeded.\n");
}

#endif