#include <util.h>

void debug_print(char *string) {
    while (*string != 0) {
        outb(0xe9, *string++);
    }
}
