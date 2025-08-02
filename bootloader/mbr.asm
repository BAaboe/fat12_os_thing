[BITS 16]
ORG 0x7c00


; Fat header stuff
jmp short boot_code
nop
; BIOS Paramter block
BPB_OEM					db 'BENJIFAT'
BPB_BytesPerSector		dw 512
BPB_SectorsPerCluster	db 1
BPB_ReservedSectors		dw 1
BPB_NumberOfFats		db 2
BPB_RootEntries			dw 224
BPB_TotalSectors		dw 2880
BPB_MediaDescriptor		db 0xf0
BPB_SectorsPerFat		dw 9
BPB_SectorsPerTrack		dw 18
BPB_NumberOfHeads		dw 2
BPB_HiddenSectors		dd 0
BPB_LargeTotalSectors	dd 0

; Extended Boot Record
BPB_DriveNumber			db 0
BPB_Reserved			db 0
BPB_BootSignatur		db 0x29
BPB_VolumeSerialNum		dd 1234
BPB_VolumeLable			db 'boot loader'
BPB_FileSystemType		db 'FAT12   '

boot_code:

; Set up the segments
xor ax, ax
mov ds, ax
mov es, ax
mov ss, ax

; Set up the stack
mov sp, 0x7c00

; Align properly, some bios load at 0x7c0:0x0000 instead of 0x0000:0x7c00
jmp 0x00:.aligned

.aligned:


pusha
mov dl, 0x00
call load_file
popa
 
cli
hlt

;
; Loads a flie from the file system
; ax: pointer to the string with the file name
; dx: drive number
; es:bx: place in memory to place the file
; 
load_file:
push ax

; Find the first cluster

push dx

; Find what sector the root directory is on
xor ax, ax
mov ax, [BPB_SectorsPerFat]
mul byte [BPB_NumberOfFats]
add ax, word [BPB_ReservedSectors]

; Find how many sectors the root directory is
push ax
mov ax, word [BPB_RootEntries]
push cx
mov cx, 32
mul cx
pop cx
add ax, word [BPB_BytesPerSector]
dec ax
div word [BPB_BytesPerSector]
mov di, ax
pop ax

mov bx, 0x7e00

pop dx

call read_sectors

call happy

cli 
hlt


; Reads sector from disk
; ax: lba
; dl: drive
; di: number of sectors to read
; es:bx: buffer
; 
read_sectors:
.loop:
push ax
push dx

call lba_to_chs

pop ax
mov dl, al

mov ah, 0x02
mov al, 1

pusha
int 0x13
popa

; TODO: Maybe try more than once before giving up
jc .disk_error

dec di
add bx, [BPB_BytesPerSector]

pop ax

test di, di
jz .loop
ret

.disk_error:
call disk_error

;
; Converts LBA to CHS address
; ax: lba
; Returns:
; cx = [0-6] sector, [7-15] cylinder
; dh = head
;
lba_to_chs:
; Follows https://en.wikipedia.org/wiki/Logical_block_addressing#CHS_conversion
; Get the sector number
xor dx, dx
div word [BPB_SectorsPerTrack]
mov cx, dx
inc cx

; Get the head number
div word [BPB_NumberOfHeads]
mov dh, dl

; Get the track number
mov ch, al
shl ah, 6
or cl, ah

ret

;
; Prints
; ax: string
;
print:
mov bx, ax
.loop:
cmp byte [bx], 0
jz .done

mov ah, 0x0e
mov al, byte [bx]

push bx
xor bx, bx

int 0x10

pop bx

inc bx
jmp .loop

.done:
ret

disk_error:
mov ax, disk_read_error_msg
call print

cli
hlt

happy:
mov ax, happy_msg
call print
ret


disk_read_error_msg: db "Could not read disk",0
happy_msg: db ":)", 0
times 510-($-$$) db 0
db 0x55, 0xaa
