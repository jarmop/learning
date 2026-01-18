#ifndef UART_H
#define UART_H

#include <stdint.h>

uint32_t mmio_read32(uint32_t addr);
void uart_putc(char c);
void uart_puts(const char *s);
void uart_putln();
void uart_putsln(const char *s);
void uart_puthex4(uint32_t v);
void uart_puthex32(uint32_t v);
void uart_putdec32(uint32_t v);
void uart_putbin32(uint32_t v);

#endif