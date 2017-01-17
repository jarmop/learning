    mov al, 0x65     ; character to print
	mov ah, 0x0e     ; 0x0e function number for displaying character 
    int 0x10         ; Raise interrupt 0x10 (17th interrupt vector), BIOS video service(?)
                     ; calls function in ah register placed above (0x0e)
hang:
    jmp hang
 
    times 510-($-$$) db 0 ; add extra bytes to make total file size 512B
    db 0x55, 0xAA         ; magic number to tell (old) BIOS this is a boot sector
