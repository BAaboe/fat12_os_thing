OUTPUT_FORMAT("binary")
ENTRY(kernel_entry)
phys = 0x00010000;
SECTIONS
{
	.text (phys) : {*(.text)}
	.data : {*(.data)}
	.rodata : {*(.rodata)}
	.bss : {*(.bss)}
}
