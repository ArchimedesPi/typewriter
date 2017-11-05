#include <stdio.h>
#include "typewriter.h"
#include "uart.h"
#include "util.h"
#include "config.h"


// starts at !/ascii 0x21
#define char_to_daisy(c) pgm_read_byte(&(CHAR_TO_DAISY_TABLE[(u8)c - 0x21]))
const u8 CHAR_TO_DAISY_TABLE[94] PROGMEM = {
	52, 22, 57, 53, 43, 40, 96, 38, 39, 47, 48, 18, 21, 16, 44,
	23, 61, 27, 30, 31, 26, 34, 35, 28, 29, 37, 42, -1, 45, -1,
	41, 55, 86, 79, 89, 78, 71, 68, 76, 77, 84, 80, 67, 92, 83,
	74, 82, 75, 73, 81, 88, 91, 87, 70, 85, 90, 72, 69, 58, -1,
	54, 50, 46, 60, 3, 14, 12, 1, 6, 94, 95, 9, 5, 33, 19,
	11, 17, 7, 4, 36, 15, 93, 10, 8, 2, 20, 0, 25, 13, 32,
	-1, -1, -1, 63,
};

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
	// float
	stepper_float(self->daisywheel);
	stepper_float(self->carriage);
}

void tw_carriage_return(tw_state_t *self) {
	#ifdef DEBUG
	uart0_puts_P("tw_carriage_return()\n");
	#endif

	step_sp(self->carriage, -CARRIAGE_SP_COL*self->col, 8);
	self->col = 0;
	stepper_float(self->carriage);
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
	stepper_float(self->carriage);
}

void tw_backspace(tw_state_t *self) {
	#ifdef DEBUG
	uart0_puts_P("tw_backspace()\n");
	#endif

	step(self->carriage, -CARRIAGE_SP_COL);
	self->col--;	
	stepper_float(self->carriage);
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
	if (dw_idx == -1) {
		#ifdef DEBUG
		uart0_puts_P("not on daisywheel");
		#endif
		return;
	}

	while (self->dw_char != dw_idx) {
		i8 ds = (dw_idx < self->dw_char) ? -1 : 1;
		step(self->carriage, CARRIAGE_SP_COL * ds);
		self->dw_char += ds;
	}
	// release
	stepper_float(self->daisywheel);
}