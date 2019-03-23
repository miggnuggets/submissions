/*
 * DA2C_T3_C_2.c
 *
 * Created: 3/23/2019 3:01:11 PM
 * Author : Migggy
 */ 
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
volatile uint8_t tof_detected;
 ISR( TIMER0_COMPA_vect ){
	TCNT0 = 0;
	int count = 0;
	if(!(PINC & (1<<PINC1) ) ){
		while(count < 77){
			while((TIFR0 & (1<<OCF0A) ) == 0){
				PORTB &= ~(1<<2);
			}
			TCNT0 = 0;
			TIFR0 |= (1<<OCF0A);
			count++;
		}
		count = 0;
	}
	else{
		PORTB |= (1<<2);
	}
}
 void timer0_init(){
	// Prescaler 1024
	TCCR0B |= (1 << CS02)|(1 << CS00);
	// Start counter
	TCNT0 = 0;
	TIMSK0 |= (1 << OCIE0A);
	sei();
	tof_detected = 0;
}
 int main(void){
	DDRB |= (1 << 2);
	DDRB |= (1 << 5);
	PORTB ^= (1 << 5);    // LED off
	timer0_init();
	while(1);
}
