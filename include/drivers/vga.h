#ifndef VGA_H
#define VGA_H
#include <stdint.h>

#define VRAM 0xA0000

#define HEIGHT 200   // In pixels
#define WIDTH 320    // In pixels
#define PITCH 320    // In bytes
#define DEPTH 8      // In bits
#define PIXELWIDTH 1 // In bytes

void vga_init();
void vga_fill_screen(uint8_t color);
void vga_set_pixel(uint8_t color, int x, int y);
void vga_flip();

#endif
