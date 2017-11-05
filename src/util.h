#pragma once

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#define UNUSED __attribute__ ((unused))

/* convienient numeric types */
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;

/* bit twiddling */
#ifndef _BV
	#define _BV(bit) (1<<bit)
#endif
#define set_bit(x, bit) (_SFR_BYTE(x) |= _BV(bit))
#define clear_bit(x, bit) (_SFR_BYTE(x) &= ~_BV(bit))
#define toggle_bit(x, bit) (_SFR_BYTE(x) ^= _BV(bit))
#define check_bit(x, bit) (_SFR_BYTE(x) & (1<<_BV(bit)))
#define is_set(x, bit) check_bit(x, bit)

static inline void delay_ms(uint16_t ms) {
	while (ms--) {
		_delay_ms(1);
	}
}
