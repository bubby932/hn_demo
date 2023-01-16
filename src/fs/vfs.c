#ifndef HACKNET_VFS
#define HACKNET_VFS

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../lib/syscall.c"
#include "../lib/mm.c"

#include "inode.h"
#include "devfs.c"

errno_t default_read(inode *node, char *outbuf, size_t len) {
    return EINVAL;
}

errno_t default_write(inode *node, char *outbuf, size_t len) {
    return EINVAL;
}

errno_t default_remove(inode *node) {
    if(node->type == node_dir && node->start != NULL) {
        inode **children = (inode **)node->start;
        for (size_t i = 0; i < node->size / sizeof(inode); i++) {
            errno_t t = children[i]->remove(children[i]);
            if(t)
                return t;
        }
    }

    if(node->parent != NULL) {
        inode *parent = node->parent;
        inode **old_ptr = (inode **)parent->start;

        size_t new_size = parent->size - sizeof(inode);
        inode **new_ptr = (inode **)kmalloc(new_size);

        if(new_ptr == NULL)
            return ENOMEM;
            

        for (size_t i = 0; i < new_size / sizeof(inode); i++) {
            if(old_ptr[i] == node) {
                i--;
                continue;
            }
            new_ptr[i] = old_ptr[i];
        }

        kfree(old_ptr);
        parent->start = (void *)new_ptr;
    }

    return ESUCCESS;
}

errno_t default_readdir(inode *node, inode **out) {
    if(node == NULL)
        return ENOENT;

    if(node->type != node_dir)
        return ENOTDIR;

    memcpy(node->start, (void *)out, node->size);

    return ESUCCESS;
}

errno_t default_dirtouch(inode *base, char *name, uint8_t type) {
    if(name == NULL)
        return EINVAL;

    if(base == NULL)
        return ENOENT;

    inode *node = (inode *)kmalloc(sizeof(inode));
    if(node == NULL)
        return ENOMEM;

    node->type = type;
    node->name = name;

    node->parent = base;

    node->created = 0;
    node->updated = 0;

    node->owner = 0;
    node->group = 0;
    node->permissions = 0b110110110000;

    node->start = NULL;
    node->size = 0;

    node->read = default_read;
    node->write = default_write;

    node->remove = default_remove;

    node->readdir = default_readdir;
    node->dirtouch = default_dirtouch;

    size_t new_size = base->size + sizeof(inode *);
    inode **new_ptr = (inode **)kmalloc(new_size);
    if(new_ptr == NULL)
        return ENOMEM;

    if(base->start != NULL) {
        memcpy(base->start, new_ptr, base->size);
        kfree(base->start);
    }

    base->start = (void *)new_ptr;
    new_ptr[base->size / sizeof(inode *)] = node;
    base->size = new_size;

    return ESUCCESS;
}

inode *vfs_sysroot;

inode *init_root() {
    char *name = (char *)kmalloc(sizeof(""));
    if(name == NULL)
        kpanic("[VFS] FAILED TO ALLOCATE 1 BYTE??? WHAT THE HELL IS YOUR MEMORY DOING???\n");

    inode *node = (inode *)kmalloc(sizeof(inode));
    if(node == NULL)
        kpanic("[VFS] Failed to allocate memory for root node, how little memory do you HAVE???\n");

    node->type = node_dir;

    node->name = name;

    node->parent = NULL;

    node->created = INT64_MIN;
    node->updated = INT64_MIN;

    node->owner = 0;
    node->group = 0;
    node->permissions = 0b1100110011000000;

    node->start = NULL;
    node->size = 0;

    node->read = default_read;
    node->write = default_write;

    node->remove = default_remove;

    node->readdir = default_readdir;
    node->dirtouch = default_dirtouch;

    return node;
}

errno_t append_file(inode *dir, inode *file) {
    if(dir == NULL || file == NULL)
        return ENOENT;

    if(dir->type != node_dir)
        return ENOTDIR;

    inode **neu = (inode **)kmalloc(dir->size + sizeof(inode *));

    if(neu == NULL)
        return ENOMEM;

    if(dir->start != NULL) {
        memcpy(dir->start, neu, dir->size);

        kfree(dir->start);
    }

    dir->start = neu;
    neu[dir->size / sizeof(inode)] = file;
    dir->size += sizeof(inode);
    return ESUCCESS;
}

errno_t init_vfs() {
    vfs_sysroot = init_root();
    if(vfs_sysroot == NULL)
        return ENOENT;

    inode *dev = (inode *)init_devfs();
    if(dev == NULL)
        return ENOENT;

    errno_t mnt_err = append_file(vfs_sysroot, dev);
    if(mnt_err)
        return mnt_err;

    return ESUCCESS;
}

#endif