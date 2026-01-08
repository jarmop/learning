nasm -f elf32 kernel.asm -o kernel.o
ld -m elf_i386 -T link.ld kernel.o -o kernel.elf

cp kernel.elf iso/boot/
grub-mkrescue iso -o kernel.iso

qemu-system-x86_64 \
  -cdrom kernel.iso \
  -boot d \
  -m 128M 
