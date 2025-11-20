extern pic_handler

global int_pic0
int_pic0:
	pushad
	cld
	mov eax, 0
	push eax
	call pic_handler
	add esp, 4
	popad
	iret

global int_pic1
int_pic1:
	pushad
	cld
	mov eax, 1
	push eax
	call pic_handler
	add esp, 4
	popad
	iretd

global int_pic2
int_pic2:
	pushad
	cld
	mov eax, 2
	push eax
	call pic_handler
	add esp, 4
	popad
	iret

global int_pic3
int_pic3:
	pushad
	cld
	mov eax, 3
	push eax
	call pic_handler
	add esp, 4
	popad
	iret

global int_pic4
int_pic4:
	pushad
	cld
	mov eax, 4
	push eax
	call pic_handler
	add esp, 4
	popad
	iret

global int_pic5
int_pic5:
	pushad
	cld
	mov eax, 5
	push eax
	call pic_handler
	add esp, 4
	popad
	iret

global int_pic6
int_pic6:
	pushad
	cld
	mov eax, 6
	push eax
	call pic_handler
	add esp, 4
	popad
	iret

global int_pic7
int_pic7:
	pushad
	cld
	mov eax, 7
	push eax
	call pic_handler
	add esp, 4
	popad
	iret

global int_pic8
int_pic8:
	pushad
	cld
	mov eax, 8
	push eax
	call pic_handler
	add esp, 4
	popad
	iret

global int_pic9
int_pic9:
	pushad
	cld
	mov eax, 9
	push eax
	call pic_handler
	add esp, 4
	popad
	iret

global int_pic10
int_pic10:
	pushad
	cld
	mov eax, 10
	push eax
	call pic_handler
	add esp, 4
	popad
	iret

global int_pic11
int_pic11:
	pushad
	cld
	mov eax, 11
	push eax
	call pic_handler
	add esp, 4
	popad
	iret

global int_pic12
int_pic12:
	pushad
	cld
	mov eax, 12
	push eax
	call pic_handler
	add esp, 4
	popad
	iret

global int_pic13
int_pic13:
	pushad
	cld
	mov eax, 13
	push eax
	call pic_handler
	add esp, 4
	popad
	iret

global int_pic14
int_pic14:
	pushad
	cld
	mov eax, 14
	push eax
	call pic_handler
	add esp, 4
	popad
	iret

global int_pic15
int_pic15:
	pushad
	cld
	mov eax, 15
	push eax
	call pic_handler
	add esp, 4
	popad
	iret
