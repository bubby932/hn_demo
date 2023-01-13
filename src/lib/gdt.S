.section .text
.align 0
.global refresh_cs
.type refresh_cs, @function
refresh_cs:
    ljmpl $0x8, $flush
flush:
    mov $0x10, %ax
    mov %ax, %ds

    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ret

.section .data
.align 0
gdtr:
    .word 0 // Limit
    .long 0 // Base

.section .text
.align 0
.global set_gdt
.type set_gdt, @function
set_gdt:    // void set_gdt(uint16_t limit, uint8_t *base)
    mov 4(%esp), %ax
    mov %ax, (gdtr)
    mov 8(%esp), %eax
    mov %eax, (gdtr + 2)
    lgdt (gdtr)
    
    ret
