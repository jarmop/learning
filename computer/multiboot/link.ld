ENTRY(_start)

SECTIONS
{
    . = 0x00100000;

    .multiboot ALIGN(4) :
    {
        *(.multiboot)
    }

    .text ALIGN(4) :
    {
        *(.text*)
    }

    .data ALIGN(4) :
    {
        *(.data*)
    }

    .bss ALIGN(4) :
    {
        *(.bss*)
        *(COMMON)
    }
}
