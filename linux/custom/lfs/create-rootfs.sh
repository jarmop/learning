#!/bin/bash

rm -rf rootfs
rm rootfs.img

mkdir -p rootfs

cd rootfs

mkdir -p bin proc

cp /bin/busybox bin
cp ../init bin

mkdir -p etc
cp ../fstab etc

cd ..

# Create disk image
dd if=/dev/zero of=rootfs.img bs=1M count=64
mkfs.ext4 rootfs.img

# Copy rootfs directory contents into the disk image
sudo mount rootfs.img /mnt
sudo cp -r rootfs/* /mnt
sudo umount /mnt