## Install

Create a storage drive for the virtual machine:

```sh
qemu-img create -f qcow2 alpine.qcow2 8G
```

Install:

```sh
qemu-system-x86_64 \
  -enable-kvm \
  -m 512 \
  -nic user \
  -boot once=d \
  -display gtk \
  -cdrom alpine-standard-3.22.2-x86_64.iso \
  -drive file=alpine.qcow2
```

F***ing Alpine requires a strong root password. This was good enough:

```sh
paskaalpine123
```

After the installation, QEMU can be started from the disk image:

```sh
qemu-system-x86_64 \
  -m 512 \
  -nic user \
  -drive file=alpine.qcow2
```

## Connect with ssh from the host machine

Start the VM with an SSH port forward:

```sh
qemu-system-x86_64 \
  -enable-kvm \
  -m 1G \
  -drive file=alpine.qcow2,format=qcow2,if=virtio \
  -netdev user,id=n0,hostfwd=tcp::2222-:22 \
  -device virtio-net,netdev=n0
```

SSH from the host:

```sh
ssh -p 2222 root@localhost
```

If ssh was not setup during the installation, do this in the guest:

```sh
apk add openssh
rc-service sshd start
rc-update add sshd
```
