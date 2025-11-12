load_kernel:
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

mov dl, byte [BPB_DriveNumber]

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
mov dl, byte [BPB_DriveNumber]
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

mov dl, byte [BPB_DriveNumber]
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
xor ax, ax
mov al, byte [BPB_SectorsPerCluster]
mul word [BPB_BytesPerSector]
add bx, ax
jnc .no_overflow

push ax
mov ax, es
add ax, 0x100
mov es, ax
pop ax

.no_overflow:
; Check if end of file
mov ax, word [cluster]
cmp ax, 0x0ff8
jl .cluster_loop

ret

file_memory_seg equ 0x0100
file_memory_offset equ 0x0000
file_name: db "KERNEL  BIN", 0
