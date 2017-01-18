#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"
#include "util.h"
#include "uart.h"
#include "typewriter.h"

tw_state_t tw_state;

_pin_t statusLed = def_pin(B,PB5);

Stepper rollStepper;
Stepper carriageStepper;

int main() {
	sei(); // turn on interrupts
	uart0_init(UART_BAUD_SELECT(BAUD, F_CPU));

	/* Initialize steppers */
	stepper_init(&rollStepper,
		def_pin(C, PC5),
		def_pin(C, PC4),
		def_pin(C, PC3),
		def_pin(C, PC2));

	stepper_init(&carriageStepper,
		def_pin(C, PC1),
		def_pin(C, PC0),
		def_pin(B, PB4),
		def_pin(B, PB3));

	/* Initialize the typewriter. (Slews motors!) */
	tw_init(&tw_state, &rollStepper, NULL, NULL);
	// tw_home(&tw_state);

	while (true) {
		if (uart0_available() > 0) {
			char c = uart0_getc();
			switch (c) {
			case 0x05: { /* ENQ */
				uart0_putc(0x06);
				break;
			}

			case '\r': { /* Carriage return */
				tw_carriage_return(&tw_state);
				break;
			}

			case '\n': { /* Newline */
				tw_newline(&tw_state);
				break;
			}

			case '\b': { /* Backspace */
				break;
			}

			case 0x1B: { /* Escape */
				break;
			}

			default: /* Print character */
				break;
			}
		}
	}

	return 0;
}
