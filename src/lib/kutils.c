#ifndef LIB_HACKNET_KERNEL_UTILS
#define LIB_HACKNET_KERNEL_UTILS

#define asm __asm__

#include "serial.c"
#include "string.c"

void debug_terminal_writestring(const char *message) {
    terminal_setcolor(vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));

    terminal_writestring(message);

    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}

void error_terminal_writestring(const char *message) {
    terminal_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));

    terminal_writestring(message);

    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}

/// @brief Kernel panic function, prints a message to the screen then halts the CPU.
/// @param message Customized message to print about what the error was.
void kpanic(const char *message) {
    error_terminal_writestring("HackNet Kernel Panic!\n(An unrecoverable internal error occurred.)\nMessage:\n");
    error_terminal_writestring(message);

    serial_writestring("Kernel panic.\n\r");
    serial_writestring(message);
    write_serial('\r');

    while(true) {
        __asm__ volatile("cli");
        __asm__ volatile("hlt");
    }
}

void kpanic(const char *) __attribute__ ((noreturn));

#endif