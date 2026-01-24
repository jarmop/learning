gcc -Wall -Wextra -O2 \
  wayland_egl.c xdg-shell-protocol.c \
  $(pkg-config --cflags --libs wayland-client wayland-egl egl glesv2) \
  -lm

./a.out