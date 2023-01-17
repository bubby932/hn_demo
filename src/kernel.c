#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "lib/rand_insecure.h"
#include "lib/syscall.c"
#include "lib/paging.c"
#include "lib/string.c"
#include "lib/serial.c"
#include "lib/irq.c"
#include "lib/gdt.c"
#include "lib/fmt.h"
#include "lib/mm.c"
#include "lib/io.h"
#include "fs/vfs.c"

#include "security.c"
#include "sched.c"

#if defined(__linux__)
#error "You aren't using a cross compiler!"
#endif

#if !defined(__i386__)
#error "This needs to be compiled with i*86-elf GCC."
#endif

#include "rescue.c"

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
    terminal_writestring("Setting up IDT...\n");

    idt_init();

    insec_rand_seed();

    char buffer[25];
    debug_terminal_writestring("[RAND_t] Random number A: ");
    debug_terminal_writestring(itoa(insec_rand_next(), buffer, 10));
    debug_terminal_writestring("\n[RAND_t] Random number B: ");
    debug_terminal_writestring(itoa(insec_rand_next(), buffer, 10));
    debug_terminal_writestring("\n[RAND_t] Insecure random suite test complete.\n");

    debug_terminal_writestring("[VFS] Setting up VFS...\n");

    errno_t vfs_err = init_vfs();
    if(vfs_err) {
        switch (vfs_err) {
            case ENOENT: {
                kpanic("[VFS] Failed to initialize root directory of VFS, panic!\n");
                break;
            }
            case ENOMEM: {
                kpanic("[VFS] Allocation error while setting up VFS!\n");
                break;
            }
            default: {
                kpanic("[VFS] Unknown error while setting up VFS, panic!\n");
                break;
            }
        }
    }

    debug_terminal_writestring("[VFS] VFS init OK! Running selftest...\n");

    inode **inbuf = (inode **)kmalloc(vfs_sysroot->size);

    vfs_sysroot->readdir(vfs_sysroot, inbuf);


    debug_terminal_writestring("[VFS] VFS selftest 1 OK!\n");

    debug_terminal_writestring("[VFS] Testing /dev/insec_random\n");

    inode *dev = inbuf[0];

    kfree(inbuf);

    inbuf = (inode **)kmalloc(dev->size);

    dev->readdir(dev, inbuf);

    inode *insec_random = inbuf[3];

    kfree(inbuf);

    debug_terminal_writestring("[VFS] Located /dev/insec_random, reading...\n");

    char insec_buffer[100];

    insec_random->read(insec_random, insec_buffer, 100);

    terminal_write(buffer, 100);

    debug_terminal_writestring("[VFS] Read and printed /dev/insec_random OK...\n");
    debug_terminal_writestring("[VFS] VFS selftests complete!\n");

    debug_terminal_writestring("[RESCUE] Initializing Rescue Mode ALPHA...\n");

    rescue();

    while(true) {
        asm volatile("hlt");
    }
}

void interrupt_handler() {
    kpanic("System calls are not implemented!\n");
}