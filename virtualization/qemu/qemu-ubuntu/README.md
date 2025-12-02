Get the install media: https://ubuntu.com/download/desktop

Latest at the time of writing this was Ubuntu 24.04.3 LTS:

```
ubuntu-24.04.3-desktop-amd64.iso
```

# Commands

Create a storage drive for the virtual machine:

```sh
qemu-img create -f qcow2 ubuntu.img 40G
```

Create a writable UEFI “variables” file:

```
sudo apt install ovmf
cp /usr/share/OVMF/OVMF_VARS_4M.fd ./OVMF_VARS.fd
```

Start the system with bootable media in the CD rom drive (the "cdrom" option):

```sh
qemu-system-x86_64 \
  -enable-kvm \
  -m 4G \
  -cpu host \
  -smp 4 \
  -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE_4M.fd \
  -drive if=pflash,format=raw,file=./OVMF_VARS.fd \
  -device virtio-vga \
  -display gtk,gl=on \
  -cdrom ubuntu-24.04.3-desktop-amd64.iso \
  -drive file=ubuntu.img,format=qcow2,if=virtio
```

After installation is complete, the system asks you to restart. During restart,
it asks you to remove the bootable media. Stop QEMU and run it again with
otherwise the same command but without the cdrom option:

```sh
qemu-system-x86_64 \
  -enable-kvm \
  -m 4G \
  -cpu host \
  -smp 4 \
  -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE_4M.fd \
  -drive if=pflash,format=raw,file=./OVMF_VARS.fd \
  -device virtio-vga \
  -display gtk,gl=on \
  -drive file=ubuntu.img,format=qcow2,if=virtio
```

# Explanations

- `m 4G` --> 4G of memory
- `cpu host` --> use the real host CPU rather than a virtual CPU
- `smp 4` --> 4 CPU cores
- `device virtio-vga` --> faster virtual GPU
- `display gtk,gl=on`
  - `gtk` --> native GTK window for the VM
  - `gl=on` --> enable 3D graphics

## The lengthy drive commands
