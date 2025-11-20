; move segment selector 0x07c0 in to data segment register       
    mov ax, 0x07c0   ; ax = 16-bit accumulator
    mov ds, ax       ; ds = 16-bit data segment register
;    mov ds, 0x07c0       ; ds = 16-bit data segment register
 
    mov si, msg      ; si = 16-bit source index (source of BIOS print?)
print_char:
    lodsb            ; loads string from si in to al
    or al, al        ; zero if al = 0 (?)
    jz hang          ; jz = jump if above instruction results in zero (i.e. no characters to print)
    mov ah, 0x0e     ; 0x0e function number for displaying character 
    int 0x10         ; Raise interrupt 0x10 (17th interrupt vector), BIOS video service(?)
                     ; calls function in ah register placed above (0x0e)
    jmp print_char
 
hang:
    jmp hang
 
msg db 'Hello World!', 13, 10 ; 13 = carriage return, 10 = new line
    times 510-($-$$) db 0 ; add extra bytes to make total file size 512B
    db 0x55, 0xAA         ; magic number to tell (old) BIOS this is a boot sector
