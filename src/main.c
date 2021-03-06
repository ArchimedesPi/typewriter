#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"
#include "util.h"
#include "uart.h"
#include "typewriter.h"

static int uart_putchar(char c, FILE *stream) {
	uart0_putc(c);
	return 0;
}

static FILE stdout_ = FDEV_SETUP_STREAM(uart_putchar, NULL,
                                         _FDEV_SETUP_WRITE);

tw_state_t tw_state;

_pin_t statusLed = def_pin(B,PB5);

Stepper rollStepper;
Stepper carriageStepper;
Stepper daisywheelStepper;
_pin_t touchoff;

int main() {
	stdout = &stdout_;

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

	stepper_init(&daisywheelStepper,
		def_pin(B, PB2),
		def_pin(B, PB1),
		def_pin(D, PD7),
		def_pin(B, PB0));

	touchoff = def_pin(D, PD6);

	/* Initialize the typewriter. (Slews motors!) */
	tw_init(&tw_state, &rollStepper, &carriageStepper, &daisywheelStepper,
				touchoff);
	tw_home(&tw_state);

	while (true) {
		if (uart0_available() > 0) {
			char c = uart0_getc();
			switch (c) {
			case 0x05: { /* ENQ */
				uart0_putc(0x06); // ACK
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

			case 0x1B: { /* Escape */
				// TODO: slurp escape code, branch and execute
				break;
			}

			case ' ': { /* Space */
				tw_space(&tw_state);
				break;
			}

			case '\b': { /* Backspace */
				tw_backspace(&tw_state);
				break;
			}

			default: /* Print character */
				tw_putch(&tw_state, c); // print character
				tw_space(&tw_state); // step one space ahead on the paper
				break;
			}
		}
	}

	return 0;
}
