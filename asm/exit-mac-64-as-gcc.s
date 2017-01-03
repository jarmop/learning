    .section __TEXT,__text
    .global  _main
    
_main:
    movl $0x2000001, %eax  # 1 is the exit syscall number
    movl $0, %ebx   # the status value to return
    syscall # execute a system call
