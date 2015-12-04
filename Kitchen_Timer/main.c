/*
 * Mega32LED1.c
 *
 * Created: 23.11.2015 16:13:00
 * Author : Bernd
 */ 
#define F_CPU 8007680UL
#define DIGIT_PWM_REG OCR0

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdbool.h>
#include "sevenseg.h"
#include "debounced_keys.h"


#define NUM_BITS_IN_INPUT 10

#define DIGIT_TIMER_OVF_VECT TIMER0_OVF_vect
#define DIGIT_TIMER_COMP_VECT TIMER0_COMP_vect
#define DIGIT_TIMER_INIT { \
	TCCR0 = _BV(CS01); /* CS01 -> -> 8007680Hz/256/8 -> 3910Hz */ \
	TIFR &= ~(_BV(OCF0) | _BV(TOV0)); \
	TCNT0 = 0; \
	OCR0 = 0; \
	TIMSK |= _BV(TOIE0) | _BV(OCIE0); \
}

#define TIMER_TICK_COUNTER_MAX 391U
static uint16_t timer_tick_counter;
static uint16_t timer_counter;
static bool timer_stopped = false;

#define START_STOP_KEY 0

ISR(DIGIT_TIMER_OVF_VECT)
{
	PORTA &= ~_BV(PORTA4);
	if ((keys_changed_bitmap & _BV(START_STOP_KEY)) && (~keys_bitmap & _BV(START_STOP_KEY)))
	{
		keys_changed_bitmap &= ~_BV(START_STOP_KEY);
		timer_stopped = !timer_stopped;
	}
	if (!timer_stopped)
	{
		if (timer_tick_counter == TIMER_TICK_COUNTER_MAX)
		{
			timer_tick_counter = 0;
			if (timer_counter < 999)
			{
				timer_counter++;
			}
			else
			{
				timer_counter = 0;
			}
			int2bcd(timer_counter, NUM_BITS_IN_INPUT);
			bcd2digits(0b010);
		}
		else
		{
			timer_tick_counter++;
		}
	}
	multiplex_next_digit();
	handle_keys();
	PORTA |= _BV(PORTA4);
}

ISR(DIGIT_TIMER_COMP_VECT)
{
	ALL_DIGITS_OFF; // switch off for the rest of the counting period
}

int main(void)
{
	set_sleep_mode(SLEEP_MODE_IDLE);
	//sleep_bod_disable();
	sleep_enable();
	DDRA |= _BV(DDRA4);
	PORTA &= ~_BV(DDRA4);
	keys_init();
	digits_init();
	DIGIT_TIMER_INIT;
	sei();
	while (1)
	{
		sleep_cpu();
	}
}


