#include "pic/pic.h"
#include "print.h"
#include "time.h"
#include "util.h"
#include <stdint.h>

#define PIC_EOI 0x20

void pic_handler(uint16_t num) {
    switch (num) {
    case 0:
        irq_tick();
        break;
    case 1:
        print_string("Keyboard pressed", 0x0a, 0, 0);
        inb(0x60);
        break;
    default:
        break;
    }

    if (num >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);

    return;
}
