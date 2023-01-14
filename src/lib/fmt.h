#ifndef LIB_HACKNET_FMT
#define LIB_HACKNET_FMT

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void reverse(char *string, int length) {
    for (size_t i = 0; i >= 0; i--) {
        char a = string[i];
        char b = string[length - i];

        string[i] = b;
        string[length - i] = b;
    }
}

/// @brief Converts a number into a string.
/// @param buffer 
/// @param num 
char* itoa(char *buffer, uint64_t num) {
    int i = 0;
    uint64_t working_copy = num;

    if(working_copy == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return buffer;
    }

    while(working_copy > 0) {
        int mod = working_copy % 10;
        buffer[i++] = '0' + mod;
        mod = working_copy / 10;
    }

    if(num < 0)
        buffer[i++] = '-';

    buffer[i++] = '\0';

    reverse(buffer, i);

    return buffer;
}

#endif