extern error_handler


global divide_by_zero
divide_by_zero:
	pushad
	cld
	mov eax, 0
	push eax
	call error_handler
	cli
	hlt
	popad
	iret

global debug_exception
debug_exception:
	pushad
	cld
	mov eax, 1
	push eax
	call error_handler
	cli
	hlt
	popad
	iret

global nmi
nmi:
	pushad
	cld
	mov eax, 2
	push eax
	call error_handler
	popad
	cli
	hlt
	iret

global breakpoint
breakpoint:
	pushad
	cld
	mov eax, 3
	push eax
	call error_handler
	popad
	cli
	hlt
	iret

global overflow
overflow:
	pushad
	cld
	mov eax, 4
	push eax
	call error_handler
	popad
	cli
	hlt
	iret

global bound
bound:
	pushad
	cld
	mov eax, 5
	push eax
	call error_handler
	popad
	cli
	hlt
	iret

global invalid_opcode
invalid_opcode:
	pushad
	cld
	mov eax, 6
	push eax
	call error_handler
	popad
	cli
	hlt
	iret

global device_not_available
device_not_available:
	pushad
	cld
	mov eax, 7
	push eax
	call error_handler
	popad
	cli
	hlt
	iret

global double_fault
double_fault:
	pushad
	cld
	mov eax, 8
	push eax
	call error_handler
	popad
	cli
	hlt
	iret

global coreprocessor_segment_overun
coreprocessor_segment_overun:
	pushad
	cld
	mov eax, 9
	push eax
	call error_handler
	popad
	cli
	hlt
	iret

global invalid_tss
invalid_tss:
	pushad
	cld
	mov eax, 10
	push eax
	call error_handler
	popad
	cli
	hlt
	iret

global segment_not_present
segment_not_present:
	pushad
	cld
	mov eax, 11
	push eax
	call error_handler
	popad
	cli
	hlt
	iret

global stack_segment_fault
stack_segment_fault:
	pushad
	cld
	mov eax, 12
	push eax
	call error_handler
	popad
	cli
	hlt
	iret

global general_protection
general_protection:
	pushad
	cld
	mov eax, 13
	push eax
	call error_handler
	popad
	cli
	hlt
	iret

global page_fault
page_fault:
	pushad
	cld
	mov eax, 14
	push eax
	call error_handler
	popad
	cli
	hlt
	iret

global control_protection_exception
control_protection_exception:
	pushad
	cld
	mov eax, 21
	push eax
	call error_handler
	popad
	cli
	hlt
	iret

