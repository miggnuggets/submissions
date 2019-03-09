/*
 * DA2B_C.c
 *
 * Created: 3/8/2019 4:21:14 PM
 * Author : Migggy
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int main(void)
{
	DDRB = 0xFF; //Set PB output
	DDRD = 0x00; // Set PD as input
	PORTD = 0x02 ; // Pull up
	PORTB = 0xFF; // Set LED off
	EICRA = 0x02; // make INT0 falling edge triggered
	EIMSK = (1<<INT0); //Enable external interrupt 0
	sei(); //Enable Interrupts
		while(1);
}
	ISR (INT0_vect) //ISR for external interrupt 0
{
	PORTB ^= 0x05; // toggle PORTB	_delay_ms(1250); // 1.25 for falling edge
}

