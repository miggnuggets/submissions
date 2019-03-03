/*
 * DesignAssignment2A.c
 *
 * Created: 3/2/2019 10:50:14 AM
 *  Author: johnsm17
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

int main(void)
{
    DDRB = 0xFF; /* Set PortB as an output */
    
    while (1)
    { PORTB = 0x02; /* PortB.2 set high */
	    _delay_ms(435); /* set high for 60% of 725ms */
	    PORTB = 0x00; /* PortB set low */ 
	    _delay_ms(290); /* Set low for 40% of 725ms */
	}	    
}

