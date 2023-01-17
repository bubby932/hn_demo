#ifndef RESCUE_STRING
#define RESCUE_STRING

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/mm.c"

typedef struct _string {
    char *buffer;
    size_t length;
    size_t capacity;
} String;

String *new_string(size_t capacity) {
    String *str = (String *)kmalloc(sizeof(String));
    if(str == NULL)
        return NULL;
    char *buf = (char *)kmalloc(capacity);
    if(buf == NULL)
        return NULL;

    str->capacity = capacity;
    str->length = 0;
    str->buffer = buf;
    return str;
}

bool ensure_capacity(String *str, size_t capacity) {
    if(str->capacity <= capacity)
        return true;

    size_t cap = str->capacity;
    while(cap < capacity)
        cap += 64;

    char *new_buffer = kmalloc(cap);
    if(new_buffer == NULL)
        return false;

    if(str->buffer != NULL) {
        memcpy(str->buffer, new_buffer, str->length);
        kfree(str->buffer);
    }

    str->buffer = new_buffer;
    str->capacity = cap;
    return true;
}

bool string_pushc(String *str, char c) {
    if(!ensure_capacity(str, str->capacity + 1))
        return false;

    str->buffer[str->length] = c;
    str->length++;
    str->buffer[str->length] = '\0';

    return true;
}

bool string_pushcstr(String *str, char* copy, size_t len) {
    if(!ensure_capacity(str, str->length + len))
        return false;

    memcpy(str->buffer + str->length, copy, len);
    str->length += len;
    str->buffer[str->length] = '\0';

    return true;
}

bool string_setc(String *str, size_t position, char c) {
    if(position >= str->length)
        return false;

    str->buffer[position] = c;
    return true;
}

bool string_insertc(String *str, size_t position, char c) {
    if(!ensure_capacity(str, str->capacity + 1))
        return false;

    memcpy(str->buffer + position, str->buffer + position + 1, str->length - position);
    str->buffer[position] = c;
    str->length++;
    str->buffer[str->length] = '\0';

    return true;
}

size_t last_newline_from(String *str, size_t position) {
    while(position != 0 && str->buffer[position] != '\n')
        position--;

    return position;
}

size_t next_newline_from(String *str, size_t position) {
    while(position < str->length && str->buffer[position] != '\n')
        position++;

    return position;
}

void string_clear(String *str) {
    memset(str->buffer, '\0', str->length);
    str->length = 0;
}

#endif