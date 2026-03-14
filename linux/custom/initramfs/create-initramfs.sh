#!/bin/bash

rm -rf initramfs
rm initramfs.cpio.gz

# Create the initramfs directory from where the kernel  
# will be looking for the init script (PID 1)
mkdir -p initramfs/bin

cd initramfs/bin

# Copy busybox
cp /bin/busybox .
# chmod +x busybox

# Create symlinks for the required binaries
# ln -s busybox sh
# ln -s busybox mount
# ln -s busybox ifconfig
# ln -s busybox ip
# ln -s busybox udhcpc

cd ..

# Add the init script
cp ../init .
# chmod +x init

# Create console node
# mkdir -p dev
# sudo mknod -m 622 dev/console c 5 1

mkdir -p proc

# Pack and compress with the "best" compression (at the expense of speed).
#
# The "find ." command lists all files and directories recursively. The 
# "-print0" flag tells find to use null bytes instead of newline characters as
# separators.
#
# The cpio command builds the initramfs archive. The "--null" argument tells 
# that the input arguments are NULL separated. The "-o" flag means "create 
# archive".
#
# Linux initramfs requires the newc format.
#
find . -print0 | cpio --null -o --format=newc | gzip --best > ../initramfs.cpio.gz

cd ..