mkdir -p CD_root/isolinux
mkdir -p CD_root/images
mkdir -p CD_root/kernel
cp isolinux.bin CD_root/isolinux/isolinux.bin
cp isolinux.cfg CD_root/isolinux/isolinux.cfg
cp *.bss CD_root/isolinux/
cp *.sys CD_root/isolinux/
cp *.c32 CD_root/isolinux/
cp *.com CD_root/isolinux/
cp *.elf CD_root/isolinux/
cp *.jpg CD_root/isolinux/
cp *.menu CD_root/isolinux/
cp *.txt CD_root/isolinux/
nasm -felf32 -o boot.o boot.S
nasm -o hello.com hello.asm
gcc -c kernel.c -o kernel.o -nostdlib
gcc link.ld boot.o kernel.o -o kernel.elf -nostdlib
nasm model.asm -o model.o
objdump -D kernel.elf > out.txt
cat out.txt | grep kernel_main
dd if=kernel.elf of=hello.c32 
dd if=model.o of=hello.c32 count=1 conv=notrunc
cp *.c32 CD_root/isolinux/
genisoimage -o myos.iso -input-charset utf-8 -b isolinux/isolinux.bin -no-emul-boot -boot-load-size 4  -boot-info-table ./CD_root 

