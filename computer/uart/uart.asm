BITS 64
GLOBAL _start

SECTION .text

_start:
    ; Set up a stack
    lea rsp, [stack_top]

    ; Initialize UART (COM1, 0x3F8)
    mov dx, 0x3F8 + 1
    mov al, 0x00
    out dx, al        ; Disable interrupts

    mov dx, 0x3F8 + 3
    mov al, 0x80
    out dx, al        ; Enable DLAB

    mov dx, 0x3F8 + 0
    mov al, 0x03
    out dx, al        ; Divisor low  (38400 baud)

    mov dx, 0x3F8 + 1
    mov al, 0x00
    out dx, al        ; Divisor high

    mov dx, 0x3F8 + 3
    mov al, 0x03
    out dx, al        ; 8 bits, no parity, one stop

    mov dx, 0x3F8 + 2
    mov al, 0xC7
    out dx, al        ; Enable FIFO

    mov dx, 0x3F8 + 4
    mov al, 0x0B
    out dx, al        ; IRQs enabled, RTS/DSR set

    ; Print string
    lea rsi, [msg]

.print:
    lodsb
    test al, al
    jz .halt

.wait:
    mov dx, 0x3F8 + 5
    in al, dx
    test al, 0x20
    jz .wait

    mov dx, 0x3F8
    out dx, al
    jmp .print

.halt:
    hlt
    jmp .halt

SECTION .rodata
msg db "Hello from bare metal UART!", 10, 0

SECTION .bss
align 16
stack resb 4096
stack_top:
