# Hello init

Experimenting by booting into nothing but a kernel and a simple init script that
will run busybox shell.

## Steps

Get the Linux kernel source and go to that directory:

```sh
git clone --depth=1 https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git

cd linux
```

Configure and build the kernel:

```sh
# Cleanup. Remove all build artifacts and config files.
make mrproper

# Create the default config file for the x86_64 architecture
make x86_64_defconfig

# Build with all cpu cores. Takes about 8 minutes.
time make -j$(nproc)
```

Go to this directory and copy the newly created kernel image here:

```sh
cd $HOME/development/git-remote/learning/linux/custom/custom-kernel

cp $HOME/development/git-external/linux/arch/x86/boot/bzImage .
```

Install busybox:
```sh
sudo apt install busybox-static
```

Create initramfs:

```sh
./create-initramfs.sh
```

Run with QEMU:

```sh
./run.sh
```

Exit busybox with `poweroff -f`, otherwise QEMU freezes in kernel panic:

```sh
poweroff -f
```
