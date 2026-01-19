gcc hello-webkit.c -o hello-webkit \
  `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.1`