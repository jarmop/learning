/* linker.ld - minimal RV32 "virt" layout */

OUTPUT_ARCH(riscv)
ENTRY(_start)

/* QEMU virt RAM typically starts at 0x80000000 */
MEMORY
{
  RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 16M
}

SECTIONS
{
  . = ORIGIN(RAM);

  .text : ALIGN(4)
  {
    KEEP(*(.text.entry))
    *(.text .text.*)
  } > RAM

  .rodata : ALIGN(4)
  {
    *(.rodata .rodata.*)
  } > RAM

  .data : ALIGN(4)
  {
    *(.data .data.*)
  } > RAM

  .bss : ALIGN(4)
  {
    __bss_start = .;
    *(.bss .bss.*)
    *(COMMON)
    __bss_end = .;
  } > RAM

  /* Simple stack at the top of RAM */
  __stack_top = ORIGIN(RAM) + LENGTH(RAM);
}