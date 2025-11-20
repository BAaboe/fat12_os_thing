#include "idt/idt.h"
#include <stdint.h>

extern void divide_by_zero();
extern void debug_exception();
extern void nmi();
extern void breakpoint();
extern void overflow();
extern void bound();
extern void invalid_opcode();
extern void device_not_available();
extern void double_fault();
extern void coreprocessor_segment_overun();
extern void invalid_tss();
extern void segment_not_present();
extern void stack_segment_fault();
extern void general_protection();
extern void page_fault();
extern void control_protection_exception();

extern void int_pic0();
extern void int_pic1();
extern void int_pic2();
extern void int_pic3();
extern void int_pic4();
extern void int_pic5();
extern void int_pic6();
extern void int_pic7();
extern void int_pic8();
extern void int_pic9();
extern void int_pic10();
extern void int_pic11();
extern void int_pic12();
extern void int_pic13();
extern void int_pic14();
extern void int_pic15();

void setUpIDT() {
    // Errors
    setIdtEntry(0, (uintptr_t)divide_by_zero, 0x08, 0xf, 0);
    setIdtEntry(1, (uintptr_t)debug_exception, 0x08, 0xf, 0);
    setIdtEntry(2, (uintptr_t)nmi, 0x08, 0xe, 0);
    setIdtEntry(3, (uintptr_t)breakpoint, 0x08, 0xf, 0);
    setIdtEntry(4, (uintptr_t)overflow, 0x08, 0xf, 0);
    setIdtEntry(5, (uintptr_t)bound, 0x08, 0xf, 0);
    setIdtEntry(6, (uintptr_t)invalid_opcode, 0x08, 0xf, 0);
    setIdtEntry(7, (uintptr_t)device_not_available, 0x08, 0xf, 0);
    setIdtEntry(8, (uintptr_t)double_fault, 0x08, 0xf, 0);
    setIdtEntry(9, (uintptr_t)coreprocessor_segment_overun, 0x08, 0xf, 0);
    setIdtEntry(10, (uintptr_t)invalid_tss, 0x08, 0xf, 0);
    setIdtEntry(11, (uintptr_t)segment_not_present, 0x08, 0xf, 0);
    setIdtEntry(12, (uintptr_t)stack_segment_fault, 0x08, 0xf, 0);
    setIdtEntry(13, (uintptr_t)general_protection, 0x08, 0xf, 0);
    setIdtEntry(14, (uintptr_t)page_fault, 0x08, 0xf, 0);
    setIdtEntry(21, (uintptr_t)control_protection_exception, 0x08, 0xf, 0);

    // Pic
    setIdtEntry(32, (uintptr_t)int_pic0, 0x08, 0xe, 0);
    setIdtEntry(33, (uintptr_t)int_pic1, 0x08, 0xe, 0);
    setIdtEntry(34, (uintptr_t)int_pic2, 0x08, 0xe, 0);
    setIdtEntry(35, (uintptr_t)int_pic3, 0x08, 0xe, 0);
    setIdtEntry(36, (uintptr_t)int_pic4, 0x08, 0xe, 0);
    setIdtEntry(37, (uintptr_t)int_pic5, 0x08, 0xe, 0);
    setIdtEntry(38, (uintptr_t)int_pic6, 0x08, 0xe, 0);
    setIdtEntry(39, (uintptr_t)int_pic7, 0x08, 0xe, 0);
    setIdtEntry(40, (uintptr_t)int_pic8, 0x08, 0xe, 0);
    setIdtEntry(41, (uintptr_t)int_pic9, 0x08, 0xe, 0);
    setIdtEntry(42, (uintptr_t)int_pic10, 0x08, 0xe, 0);
    setIdtEntry(43, (uintptr_t)int_pic11, 0x08, 0xe, 0);
    setIdtEntry(44, (uintptr_t)int_pic12, 0x08, 0xe, 0);
    setIdtEntry(45, (uintptr_t)int_pic13, 0x08, 0xe, 0);
    setIdtEntry(46, (uintptr_t)int_pic14, 0x08, 0xe, 0);
    setIdtEntry(47, (uintptr_t)int_pic15, 0x08, 0xe, 0);

    struct IDTR idtr;
    idtr.size = (biggest_int * sizeof(IDTEntry)) - 1;
    idtr.offset = IDTADDRESS;

    __asm__ __volatile__("lidt %0" : : "m"(idtr) : "memory");
}
