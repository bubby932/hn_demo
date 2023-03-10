#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// FIXME: We shouldn't be importing .cpp files. Everything 
//        should be defined in .h files and then we should
//        compile and link all the .cpp files seperately.
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

/// @brief 32-Bit entry point for the kernel.
/// This is called by the bootloader. Later, we should add 64-bit support 
//  and seperate the boot process into different architectures.
extern "C"
void kernel_main(multiboot_info_t *multiboot) {

    // TODO: Rewrite GDT and Paging handlers to be more dynamic
    //       and overall better organized.
    gdt_init();
    paging_init();

    // TODO: Remove legacy KHeap.
    kheap_init();

    // TODO: Implement a more open / idiomatic API for the
    //       interrupt descriptor table.
    idt_init();

    // TODO: Implement a generic graphics driver and then assign
    //       the proper implementation (text/VESA) at runtime.

    while(true) {
        asm volatile("hlt");
    }
}