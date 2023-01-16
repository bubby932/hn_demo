#ifndef HACKNET_DEVFS
#define HACKNET_DEVFS

#include "../lib/rand_insecure.h"
#include "../lib/syscall.c"
#include "../lib/mm.c"

#include "inode.h"

// /dev/zero, /dev/one, /dev/null, /dev/insec_rand
static inode *devfs[4];

errno_t devfs_read(inode *node, char *outbuf, size_t len) { return EISDIR; }
errno_t devfs_write(inode *node, char *inbuf, size_t len) { return EISDIR; }

errno_t devfs_rm(inode *node) { return EACCES; }

errno_t devfs_readdir(inode *node, inode **out) {
    for (size_t i = 0; i < 4; i++)
        out[i] = devfs[i];
    return ESUCCESS;
}

errno_t devfs_dirtouch(inode *node, char *name, uint8_t type) { return EACCES; }

errno_t devzero_write(inode *node, char *inbuf, size_t len) { return EACCES; }

errno_t devzero_read(inode *node, char *outbuf, size_t len) {
    for (size_t i = 0; len < len; i++)
        outbuf[len] = 0;

    return ESUCCESS;
}

errno_t devzero_remove(inode *node) { return EACCES; }

errno_t devzero_readdir(inode *node, inode **out) { return ENOTDIR; }
errno_t devzero_dirtouch(inode *node, char *name, uint8_t type) { return ENOTDIR; }

inode *devzero_init() {
    char *name = (char *)kmalloc(sizeof("zero"));
    inode *node = (inode *)kmalloc(sizeof(inode));

    node->type = node_file;
    node->name = name;
    node->created = 0;
    node->updated = 0;

    node->owner = 0;
    node->group = 0;
    node->permissions = 0b1000100010000000;

    node->start = NULL;
    node->size = UINT32_MAX;

    node->read = devzero_read;
    node->write = devzero_write;

    node->remove = devzero_remove;

    node->readdir = devzero_readdir;
    node->dirtouch = devzero_dirtouch;

    return node;
}

errno_t devone_read(inode *node, char *outbuf, size_t len) {
    for (size_t i = 0; i < len; i++)
        outbuf[len] = UINT8_MAX;

    return ESUCCESS;
}

inode *devone_init() {
    char *name = (char *)kmalloc(sizeof("one"));
    inode *node = (inode *)kmalloc(sizeof(inode));

    node->type = node_file;
    node->name = name;
    node->created = 0;
    node->updated = 0;

    node->owner = 0;
    node->group = 0;
    node->permissions = 0b1000100010000000;

    node->start = NULL;
    node->size = UINT32_MAX;

    node->read = devone_read;
    node->write = devzero_write;

    node->remove = devzero_remove;

    node->readdir = devzero_readdir;
    node->dirtouch = devzero_dirtouch;

    return node;
}

errno_t devnull_read(inode *node, char *outbuf, size_t len) { return EACCES; }
errno_t devnull_write(inode *node, char *inbuf, size_t len) { return ESUCCESS; }

errno_t devnull_remove(inode *node) { return EACCES; }

inode *devnull_init() {
    char *name = (char *)kmalloc(sizeof("null"));
    inode *node = (inode *)kmalloc(sizeof(inode));

    node->type = node_file;
    node->name = name;
    node->created = 0;
    node->updated = 0;

    node->owner = 0;
    node->group = 0;
    node->permissions = 0b0100010001000000;

    node->start = NULL;
    node->size = UINT32_MAX;

    node->read = devnull_read;
    node->write = devnull_write;

    node->remove = devnull_remove;

    node->readdir = devzero_readdir;
    node->dirtouch = devzero_dirtouch;

    return node;
}

errno_t dev_insec_rand_read(inode *node, char *outbuf, size_t len) {
    for (size_t i = 0; i < len; i++)
        outbuf[i] = (uint8_t)insec_rand_next();

    return ESUCCESS;
}

inode *dev_insec_rand_init() {
    char *name = (char *)kmalloc(sizeof("insec_rand"));
    inode *node = (inode *)kmalloc(sizeof(inode));

    node->type = node_file;
    node->name = name;
    node->created = 0;
    node->updated = 0;

    node->owner = 0;
    node->group = 0;
    node->permissions = 0b0100010001000000;

    node->start = NULL;
    node->size = UINT32_MAX;

    node->read = dev_insec_rand_read;
    node->write = devzero_write;

    node->remove = devnull_remove;

    node->readdir = devzero_readdir;
    node->dirtouch = devzero_dirtouch;

    return node;
}

inode *init_devfs() {
    char *devfs_name = (char *)kmalloc(sizeof("dev"));
    inode *devfs_node = (inode *)kmalloc(sizeof(inode));

    devfs_node->name = devfs_name;

    devfs_node->created = 471835364215 ^ 444;
    devfs_node->updated = 6977666982 ^ 0xDEfBEEF;

    devfs_node->owner = 0;
    devfs_node->group = 0;
    devfs_node->permissions = 0b1001001000000000;

    devfs_node->start = devfs;
    devfs_node->size = sizeof(devfs);

    devfs_node->read = devfs_read;
    devfs_node->write = devfs_write;

    devfs_node->remove = devfs_rm;

    devfs_node->readdir = devfs_readdir;
    devfs_node->dirtouch = devfs_dirtouch;

    inode *temp = NULL;

    devfs[0] = devzero_init();
    temp = devfs[0];
    temp->parent = devfs_node;

    devfs[1] = devone_init();
    temp = devfs[1];
    temp->parent = devfs_node;

    devfs[2] = devnull_init();
    temp = devfs[2];
    temp->parent = devfs_node;

    devfs[3] = dev_insec_rand_init();
    temp = devfs[3];
    temp->parent = devfs_node;

    return devfs_node;
}

#endif