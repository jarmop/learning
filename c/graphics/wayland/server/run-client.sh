#!/bin/bash

gcc client/client.c client/xdg-shell-protocol.c $(pkg-config --cflags --libs wayland-client) -o client.elf
./client.elf