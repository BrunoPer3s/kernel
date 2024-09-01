CC_OPTIONS := -Wall -O -fstrength-reduce -fomit-frame-pointer \
              -finline-functions -nostdinc -fno-builtin -fno-pie \
              -I./include -c -m32

OUTPUT = kernel.bin

build:
	
	nasm -f elf32 -o start.o start.asm
	
	gcc $(CC_OPTIONS) -o main.o main.c
	gcc $(CC_OPTIONS) -o scrn.o scrn.c
	gcc $(CC_OPTIONS) -o gdt.o gdt.c
	gcc $(CC_OPTIONS) -o idt.o idt.c
	gcc $(CC_OPTIONS) -o isrs.o isrs.c



	
	ld -m elf_i386 -T link.ld -o $(OUTPUT) main.o start.o scrn.o gdt.o idt.o isrs.o


clean:
	rm -f *.o ${OUTPUT}

test: 
	chmod +x ./floppy.sh
	./floppy.sh