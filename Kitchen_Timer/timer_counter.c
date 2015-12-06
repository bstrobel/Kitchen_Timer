/*
 * timer_counter.c
 *
 * Created: 05.12.2015 13:09:19
 *  Author: Bernd
 */ 
#include <stdint.h>
#include <stdbool.h>
#include "timer_counter.h"
#include "sevenseg.h"

int8_t hours;
int8_t minutes_tens;
int8_t minutes_ones;
int8_t seconds_tens;
int8_t seconds_ones;
int8_t tenth_seconds;

void zero_timer()
{
	hours = 0;
	minutes_tens = 0;
	minutes_ones = 0;
	seconds_tens = 0;
	seconds_ones = 0;
	tenth_seconds = 0;
}

static bool count_hours_int(count_direction_t dir)
{
	switch(dir)
	{
		case UP:
		{
			hours++;
			if (hours>9)
			{
				hours = 9;
				return true;
			}
			return false;
		}
		case DOWN:
		{
			hours--;
			if (hours<0)
			{
				hours = 0;
				return true;
			}
			return false;
		}
	}
	return false;
}

static bool count_minutes_tens_int(count_direction_t dir)
{
	switch(dir)
	{
		case UP:
		{
			minutes_tens++;
			if (minutes_tens>5)
			{
				if (count_hours_int(dir))
				{
					minutes_tens = 5;
					return true;
				}
				minutes_tens = 0;
			}
			return false;
		}
		case DOWN:
		{
			minutes_tens--;
			if (minutes_tens<0)
			{
				if (count_hours_int(dir))
				{
					minutes_tens=0;
					return true;
				}
				minutes_tens=5;
			}
			return false;
		}
	}
	return false;
}

static bool count_minutes_ones_int(count_direction_t dir)
{
	switch(dir)
	{
		case UP:
		{
			minutes_ones++;
			if (minutes_ones>9)
			{
				if (count_minutes_tens_int(dir))
				{
					minutes_ones = 9;
					return true;
				}
				minutes_ones = 0;
			}
			return false;
		}
		case DOWN:
		{
			minutes_ones--;
			if (minutes_ones<0)
			{
				if (count_minutes_tens_int(dir))
				{
					minutes_ones = 0;
					return true;
				}
				minutes_ones=9;
			}
			return false;
		}
	}
	return false;
}

static bool count_seconds_tens_int(count_direction_t dir)
{
	switch(dir)
	{
		case UP:
		{
			seconds_tens++;
			if (seconds_tens>5)
			{
				if (count_minutes_ones_int(dir))
				{
					seconds_tens = 5;
					return true;
				}
				seconds_tens = 0;
			}
			return false;
		}
		case DOWN:
		{
			seconds_tens--;
			if (seconds_tens<0)
			{
				if (count_minutes_ones_int(dir))
				{
					seconds_tens = 0;
					return true;
				}
				seconds_tens=5;
			}
			return false;
		}
	}
	return false;
}

static bool count_seconds_ones_int(count_direction_t dir)
{
	switch(dir)
	{
		case UP:
		{
			seconds_ones++;
			if (seconds_ones>9)
			{
				if (count_seconds_tens_int(dir))
				{
					seconds_ones = 9;
					return true;
				}
				seconds_ones = 0;
			}
			return false;
		}
		case DOWN:
		{
			seconds_ones--;
			if (seconds_ones<0)
			{
				if (count_seconds_tens_int(dir))
				{
					seconds_ones = 0;
					return true;
				}
				seconds_ones=9;
			}
			return false;
		}
	}
	return false;
}

bool count_tenth_secs(count_direction_t dir)
{
	switch(dir)
	{
		case UP:
		{
			tenth_seconds++;
			if (tenth_seconds>9)
			{
				if (count_seconds_ones_int(dir))
				{
					tenth_seconds = 9;
				}
				tenth_seconds = 0;
			}
			return false;
		}
		case DOWN:
		{
			tenth_seconds--;
			if (tenth_seconds<0)
			{
				if (count_seconds_ones_int(dir))
				{
					tenth_seconds = 0;
					return true;
				}
				tenth_seconds=9;
			}
			return false;
		}
	}
	return false;
}

bool adjust_timer(count_direction_t dir, adjust_steps_t step)
{
	seconds_tens = 0;
	seconds_ones = 0;
	tenth_seconds = 0;
	if (step == HOUR)
	{
		switch(dir)
		{
			case UP:
			{
				hours++;
				if (hours > 9)
				{
					hours = 9;
					return true;
				}
				return false;
			}
			case DOWN:
			{
				hours--;
				if (hours < 0)
				{
					hours = 0;
					return true;
				}
				return false;
				break;
			}
		}
	}
	else
	{
		switch(step)
		{
			case MIN_1:
			{
				return count_minutes_ones_int(dir);
			}
			case MIN_5:
			{
				bool state = false;
				for (uint8_t i = 0; i < 5; i++)
				{
					state = count_minutes_ones_int(dir);
				}
				return state;
			}
			case MIN_10:
			{
				bool state = false;
				for (uint8_t i = 0; i < 10; i++)
				{
					state = count_minutes_ones_int(dir);
				}
				return state;
			}
			case MIN_30:
			{
				bool state = false;
				for (uint8_t i = 0; i < 30; i++)
				{
					state = count_minutes_ones_int(dir);
				}
				return state;
			}
		}
		return false;
	}
	return false;
}

void put_time_in_bcd()
{
	uint8_t dots;
	if (hours > 0 || minutes_tens > 0)
	{
		// display like h.mm
		bcd[0] = minutes_ones;
		bcd[1] = minutes_tens;
		bcd[2] = hours;
		dots = 0b100;
	}
	else if (minutes_ones > 0)
	{
		// display like m.ss
		bcd[0] = seconds_ones;
		bcd[1] = seconds_tens;
		bcd[2] = minutes_ones;
		dots = 0b100;
	}
	else
	{
		// display like ss.t
		bcd[0] = tenth_seconds;
		bcd[1] = seconds_ones;
		bcd[2] = seconds_tens;
		dots = 0b010;
	}
	if (tenth_seconds >= 5)
	{
		dots = 0; // make the dot blink
	}
	bcd2digits(dots);
}