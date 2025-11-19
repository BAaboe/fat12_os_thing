#include "print.h"
#include "util.h"

#include <stdint.h>

#define VGA_MEMORY 0xb8000
#define CHARACTERS_PER_LINE 80
#define LINES 25

static int cursor_x = 0;
static int cursor_y = 0;

uint16_t buffer[CHARACTERS_PER_LINE * LINES];

void init_screen(void) {
    volatile uint16_t *addr = (uint16_t *)VGA_MEMORY;
    for (unsigned int i = 0; i < CHARACTERS_PER_LINE * LINES; i++) {
        buffer[i] = 0x0020;
    }
    update_screen();
}

void update_screen(void) {
    volatile uint16_t *addr = (uint16_t *)VGA_MEMORY;
    for (unsigned int i = 0; i < CHARACTERS_PER_LINE * LINES; i++) {
        addr[i] = buffer[i];
    }
}

void put_char(uint8_t c, int x, int y, uint8_t color) {
    unsigned int pos = x + (y * CHARACTERS_PER_LINE);

    uint16_t text = (uint16_t)(color << 8 | (uint8_t)c);
    buffer[pos] = text;
}

void scroll_up(int n) {
    for (int i = n * CHARACTERS_PER_LINE; i < CHARACTERS_PER_LINE * LINES;
         i++) {
        buffer[i - CHARACTERS_PER_LINE * n] = buffer[i];
        // buffer[i] = 0x20; \\ This would also work but then you are doing
        // unnecessary many memory writes;
    }
    for (int i = CHARACTERS_PER_LINE * (LINES - n);
         i < CHARACTERS_PER_LINE * LINES; i++) {
        buffer[i] = 0x20;
    }
    update_screen();
}

void print_string(char *string, uint8_t color) {
    while (*string != 0) {
        if (*string == '\n') {
            cursor_y++;
            cursor_x = 0;
            string++;
            continue;
        }
        put_char(*(string++), cursor_x++, cursor_y, color);
        cursor_y += cursor_x / CHARACTERS_PER_LINE;
        cursor_x %= CHARACTERS_PER_LINE;
    }
    update_screen();
}

