#include <stdint.h>

#include "drivers/vga.h"
#include "idt/idt.h"
#include "pic/pic.h"
#include "print.h"
#include "util.h"

void setup() {
    setUpIDT();
    PIC_remap(32, 40);
    // PIC_disable();
    __asm__ __volatile__("sti");
}

void kernel() {
    setup();
    vga_init();
    vga_flip();
    print_string("Hello, World\nTest", 0x01, 0, 0);

    while (1) {
    }
}
