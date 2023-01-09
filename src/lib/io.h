#define outbyte(port, value) \
    __asm__ ("outb %%al, %%dx"::"a" (value), "d" (port))

#define inbyte(port) ({ \
        unsigned char _v; \
        __asm__ volatile ("inb %%dx, %%al":"=a" (_v):"d" (port)); \
        _v; \
    })