/*
 * sevenseg.c
 *
 * Created: 02.12.2015 09:37:08
 *  Author: Bernd
 */

#include "sevenseg.h"
#include <avr/io.h>
#include <stdbool.h>

volatile uint8_t bcd[NUM_DIGITS];
volatile uint8_t digits[NUM_DIGITS];
uint8_t digits_pinmap[NUM_DIGITS] = DIGITS_PIN_MAP;
volatile uint8_t selected_digit_idx;
volatile uint8_t brightness;

uint8_t map_seven_seg_hex(uint8_t hex, bool dot)
{
	uint8_t bw_dot = 0;
	if (dot)
	{
		bw_dot = _BV(SEG_DOT);
	}
	if (hex == 0xff)
	{
		if (dot)
		{
			return _BV(SEG_A) | _BV(SEG_B) | _BV(SEG_C) | _BV(SEG_D) | _BV(SEG_E) | _BV(SEG_F) | bw_dot;
		}
		else
		{
			return 0;
		}
		
	}
	switch (hex & 0xf)
	{
		case 0: return _BV(SEG_A) | _BV(SEG_B) | _BV(SEG_C) | _BV(SEG_D) | _BV(SEG_E) | _BV(SEG_F) | bw_dot;
		case 1: return _BV(SEG_B) | _BV(SEG_C) | bw_dot;
		case 2: return _BV(SEG_A) | _BV(SEG_B) | _BV(SEG_G) | _BV(SEG_D) | _BV(SEG_E) | bw_dot;
		case 3: return _BV(SEG_A) | _BV(SEG_B) | _BV(SEG_C) | _BV(SEG_D) | _BV(SEG_G) | bw_dot;
		case 4: return _BV(SEG_B) | _BV(SEG_C) | _BV(SEG_G) | _BV(SEG_F) | bw_dot;
		case 5: return _BV(SEG_A) | _BV(SEG_C) | _BV(SEG_D) | _BV(SEG_F) | _BV(SEG_G) | bw_dot;
		case 6: return _BV(SEG_A) | _BV(SEG_C) | _BV(SEG_D) | _BV(SEG_E) | _BV(SEG_F) | _BV(SEG_G) | bw_dot;
		case 7: return _BV(SEG_A) | _BV(SEG_B) | _BV(SEG_C) | bw_dot;
		case 8: return _BV(SEG_A) | _BV(SEG_B) | _BV(SEG_C) | _BV(SEG_D) | _BV(SEG_E) | _BV(SEG_F) | _BV(SEG_G) | bw_dot;
		case 9: return _BV(SEG_A) | _BV(SEG_B) | _BV(SEG_C) | _BV(SEG_D) | _BV(SEG_F) | _BV(SEG_G) | bw_dot;
		case 0xa: return _BV(SEG_A) | _BV(SEG_B) | _BV(SEG_C) | _BV(SEG_E) | _BV(SEG_F) | _BV(SEG_G) | bw_dot;
		case 0xb: return _BV(SEG_C) | _BV(SEG_D) | _BV(SEG_E) | _BV(SEG_F) | _BV(SEG_G) | bw_dot;
		case 0xc: return _BV(SEG_A) | _BV(SEG_D) | _BV(SEG_E) | _BV(SEG_F) | bw_dot;
		case 0xd: return _BV(SEG_B) | _BV(SEG_C) | _BV(SEG_D) | _BV(SEG_E) | _BV(SEG_G) | bw_dot;
		case 0xe: return _BV(SEG_A) | _BV(SEG_D) | _BV(SEG_E) | _BV(SEG_F) | _BV(SEG_G) | bw_dot;
		case 0xf: return _BV(SEG_A) | _BV(SEG_E) | _BV(SEG_F) | _BV(SEG_G) | bw_dot;
		default: return  _BV(SEG_A) | _BV(SEG_D) | _BV(SEG_G) | bw_dot;
	}
}

void handle_digit_shift()
{
	DIGIT_PWM_REG = brightness;
	ALL_DIGITS_OFF; // switch off before changing segments
	selected_digit_idx++;
	if (selected_digit_idx >= NUM_DIGITS)
	{
		selected_digit_idx = 0;
	}
	SEG_PORT = digits[selected_digit_idx];
	DIGIT_PORT &= ~(digits_pinmap[selected_digit_idx]);
}

void digits_init()
{
	SEG_DDR = 0xff; // every bit is output
	SEG_PORT = 0x0; // all segments off
	DIGIT_DDR |= ALL_DIGITS;
	DIGIT_PORT |= ALL_DIGITS;
	brightness = DEFAULT_BRIGHTNESS;
	DIGIT_PWM_REG = brightness;
}
