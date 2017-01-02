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

## Header files

If code in one source file calls a function from another source file, the function must be declared in the first source file before it is called. The actual function definition can be after the call.

In most cases the declaration is placed in a separate header file which is then included where ever the declaration is needed.

User defined header files are surrounded in quotes.
```c
#include "header.h"
```
This tells compiler to look in the same directory where the source code is.

System header files are surrounded with arrows.
```c
#include <stdio.h>
```
This tells compiler to look in the standard list of system directories.

When compiling, both source files must be included in the command:
```
gcc test.c test2.c
```

Header file only has to be somewhere where compiler can find it. As default compiler searches from the directory where the source file is. Additional directories can be added with -I flag.
```
gcc -I lib test.c test2.c
```
