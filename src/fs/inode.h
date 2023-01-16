#ifndef HACKNET_INODE
#define HACKNET_INODE

#include <stdint.h>

#include "../lib/syscall.c"

enum node_type {
    node_file,
    node_dir,
    node_link,
    node_device
};

typedef struct _inode {
    // Identification
    uint8_t type;

    // Basics
    char *name;

    // Traversal
    struct _inode *parent;

    // Meta
    int64_t created;
    int64_t updated;
    
    // Permissions
    uint16_t owner;
    uint16_t group;
    uint16_t permissions;

    // Location
    void *start;
    size_t size;

    // Basic Ops
    errno_t (*read)(struct _inode *node, char *outbuf, size_t len);
    errno_t (*write)(struct _inode *node, char *inbuf, size_t len);

    // Deletion
    errno_t (*remove)(struct _inode *node);

    // Directories
    errno_t (*readdir)(struct _inode *node, struct _inode **out);
    errno_t (*dirtouch)(struct _inode *node, char *name, uint8_t type);
} inode;

#endif