.section .text
.global test_handler_asm
.type test_handler_asm, @function
test_handler_asm:
    pushal

    // cld
    // call test_handler_c

    popal
    hlt
    iret

.section .data
idtr:
    .word 0 // size
    .long 0 // Offset

string:
    .ascii "i am going to kms"
    .byte 0

.section .text
.global load_idt
.type load_idt, @function
load_idt: // load_idt (uint32_t offset, uint16_t size)
    mov 4(%esp), %eax       // EAX_32   = offset
    mov 8(%esp), %cx        // CX_16    = size

    mov %eax, (idtr + 2)    // idtr_o   = EAX_32
    mov %cx, (idtr)         // itdr_s   = CS_16

    lidt (idtr)
    ret
