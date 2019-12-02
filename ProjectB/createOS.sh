#!/bin/bash
#GiuseppeCarchidi, Federico Read, JoeyGaruti, Fall 2019
rm diskc.img
dd if=/dev/zero of=diskc.img bs=512 count=1000
nasm bootload.asm
dd if=bootload of=diskc.img bs=512 count=1 conv=notrunc
echo "building kernal c"
bcc -ansi -c -o kernel_c.o kernel.c
echo "building kernal asm"
as86 kernel.asm -o kernel_asm.o
echo "building kernal"
ld86 -o kernel -d kernel_c.o kernel_asm.o
echo "coping kernel to diskc"
dd if=kernel of=diskc.img bs=512 conv=notrunc seek=3
dd if=message.txt of=diskc.img bs=512 count=1 seek=30 conv=notrunc
