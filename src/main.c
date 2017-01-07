#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>

int main() {
	DDRB  = (1<<PB5);
	PORTB = (1<<PB5);
	while (true) {
		PORTB ^= (1<<PB5);
		_delay_ms(1000);
	}
	return 0;
}
