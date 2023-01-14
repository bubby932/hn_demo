#ifndef LIB_HACKNET_SYSCALL
#define LIB_HACKNET_SYSCALL

#include <stdint.h>

#include "kutils.c"
#include "irq.c"

typedef uint16_t errno_t;

enum ERRNO {
    /// @brief Success.
    EOK = 0,
    /// @brief Invalid Operation
    EINVOP = 1
};

errno_t syscall(
    uint32_t eax,
    uint32_t ebx,
    uint32_t ecx,
    uint32_t edx,
    uint32_t esi,
    uint32_t edi,
    uint32_t ebp
) {
    kpanic("[syscall] Unimplemented!\n");
    eoi(0x80);
}

#endif