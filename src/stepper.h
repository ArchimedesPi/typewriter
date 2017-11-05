#pragma once

#include "pin.h"

typedef struct Stepper {
	u8 step_num;

	int speed; // steps/sec

	_pin_t pin1, pin2, pin3, pin4;
} Stepper;

void stepper_init(Stepper *self, _pin_t pin1, _pin_t pin2, _pin_t pin3, _pin_t pin4);

void _doStep(Stepper *self);
void step(Stepper *self, int steps);
void stepper_float(Stepper *self);