#pragma once
#include "util.h"
#include "config.h"
#include "stepper.h"

typedef struct tw_state_t {
	// head position in spaces/newlines
	u8 col; u8 row;

	// daisywheel position as a character index
	u8 dw_char;

	/* the steppers */
	Stepper *carriage;
	Stepper *roll;
	Stepper *daisywheel;
} tw_state_t;

void tw_init(tw_state_t *self, Stepper *roll, Stepper *carriage, Stepper *daisywheel);
void tw_home(tw_state_t *self);
void tw_carriage_return(tw_state_t *self);
void tw_newline(tw_state_t *self);