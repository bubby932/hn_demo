#ifndef RH_MUST_STDIO
#define MUST_LIBC_STDIO

#include <stddef.h>

void memset(void *, size_t, unsigned char);
void memcpy(void *, void *, size_t);

void terminal_initialize();
void terminal_writestring(const char *);

#endif