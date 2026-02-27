#!/bin/bash

gcc src/client.c src/xdg-shell-protocol.c $(pkg-config --cflags --libs wayland-client)
./client