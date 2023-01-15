.section .text
.global raw_ata_lba_write
.type raw_ata_lba_write, @function

// ATA LBA write
// EAX  = LBA address of first sector
// CL   = Number of sectors to write
// RDI  = Buffer to write from
raw_ata_lba_write:
    pushfl
    and $0x0FFFFFFF, %eax
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %edi

    mov %eax, %ebx

    mov $0x01F6, %edx
    shr $24, %eax
    or $0b11100000, %al
    out %al, %dx

    mov $0x01F2, %edx
    mov %cl, %al
    out %al, %dx

    mov $0x1F3, %edx
    mov %ebx, %eax
    shr $8, %eax
    out %al, %dx


    mov $0x1F5, %edx
    mov %ebx, %eax
    shr $16, %eax
    out %al, %dx

    mov $0x1F7, %edx
    mov $0x30, %al
    out %al, %dx

.still_going:
    in %dx, %al
    test $8, %al
    jz .still_going

    mov $256, %eax
    xor %bx, %bx
    mov %cl, %bl
    mul %bx
    mov %eax, %ecx
    mov $0x1F0, %edx
    mov %edi, %esi
    rep outsw

    pop %edi
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    popfl
    ret
