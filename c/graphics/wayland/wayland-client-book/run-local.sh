#!/bin/bash

gcc src/*.c $(pkg-config --cflags --libs wayland-client)
./a.out