hacknet: iso

iso: binary ensure_out_dir
	mkdir -p build/isodir/boot/grub
	cp build/hn_demo.bin build/isodir/boot/hn_demo.bin
	cp grub.cfg build/isodir/boot/grub/grub.cfg
	grub-mkrescue -o build/hn_demo.iso build/isodir

binary: boot kernel ensure_out_dir
	i686-elf-gcc -T linker.ld -o build/hn_demo.bin -ffreestanding -O2 -nostdlib build/boot.o build/kernel.o -lgcc

kernel: ensure_out_dir
	i686-elf-gcc -c src/kernel.c -o build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

boot: ensure_out_dir
	i686-elf-as src/boot.s -o build/boot.o

ensure_out_dir:
	mkdir build

clean:
	rm -rf build/
