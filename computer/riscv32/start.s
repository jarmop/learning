/* start.S - minimal RV32 reset/entry */

    .section .text.entry
    .globl _start
_start:
    /* Set stack pointer */
    la      sp, __stack_top

    /* Optional: set trap vector to a simple handler */
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
    /* Call C main() */
    call    main

    /* If main returns, just idle */
hang:
    wfi
    j hang

    .align 4
trap_vector:
    /* Minimal trap handler: loop forever */
    j trap_vector
