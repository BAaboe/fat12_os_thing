#include <stdint.h>

#include "print.h"
#include "util.h"

void debug_print(char *string) {
    while (*string != 0) {
        outb(0xe9, *string++);
    }
}

void kernel() {
    init_screen();
    print_string("BenjiOS", 0x0a);
}
