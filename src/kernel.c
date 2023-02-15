#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdio.h>

void kernel_main() {
    terminal_initialize();
    terminal_writestring("bawls");

    // explod
    uint64_t null_idtr = 0;
    asm("xor %%eax, %%eax; lidt %0; int3" :: "m" (null_idtr));
}

void interrupt_handler() {

}