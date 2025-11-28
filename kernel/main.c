#include <stdint.h>

#include "drivers/floppy.h"
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
    init_screen();
    print_string("Hello, Wolrd!\n", 0x0e);

    print_string("Init disk\n", 0x0e);
    floppy_init();
    print_string("Reading from disk\n", 0x0e);
    floppy_read(0, 0x20000, 512, 0);
    uint8_t *addr = (uint8_t *)0x20000;
    for (int i = 0; i < 512; i++) {
        outb(0xe9, addr[i]);
    }
    print_string("Read from disk\n", 0x0e);

    while (1) {
    }
}
