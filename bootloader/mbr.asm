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

; Save the drive number
mov [drive_number], dl

; Find the first cluster
; Read the root directory into memory

; Find what sector the root directory is on
; ax = ReservedSectors + NumberOfFats*SectorsPerFat
xor ax, ax
mov ax, [BPB_SectorsPerFat]
mul byte [BPB_NumberOfFats]
add ax, word [BPB_ReservedSectors]

; Find how many sectors the root directory is
push ax ; Save ax since mul and div uses ax
; di = (RootEntries*32 + BytesPerSector-1)/BytesPerSector
mov ax, word [BPB_RootEntries]
mov cx, 32
mul cx
add ax, word [BPB_BytesPerSector]
dec ax
div word [BPB_BytesPerSector]
mov di, ax

mov dl, byte [drive_number]

; Restor ax
pop ax

mov bx, buffer

call read_sectors

; Find entry for correct file

mov di, buffer

mov dx, word [BPB_RootEntries]
.lookup_loop:
mov si, file_name
mov cx, 11
push di
cld
repe cmpsb
pop di
jz .found

add di, 32
dec dx
test dx, dx
jnz .lookup_loop

.not_found:
mov ax, file_not_found
call print
cli
hlt

.found:
; File entry should be at ES:DI
; read file into memory
mov ax, [di + 0x1a]
mov [cluster], ax

; Read fat into memory
mov ax, word [BPB_ReservedSectors]
mov dl, byte [drive_number]
mov di, word [BPB_SectorsPerFat]
mov bx, buffer
call read_sectors


; Load current cluster into memory

mov ax, file_memory_seg
mov es, ax
mov bx, file_memory_offset

.cluster_loop:

; Calculate where the data region starts
; First find the where the root directory is
mov ax, word [BPB_SectorsPerFat]
mul byte [BPB_NumberOfFats]
add ax, word [BPB_ReservedSectors]
push ax

; Find how many sectors the root directory is
; di = (RootEntries*32 + BytesPerSector-1)/BytesPerSector
mov ax, word [BPB_RootEntries]
mov cx, 32
mul cx
add ax, word [BPB_BytesPerSector]
dec ax
div word [BPB_BytesPerSector]

; Add them togheter and you get where the data region starts
pop dx
add ax, dx

add ax, word [cluster]
add ax, -2

mov dl, byte [drive_number]
mov di, word [BPB_SectorsPerCluster]
; Only want a byte
and di, 0xff

call read_sectors

; Find offset into the fat table. cluster*1.5
mov ax, word [cluster]
shr ax, 1
add ax, word [cluster]

; Load the correct word into ax
push bx
mov bx, buffer
add bx, ax
mov ax, word [bx]
pop bx
push ax

; If the cluster number is even we read the current byte plus half of the next
; If the cluster number is odd we read half of the current byte plus the next byte
mov ax, [cluster]
and ax, 1
test ax, ax

jz .even

.odd:
pop ax
shr ax, 4
jmp .eo_done

.even:
pop ax
and ax, 0xfff

.eo_done:

; Save the new cluster number
mov word [cluster], ax

; Increment the pointer to the data
; TODO: Increment segment if needed
xor ax, ax
mov al, byte [BPB_SectorsPerCluster]
mul word [BPB_BytesPerSector]
add bx, ax

; Check if end of file
mov ax, word [cluster]
cmp ax, 0x0ff8
jl .cluster_loop

mov ax, file_memory_seg
mov ds, ax
xor ax, ax
call print


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
inc ax


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
xor dx, dx
div word [BPB_NumberOfHeads]
mov dh, dl

; Get the track number
mov ch, al
shl ah, 6
or cl, ah

ret

;
; Prints
; ds:ax: string
;
print:
mov si, ax
.loop:
lodsb
cmp al, 0
jz .done

mov ah, 0x0e

xor bx, bx

int 0x10

jmp .loop

.done:
mov ah, 0x0e
mov al, 10
int 0x10
mov al, 0x0d
int 0x10
ret

disk_error:
xor ax, ax
mov ds, ax
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
file_not_found: db "File not found", 0
file_name: db "TEST2   TXT", 0
drive_number: db 0
cluster: dw 0

file_memory_seg equ 0x200
file_memory_offset equ 0

times 510-($-$$) db 0
db 0x55, 0xaa

buffer:
