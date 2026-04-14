Install SDL vendor
```sh
git clone --depth=1 --branch=main https://github.com/libsdl-org/SDL.git vendored/SDL
rm -rf vendored/SDL/.git
```

Build
```sh
cmake -S . -B build
cmake --build build
```

Run
```sh
build/hello
```