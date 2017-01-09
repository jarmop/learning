# x86 64-bit assembly

To run pure assembly programs, you need to run the program from the boot sector.

Simplest test program, the endless loop:
```
hang:
    jmp hang

    times 512-($-$$) db 0
```

The times prefix causes instrucion to be assembled multiple times. "$" evaluates to the beginning of the line and "$$" evaluates to the beginning of the current section; so you can tell how far into the section you are by using ($-$$). So "times 512-($-$$) db 0" tells yasm to fill up zeros up to 512th byte. [http://www.tortall.net/projects/yasm/manual/html/manual.html]

(Without zeros BIOS/UEFI won't recognize the boot sector. Is this behaviour documented somewhere?)

Assemble:
```sh
yasm -f bin boot.asm -o boot.bin
```

Hexdump of the resulting binary file can be inspected with xxd:
```sh
cat boot.bin | xxd
```

Create disk image:
```sh
dd if=boot.bin of=boot.img
```

Now you can insert boot.img into a Floppy device of a Virtual Machine and see that the machine finds our boot sector and hangs (as told to) instead of giving FATAL when boot sector is missing. 

N.B. To use optical drive you need to create ISO file which requires some special formatting to be recognized by machines. 
