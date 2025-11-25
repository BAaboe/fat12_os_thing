#include "print.h"
#include "drivers/vga.h"
#include "font.h"
#include "util.h"

#include <stdint.h>

#define VGA_MEMORY 0xb8000
#define CHARACTERS_PER_LINE WIDTH / 8
#define LINES HEIGHT / 8

void put_char(uint8_t c, int x, int y, uint8_t color) {
    const uint8_t *bitmap;
    bitmap = FONT[c];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (bitmap[i] & (1 << j)) {
                vga_set_pixel(color, x + j, y + i);
            }
        }
    }
}

void print_string(char *string, uint8_t color, int x, int y) {
    int cx = x;
    int cy = y;
    while (*string != 0) {
        if (*string == '\n') {
            cy += 8;
            cx = x;
            string++;
            continue;
        }
        put_char(*(string++), cx, cy, color);
        cx += 8;
    }
    vga_flip();
}

