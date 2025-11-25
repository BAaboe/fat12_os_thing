#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>

void put_char(uint8_t c, int x, int y, uint8_t color);
void print_string(char *string, uint8_t color, int x, int y);

#endif
