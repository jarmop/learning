#!/bin/bash

git clone https://github.com/raysan5/raylib.git raylib
cd raylib
mkdir build && cd build
cmake -DGLFW_BUILD_WAYLAND=ON ..
make -j$(nproc)
sudo make install
sudo ldconfig