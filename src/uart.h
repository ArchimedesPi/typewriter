#ifndef __UART_H__
#define __UART_H__

#include <stdio.h>
#include <avr/io.h>
#include <util/setbaud.h>
#include "util.h"

void uart_init();

void uart_putchar(char c);
char uart_getchar();

int uart_strm_putchar(char c, FILE *stream);
int uart_strm_getchar(FILE *stream);

FILE uart0out;
FILE uart0in;
FILE uart0io;

#endif
