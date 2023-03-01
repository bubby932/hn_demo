#pragma once

#include <stddef.h>
#include <stdint.h>

namespace VgaText
{
	void clear_screen(char character, uint8_t color)
	{
		const uint16_t *base = (uint16_t *)0xB8000;
		const uint16_t *max = base + (80 * 25);

		for (uint16_t *i = (uint16_t *)base; i < max; i++)
			*i = (uint16_t)character | (uint16_t)color << 8;
	}
	void startup_screen(const char *message, size_t len, uint8_t color) {
		uint16_t *base = (uint16_t *)0xB8000;

		for (size_t i = 0; i < (80 * 25); i++)
			base[i] = i < len ? (uint16_t)message[i] | (uint16_t)color << 8 : (uint16_t)' ' | (uint16_t)color << 8;
	}
} // namespace VgaText
