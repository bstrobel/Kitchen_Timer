/*
 * debounced_keys.h
 *
 * Created: 02.12.2015 11:54:28
 *  Author: Bernd
 */ 


#ifndef DEBOUNCED_KEYS_H_
#define DEBOUNCED_KEYS_H_
#include <avr/io.h>

#ifndef DEBOUNCE_COUNTER_MAX
#	define DEBOUNCE_COUNTER_MAX 20U
#endif

#define PORT_ID_PORTA 0
#define PORT_ID_PORTB 1
#define PORT_ID_PORTC 2
#define PORT_ID_PORTD 3

#define NUM_KEYS 3

#define KEY_PIN_MAP { \
	{PORT_ID_PORTB, PINB0}, \
	{PORT_ID_PORTB, PINB1}, \
	{PORT_ID_PORTB, PINB2} \
}

extern volatile uint8_t keys_bitmap;
extern volatile uint8_t keys_changed_bitmap;

extern void handle_keys(void);
extern void keys_init(void);

#endif /* DEBOUNCED_KEYS_H_ */