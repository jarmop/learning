#!/bin/bash

# install
git clone https://github.com/raysan5/raylib.git raylib
cd raylib
mkdir build && cd build
cmake -DGLFW_BUILD_WAYLAND=ON -DGLFW_BUILD_X11=OFF ..
# cmake -DGLFW_BUILD_WAYLAND=ON ..
make -j$(nproc)
sudo make install
sudo ldconfig

# uninstall (in raylib/build)
sudo make uninstall
cd ..
rm -rf build