Basic data structures and algorithms implemented in c++.

Install dependencies:
```
git submodule init
git submodule update
mkdir library/googletest/build
cd library/googletest/build
cmake ..
make
```

Compile:
```
g++ src/data-structures/*.cpp -I library/googletest/googletest/include library/googletest/build/googlemock/gtest/libgtest.a
```
