#include <stdint.h>

#include "idt/idt.h"
#include "pic/pic.h"
#include "print.h"
#include "util.h"

void debug_print(char *string) {
    while (*string != 0) {
        outb(0xe9, *string++);
    }
}

void setup() {
    setUpIDT();
    PIC_remap(32, 40);
    // PIC_disable();
    __asm__ __volatile__("sti");
}

void kernel() {
    setup();
    init_screen();
    print_string("Hello, Wolrd!", 0x0e);

    while (1) {
    }
}
