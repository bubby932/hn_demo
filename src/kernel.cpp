#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "lib/syscall.cpp"
#include "lib/paging.cpp"
#include "lib/serial.cpp"
#include "lib/irq.cpp"
#include "lib/gdt.cpp"
#include "lib/mm.cpp"
#include "lib/io.h"

#include "security.cpp"
#include "sched.cpp"

#include <drivers/vga.h>
#include <multiboot.h>

extern "C"
void kernel_main(multiboot_info_t *multiboot) {
    gdt_init();

    paging_init();

    kheap_init();

    idt_init();

    // TODO: impl graphics driver

    while(true) {
        asm volatile("hlt");
    }
}