/*
 * DA2C_T1_C.c
 *
 * Created: 3/23/2019 9:54:15 AM
 * Author : Migggy
 */ 
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
// Task 1_1
int main(void){
	DDRB = 0xFF; // Set PORTB as output
	PORTB = 0xFF; // Set LED off
	TCCR0B = 0b0000101; // Prescaler 1024
	while(1)
	{
		TCNT0 = 0;
		int START = 0;
		while(START != 18)
		{ 
			while(TCNT0 != 255);
			TCNT0 = 0;
			START++;
		} 
		PORTB ^= (1<<2); // Toggle on
		START = 0;
		TCNT0 = 0;
		while(START !=27)
		{ 
			while(TCNT0 != 255);
			TCNT0 = 0;
			START++;
		}
		PORTB ^= (1<<2); // Toggle off
		START = 0;
		TCNT0 = 0;
	}
}
