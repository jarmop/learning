gcc -O2 elf_loader.c -o elf_loader.elf

gcc -nostdlib -static -fno-pie -no-pie test.c -o test.elf