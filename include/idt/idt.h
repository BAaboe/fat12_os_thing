#ifndef IDT_H
#define IDT_H

#include "util.h"
#include <stddef.h>
#include <stdint.h>

#define IDTADDRESS 0x500

static uint8_t biggest_int = 0;

struct InterruptDescriptor {
    uint16_t offset_1;       // offset bits 0..15
    uint16_t selector;       // a code segment selector in GDT or LDT
    uint8_t zero;            // unused, set to 0
    uint8_t type_attributes; // gate type, dpl, and p fields
    uint16_t offset_2;       // offset bits 16..31
} __attribute__((packed));

struct IDTR {
    uint16_t size;
    uint32_t offset;
} __attribute((packed));

typedef struct InterruptDescriptor IDTEntry;

static inline void setIdtEntry(size_t interupt_number, uintptr_t offset,
                               uint16_t selector, uint8_t type, uint8_t DPL) {
    IDTEntry *entry = (IDTEntry *)(IDTADDRESS + interupt_number * 8);
    entry->offset_1 = (uint16_t)(offset & 0xffff);
    entry->offset_2 = (uint16_t)(offset >> 16);
    entry->selector = selector;

    entry->type_attributes = (1 << 7) | (DPL << 5) | (0 << 4) | (type & 0x0f);

    entry->zero = 0;

    if (interupt_number > biggest_int)
        biggest_int = interupt_number;
}

void setUpIDT();

#endif
