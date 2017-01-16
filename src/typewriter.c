#include "typewriter.h"

void tw_init(tw_state_t *self) {
	// wherever we started up on the paper is defined as 0
	self->row = 0;

	while (!is_set(PORTB, PB1)) { /* until the touchoff is bumped */
		step(self->carriage, -1); // step to the left
	}
	self->col = 0;

	while (is_set(PORTB, PB1)) { /* while the touchoff is pressed */
		step(self->daisywheel, -1);
	}
	self->dw_char = 0;
}

void tw_carriage_return(tw_state_t *self) {
	step(self->carriage, -CARRIAGE_SP_COL*self->col);
	self->col = 0;
}

void tw_newline(tw_state_t *self) {
	step(self->roll, ROLL_SP_LINE);
	self->row++;
}