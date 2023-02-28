#ifndef LIB_HACKNET_RAND_INSECURE
#define LIB_HACKNET_RAND_INSECURE

#include <stdint.h>

#include "kutils.cpp"

uint32_t current;

uint32_t insec_rand_next() {
    current = current * 1103515245 + 12345;
    return (current / 65536) % 32768;
}

void insec_rand_seed() {
    debug_terminal_writestring("[RAND] Seeding random number generator...\n");

    current = rdtsc() % 32768;

    debug_terminal_writestring("[RAND] Random number generator seeded.\n");
}

#endif