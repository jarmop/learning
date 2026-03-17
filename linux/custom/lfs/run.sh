#!/bin/bash

# ./create-lfs-img.sh
# ./create-rootfs.sh
# ./create-rootfs-apk.sh

qemu-system-x86_64 \
  -kernel linux-6.18.10/arch/x86/boot/bzImage \
  -drive file=lfs.img,format=raw \
  -append "root=/dev/sda console=ttyS0" \
  -nographic