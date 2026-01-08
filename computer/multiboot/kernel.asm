MB_MAGIC    equ 0x1BADB002
MB_FLAGS    equ 0
MB_CHECKSUM equ -(MB_MAGIC + MB_FLAGS)

; The header for Multiboot. Must be aligned in 4 bytes. dd = double word (4 bytes).
; Three required fields: magic, flags, and checksum.
section .multiboot
align 4
    dd MB_MAGIC         ; Magic number identifying the header
    dd MB_FLAGS         ; Flags indicate which boot loader features the image is expecting
    dd MB_CHECKSUM      ; "Checksum" + "other fields" must equal 0

section .text           ; Section containing the actual x86 instructions
global  _start          ; Ld expects there to be a global _start symbol

_start:
    mov edi, 0xB8000    ; VGA text buffer
    mov eax, 0x1F41     ; 'A' with bright white on blue
    mov [edi], eax      ; write character

.hang:
    hlt
    jmp .hang
