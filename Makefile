KERNEL_OUT := build/kernel/kernel.bin
IMAGE := fat_bilde.img

CC := ~/opt/cross/bin/i686-elf-gcc
CCFLAGS := -ffreestanding -nostdlib -nostartfiles -fno-stack-protector -fno-PIC -g -I include/ -m32 -T kernel/kernel.ld

BSRC := bootloader/bootloader.asm
BOBJ := build/bootloader/bootloader.o

KSRC_SUBDIRS := $(wildcard kernel/*/)
KOBJS_SUBDIRS := $(patsubst %/, build/%/, $(KSRC_SUBDIRS))
KCFILES := $(wildcard kernel/*.c) $(foreach d,$(KSRC_SUBDIRS), $(wildcard $(d)*.c))
KASMFILES := $(wildcard kernel/*.asm) $(foreach d,$(KSRC_SUBDIRS), $(wildcard $(d)*.asm))
KOBJS := $(patsubst %.asm, build/%.o, $(KASMFILES)) $(patsubst %.c, build/%.o, $(KCFILES))

.PHONY: all fat kernel bootloader image clean test

all: image
	
%/:
	@mkdir -p $@

build/%.o: %.c
	@echo "Compiling C file" $< " -> " $@
	@$(CC) $(CCFLAGS) -c $< -o $@

build/%.o: %.asm
	@echo "Compiling assembly file" $< " -> " $@
	@nasm -f elf32 $< -o $@

kernel: $(KOBJS_SUBDIRS) $(KOBJS)
	@echo "Linking kernel  -> " $(KERNEL_OUT)
	@$(CC) $(CCFLAGS) $(KOBJS) -o $(KERNEL_OUT) -lgcc


bootloader:
	@echo "Compiling bootloader  -> " $(BOBJ)
	@mkdir -p build/bootloader
	@nasm -f bin $(BSRC) -o $(BOBJ)

fat:
	@echo "Creating empty fat12 image"
	@mkdir -p build
	@dd if=/dev/zero of=build/$(IMAGE) bs=512 count=2880 conv=notrunc 1> /dev/null 2>&1
	@mkfs.fat -F12 -R 4 build/$(IMAGE) 1> /dev/null 2>&1

image: fat bootloader kernel
	@echo "Installing the bootloader in the fat12 image"
	@dd if=build/bootloader/bootloader.o of=build/$(IMAGE) conv=notrunc 1> /dev/null 2>&1

	@echo "Copying the kernel to the fat12 image"
	@mcopy -i build/$(IMAGE) $(KERNEL_OUT) "::kernel.bin" 1> /dev/null 2>&1


clean:
	rm -rf build/*

run: all
	qemu-system-x86_64 -drive file=build/fat_bilde.img,if=floppy,format=raw -debugcon stdio
