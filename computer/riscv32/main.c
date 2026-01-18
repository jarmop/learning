#include <stdint.h>

#include "uart.h"
#include "fdt.h"

/**
 * mcause = error code
 * mepc   = Machine Exception Program Counter (pc value at error)
 * mtval  = Machine Trap VALue(exception-specific information to assist
 *          software in handling the trap)
 * Called from trap_vector in start.S 
 */
__attribute__((noreturn))
void trap_c(uint32_t mcause, uint32_t mepc, uint32_t mtval) {
    uart_puts("\n*** TRAP ***\nmcause=");
    uart_puthex32(mcause);
    uart_puts("\nmepc  =");
    uart_puthex32(mepc);
    uart_puts("\nmtval =");
    uart_puthex32(mtval);

    uart_puts("\ninsn  =");
    uart_puthex32(mmio_read32(mepc));

    uart_puts("\n");
    for (;;) asm volatile ("wfi");
}

void main(uint32_t hartid, const void *dtb) {
    uart_puts("hartid=");
    uart_puthex32(hartid);
    uart_puts(" dtb=");
    uart_puthex32((uint32_t)(uintptr_t)dtb);
    uart_puts("\n");

    fdt_dump(dtb);

    /**
     * Deliberately trigger an illegal instruction trap to test trap handler.
     * The trap handler should show "mcause = 2" which means "Illegal instruction"
     */
    // asm volatile (".word 0x00000000");

    for (;;) {
        asm volatile ("wfi");
    }
}
