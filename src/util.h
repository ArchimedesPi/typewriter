#ifndef __UTIL_H__
#define __UTIL_H__

#define UNUSED __attribute__ ((unused))

/* convienient arithmetic types */
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

#endif
