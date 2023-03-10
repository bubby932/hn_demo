CFLAGS = 	-std=gnu99 -ffreestanding -Wall -Wextra -fstack-protector \
			-fstack-protector-strong -fstack-protector-all -Werror=implicit-function-declaration \
			-fno-rtti -fno-exceptions -nostdlib -Ofast -lgcc -Iinclude
OBJS = build/boot.o build/kernel.o build/libpaging.o build/libgdt.o build/libirq.o build/libsyscall.o

CXX = i686-elf-g++
CC = i686-elf-gcc
LD = i686-elf-gcc
AS = i686-elf-as

iso: link ensure_out_dir
	mkdir -p build/isodir/boot/grub
	cp build/hn_demo.bin build/isodir/boot/hn_demo.bin
	cp grub.cfg build/isodir/boot/grub/grub.cfg
	grub-mkrescue -o build/hn_demo.iso build/isodir

run:
	qemu-system-x86_64 -cdrom build/hn_demo.iso

link: asm kernel drivers ensure_out_dir
	$(LD) -T linker.ld -o build/hn_demo.bin $(OBJS) $(CFLAGS)

kernel: ensure_out_dir
	$(CXX) -c src/kernel.cpp -o build/kernel.o $(CFLAGS)

asm: ensure_out_dir
	$(AS) src/boot.s -o build/boot.o
	$(AS) src/lib/paging.S -o build/libpaging.o
	$(AS) src/lib/gdt.S -o build/libgdt.o
	$(AS) src/lib/irq.S -o build/libirq.o
	$(AS) src/lib/syscall.S -o build/libsyscall.o

drivers:

ensure_out_dir:
	mkdir -p build

clean:
	rm -rf build/
