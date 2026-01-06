#!/bin/bash

gcc -I/usr/include/efi \
    -I/usr/include/efi/x86_64 \
    -fno-stack-protector \
    -fpic \
    -fshort-wchar \
    -mno-red-zone \
    -c hello.c

gcc -nostdlib \
    -Wl,-znocombreloc \
    -Wl,-T,/usr/lib/elf_x86_64_efi.lds \
    /usr/lib/crt0-efi-x86_64.o \
    hello.o \
    -L /usr/lib \
    -lefi -lgnuefi \
    -o hello.so

ld -nostdlib \
   -znocombreloc \
   -T /usr/lib/elf_x86_64_efi.lds \
   /usr/lib/crt0-efi-x86_64.o \
   hello.o \
   -L /usr/lib \
   -lefi -lgnuefi \
   -o hello.so

objcopy \
  -j .text -j .sdata -j .data -j .dynamic \
  -j .dynsym -j .rel -j .rela -j .reloc \
  --target=efi-app-x86_64 \
  hello.so hello.efi


dpkg -L gnu-efi | grep -e "crt0-efi-x86_64.o" \
 -e "elf_x86_64_efi.lds" \
 -e "libefi.a" \
 -e "libgnuefi.a" \
 -e "efi\.h" \
 -e "efilib.h"
