/*
 * DA2C_T3_C_1.c
 *
 * Created: 3/23/2019 2:25:57 PM
 * Author : Migggy
 */ 
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
volatile uint8_t tof_detected;
 ISR( TIMER0_COMPA_vect ){
	TCNT0 = 0;
	int START = 0;
	while(START != 27){
		while(TCNT0 != 255);
		TCNT0 = 0;
		START++;
	}
	PORTB ^= (1<<2);
	START = 0;
	TCNT0 = 0;
	while(START != 18){
		while(TCNT0 != 255);
		TCNT0 = 0;
		START++;
	}
	PORTB ^= (1<<2);
	START = 0;
	TCNT0 = 0;
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
	PORTB ^= (1 << 5);  // LED off
	timer0_init();
	while(1);
}

