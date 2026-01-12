MB_MAGIC            equ 0x1BADB002
MB_FLAG_GRAPHICS    equ 0b100
MB_FLAGS            equ MB_FLAG_GRAPHICS
MB_CHECKSUM         equ -(MB_MAGIC + MB_FLAGS)
FB_WIDTH            equ 1024
FB_HEIGHT           equ 768
FB_BYTES_PER_PIXEL  equ 4
FB_BITS_PER_PIXEL   equ FB_BYTES_PER_PIXEL * 8
FB_PITCH            equ FB_BYTES_PER_PIXEL * FB_WIDTH

; When the graphics flag is set, the boot loader stores the address of this structure into the ebx register 
struc multiboot_info
    .flags           resd 1
    .mem_lower       resd 1
    .mem_upper       resd 1
    .boot_device     resd 1
    .cmdline         resd 1
    .mods_count      resd 1
    .mods_addr       resd 1
    .syms            resd 4
    .mmap_length     resd 1
    .mmap_addr       resd 1
    .drives_length   resd 1
    .drives_addr     resd 1
    .config_table    resd 1
    .boot_loader     resd 1
    .apm_table       resd 1
    .vbe_control     resd 1
    .vbe_mode        resd 1
    .vbe_interface   resd 2
    .fb_addr_low     resd 1
    .fb_addr_high    resd 1
    .fb_pitch        resd 1
    .fb_width        resd 1
    .fb_height       resd 1
    .fb_bpp          resd 1
    .fb_type         resd 1
endstruc

; The header for Multiboot. Must be aligned in 4 bytes. dd = double word (4 bytes).
; Three required fields: magic, flags, and checksum.
section .multiboot
align 4
    dd MB_MAGIC         ; Magic number identifying the header
    dd MB_FLAGS         ; Flags indicate which boot loader features the image is expecting
    dd MB_CHECKSUM      ; "Checksum" + "other fields" must equal 0

    ; padding for unused address fields (offsets 12–28)
    dd 0                ; header_addr
    dd 0                ; load_addr
    dd 0                ; load_end_addr
    dd 0                ; bss_end_addr
    dd 0                ; entry_addr

    ; graphics fields (offsets 32-44)
    dd 0                    ; mode_type (0 = linear framebuffer)
    dd FB_WIDTH             ; width
    dd FB_HEIGHT            ; height
    dd FB_BITS_PER_PIXEL    ; depth (bits per pixel)

section .text           ; Section containing the actual x86 instructions
global  _start          ; Ld expects there to be a global _start symbol

_start:
    ;cli
    mov esp, stack_top

; Draw pixel
    ; Copy framebuffer address from multiboot to kernel
    mov esi, ebx                                ; ebx is pointing to multiboot_info
    mov eax, [esi + multiboot_info.fb_addr_low] ; eax = the low 32 bits of the fb address
    mov [boot_fb_addr], eax                     ; boot_fb_addr 32 low bits = the whole fb address (32 bits is enough)
    mov dword [boot_fb_addr+4], 0               ; boot_fb_addr 32 high bits = 0 (ensure that resq is fully initialized)

    mov edi, [boot_fb_addr]     ; edi points to the beginning of the framebuffer (top-left corner)

    ; Move edi 100 pixels along the y-axis
    mov eax, FB_PITCH   ; eax holds bytes per row
    imul eax, 100       ; eax holds bytes per 100 rows
    add edi, eax        ; edi points to the beginning of the 100th row

    ; Move edi 100 pixels along the x-axis
    mov eax, FB_BYTES_PER_PIXEL ; eax holds bytes per pixel
    imul eax, 100               ; eax holds bytes per 100 pixels
    add edi, eax                ; edi points to the 100th pixel at the 100th row

    mov dword [edi], 0x0000FF00 ; give green color to the pixel pointed by edi
; End draw pixel

    lgdt [gdt64_descriptor] ; A2.2 Load GDT

; A3.2 Build identity mapping (0–2MB)
    ; PD[0] --> 2 MiB identity page
    mov eax, 0x00000000
    or eax, 0x83              ; P | RW | PS
    mov dword [pd], eax
    mov dword [pd + 4], 0

    ; PDPT[0] --> PD
    mov eax, pd
    or eax, 0x03              ; P | RW
    mov dword [pdpt], eax
    mov dword [pdpt + 4], 0

    ; PML4[0] → PDPT
    mov eax, pdpt
    or eax, 0x03              ; P | RW
    mov dword [pml4], eax
    mov dword [pml4 + 4], 0
; A3.2 End

; A3.3 Enable long mode (strict order). CR = control register
    mov eax, pml4
    mov cr3, eax

    mov eax, cr4
    or eax, 1 << 5      ; PAE
    mov cr4, eax

    mov ecx, 0xC0000080 ; EFER
    rdmsr
    or eax, 1 << 8      ; LME
    wrmsr

    mov eax, cr0
    or eax, 1 << 31     ; PG
    mov cr0, eax
; End A3.3

.hang:
    hlt
    jmp .hang

; A2.1 GDT (required for long mode)
section .rodata
align 8
gdt64:
    dq 0                      ; null
    dq 0x00AF9A000000FFFF     ; 64-bit code
    dq 0x00AF92000000FFFF     ; 64-bit data

gdt64_descriptor:
    dw gdt64_descriptor - gdt64 - 1
    dd gdt64
; End A2.1

section .bss
alignb 8
boot_fb_addr: resq 1

; Stack
alignb 16
stack_bottom: resb 16384
stack_top:

; A3.1 Page tables (identity map first)
alignb 4096
pml4: resq 512
pdpt: resq 512
pd:   resq 512
; End A3.1