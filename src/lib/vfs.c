#ifndef LIB_HACKNET_VFS
#define LIB_HACKNET_VFS

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "syscall.c"
#include "../drivers/ata/ata.c"

enum FileType {
    FTFile      = 0,
    FTDir       = 1,
    FTLink      = 2,
    FTMount     = 3
};

typedef struct _File {
    uint8_t type;

    struct _File *parent;

    char *name;

    uint32_t created;
    uint16_t permissions;

    errno_t (*Read)(char *buffer, struct _File file);
    errno_t (*Write)(char *buffer, struct _File file);
    // errno_t (*ReadDir)(DirEntry *buffer, struct _File file);
} File;

#endif