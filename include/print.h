#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>

void init_screen(void);
void update_screen(void);
void put_char(uint8_t c, int x, int y, uint8_t color);
void print_string(char *string, uint8_t color);
void print_int(uint64_t num, uint8_t color);
void print_hex(uint64_t num, uint8_t color);
void scroll_up(int n);

#endif
