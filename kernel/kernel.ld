OUTPUT_FORMAT("binary")
ENTRY(kernel_entry)
phys = 0x00001000;
SECTIONS
{
	.text (phys) : {*(.text)}
	.data : {*(.data)}
	.rodata : {*(.rodata)}
	.bss : {*(.bss)}
}
