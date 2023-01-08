#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "lib/string.c"

#if defined(__linux__)
#error "You aren't using a cross compiler!"
#endif

#if !defined(__i386__)
#error "This needs to be compiled with i*86-elf GCC."
#endif


void kernel_main(void) {
    terminal_initialize();
    terminal_writestring("(hn_demo v0.0.1) Kernel init, beginning load...\n");
}