# Hello init

Experimenting by booting into nothing but a kernel and a simple init script that
will run busybox shell. This uses whatever Linux kernel image is available in
the boot directory. Network doesn't work because the required kernel modules are
not built into the basic kernel binary. That maybe could be fixed by copying the
modules into initramfs.

## Steps

Copy the kernel image from "/boot" and change the ownership show you don't have
to run QEMU as root.

```sh
sudo cp /boot/vmlinuz-$(uname -r) vmlinuz
sudo chown $USER:$USER vmlinuz
```

Create initramfs, Add stuff to it. Package and compress it.

```sh
sudo apt install busybox-static

# Create the initramfs directory from where the kernel  
# will be looking for the init script (PID 1)
mkdir -p initramfs/{bin,proc,sys,dev}

cd initramfs/bin

# Copy busybox
cp /bin/busybox .
chmod +x busybox

# Create symlinks for the required binaries
ln -s busybox sh
ln -s busybox mount
ln -s busybox ifconfig
ln -s busybox ip
ln -s busybox udhcpc

cd ..

# Create the init script
cat > init <<'EOF'
#!/bin/sh
# Mount the file system
mount -t proc proc /proc
mount -t sysfs sys /sys
mount -t devtmpfs dev /dev
# Add network support
ifconfig eth0 up
udhcpc -i eth0
# Enter the shell
exec /bin/sh
EOF
chmod +x init

# Create console node
sudo mknod -m 622 dev/console c 5 1

# Pack and compress
find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../initramfs.cpio.gz

cd ..
```

Run with QEMU:

```sh
qemu-system-x86_64 \
  -kernel vmlinuz \
  -initrd initramfs.cpio.gz \
  -append "console=ttyS0 rdinit=/init" \
  -nographic
  -netdev user,id=net0 \
  -device virtio-net,netdev=net0
```

Best way to stop the vm seems to be to terminate the QEMU process, e.g. from the
system monitor. Killing the process instead of terminating it seems to lead to
some unexpected behavior in the terminal afterwards.
