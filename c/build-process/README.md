# Build process

Building an executable file out of c source code is a four-step process.
1. Preprocess
  - copy included header files into the source code file
  - generate code by macros
  - replace symbolic constants (defined using #define) with their values
2. Compile
  - compile c code into assembly code
3. Assemble
  - assemble assembly code into object code
4. Link
  - link assembled object code with object code for required library files  

Simple build with one source file:

```
gcc test.c
```

This goes through each step and produces an executable file named a.out.

Flags can be set to stop build process after certain step

Stop after preprocess (prints to standard out)
```
gcc -E test.c
```

Stop after compiling to assembly code (creates test.s)
```
gcc -S test.c
```

Stop after assembling (creates test.o)
```
gcc -c test.c
```

