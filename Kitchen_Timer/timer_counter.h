/*
 * timer_counter.h
 *
 * Created: 05.12.2015 13:08:13
 *  Author: Bernd
 */ 


#ifndef TIMER_COUNTER_H_
#define TIMER_COUNTER_H_

#include <stdint.h>
#include <stdbool.h>

extern int8_t hours;
extern int8_t minutes_tens;
extern int8_t minutes_ones;
extern int8_t seconds_tens;
extern int8_t seconds_ones;
extern int8_t tenth_seconds;

typedef enum { UP, DOWN } count_direction_t;
typedef enum { MIN_1=1, MIN_5=5, MIN_10=10, MIN_30=30, HOUR=60} adjust_steps_t;

extern void zero_timer(void);
extern bool count_tenth_secs(count_direction_t);
extern bool adjust_timer(count_direction_t d, adjust_steps_t step);
extern void put_time_in_bcd(void);

#endif /* TIMER_COUNTER_H_ */