#!/bin/bash

cp /usr/share/OVMF/OVMF_VARS_4M.fd OVMF_VARS.fd

gcc -c hello.c \
  -ffreestanding \
  -fno-stack-protector \
  -fshort-wchar \
  -mno-red-zone \
  -fpie \
  -fno-asynchronous-unwind-tables \
  -fno-unwind-tables \
  -I/usr/include/efi

ld -nostdlib \
   -znocombreloc \
   -T /usr/lib/elf_x86_64_efi.lds \
   --entry=efi_main \
   --emit-relocs \
   hello.o \
   -o hello.elf

objcopy \
  -O efi-app-x86_64 \
  --subsystem=10 \
  hello.elf hello.efi

mkdir -p esp/EFI/BOOT
cp hello.efi esp/EFI/BOOT/BOOTX64.EFI

qemu-system-x86_64 \
  -machine q35 \
  -cpu host \
  -enable-kvm \
  -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE_4M.fd \
  -drive if=pflash,format=raw,file=OVMF_VARS.fd \
  -drive format=raw,file=fat:rw:esp
