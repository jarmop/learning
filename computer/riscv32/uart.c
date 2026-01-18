#include "uart.h"

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

inline uint32_t mmio_read32(uint32_t addr) {
    return *(volatile uint32_t *)(uintptr_t)addr;
}

void uart_putc(char c) {
    while ((mmio_read8(UART0_BASE + UART_LSR) & LSR_THRE) == 0) {}
    mmio_write8(UART0_BASE + UART_THR, (uint8_t)c);
}

void uart_puts(const char *s) {
    // Loop until NUL character
    for (; *s; s++) {
        if (*s == '\n') uart_putc('\r');
        uart_putc(*s);
    }
}

void uart_putln() {
    uart_putc('\n');
}

void uart_putsln(const char *s) {
    uart_puts(s);
    uart_putln();
}

void uart_putbin32(uint32_t v) {
    uart_puts("0b");
    int number_started = 0;
    for (int i = 31; i >= 0; i--) {
        int bit = v >> i;
        bit &= 0b1;
        if (bit > 0 || number_started) {
            uart_putc('0' + bit);
            number_started = 1;
        }
    }
}

static uint32_t udivr(uint32_t n, uint32_t d, short mod) {
    uint32_t q = 0;
    uint32_t r = 0;

    for (int i = 31; i >= 0; i--) {
        r <<= 1;
        r |= (n >> i) & 1;
        if (r >= d) {
            r -= d;
            q |= (1u << i);
        }
    }

    return mod ? r : q;
}

static uint32_t udiv(uint32_t n, uint32_t d) {
    return udivr(n, d, 0);
}

static uint32_t umod(uint32_t n, uint32_t d) {
    return udivr(n, d, 1);
}

void uart_putdec32(uint32_t v) {
    if (v <= 9) {
        uart_putc('0' + v);
        return;
    }
    int r = umod(v, 10);
    v = udiv(v, 10);
    if (v > 9) {
        uart_putdec32(v);
    } else {
        uart_putc('0' + v);
    }
    uart_putc('0' + r);
}

void uart_puthex4(uint32_t v) {
    v &= 0b1111; // Set bits that go beyond 4 to 0 (0001 1111 --> 0000 1111) 
    uart_putc((char)(v < 10 ? ('0' + v) : ('a' + (v - 10))));
}

void uart_puthex32(uint32_t v) {
    uart_puts("0x");
    for (int i = 7; i >= 0; i--) {
        uart_puthex4(v >> (i * 4));
    }
}