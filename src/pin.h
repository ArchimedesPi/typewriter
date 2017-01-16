#pragma once
#include "util.h"

#define PIN(x) u8* x##_port; const u8 x##_pin

#define SET_PIN(x, v) *x##_port ^= (-v ^ *x##_port) & _BV(x##_pin);
