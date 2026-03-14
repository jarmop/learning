#!/bin/bash

./create-rootfs.sh

qemu-system-x86_64 \
  -kernel bzImage \
  -drive file=rootfs.img,format=raw \
  -append "root=/dev/sda console=ttyS0" \
  -nographic