#include "stepper.h"
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "uart.h"

void stepper_init(Stepper *self, _pin_t pin1, _pin_t pin2, _pin_t pin3, _pin_t pin4) {
	self->pin1 = pin1;
	self->pin2 = pin2;
	self->pin3 = pin3;
	self->pin4 = pin4;

	set_output(self->pin1);
	set_output(self->pin2);
	set_output(self->pin3);
	set_output(self->pin4);	
}

void stepper_float(Stepper *self) {
	// deenergize all coils
	set_pin(self->pin1, false);
	set_pin(self->pin2, false);
	set_pin(self->pin3, false);
	set_pin(self->pin4, false);
}

void step_sp(Stepper *self, int steps, int del) {
	int steps_to_go = abs(steps);
	
	while (steps_to_go > 0) {
		self->step_num += (steps>0) ? 1 : -1;
		_doStep(self);
		delay_ms(del);
		steps_to_go--;
	}
}

void step(Stepper *self, int steps) {
	step_sp(self, steps, 10);
}

void _doStep(Stepper *self) {
	switch (self->step_num % 4) {
	case 0: /* 1010 */
		set_pin(self->pin1, true);
		set_pin(self->pin2, false);
		set_pin(self->pin3, true);
		set_pin(self->pin4, false);
		break;
	case 1: /* 0110 */
		set_pin(self->pin1, false);
		set_pin(self->pin2, true);
		set_pin(self->pin3, true);
		set_pin(self->pin4, false);
		break;
	case 2: /* 0101 */
		set_pin(self->pin1, false);
		set_pin(self->pin2, true);
		set_pin(self->pin3, false);
		set_pin(self->pin4, true);
		break;
	case 3: /* 1001 */
		set_pin(self->pin1, true);
		set_pin(self->pin2, false);
		set_pin(self->pin3, false);
		set_pin(self->pin4, true);
		break;
	}
}
