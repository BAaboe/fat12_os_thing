#include <stdint.h>

#include "drivers/floppy.h"
#include "idt/idt.h"
#include "mm.h"
#include "pic/pic.h"
#include "print.h"
#include "util.h"

void setup() {
    setUpIDT();
    PIC_remap(32, 40);
    // PIC_disable();
    __asm__ __volatile__("sti");
    init_screen();
    // print_string("Init floppy disk\n", 0x0e);
    // floppy_init();
}

void kernel() {
    setup();
    mm_t mm = get_mm();
    print_string("Num of memory map entries: ", 0x0e);
    print_int(mm.len, 0x0e);
    size_t mem_size = 0;
    for (int i = 0; i < mm.len; i++) {
        print_string("\n\n", 0x0e);
        print_string("Base address: ", 0x0e);
        print_hex(mm.entries[i].base_addr, 0x0e);
        print_string(", Size: ", 0x0e);
        if (mm.entries[i].len / 1024 > 1024) {
            print_int(mm.entries[i].len / 1024 / 1024, 0x0e);
            print_string(" MiB", 0x0e);

        } else {
            print_int(mm.entries[i].len / 1024, 0x0e);
            print_string(" KiB", 0x0e);
        }
        print_string(", Type: ", 0x0e);
        print_int(mm.entries[i].type, 0x0e);
        if (mm.entries[i].type == MM_TYPE_USABLE) {
            mem_size += mm.entries[i].len;
        }
    }

    print_string("\n\n", 0x0e);
    print_string("Total memory size: ", 0x0e);
    print_int(mem_size / 1024 / 1024, 0x0e);
    print_string("MiB", 0x0e);

    // print_string("Reading from disk\n", 0x0e);
    // floppy_read(1, 0x20000, 1, 0);
    // print_string("Read from disk\n", 0x0e);

    while (1) {
    }
}
