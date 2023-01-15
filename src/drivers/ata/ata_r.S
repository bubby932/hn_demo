.section .text
.global raw_ata_lba_read
.type raw_ata_lba_read, @function

// LBA ATA read
// EAX  = LBA address of first sector
// CL   = Number of sectors to read
// RDI  = The buffer to write into
raw_ata_lba_read:
    pushfl
    and $0xFFFFFFFF, %eax
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
    out %al, %dx

    mov $0x1F4, %edx
    mov %ebx, %eax
    shr $16, %eax
    out %al, %dx

    mov $0x1F7, %edx
    mov $0x20, %al
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
    rep insw

    pop %edi
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    popfl
    ret
