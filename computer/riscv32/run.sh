rm -f kernel.elf kernel.map

riscv64-unknown-elf-gcc \
  -march=rv32i_zicsr -mabi=ilp32 \
  -ffreestanding -nostdlib -fno-builtin \
  -Wl,-T,linker.ld -Wl,-Map,kernel.map \
  start.s main.c -o kernel.elf

# Run with QEMU GUI
# qemu-system-riscv32 -M virt -m 16M -bios none -kernel kernel.elf -serial stdio

qemu-system-riscv32 \
  -M virt -m 16M \
  -bios none \
  -kernel kernel.elf \
  -nographic

# qemu-system-riscv32 \
#   -M virt -m 16M \
#   -bios none \
#   -kernel kernel.elf \
#   -nographic \
#   -S -gdb tcp::1234

# Terminate QEMU: Ctrl+A, X
