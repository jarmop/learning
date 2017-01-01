# Build process

Building an executable file out of c++ source code is a four-step process.
1. Preprocess
  - copy included header files into the source code file
  - generate code by macros
  - replace symbolic constants (defined using #define) with their values
2. Compile
  - compile c++ code into assembly code
3. Assemble
  - assemble assembly code into object code
4. Link
  - link assembled object code with object code for required library files  

Simple build with one source file:

```
g++ test.cpp
```

This goes through each step and produces an executable file named a.out.

G++ can also be used to go through each step separately.

1. Preprocess
```
g++ -E test.cpp
```

Iostream code seems to be added to the source file at this point. Shouldn't it be a shared library?


