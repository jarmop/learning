# Assembly tutorial

Assembly code differs based on various things, eg. OS, amount of bits, assembler.

Some popular assemblers: gas, nasm, yasm. Gas is part of gcc and yasm is a rewrite of nasm. Default gcc uses the executable file format appropriate for the machine where gcc is installed, eg. elf64 for linux. For yasm executable file format can be specified when compiling.

## Simple exit program

### linux 64, yasm + ld
code:
```
    segment .text
    global  _start
    
_start:
    mov eax,1   ; 1 is the exit syscall number
    mov ebx,5   ; the status value to return
    int 0x80 ; execute a system call
```
compile commands:
```
yasm -f elf64 exit-linux-64-ld.s
ld exit-linux-64.o
./a.out
```

### linux 64, yasm + gcc
code:
```
    segment .text
    global  main
    
main:
    mov eax,1   ; 1 is the exit syscall number
    mov ebx,5   ; the status value to return
    int 0x80 ; execute a system call
```
compile commands:
```
yasm -f elf64 exit-linux-64-gcc.s
gcc exit-linux-64.o
./a.out
```

### mac 64, as + gcc
code:
```
    .section __TEXT,__text
    .global  _main

_main:
    movl $0x2000001, %eax  # 1 is the exit syscall number
    movl $0, %ebx   # the status value to return
    syscall # execute a system call
```
compile commands:
```
as exit-mac-64-gcc.s -o exit-mac-64-gcc.o
gcc exit-mac-64-gcc.o
./a.out
```

"As" is a generic command for the default assembler on the given machine [https://en.wikipedia.org/wiki/As_(Unix)]. On my Mac it is Apple LLVM version 8.0.0. LLVM Assembler should be "extremely compatible with GAS" [http://clang.llvm.org/compatibility.html#inline-asm].
