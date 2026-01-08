nasm -f elf64 uart.asm -o uart.o
ld -Ttext=0x100000 uart.o -o uart.elf
objcopy -O binary uart.elf uart.bin

qemu-system-x86_64 \
  -machine none \
  -m 64M \
  -cpu qemu64,apic-id=0 \
  -smp 1 \
  -device loader,file=uart.bin,addr=0x100000,force-raw=on \
  -serial stdio
#   -nographic
#   -mon none

