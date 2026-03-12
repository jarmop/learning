#!/bin/bash

./create-initramfs.sh

qemu-system-x86_64 \
  -kernel bzImage \
  -initrd initramfs.cpio.gz \
  -nographic \
  -append "console=ttyS0"

# The "-nographic" option tells QEMU to not use graphical display.
#
# The "-append" option allows passing a kernel command-line argument to the 
# guest kernel. The kernel command-line argument "console=ttyS0" means: 
# Connect input/output to the serial port (which is now connected to the host 
# terminal because there's no graphical display).

  # -netdev user,id=net0 \
  # -device virtio-net,netdev=net0

  # -append "console=ttyS0 rdinit=/init" \
