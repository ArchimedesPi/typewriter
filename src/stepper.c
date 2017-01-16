#include "stepper.h"
#include <stdlib.h>
#include "util.h"

void step(Stepper *self, int steps) {
	int steps_to_go = abs(steps);
	if (steps_to_go > 0) {
		self->step_num += (steps>0) ? 1 : -1;
		_doStep(self);
		steps_to_go--;
	}
}

void _doStep(Stepper *self) {
	switch (self->step_num % 4) {
	case 0: /* 0101 */
	case 2:
		SET_PIN(self->pin1, 0);
		SET_PIN(self->pin2, 1);
		SET_PIN(self->pin3, 0);
		SET_PIN(self->pin4, 1);
		break;
	case 1: /* 0110 */
		SET_PIN(self->pin1, 0);
		SET_PIN(self->pin2, 1);
		SET_PIN(self->pin3, 1);
		SET_PIN(self->pin4, 0);
		break;
	case 3: /* 1001 */
		SET_PIN(self->pin1, 1);
		SET_PIN(self->pin2, 0);
		SET_PIN(self->pin3, 0);
		SET_PIN(self->pin4, 1);
		break;
	}
}
