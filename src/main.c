#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>

#include "util.h"
#include "uart.h"

int main() {
	uart_init();
	stdout = &uart0out;

	while (true) {
		printf("hello world!\n");
	}
	return 0;
}
