Trying to follow the official Vulkan tutorial: https://docs.vulkan.org/tutorial/latest

# Build

Get Vulkan-Tutorial source code and install dependencies with the script there.
```sh
cd ~/development/git-external
git clone https://github.com/KhronosGroup/Vulkan-Tutorial
cd Vulkan-Tutorial
./scripts/install_dependencies_linux.sh
```

The script will tell you to install Vulkan SDK manually, so install that following the instructions:
```sh
mkdir -p ~/development/lib/vulkansdk
tar -xf vulkansdk-linux-x86_64-<version>.tar.xz -C ~/development/lib/vulkansdk
cd ~/development/lib/vulkansdk
ln -s <version> default

```

Requires cmake 3.9+, apt only has 3.8, so need to install cmake manually. Install cmake 4.3.1 (latest at the moment, 2026-4-7):
```sh
version=4.3
build=1
mkdir ~/temp
cd ~/temp
wget https://cmake.org/files/v$version/cmake-$version.$build.tar.gz
tar -xzvf cmake-$version.$build.tar.gz
cd cmake-$version.$build/

./bootstrap
make -j$(nproc)
sudo make install
```

Build:
```sh
cmake -B build -S . -G Ninja
cmake --build build
```

Needed to comment out a whole bunch of stuff from the CmakeLists "template" (that I copied from the Vulkan-Tutorial reposioty) to make the thing build. But of course the app still won't open any window and just eats all the CPU.