#pragma once

#include <stddef.h>
#include <stdint.h>

#define VGA_RED 4
#define VGA_BLACK 0
#define VGA_RED_ON_BLACK (VGA_RED | (VGA_BLACK << 4))
#define VGA_BLACK_ON_RED (VGA_BLACK | (VGA_RED << 4))
#define VGA_ENTRY(c, color) ((uint16_t)c | (uint16_t)color << 8)

#define ASSERT(x) if (!(x)) panic("Assertion failed")
#define ASSERT(x, msg) if (!(x)) panic(msg)

static inline void spin(size_t cycles) {
	for (size_t i = 0; i < cycles; i++)
		__asm__ volatile("nop");
}

void panic(const char *msg) {
	__asm__ volatile("cli");

	serial_writestring("[Panic] ");
	serial_writestring(msg);

	while (true) {
		const uint16_t *last = (uint16_t *)0xB8000 + (80*25);
		for (uint16_t *i = (uint16_t *)0xB8000; i < last; i++)
			*i = VGA_ENTRY('X', VGA_RED_ON_BLACK);

		spin(5000000000);

		for (uint16_t *i = (uint16_t *)0xB8000; i < last; i++)
			*i = VGA_ENTRY('X', VGA_BLACK_ON_RED);

		spin(5000000000);
	}
}