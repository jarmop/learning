Takes over the screen. Consider running in a virtual machine.

Install dependencies, e.g. on Alpine:

```sh
apk add libdrm-dev linux-headers musl-dev
```

```sh
gcc drm-demo.c  $(pkg-config --cflags --libs libdrm) -std=gnu11
```

# Useful commands for Alpine:

List input devices:

```sh
cat /proc/bus/input/devices
```

# Troubleshooting

### Accessing input events requires root privileges or adding user to the "input" group
```sh
sudo adduser jarmovm input
```

### Adjust resolution (in Ubuntu)
```sh
sudo nano /etc/default/grub
```
GRUB_GFXMODE=925x650
```sh
sudo update-grub
```
