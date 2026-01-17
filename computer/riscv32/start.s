    .section .text.entry
    .globl _start
_start:
    la      sp, __stack_top

    /**
     * Set machine trap-vector base address
     * la = Load Address (pseudoinstruction)
     * mtvec = Machine Trap-Vector Base-Address
     * csrw = Control and Status Register Write
     */
    la      t0, trap_vector
    csrw    mtvec, t0

    /* Zero .bss */
    la      t0, __bss_start
    la      t1, __bss_end
1:
    bgeu    t0, t1, 2f
    sw      zero, 0(t0)
    addi    t0, t0, 4
    j       1b
2:
    call    main

hang:
    wfi
    j hang


    .align 4
    .globl trap_vector
trap_vector:
    /**
     * Save few temporaries (t0-t2, a0-a2, ra) so printing doesn’t instantly corrupt things
     * addi = add immediate 
     * sw = store word
     */
    addi    sp, sp, -32
    sw      ra, 28(sp)
    sw      t0, 24(sp)
    sw      t1, 20(sp)
    sw      t2, 16(sp)
    sw      a0, 12(sp)
    sw      a1, 8(sp)
    sw      a2, 4(sp)

    /* Read trap data from control and status registers */
    csrr    a0, mcause
    csrr    a1, mepc
    csrr    a2, mtval

    call    trap_c

    /* Restore and hang (trap_c should not return, but just in case) */
    lw      a2, 4(sp)
    lw      a1, 8(sp)
    lw      a0, 12(sp)
    lw      t2, 16(sp)
    lw      t1, 20(sp)
    lw      t0, 24(sp)
    lw      ra, 28(sp)
    addi    sp, sp, 32

trap_hang:
    wfi
    j trap_hang
