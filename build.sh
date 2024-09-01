#!/bin/sh
echo Agora montando, compilando, e juntando seu kernel:
nasm -f elf32 -o start.o start.asm
ld -m elf_i386 -T link.ld -o kernel.bin start.o main.o scrn.o gdt.o
echo Feito!
