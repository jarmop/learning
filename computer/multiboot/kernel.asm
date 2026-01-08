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
    mov edi, 0xB8000    ; store the VGA text buffer address into the destination index register
    mov eax, 0x0F20     ; VGA encoding: 0 => black background, F => white text, 20 => space character
    mov ecx, 80*25      ; Run the next loop once for each character spot on a VGA screen (80*25)

.clear:
    mov [edi], eax
    add edi, 2
    loop .clear

.hang:
    hlt
    jmp .hang
