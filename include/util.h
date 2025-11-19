// Copied from osdev, no reason to do work thats already done
#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

static inline void io_wait(void) {
    outb(0x80, 0);
}

#endif // !UTIL_h

