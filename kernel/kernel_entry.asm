[BITS 32]
global kernel_entry
extern kernel
section .text

kernel_entry:

call kernel

cli
hlt
