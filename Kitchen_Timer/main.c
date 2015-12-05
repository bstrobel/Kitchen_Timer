/*
 * main.c
 *
 * Created: 23.11.2015 16:13:00
 * Author : Bernd
 */ 
#define F_CPU 8007680UL
#define DIGIT_PWM_REG OCR0

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdbool.h>
#include "sevenseg.h"
#include "debounced_keys.h"
#include "timer_counter.h"

#define TIMER0_INIT { \
	TCCR0 = _BV(CS01); /* CS01 -> -> 8007680Hz/256/8 -> 3910Hz */ \
	TIFR &= ~(_BV(OCF0) | _BV(TOV0)); \
	TCNT0 = 0; \
	OCR0 = 0; \
	TIMSK |= _BV(TOIE0) | _BV(OCIE0); \
}
#define TIMER0_TICK_COUNTER_MAX 391U

#define TIMER1_INIT { \
}

#define NUM_BITS_IN_INPUT 10

#define DIGIT_TIMER_OVF_VECT TIMER0_OVF_vect
#define DIGIT_TIMER_COMP_VECT TIMER0_COMP_vect
#define DIGIT_TIMER_INIT TIMER0_INIT
#define TIMER_TICK_COUNTER_MAX TIMER0_TICK_COUNTER_MAX

#define TIMER_OVF_MONITOR_PORT PORTA
#define TIMER_OVF_MONITOR_DDR DDRA
#define TIMER_OVF_MONITOR_BIT PORTA4

#define TIMER_COMP_MONITOR_PORT PORTA
#define TIMER_COMP_MONITOR_DDR DDRA
#define TIMER_COMP_MONITOR_BIT PORTA5

static uint16_t timer_tick_counter;
static bool timer_stopped = true;

#define START_STOP_KEY 0

ISR(DIGIT_TIMER_OVF_VECT)
{
	TIMER_OVF_MONITOR_PORT &= ~_BV(TIMER_OVF_MONITOR_BIT);
	TIMER_COMP_MONITOR_PORT &= ~_BV(TIMER_COMP_MONITOR_BIT);
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
			if (count_tenth_secs(UP))
			{
				timer_stopped = true;
			}
		}
		else
		{
			timer_tick_counter++;
		}
	}
	multiplex_next_digit();
	handle_keys();
	TIMER_OVF_MONITOR_PORT |= _BV(TIMER_OVF_MONITOR_BIT);
}

ISR(DIGIT_TIMER_COMP_VECT)
{
	TIMER_COMP_MONITOR_PORT |= _BV(TIMER_COMP_MONITOR_BIT);
	ALL_DIGITS_OFF; // switch off for the rest of the counting period
}

int main(void)
{
	uint8_t osccal = eeprom_read_byte(0);
	if (osccal != 0 || osccal != 0xFF)
	{
		OSCCAL = osccal;
	}
	set_sleep_mode(SLEEP_MODE_IDLE);
	//sleep_bod_disable();
	sleep_enable();
	TIMER_OVF_MONITOR_DDR |= _BV(TIMER_OVF_MONITOR_BIT);
	TIMER_OVF_MONITOR_PORT &= ~_BV(TIMER_OVF_MONITOR_BIT);
	TIMER_COMP_MONITOR_DDR |= _BV(TIMER_COMP_MONITOR_BIT);
	TIMER_COMP_MONITOR_PORT &= ~_BV(TIMER_COMP_MONITOR_BIT);
	
	zero_timer();
	//minutes_ones = 1;
	//seconds_ones = 5;
	keys_init();
	digits_init();
	DIGIT_TIMER_INIT;
	sei();
	while (1)
	{
		put_time_in_bcd();
		sleep_cpu();
	}
}


