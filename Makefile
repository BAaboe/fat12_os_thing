CC := ~/opt/cross/bin/i686-elf-gcc
CCFLAGS := -ffreestanding -nostdlib -nostartfiles -fno-stack-protector -fno-PIC -g -I include/ -m32 -T kernel/kernel.ld
CFILES := kernel/main.c kernel/print.c kernel/idt/idt.c kernel/idt/error_handler.c kernel/pic/pic.c kernel/pic/pic_handler.c kernel/time.c

IMAGE := fat_bilde.img

.PHONY: all fat kernel bootloader

all: fat bootloader kernel
	dd if=build/bootloader/bootloader.o of=build/$(IMAGE) conv=notrunc

	mcopy -i build/$(IMAGE) build/kernel/kernel.bin "::kernel.bin"
	

kernel:
	mkdir -p build/kernel
	mkdir -p build/kernel/idt
	mkdir -p build/kernel/pic

	nasm -f elf32 kernel/kernel_entry.asm -o build/kernel/kernel_entry.o
	nasm -f elf32 kernel/idt/error_handlers.asm -o build/kernel/idt/error_handlers.o
	nasm -f elf32 kernel/pic/pic_handlers.asm -o build/kernel/pic/pic_handlers.o

	$(CC) $(CCFLAGS) build/kernel/kernel_entry.o build/kernel/pic/pic_handlers.o build/kernel/idt/error_handlers.o $(CFILES) -o build/kernel/kernel.bin



bootloader:
	mkdir -p build/bootloader
	nasm -f bin bootloader/bootloader.asm -o build/bootloader/bootloader.o

fat:
	mkdir -p build
	dd if=/dev/zero of=build/$(IMAGE) bs=512 count=2880 conv=notrunc
	mkfs.fat -F12 -R 4 build/$(IMAGE)

run: all
	qemu-system-x86_64 -drive file=build/fat_bilde.img,if=floppy,format=raw -debugcon stdio
