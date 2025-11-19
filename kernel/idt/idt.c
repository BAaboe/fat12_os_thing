#include "idt/idt.h"

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

void setUpIDT() {
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

    struct IDTR idtr;
    idtr.size = (biggest_int * sizeof(IDTEntry)) - 1;
    idtr.offset = IDTADDRESS;

    __asm__ __volatile__("lidt %0" : : "m"(idtr) : "memory");
    __asm__ __volatile__("sti");
}
