// main.c

#include <stdint.h>

#define UART0_BASE 0x10000000u
#define UART_THR   0x00u
#define UART_LSR   0x05u
#define LSR_THRE   (1u << 5)

static inline void mmio_write8(uint32_t addr, uint8_t v) {
    *(volatile uint8_t *)(uintptr_t)addr = v;
}

static inline uint8_t mmio_read8(uint32_t addr) {
    return *(volatile uint8_t *)(uintptr_t)addr;
}

static inline uint32_t mmio_read32(uint32_t addr) {
    return *(volatile uint32_t *)(uintptr_t)addr;
}

static void uart_putc(char c) {
    while ((mmio_read8(UART0_BASE + UART_LSR) & LSR_THRE) == 0) {}
    mmio_write8(UART0_BASE + UART_THR, (uint8_t)c);
}

static void uart_puts(const char *s) {
    for (; *s; s++) {
        if (*s == '\n') uart_putc('\r');
        uart_putc(*s);
    }
}

static void uart_puthex4(uint32_t v) {
    v &= 0xF;
    uart_putc((char)(v < 10 ? ('0' + v) : ('a' + (v - 10))));
}

static void uart_puthex32(uint32_t v) {
    uart_puts("0x");
    for (int i = 7; i >= 0; i--) {
        uart_puthex4(v >> (i * 4));
    }
}

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

void main(void) {
    // uart_puts("OK from RV32 bare metal!\n");
    uart_puts("A\0");

    /**
     * Deliberately trigger an illegal instruction trap to test trap handler.
     * The trap handler should show "mcause = 2" which means "Illegal instruction"
     */
    asm volatile (".word 0x00000000");

    for (;;) {
        asm volatile ("wfi");
    }
}
