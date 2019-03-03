/*
 * DesignAssignment2A_2C.c
 *
 * Created: 3/2/2019 11:50:46 AM
 *  Author: johnsm17
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

int main(void)
{
	DDRB = 0xFF; /* Set PortB as an output */
	DDRC = 0x00; /* Set PortC as an input */
	PORTC = 0xFF; /* Set switches on */
		while (1)
	{	PORTB = 0xFF; /* PortB is off*/
		if(PINC == 0x7D)
		{ PORTB = 0x00; /* PortB set high */
			_delay_ms(1250); /* set high for 1.25s */
	}
	
	}
}

