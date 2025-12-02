Takes over the screen. Consider running in a virtual machine.

```sh
gcc drm-demo.c  $(pkg-config --cflags --libs libdrm) -std=gnu11
```
