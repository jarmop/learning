# x86 64-bit assembly

## How to run

```sh
./run.sh print.asm
```

## Some explanations

To run pure assembly programs, you need to run the program from the boot sector.

Simplest test program, the endless loop:

```
hang:
    jmp hang

    times 512-($-$$) db 0
```

The times prefix causes instruction to be assembled multiple times. "\$" evaluates to the beginning of the line and "\$\$" evaluates to the beginning of the current section; so you can tell how far into the section you are by using (\$-\$$). So "times 512-(\$-\$$) db 0" tells yasm to fill up zeros up to 512th
byte. [http://www.tortall.net/projects/yasm/manual/html/manual.html]

db = define byte (trying to use numbers larger than byte results in overflow)

Without zeros BIOS/UEFI won't recognize the boot sector. Is this behaviour
documented somewhere?

They say BIOS loads the boot code to address 0x0000:7C00 (32K - 1K). Why?

"The address 0000hex:7C00hex is the first byte of the 32nd KB of RAM. As a historical note, the loading of the boot program at this address was the obvious reason why, while the minimum RAM size of an original IBM PC (type 5150) was 16 KB, 32 KB were required for the disk option in the IBM XT"
[https://en.wikipedia.org/wiki/Master_boot_record#cite_note-NB_Load-address-40]

If one bootloader doesn't jump outside of BIOS (eg. into kernel), does BIOS run next boot device in order?

Assemble:

```sh
yasm -f bin boot.asm -o boot.bin
```

Hexdump of the resulting binary file can be inspected with xxd:

```sh
xxd boot.bin
```

Create disk image:

```sh
dd if=boot.bin of=boot.img
```

Now you can insert boot.img into a Floppy device of a Virtual Machine and see that the machine finds our boot sector and hangs (as told to) instead of giving FATAL when boot sector is missing.

N.B. To use optical drive you need to create ISO file which requires some special formatting to be recognized by machines.

## Vim shortcut for convenience

To enable running whole build process and starting virtual machine directly from vim editor, do the following:

Enable local .vimrc by adding following lines to ~/.vimrc:

```sh
set exrc
set secure
```

Now add following line to .vimrc in the directory where boot.asm resides:

```sh
map <C-R> :!./run.sh
```

Now you can run the script by pressing C-R + Enter.
