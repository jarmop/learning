#!/bin/bash

# qemu-system-x86_64 \
#   -enable-kvm \
#   -m 4G \
#   -cpu host \
#   -smp 4 \
#   -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE_4M.fd \
#   -drive if=pflash,format=raw,file=./OVMF_VARS.fd \
#   -vga none \
#   -device virtio-gpu-gl-pci \
#   -display sdl,gl=on \
#   -drive file=ubuntu.img,format=qcow2,if=virtio

# qemu-system-x86_64 \
#   -enable-kvm \
#   -m 4G \
#   -cpu host \
#   -smp 4 \
#   -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE_4M.fd \
#   -drive if=pflash,format=raw,file=./OVMF_VARS.fd \
#   -device virtio-vga-gl \
#   -display gtk,gl=on \
#   -drive file=ubuntu.img,format=qcow2,if=virtio

# qemu-system-x86_64 \
#   -enable-kvm \
#   -m 4G \
#   -cpu host \
#   -smp 4 \
#   -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE_4M.fd \
#   -drive if=pflash,format=raw,file=./OVMF_VARS.fd \
#   -vga qxl \
#   -display gtk,gl=on \
#   -drive file=ubuntu.img,format=qcow2,if=virtio

qemu-system-x86_64 \
  -enable-kvm \
  -m 4G \
  -cpu host \
  -smp 4 \
  -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE_4M.fd \
  -drive if=pflash,format=raw,file=./OVMF_VARS.fd \
  -device qxl \
  -display gtk,gl=on \
  -drive file=ubuntu.img,format=qcow2,if=virtio