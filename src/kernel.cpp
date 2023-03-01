#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "lib/rand_insecure.h"
#include "lib/syscall.cpp"
#include "lib/paging.cpp"
#include "lib/string.cpp"
#include "lib/serial.cpp"
#include "lib/irq.cpp"
#include "lib/gdt.cpp"
#include "lib/fmt.h"
#include "lib/mm.cpp"
#include "lib/io.h"

#include "security.cpp"
#include "sched.cpp"

#include <must/panic.h>
#include <drivers/vga.h>

extern "C"
void kernel_main() {

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

    char *ptr = (char *)kmalloc(sizeof(char) * 28);
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
    terminal_writestring("Setting up IDT...\n");

    idt_init();

    insec_rand_seed();

    char buffer[25];
    debug_terminal_writestring("[RAND_t] Random number A: ");
    debug_terminal_writestring(itoa(insec_rand_next(), buffer, 10));
    debug_terminal_writestring("\n[RAND_t] Random number B: ");
    debug_terminal_writestring(itoa(insec_rand_next(), buffer, 10));
    debug_terminal_writestring("\n[RAND_t] Insecure random suite test complete.\n");

    VgaText::startup_screen("Initializing VGA driver for graphics mode. Please note, everything is working,  this is just a quick text mode message to let you know what's going on.", 152, 15 | 0 << 4);

    while(true) {
        asm volatile("hlt");
    }
}

void interrupt_handler() {
    kpanic("System calls are not implemented!\n");
}