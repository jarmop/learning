wayland-scanner client-header \
  /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml \
  client/xdg-shell-client-protocol.h

# wayland-scanner client-header \
#   /usr/share/wayland/wayland.xml \
#   client/wayland-client-protocol.h

wayland-scanner private-code \
  /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml \
  client/xdg-shell-protocol.c