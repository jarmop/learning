; Prints the value matching the envname.
; If not found, prints nothing and exits.
;
; Some instruction explanations:
; je = jump if equal
; jne = jump if not equal
;
; register usage:
; r8    = envname (char*)
; r9    = compared_envname_ptr (char**)
; r10   = compared_envname (char*)
; r11   = char_i
; r12b  = envname_char (char)

global _start

section .text
_start:
    lea     r8, [rel envname]       ; r8 <-- envname (char*)

    ; ----- locate envp on the initial stack -----
    ; [rsp]             argc        (arg count)
    ; [rsp+8]           argv[0]     (arg values)
    ; [rsp+8*argc]      argv[argc-1]
    ; [rsp+8*(argc+1)]  NULL
    ; [rsp+8*(argc+2)]  envp[0]
    mov     r9, [rsp]               ; r9 <-- argc
    lea     r9, [rsp + 8*(r9+2)]    ; r9 <-- envp[0] (char**)

.next_env:
    mov     r10, [r9]               ; r10 = envp[i] (char*)
    cmp     r10, 0
    je      .exit                   ; end of envp => exit

    ; ----- compare the envname with an envname from the list -----
    mov     r11, 0                  ; r11 = index

.cmp_envname:
    ; Temporarily store the current r8 character to a register so it can be used as the first operand 
    ; of the cmp instruction. Must use one byte register because we are comparing single characters
    mov     r12b, [r8 + r11]        ; r12b = envname[r11]
    cmp     r12b, 0
    je      .found                  ; Reached the end of envname, so all characters have matched

    cmp     r12b, [r10 + r11]       ; compare characters
    jne     .no_match               ; Move to the next environment variable

    inc     r11
    jmp     .cmp_envname            ; Compare the next character in the current environment variable

.no_match:
    add     r9, 8                   ; next envp pointer
    jmp     .next_env

.found:
    ; r10 points to "[envname]=....", r11 = length of "[envname]="
    lea     r13,  [r10 + r11]       ; r13 = value pointer (after '=')

    ; ----- compute length of value (scan to NUL) -----
    mov     r11, 0
.len_loop:
    cmp     byte [r13 + r11], 0
    je      .write
    inc     r11
    jmp     .len_loop

.write:
    ; write(1, r13, r11)
    mov     eax, 1                  ; SYS_write
    mov     edi, 1                  ; fd = 1 (stdout)
    mov     rsi, r13                ; buf
    mov     rdx, r11                ; count
    syscall

.exit:
    mov     eax, 60                 ; SYS_exit
    mov     edi, 0                  ; status = 0
    syscall

section .rodata
envname: db "XDG_RUNTIME_DIR=", 0   ; Name of the environment variable + NUL character at the end