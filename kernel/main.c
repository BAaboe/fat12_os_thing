#include <stdint.h>

#include "idt/idt.h"
#include "pic.h"
#include "print.h"
#include "util.h"

void debug_print(char *string) {
    while (*string != 0) {
        outb(0xe9, *string++);
    }
}

void setup() {
    setUpIDT();
    PIC_remap(22, 28);
    PIC_disable();
    __asm__ __volatile__("sti");
}

void kernel() {
    setup();
    init_screen();
    int a = 1 / 0;
}
