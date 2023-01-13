#ifndef HACKNET_UTIL_KBUFFER
#define HACKNET_UTIL_KBUFFER

#include <stdint.h>
#include <stdbool.h>

#include "../lib/mm.c"

#define KBUFFER_DEFAULT_THRESHOLD   0.75f
#define KBUFFER_DEFAULT_GROWTH      1.1f

typedef struct _KBuffer {
    float   growth_factor;
    float   growth_threshold;

    size_t  length;
    size_t  capacity;

    uint8_t *base;
} Buffer;

Buffer *kbuf_new_empty() {
    Buffer *kb = (Buffer *)kmalloc(sizeof(Buffer));

    kb->growth_factor = KBUFFER_DEFAULT_GROWTH;
    kb->growth_threshold = KBUFFER_DEFAULT_THRESHOLD;

    kb->length = 0;
    kb->capacity = 0;

    kb->base = NULL;

    return kb;
}

void kbuf_free(Buffer *kb) {
    if(kb->base != NULL)
        kfree(kb->base);

    kfree(kb);
}

bool kbuf_expand(Buffer *kb, size_t new_capacity) {
    uint8_t *new = (uint8_t *)kmalloc(new_capacity);
    
    if(new == NULL)     // OOM
        return false;

    memcpy(kb->base, new, kb->length);

    kfree(kb->base);

    kb->base = new;
    kb->capacity = new_capacity;

    return true;
}

bool kbuf_push(Buffer *kb, uint8_t byte) {
    if(kb->length + 1 / kb->capacity >= kb->growth_factor) {
        if(!kbuf_expand(kb, kb->length * kb->growth_factor))
            return false;
    }

    kb->base[kb->length++] = byte;

    return true;
}

uint8_t kbuf_pop(Buffer *kb) {
    if(kb->length < 1)
        return 0;

    
}

bool kbuf_truncate(Buffer *kb, size_t len) {
    if(kb->length < len)
        return false;

    
}

#endif