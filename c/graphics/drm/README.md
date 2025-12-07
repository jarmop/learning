Takes over the screen. Consider running in a virtual machine.

Install dependencies, e.g. on Alpine:

```sh
apk add libdrm-dev linux-headers musl-dev
```

```sh
gcc drm-demo.c  $(pkg-config --cflags --libs libdrm) -std=gnu11
```
