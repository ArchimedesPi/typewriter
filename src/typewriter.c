#include <stdio.h>
#include "typewriter.h"
#include "uart.h"
#include "util.h"
#include "config.h"


// starts at space/ascii 0x20
#define char_to_daisy(c) pgm_read_byte(&(CHAR_TO_DAISY_TABLE[(u8)c - 0x20]))
const u8 CHAR_TO_DAISY_TABLE[95] PROGMEM = {};

void tw_init(tw_state_t *self, Stepper *roll, Stepper *carriage, Stepper *daisywheel,
		     _pin_t touchoff)
{
	#ifdef DEBUG
	uart0_puts_P("tw_init()\n");
	#endif
	self->roll = roll;
	self->carriage = carriage;
	self->daisywheel = daisywheel;

	self->touchoff = touchoff;
	set_input_pullup(self->touchoff);
	
	// wherever we started up on the paper is defined as 0
	self->row = 0;
}

void tw_home(tw_state_t *self) {
	#ifdef DEBUG
	uart0_puts_P("tw_home()\n");
	#endif

	while (get_pin(self->touchoff)) { /* until the touchoff is bumped */
		step(self->carriage, -1); // step to the left
	}
	self->col = 0;

	while (!get_pin(self->touchoff)) { /* while the touchoff is pressed */
		step(self->daisywheel, -1);
	}
	self->dw_char = 0;
}

void tw_carriage_return(tw_state_t *self) {
	step(self->carriage, -CARRIAGE_SP_COL*self->col);
	#ifdef DEBUG
	uart0_puts_P("tw_carriage_return()\n");
	#endif

	self->col = 0;
}

void tw_newline(tw_state_t *self) {
	#ifdef DEBUG
	uart0_puts_P("tw_newline()\n");
	#endif

	step(self->roll, ROLL_SP_LINE);
	self->row++;
	stepper_float(self->roll);
}

void tw_space(tw_state_t *self) {
	#ifdef DEBUG
	uart0_puts_P("tw_space()\n");
	#endif

	step(self->carriage, CARRIAGE_SP_COL);
	self->col++;
}

void tw_backspace(tw_state_t *self) {
	#ifdef DEBUG
	uart0_puts_P("tw_backspace()\n");
	#endif

	step(self->carriage, -CARRIAGE_SP_COL);
	self->col--;	
}

void tw_putch(tw_state_t *self, char c) {
	#ifdef DEBUG
	printf_P(PSTR("tw_putch(%c)\n"), c);
	#endif

	// get daisywheel index
	u8 dw_idx = char_to_daisy(c);
	#ifdef DEBUG
	printf_P(PSTR("dw_idx = %d\n"), dw_idx);
	#endif
	while (self->dw_char != dw_idx) {
		i8 ds = (dw_idx < self->dw_char) ? -1 : 1;
		step(self->carriage, CARRIAGE_SP_COL * ds);
		self->dw_char += ds;
	}
}