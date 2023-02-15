ASM = i686-elf-as
CC = i686-elf-gcc
LD = i686-elf-gcc
CFLAGS = -ffreestanding -O5 -Wall -Wextra -lgcc -Isrc/lib/include

SOURCES = serial.c stdio.c kernel.c
OBJECTS = build/kernel.o build/serial.o build/stdio.o 

must: iso

iso: ensure_out_dir link
	mkdir -p build/isodir/boot/grub
	cp build/must.bin build/isodir/boot/must.bin
	cp grub.cfg build/isodir/boot/grub/grub.cfg
	grub-mkrescue -o build/must.iso build/isodir

link: asm ensure_out_dir $(SOURCES)
	$(LD) -T linker.ld $(OBJECTS) -o build/must.bin

asm: ensure_out_dir
	$(ASM) src/boot.s -o build/boot.o

kernel.c: ensure_out_dir
	$(CC) -c src/kernel.c $(CFLAGS) -o build/kernel.o

serial.c: ensure_out_dir
	$(CC) -c src/lib/include/serial.c $(CFLAGS) -o build/serial.o

stdio.c: ensure_out_dir
	$(CC) -c src/lib/include/stdio.c $(CFLAGS) -o build/stdio.o

ensure_out_dir:
	mkdir -p build

clean:
	rm -rf build/
