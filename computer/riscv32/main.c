// main.c - minimal UART "hello" for QEMU virt (UART0 @ 0x10000000)

#include <stdint.h>

#define UART0_BASE 0x10000000u
#define UART_THR   0x00u  // Transmit Holding Register (write)
#define UART_LSR   0x05u  // Line Status Register
#define LSR_THRE   (1u << 5) // THR empty

static inline void mmio_write8(uint32_t addr, uint8_t v) {
    *(volatile uint8_t *)(uintptr_t)addr = v;
}

static inline uint8_t mmio_read8(uint32_t addr) {
    return *(volatile uint8_t *)(uintptr_t)addr;
}

static void uart_putc(char c) {
    // Wait until THR is empty
    while ((mmio_read8(UART0_BASE + UART_LSR) & LSR_THRE) == 0) {}
    mmio_write8(UART0_BASE + UART_THR, (uint8_t)c);
}

static void uart_puts(const char *s) {
    for (; *s; s++) {
        if (*s == '\n') uart_putc('\r');
        uart_putc(*s);
    }
}

void main(void) {
    uart_puts("OK from RV32 bare metal!\n");
    // for (;;) {
    //     //Wait For Interrupt
    //     asm volatile ("wfi");
    // }
}
