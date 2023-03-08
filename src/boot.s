/* Constants for the multiboot header. */
.set ALIGN,     1<<0                    /* align loaded modules on page boundaries */
.set MEMINFO,   1<<1                    /* provide memory map */
.set FLAGS,     4                       /* Multiboot 'flag' field */
.set MAGIC,     0x1BADB002              /* Multiboot magic number */
.set CHECKSUM,  -(MAGIC + FLAGS)        /* Checksum for multiboot */

/* Begin multiboot header */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.skip 24

/* Graphics Fields */
// Linear Graphics Mode
.long 0
// Width
.long 640
// Height
.long 480
// Depth
.long 16

/* Kernel stack */
.section .bss
.align 16
stack_bottom:
.skip 16384     # 16 KiB
stack_top:

/* Called by GRUB */
.section .text
.global _start
.type _start, @function
_start:
    /* 
        welcome to the rice fields of kernel space
    */
    mov $stack_top, %esp

    /*
        INITIALIZE KERNEL STATE HERE
    */

    push %ebx
    call kernel_main

    /*
        Something has gone HORRIBLY wrong,
        and the kernel has returned.

        1) Disable maskable interrupts with `cli`
        2) Wait for the next interrupt to arrive with `hlt`
           They are disabled, this should freeze *everything*
        3) Jump back to the halt instruction if we wake up somehow. (NMI or SMM)
    */
    cli
1:  hlt
    jmp 1b

/*
    Set the size of `_start` for debugging and stack traces.
*/
.size _start, . - _start
