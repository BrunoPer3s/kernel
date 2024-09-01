#!/bin/sh
mkdir -p disquete
sudo mount -o loop floppy.img disquete
sudo cp kernel.bin disquete/kernel.bin
sudo umount disquete
rmdir disquete

qemu-system-i386 -drive format=raw,file=floppy.img,if=floppy