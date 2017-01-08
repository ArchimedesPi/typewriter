#include "uart.h"

void uart_init() {
	/* USE_2X, UBRRH_VALUE, UBRRL_VALUE are defined by util/setbaud.h */
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
#if USE_2X
	UCSR0A |= (1 << U2X0);
#else
	UCSR0A &= ~(1 << U2X0);
#endif

	UCSR0B = (1<<RXEN0) | (1<<TXEN0); // enable RX + TX
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // 8-bit data
}

void uart_putchar(char c) {
	loop_until_bit_is_set(UCSR0A, UDRE0); // wait until TX buffer has emptied
	UDR0 = c; // send data
}

/* just a wrapper to make FDEV_SETUP_STREAM happy */
int uart_strm_putchar(char c, FILE *stream) {
	uart_putchar(c);
	return 0;
}

char uart_getchar() {
	loop_until_bit_is_set(UCSR0B, RXC0); // wait for incoming data
	return UDR0; // return read value
}

FILE uart0out = FDEV_SETUP_STREAM(uart_strm_putchar, NULL,
                                  _FDEV_SETUP_WRITE);

 
 
