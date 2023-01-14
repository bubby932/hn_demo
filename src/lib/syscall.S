.section .text
.global syscall_asm
.type syscall_asm, @function
syscall_asm:
    // State
    pushl %gs
    pushl %fs
    pushl %es
    pushl %ds

    fxsave fxsave_dest

    // Parameters
    push %ebp
    push %edi
    push %edx
    push %ecx
    push %ebx
    push %eax

    call syscall

    add $28, %esp

    fxrstor fxsave_dest

    popl %ds
    popl %es
    popl %fs
    popl %gs

    iret

.section .data
.align 16
fxsave_dest:
    .skip 511
    .byte 0
