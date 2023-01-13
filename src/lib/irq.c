#ifndef LIB_HACKNET_IRQ
#define LIB_HACKNET_IRQ

#include <stdint.h>

#include "paging.c"
#include "kutils.c"
#include "serial.c"
#include "string.c"
#include "io.h"

typedef struct _IdtEntry {
    uint16_t    base_lo;
    uint16_t    segment;
    uint8_t     reserved;
    uint8_t     flags;
    uint16_t    base_hi;
} __attribute__((packed)) IdtEntry;

extern void load_idt(uint32_t offset, uint16_t size);
extern void test_handler_asm(void);

static IdtEntry IDT[256];

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04    /* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08	/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C	/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

void remap_pic(int offset1, int offset2)
{
	unsigned char a1, a2;

	a1 = inbyte(PIC1_DATA);                        // save masks
	a2 = inbyte(PIC2_DATA);

	outbyte(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outbyte(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outbyte(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outbyte(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outbyte(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outbyte(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();

	outbyte(PIC1_DATA, ICW4_8086);
	io_wait();
	outbyte(PIC2_DATA, ICW4_8086);
	io_wait();

	outbyte(PIC1_DATA, a1);   // restore saved masks.
	outbyte(PIC2_DATA, a2);
}

static void idt_set_gate(uint8_t index, uint32_t base, uint16_t segment, uint8_t flags) {
    IDT[index].base_lo = base & 0xFFFF;
    IDT[index].base_hi = (base >> 16) & 0xFFFF;

    IDT[index].segment = segment;
    IDT[index].reserved = 0;
    IDT[index].reserved = flags | 0x60;
}

void idt_init() {
    // remap_pic(49, 49+7);

    uint8_t flags = 0;

    

    idt_set_gate(49, (uint32_t)test_handler_asm, 0x08, 0x8E);

    load_idt((uint32_t)IDT, sizeof(IDT));

    serial_writestring("bp6\n\r");

    __asm__ volatile("int $49");

    serial_writestring("bp7\n\r");
}

#define PIC_EOI		0x20		/* End-of-interrupt command code */

void PIC_sendEOI(unsigned char irq)
{
	if(irq >= 8)
		outbyte(PIC2_COMMAND,PIC_EOI);

	outbyte(PIC1_COMMAND,PIC_EOI);
}

void IRQ_set_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inbyte(port) | (1 << IRQline);
    outbyte(port, value);        
}

void IRQ_clear_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inbyte(port) & ~(1 << IRQline);
    outbyte(port, value);        
}

void test_handler_c(void) {
    serial_writestring("int 49 OK\n\r");
    PIC_sendEOI(49);
}

void gp_fault_c(void) {
    __asm__ volatile("cli");

    for (size_t i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++)
    {
        terminal_buffer[i] = vga_entry(' ', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED));
        terminal_column = 0;
        terminal_row = 0;
    }

    terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED);

    terminal_writestring("WARNING\n");
    terminal_writestring("GENERAL PROTECTION FAULT : EMERGENCY RECOVERY MODE ACTIVE\n\n");
    
    terminal_writestring("The CPU has raised a general protection fault.\n");
    terminal_writestring("::Emergency recovery mode activated\n");
    terminal_writestring("--------------------------------------------------------------------------------\n\n");

    terminal_writestring("This system will restart IMMEDIATELY.\n");
    terminal_writestring("This may have any number of causes, from a program attempting to access kernel\nmemory or something more malicious.");
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