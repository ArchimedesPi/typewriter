#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>

#include "util.h"
#include "uart.h"

int main() {
	uart_init();
	stdin = &uart0in;
	stdout = &uart0out;

	return 0;
}
