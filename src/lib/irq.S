.section .text
.global gp_fault_asm
.type gp_fault_asm, @function
gp_fault_asm:
    pushal

    cld
    call gp_fault_c

    popal
    iret

.section .text
.global pit_interrupt_asm
.type pit_interrupt_asm, @function
pit_interrupt_asm:
    pushal

    cld
    call sched_tick_irq

    popal
    iret

.section .text
.global keyboard_irq_asm
.type keyboard_asm, @function
keyboard_irq_asm:
    pushal

    call key_pressed_irq

    popal
    iret

.section .text
.global no_handler
.type no_handler, @function
no_handler:
    pushal

    call no_handler_c

    popal
    iret

.section .data
idtr:
    .word 0 // size
    .long 0 // Offset

.section .text
.global load_idt
.type load_idt, @function
load_idt: // load_idt (uint32_t offset, uint16_t size)
    mov 4(%esp), %eax       // EAX_32   = offset
    mov 8(%esp), %cx        // CX_16    = size

    mov %eax, (idtr + 2)    // idtr_o   = EAX_32
    mov %cx, (idtr)         // itdr_s   = CS_16

    lidt idtr
    ret
