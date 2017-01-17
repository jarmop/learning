; boot.asm
hang:
    jmp hang
 
    times 510-($-$$) db 0
    db 0x55, 0xAA
