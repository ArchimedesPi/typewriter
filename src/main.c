#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "util.h"
#include "uart.h"

int main() {
	sei(); // turn on interrupts
	uart0_init(UART_BAUD_SELECT(BAUD, F_CPU));

	return 0;
}
