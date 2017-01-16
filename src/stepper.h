#pragma once
#include "pin.h"

typedef struct Stepper {
	int step_num;

	int speed; // steps/sec

	PIN(pin1); PIN(pin2); PIN(pin3); PIN(pin4);
} Stepper;

void _doStep(Stepper *self);
void step(Stepper *self, int steps);