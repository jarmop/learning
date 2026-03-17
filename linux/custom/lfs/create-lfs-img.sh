#!/bin/bash

# rm -rf rootfs
# rm rootfs.img
# mkdir -p rootfs

# cd rootfs
# cd lfs

# mkdir -p proc
# mkdir -p bin

# cp /bin/busybox bin
# cp ../init bin

# mkdir -p etc
# cp ../fstab etc

# cd ..

sudo cp init lfs/bin

# Create disk image
dd if=/dev/zero of=lfs.img bs=1M count=4096
mkfs.ext4 lfs.img

# Copy rootfs directory contents into the disk image
sudo mount lfs.img /mnt
sudo cp -r lfs/* /mnt
sudo umount /mnt