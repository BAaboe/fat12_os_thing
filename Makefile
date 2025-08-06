IMAGE := fat_bilde.img

.phony: all fat

all: fat
	mkdir -p build/bootloader
	nasm -f bin bootloader/mbr.asm -o build/bootloader/mbr.o

	dd if=build/bootloader/mbr.o of=build/$(IMAGE) conv=notrunc

	mcopy -i build/$(IMAGE) build/test.txt "::test.txt"
	mcopy -i build/$(IMAGE) build/test2.txt "::test2.txt"
	


fat:
	mkdir -p build
	dd if=/dev/zero of=build/$(IMAGE) bs=512 count=2880 conv=notrunc
	mkfs.fat build/$(IMAGE)

run: all
	qemu-system-x86_64 -drive file=build/fat_bilde.img,if=floppy,format=raw
