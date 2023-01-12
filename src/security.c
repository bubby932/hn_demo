#ifndef HACKNET_KERNEL_SEC
#define HACKNET_KERNEL_SEC

#include <stdint.h>

#include "lib/string.c"

// Self-hosters - you'll need to create this file yourself and #define STACK_CHK_GUARD manually.
// Change it between builds!
#include "SECURITY.h"

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

__attribute__((noreturn))
void __stack_chk_fail(void) {
    __asm__ volatile("cli");

    for (size_t i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++)
    {
        terminal_buffer[i] = vga_entry(' ', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED));
        terminal_column = 0;
        terminal_row = 0;
    }

    terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED);

    terminal_writestring("WARNING\n");
    terminal_writestring("STACK OVERRUN DETECTED : EMERGENCY RECOVERY MODE ACTIVE\n\n");
    
    terminal_writestring("Buffer overflow exploit detected in kernel space\n");
    terminal_writestring("::Emergency recovery mode activated\n");
    terminal_writestring("--------------------------------------------------------------------------------\n\n");

    terminal_writestring("This system will restart IMMEDIATELY - this may be a malicious exploit of your system.\n");
    terminal_writestring("THIS IS NOT REPEATABLE AND CANNOT BE DELAYED\n");

    io_wait_long();
    terminal_writestring("Restart in 5...");
    io_wait_long();
    terminal_writestring("4...");
    io_wait_long();
    terminal_writestring("3...");
    io_wait_long();
    terminal_writestring("2...");
    io_wait_long();
    terminal_writestring("1...\n");
    terminal_writestring("!! RESTARTING HACKNET KERNEL NOW !!\n");

    io_wait_long();

    __asm__ volatile("ljmp 0x0");
    while(true);
}

#endif