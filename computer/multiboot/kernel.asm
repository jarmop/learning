MB_MAGIC            equ 0x1BADB002
MB_FLAG_GRAPHICS    equ 0b100
MB_FLAGS            equ MB_FLAG_GRAPHICS
MB_CHECKSUM         equ -(MB_MAGIC + MB_FLAGS)

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
    dd 0                ; mode_type (0 = linear framebuffer)
    dd 1024             ; width
    dd 768              ; height
    dd 32               ; depth (bits per pixel)

section .bss
align 8
boot_fb_addr   resq 1
boot_fb_pitch  resd 1
boot_fb_width  resd 1
boot_fb_height resd 1
boot_fb_bpp    resd 1

section .text           ; Section containing the actual x86 instructions
global  _start          ; Ld expects there to be a global _start symbol

_start:
    mov esi, ebx                                ; ebx = multiboot_info

    mov eax, [esi + multiboot_info.fb_addr_low]
    mov [boot_fb_addr], eax
    mov dword [boot_fb_addr+4], 0

    mov eax, [esi + multiboot_info.fb_pitch]
    mov [boot_fb_pitch], eax

    mov eax, [esi + multiboot_info.fb_width]
    mov [boot_fb_width], eax

    mov eax, [esi + multiboot_info.fb_height]
    mov [boot_fb_height], eax

    mov eax, [esi + multiboot_info.fb_bpp]
    mov [boot_fb_bpp], eax

    mov edi, [boot_fb_addr]
    mov eax, [boot_fb_pitch]
    imul eax, 100
    add edi, eax
    add edi, 400
    mov dword [edi], 0x00FF0000

.hang:
    hlt
    jmp .hang
