#!/bin/bash

qemu-system-x86_64 \
  -enable-kvm \
  -m 1G \
  -drive file=alpine.qcow2,format=qcow2,if=virtio \
  -netdev user,id=n0,hostfwd=tcp::2222-:22 \
  -device virtio-net,netdev=n0 \
  # -device qemu-xhci \
  # -device usb-tablet
