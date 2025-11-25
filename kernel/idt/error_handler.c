#include "print.h"
#include <stdint.h>

typedef enum {
    DIVIDEBYZERO = 0,
    DEBUGEXCEPTION = 1,
    NMI = 2,
    BREAKPOINT = 3,
    OVERFLOW = 4,
    BOUND = 5,
    INVALIDOPCODE = 6,
    DEVICENOTAVAILABLE = 7,
    DOUBLEFAULT = 8,
    COREPROCESSOROVERRUN = 9,
    INVALIDTSS = 10,
    SEGMENTNOTPRESENT = 11,
    STACKSEGMENTFAULT = 12,
    GENEALPROTECTION = 13,
    PAGEFAULT = 14,
    CONTROLPROTECTIONEXCEPTION = 21
} ERROR_TYPE;

void error_handler(uint32_t error) {
    switch ((ERROR_TYPE)error) {
    case DIVIDEBYZERO:
        print_string("Divide by zero", 0x04, 0, 0);
        break;
    case DEBUGEXCEPTION:
        print_string("Debug exception", 0x04, 0, 0);
        break;
    case NMI:
        print_string("NMI", 0x04, 0, 0);
        break;
    case BREAKPOINT:
        print_string("Breakpoint", 0x04, 0, 0);
        break;
    case OVERFLOW:
        print_string("Overflow", 0x04, 0, 0);
        break;
    case BOUND:
        print_string("Bound", 0x04, 0, 0);
        break;
    case INVALIDOPCODE:
        print_string("Invalid opcode", 0x04, 0, 0);
        break;
    case DEVICENOTAVAILABLE:
        print_string("Device not available", 0x04, 0, 0);
        break;
    case DOUBLEFAULT:
        print_string("Double fault", 0x04, 0, 0);
        break;
    case COREPROCESSOROVERRUN:
        print_string("Coreprocess overun", 0x04, 0, 0);
        break;
    case INVALIDTSS:
        print_string("Invalid TSS", 0x04, 0, 0);
        break;
    case SEGMENTNOTPRESENT:
        print_string("Segment not present", 0x04, 0, 0);
        break;
    case STACKSEGMENTFAULT:
        print_string("Stack-Segment fault", 0x04, 0, 0);
        break;
    case GENEALPROTECTION:
        print_string("General protection", 0x04, 0, 0);
        break;
    case PAGEFAULT:
        print_string("Page fualt", 0x04, 0, 0);
        break;
    case CONTROLPROTECTIONEXCEPTION:
        print_string("Control protection exception", 0x04, 0, 0);
        break;
    default:
        print_string("Something is wrong", 0x04, 0, 0);
        break;
    }
    return;
}
