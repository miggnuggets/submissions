/*
 * DA2C_T1_C_2.c
 *
 * Created: 3/23/2019 1:25:34 PM
 * Author : Migggy
 */ 
#include <avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>
int overflow = 0;
int main()
{
	DDRB |= (1<<2); 				// Set PORTB2 as output
	PORTB |= (1<<2); 				// LED off
	DDRC &= (0<<2);				// Set PORTC switch
	PORTC |= (1<<2); 				// Enable pull up

	TCCR0A = 0;
	TCCR0B = (1 << CS02) | (1 << CS00);	// Prescaler 1024
	while (1) {
		if (!(PINC & (1<<PINC1)))
		{
			overflow = 0;
			TCNT0 = 0;
		}
		while ((TIFR0 & 0x01) == 0); // Counts overflow
		TCNT0 = 0x00;		// Reset counter
		TIFR0 = 0x01;		// Reset overflow
		overflow++;			// Increase overflow
		if (overflow <= 67)	// LED on
		PORTB = (0 << DDB2);
		else PORTB = (1 << DDB2);	// LED off
	}
}

