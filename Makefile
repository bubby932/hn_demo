CFLAGS = -std=gnu99 -ffreestanding -O5 -Wall -Wextra -fstack-protector -fstack-protector-strong -fstack-protector-all -Werror=implicit-function-declaration
OBJS = build/boot.o build/kernel.o build/libpaging.o build/libgdt.o build/libirq.o build/libsyscall.o build/ata_r.o build/ata_w.o

hacknet: iso

iso: link ensure_out_dir
	mkdir -p build/isodir/boot/grub
	cp build/hn_demo.bin build/isodir/boot/hn_demo.bin
	cp grub.cfg build/isodir/boot/grub/grub.cfg
	grub-mkrescue -o build/hn_demo.iso build/isodir

link: asm kernel drivers ensure_out_dir
	i686-elf-gcc -T linker.ld -o build/hn_demo.bin -ffreestanding -O5 -nostdlib $(OBJS) -O5 -lgcc

kernel: ensure_out_dir
	i686-elf-gcc -c src/kernel.c -o build/kernel.o $(CFLAGS)

asm: ensure_out_dir
	i686-elf-as src/boot.s -o build/boot.o
	i686-elf-as src/lib/paging.S -o build/libpaging.o
	i686-elf-as src/lib/gdt.S -o build/libgdt.o
	i686-elf-as src/lib/irq.S -o build/libirq.o
	i686-elf-as src/lib/syscall.S -o build/libsyscall.o

drivers: driver_ata

driver_ata:
	i686-elf-as src/drivers/ata/ata_r.S -o build/ata_r.o
	i686-elf-as src/drivers/ata/ata_w.S -o build/ata_w.o

ensure_out_dir:
	mkdir -p build

clean:
	rm -rf build/
