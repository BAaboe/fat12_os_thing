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
    init_screen();
    print_string("Init floppy disk\n", 0x0e);
    floppy_init();
}

void kernel() {
    setup();
    print_string("Hello, Wolrd!\n", 0x0e);

    print_string("Reading from disk\n", 0x0e);
    floppy_read(1, 0x20000, 1, 0);
    print_string("Read from disk\n", 0x0e);

    while (1) {
    }
}
