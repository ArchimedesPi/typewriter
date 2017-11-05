#pragma once

#include "util.h"
#include <stdbool.h>

typedef struct _pin_t {
	volatile u8 *r_port;
	volatile u8 *r_ddr;
	volatile u8 *r_pin;
	u8 pin;
} _pin_t;

#define _def_pin(po, pi) (_pin_t){.r_port=&PORT##po,.r_ddr=&DDR##po,.r_pin=&PIN##po,.pin=pi}
#define def_pin(po, pi) _def_pin(po, pi)

#define set_pin(x, v) *x.r_port ^= (-v ^ *x.r_port) & _BV(x.pin)
#define get_pin(x) (!!(*x.r_pin & _BV(x.pin)))
#define set_output(x) *x.r_ddr |= _BV(x.pin)
#define set_input(x) *x.r_ddr &= ~_BV(x.pin)
#define set_input_pullup(x) set_input(x); set_pin(x, 1)