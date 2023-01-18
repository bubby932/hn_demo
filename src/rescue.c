#ifndef HACKNET_RESCUE_MODE
#define HACKNET_RESCUE_MODE

#include "stdbool.h"
#include "stdint.h"

#include "lib/syscall.c"
#include "lib/string.c"

#include "rescue_str.c"

typedef uint16_t errno_t;

static char *header = "                    HackNet rescue t-edit v0.0.1                          ";

#define KEY_NULL 0

#define KEY_CTRL_P 1
#define KEY_LSHIFT_P 2
#define KEY_RSHIFT_P 10

#define KEY_CAPSLOCK 3

#define ARROW_UP 6
#define ARROW_DOWN 7
#define ARROW_RIGHT 8
#define ARROW_LEFT 9

static bool ctrl = false;
static bool caps_lock = false;

static char keymap_lower[256] = {
    KEY_NULL, KEY_NULL, '1', '2',
    '3', '4', '5', '6',
    '7', '8', '9', '0',
    '-', '=',' \b', '\t',
    'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i',
    'o', 'p', '[', ']',
    '\n', KEY_CTRL_P, 'a', 's',
    'd', 'f', 'g', 'h',
    'j', 'k', 'l', ';',
    '\'', '`', KEY_LSHIFT_P, '\\',
    'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',',
    '.', '/', KEY_RSHIFT_P, '*',
    KEY_CTRL_P, ' ', KEY_CAPSLOCK, KEY_NULL,
    KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
    KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
    KEY_NULL, KEY_NULL, KEY_NULL, '7',
    '8', '9', '-', '4',
    '5', '6', '+', '1',
    '2', '3', '0', '.'
};
static char keymap_upper[256] = {
    KEY_NULL, KEY_NULL, '!', '@',
    '#', '$', '%', '^',
    '&', '*', '(', ')',
    '_', '+',' \b', KEY_NULL,
    'Q', 'W', 'E', 'R',
    'T', 'Y', 'U', 'I',
    'O', 'P', '{', '}',
    '\n', KEY_CTRL_P, 'A', 'S',
    'D', 'F', 'G', 'H',
    'J', 'K', 'L', ':',
    '"', '~', KEY_LSHIFT_P, '|',
    'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<',
    '>', '?', KEY_RSHIFT_P, '*',
    KEY_CTRL_P, ' ', KEY_CAPSLOCK, KEY_NULL,
    KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
    KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
    KEY_NULL, KEY_NULL, KEY_NULL, '7',
    '8', '9', '-', '4',
    '5', '6', '+', '1',
    '2', '3', '0', '.'
};

static String *backbuffer;
static size_t cursor;

void render_header() {
    terminal_buffer[0] = vga_entry('%', ctrl ? vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK) : vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_WHITE));

    terminal_buffer[1] = vga_entry(' ', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_WHITE));

    terminal_buffer[2] = vga_entry('C', caps_lock ? vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK) : vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_WHITE));
    terminal_buffer[3] = vga_entry('L', caps_lock ? vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK) : vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_WHITE));

    // SACL
    for (size_t i = 4; i < VGA_WIDTH; i++)
        terminal_buffer[i] = vga_entry(header[i - 5], vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE));
}


void render_buffer() {
    size_t start_index = last_newline_from(backbuffer, cursor);

    size_t column = 0;
    size_t row = 1;

    size_t str_index = start_index;

    while(row < VGA_HEIGHT && str_index < backbuffer->length) {
        if(column == VGA_WIDTH) {
            column = 0;
            row++;
            if(row == VGA_HEIGHT)
                break;
            str_index = next_newline_from(backbuffer, str_index);
            continue;
        }
        char c = backbuffer->buffer[str_index];
        size_t vga_buffer_index = row * VGA_WIDTH + column;

        if(c == '\n') {
            row++;
            column = 0;
            str_index++;
            continue;
        }

        terminal_buffer[vga_buffer_index] = vga_entry(
            backbuffer->buffer[str_index],
            str_index == cursor ?
                vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY) :
                vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK)
        );
        column++;
        str_index++;
    }
}

/// @brief Super simple kernel rescue module, used for note taking, debugging, and basic file manipulation.
/// @return Exit code.
errno_t rescue() {
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        terminal_buffer[i] = vga_entry(' ', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));

    render_header();

    backbuffer = new_string(VGA_WIDTH * VGA_HEIGHT);
    if(backbuffer == NULL || backbuffer->buffer == NULL)
        kpanic("[RESCUE] Backbuffer allocation failed!\n");
    serial_writestring("bp1\n\r");

    string_pushcstr(backbuffer, "Welcome to [test doc]!", 22);
    serial_writestring("bp2\n\r");

    render_buffer();
    serial_writestring("bp3\n\r");

    terminal_column = 0;
    terminal_row = 1;

    while(true)
        asm volatile("hlt");
}

void writec(char c) {
    if(c == '\b') {
        if(cursor < 1)
            return;
        backbuffer->buffer[--cursor] = '\0';
        render_buffer();
        return;
    }

    if(!string_insertc(backbuffer, cursor, c))
        kpanic("OOM in Rescue Mode!\n");
    cursor++;
    render_buffer();
}

void shortcut(uint8_t scan) {
    ctrl = false;
    render_header();

    switch(keymap_lower[scan]) {
        case 'j': {
            cursor--;
            render_buffer();
            return;
        }
        case 'l': {
            cursor++;
            render_buffer();
            return;
        }
        case '0': {
            cursor = 0;
            render_buffer();
            return;
        }
        case '9': {
            cursor = backbuffer->length;
            render_buffer();
            return;
        }
        default:
            return;
    }
}

void putc(uint8_t scan) {
    if(ctrl) {
        shortcut(scan);
        ctrl = false;
        render_header();
        return;
    }

    if (caps_lock) {
        writec(keymap_upper[scan]);
    } else {
        writec(keymap_lower[scan]);
    }
}

void rescue_keypress(uint8_t scancode) {
    switch(keymap_lower[scancode]) {
        case KEY_LSHIFT_P: {
            if(cursor > 0)
                cursor--;
            render_buffer();
            break;
        }
        case KEY_RSHIFT_P: {
            if(cursor < backbuffer->length)
                cursor++;
            render_buffer();
            break;
        }
        case KEY_CTRL_P: {
            ctrl = !ctrl;
            render_header();
            break;
        }
        case KEY_CAPSLOCK: {
            caps_lock = !caps_lock;
            render_header();
            break;
        }
        case KEY_NULL:
            break;

        default: {
            putc(scancode);
            break;
        }
    }
}

#endif