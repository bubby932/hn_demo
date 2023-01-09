#ifndef LIB_HACKNET_KERNEL_UTILS
#define LIB_HACKNET_KERNEL_UTILS

#include "string.c"

void kpanic(const char *message) {
    terminal_writestring("kernel panic, cry about it.\nmessage:\n");
    terminal_writestring(message);

    while(true) {
        __asm volatile("cli");
        __asm volatile("hlt");
    }
}

#endif