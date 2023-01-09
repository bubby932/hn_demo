#ifndef LIB_HACKNET_MM
#define LIB_HACKNET_MM

#define HACKNET_MM_DEBUG true

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "kutils.c"
#include "string.c"

/// @brief Used to define a single memory allocation in the list.
typedef struct _Allocation {
    // Allocation Metadata
    size_t length;
    bool free;

    // Linked List metadata
    struct _Allocation *previous;
    struct _Allocation *next;

    // The actual allocation
    void *base;
} Allocation;

#define HACKNET_KHEAP_SIZE 256000

static uint8_t kheap[HACKNET_KHEAP_SIZE];

/// @brief Iniitalizes the kernel heap to a static size of HACKNET_KHEAP_SIZE.
void kheap_init() {
#if HACKNET_MM_DEBUG
    debug_terminal_writestring("[MM_DEBUG] kheap init internal\n");
#endif

    Allocation *base = (Allocation *)kheap;

    base->base = kheap + sizeof(Allocation);
    base->free = true;
    base->length = HACKNET_KHEAP_SIZE - sizeof(Allocation);
    base->previous = NULL;
    base->next = NULL;
}

/// @brief Merges all contiguous free allocation blocks into larger ones for faster traversal & general defragmentation.
static void merge_all_contiguous() {
#if HACKNET_MM_DEBUG
    debug_terminal_writestring("[MM_DEBUG] defragmenting kheap\n");
#endif

    Allocation *current = (Allocation *)kheap;

    while(true) {
        if(current->next == NULL)
            return;

        Allocation *next = current->next;

        if(current->free && next->free) {
            current->length = current->length + (sizeof(Allocation) + next->length - 1);
            current->next = next->next;
        }

        current = current->next;
    }
}

/// @brief Locates the first Allocation equal or larger than min_size bytes.
/// @param min_size The minimum (inclusive) size in bytes the allocation must meet.
/// @return The first viable free allocation, or NULL if one is not found.
static Allocation *find_first_minimal_entry(size_t min_size) {
#if HACKNET_MM_DEBUG
    debug_terminal_writestring("[MM_DEBUG] finding first viable alloc block\n");
#endif

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

/// @brief Allocates a block of kernel memory of `size` bytes or larger.
/// @param size The minimum size -in bytes- of memory that must be allocated.
/// @return A pointer to the beginning of the block, or NULL if the memory cannot be allocated.
void *kmalloc(size_t size) {
#if HACKNET_MM_DEBUG
    debug_terminal_writestring("[MM_DEBUG] kernel heap allocation!\n");
#endif

    Allocation *a = find_first_minimal_entry(size);

    if(a == NULL) {
#if HACKNET_MM_DEBUG
        debug_terminal_writestring("[MM_DEBUG] alloc failed, defragmenting and retrying");
#endif
        // No allocations of proper size, defrag and try again

        merge_all_contiguous();
        a = find_first_minimal_entry(size);

        // welp, we tried
        if(a == NULL)
            return NULL;
    }

    if(a->length > size + sizeof(Allocation)) {
#if HACKNET_MM_DEBUG
        debug_terminal_writestring("[MM_DEBUG] alloc success, finishing up\n");
#endif
        // Subdivide to minimal size.
        Allocation *b = (a->base + size);

        b->previous = a;
        b->next = a->next;
        b->length = a->length - (size + sizeof(Allocation));
        b->base = b + sizeof(Allocation);
        b->free = true;

        a->length = size;
        a->next = b;
    }

    a->free = false;
    return a->base;
}

void kfree(void *ptr) {
#if HACKNET_MM_DEBUG
    debug_terminal_writestring("[MM_DEBUG] freeing ptr in kheap\n");
#endif
    Allocation *base = (Allocation *)kheap;

    if(base->base == ptr) {
        base->free = true;
        merge_all_contiguous();
        return;
    }

    Allocation *current = base->next;
    while(true) {
        if(current == NULL)
            kpanic("Free of undefined allocation. Double free w/ merge?");

        if(current->base == ptr) {
            if(current->free)
                kpanic("Double free!");

            current->free = true;
            merge_all_contiguous();
            return;
        }

        current = current->next;
    }
}
#endif