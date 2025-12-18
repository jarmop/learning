#!/bin/bash

gcc src/client.c $(pkg-config --cflags --libs wayland-client)
./a.out