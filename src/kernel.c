#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "lib/paging.c"
#include "lib/string.c"
#include "lib/serial.c"
#include "lib/gdt.c"
#include "lib/mm.c"
#include "lib/io.h"

#include "security.c"
#include "sched.c"

#if defined(__linux__)
#error "You aren't using a cross compiler!"
#endif

#if !defined(__i386__)
#error "This needs to be compiled with i*86-elf GCC."
#endif

void kernel_main(void) {
    terminal_initialize();
    
    terminal_writestring("Terminal init OK...\n");
    terminal_writestring("(hn_demo v0.0.1) Kernel init, beginning load...\n");

    terminal_writestring("Setting up serial port...\n");
    if(!init_serial()) {
        error_terminal_writestring("[ERROR] Faulty serial port, serial init failed.\n");
    } else {
        terminal_writestring("Serial init OK...\n");
    }

    serial_writestring("(HackNet v0.0.1) Serial init...\n\r");
    serial_writestring("Serial out test OK\n\r");

    serial_writestring("Setting up GDT...\n\r");
    gdt_init();
    serial_writestring("GDT init OK...\n\r");
    terminal_writestring("GDT init OK...\n");

    serial_writestring("Setting up paging...\n\r");

    paging_init();

    serial_writestring("Paging init OK...\n\r");
    terminal_writestring("Paging init OK...\n");

    kheap_init();
    terminal_writestring("Kernel heap init OK...\n");
    terminal_writestring("Double-checking kheap allocation...\n");

    char *ptr = kmalloc(sizeof(char) * 28);
    for (size_t i = 0; i < 26; i++){
        ptr[i] = i + 97;
    }

    ptr[27] = '\0';

    terminal_writestring(ptr);
    terminal_putchar('\n');

    for (size_t i = 0; i < 26; i++){
        ptr[i] = i + 65;
    }

    ptr[27] = '\0';

    terminal_writestring(ptr);
    terminal_putchar('\n');

    kfree(ptr);

    terminal_writestring("kheap selftest OK...\n");

    terminal_writestring("End of kernel reached!\n");

    while(true) {
        __asm__ volatile("hlt");
    }
}

void interrupt_handler() {
    kpanic("System calls are not implemented!\n");
}