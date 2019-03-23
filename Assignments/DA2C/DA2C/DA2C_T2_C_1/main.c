/*
 * DA2C_T2_C_1.c
 *
 * Created: 3/23/2019 1:42:20 PM
 * Author : Migggy
 */ 
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
// Counts overflow
volatile uint8_t tof_detected;
// TIMER0 overflow interrupt
 ISR(TIMER0_OVF_vect)
{
	// Number of overflow
	tof_detected++;
}
 void timer0_init()
{
	// Prescaler of 1024
	TCCR0B |= (1 << CS02)|(1 << CS00);
	// Start counter
	TCNT0 = 0;
	TIMSK0 |= (1 << TOIE0);
	sei();
	tof_detected = 0;
}
 void func(unsigned int of_detection_num, unsigned int tcount)
 {
	// Check if overflow = of_detection_num
	if (tof_detected >= of_detection_num)
	{
		// Check if timer reaches tcount
		if (TCNT0 >= tcount)
		{
			PORTB ^= (1 << 2);    // Toggle LED
			TCNT0 = 0;            // Reset counter
			tof_detected = 0;     // Reset overflow
		}
	}
}
 int main(void)
	{
	// Set LED to pin PB2
	DDRB |= (1 << 2);
	DDRB |= (1 << 5);
	PORTB ^= (1 << 5);    // LED off
	// Start timer
	timer0_init();
	// Continuous loop
	while(1)
	{
		func(26, 165);
		func(17, 195);
	}
}
