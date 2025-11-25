#include <stdint.h>

#include "drivers/vga.h"
#include "util.h"

static uint8_t buffer[HEIGHT * PITCH];

void vga_init() {
    for (int i = 0; i < HEIGHT * PITCH; i++) {
        buffer[i] = 0x00;
    }
}
void vga_fill_screen(uint8_t color) {
    for (int i = 0; i < HEIGHT * PITCH; i++) {
        buffer[i] = color;
    }
}
void vga_set_pixel(uint8_t color, int x, int y) {
    buffer[x + y * PITCH] = color;
}
void vga_flip() {
    uint8_t *mem = (uint8_t *)VRAM;
    for (int i = 0; i < HEIGHT * PITCH; i++) {
        mem[i] = buffer[i];
    }
}
