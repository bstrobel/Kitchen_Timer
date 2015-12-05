/*
 * sevenseg.h
 *
 * Created: 02.12.2015 09:36:55
 *  Author: Bernd
 */ 


#ifndef SEVENSEG_H_
#define SEVENSEG_H_

#include <avr/io.h>
#include <stdbool.h>

#define SEG_PORT PORTD
#define SEG_DDR DDRD
#define SEG_A PORTD7
#define SEG_B PORTD0
#define SEG_C PORTD1
#define SEG_D PORTD6
#define SEG_E PORTD5
#define SEG_F PORTD4
#define SEG_G PORTD3
#define SEG_DOT PORTD2

#ifndef DIGIT_PWM_REG
#	define DIGIT_PWM_REG OCR0
#endif

#define NUM_DIGITS 3
#define DIGIT_PORT PORTA
#define DIGIT_DDR DDRA
#define DIGIT_1_MSB	PORTA0
#define DIGIT_2 PORTA1
#define DIGIT_3_LSB PORTA2
#define ALL_DIGITS _BV(DIGIT_3_LSB) | _BV(DIGIT_2) | _BV(DIGIT_1_MSB)
#define ALL_DIGITS_OFF (DIGIT_PORT |= ALL_DIGITS)
#define DIGITS_PIN_MAP \
{ \
	_BV(DIGIT_3_LSB), \
	_BV(DIGIT_2), \
	_BV(DIGIT_1_MSB) \
}
extern volatile uint8_t bcd[NUM_DIGITS];
extern volatile uint8_t digits[NUM_DIGITS];

#define DEFAULT_BRIGHTNESS 127U
extern volatile uint8_t brightness;

extern uint8_t map_seven_seg_hex(uint8_t, bool);
extern void multiplex_next_digit(void);
extern void digits_init(void);
extern void int2bcd(uint16_t inp, uint8_t num_bits_in_input);

#define bcd2digits_supress_leading_zeros(dots) ({ \
	uint8_t preceeding_digit_bcd = 0; \
	for (int8_t i = NUM_DIGITS - 1; i >= 0; --i) \
	{ \
		if (bcd[i] || preceeding_digit_bcd || i == 0) \
		{ \
			digits[i] = map_seven_seg_hex(bcd[i],dots & (1 << i)); \
		} \
		else \
		{ \
			digits[i] = map_seven_seg_hex(0xff,dots & (1 << i)); \
		} \
		preceeding_digit_bcd = bcd[i]; \
	} \
})

#define bcd2digits(dots) ({ \
	for (int8_t i = NUM_DIGITS - 1; i >= 0; --i) \
	{ \
		digits[i] = map_seven_seg_hex(bcd[i],dots & (1 << i)); \
	} \
})

#endif /* SEVENSEG_H_ */