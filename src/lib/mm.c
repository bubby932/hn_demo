#ifndef LIB_HACKNET_MM
#define LIB_HACKNET_MM

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "kutils.c"

/// @brief Used to define a single memory allocation in the list.
typedef struct {
    // Allocation Metadata
    size_t length;
    bool free;

    // Linked List metadata
    Allocation *previous;
    Allocation *next;

    // The actual allocation
    void *base;
} Allocation;

static const size_t HACKNET_KHEAP_SIZE = 256000;

static uint8_t kheap[HACKNET_KHEAP_SIZE];

void *kmalloc(size_t size) {
    Allocation *a = find_first_minimal_entry(size);

    // TODO if no entries are found of suitable size, merge all contiguous entries and try again
    if(a == NULL) return NULL;

    if(a->length > size * 2) {
        // subdivide if larger??

        kpanic("fuck you");
    }

    a->free = false;
    return a;
}

void kfree(void *ptr) {
    kpanic("use of unimplemented kfree(void*) call");
}

void merge_all_contiguous() {
    kpanic("unimplemented");
}

Allocation *find_first_minimal_entry(size_t min_size) {
    Allocation *base = (Allocation *)kheap;
    if(base->free && base->length >= min_size + 1) return base;

    Allocation *current = base->next;
    while(true) {
        if(current == NULL)
            return NULL;

        if(current->free && current->length >= min_size + 1)
            return current;

        if(current->next == NULL)
            return NULL;

        current = current->next;
    }
}

#endif