/*
 * DA4B.c
 *
 * Created: 4/22/2019 3:36:54 PM
 * Author : johnsm17
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
int start_value = 1699; // Initialize timer1

void read_adc(void);        // Function Declarations
void adc_init(void);
volatile unsigned int adc_value;
volatile int OVFCount; // Setup global overflow counter 
volatile double	float_value;
volatile int RPMvalue;
float c = 1699; // Needed offset for Affine Transformation
char outs[20];

int main(void)
{
    adc_init();             // Initialize the ADC
	_delay_ms(125);         // wait a bit
	
	TCCR1B	|= (1<<WGM12)|(1<<CS11);
	/*
	Turn on CTC Mode
	Set Prescalar to 8
	*/
	TIMSK1 |= (1<<OCIE1A);
	/*
	Turn on interrupt Mask for OCR1A
	*/
	OCR1A = start_value; // Initialize Timer at longest possible value, 20,000us
	sei(); 
	//Test of timer
	DDRD = 0xFF; // Set PortD as an output
	OVFCount = 0; // Initialize global overflow counter
    while (1) 
    {
		read_adc();
		/*
		Affine Transformation
		PWM output = adc_value*(39999-1699)/(1024-0) + 1699
		*/
		float_value = adc_value;
		RPMvalue = (float_value*37.4033) + c;
		OCR1A = RPMvalue; // Load newest time measurement value
    }
}


/* Timer 1 Compare Interrupt Routine */
ISR(TIMER1_COMPA_vect)
{
OVFCount++; // Increment overflow flag counter
int count; // Initialize local counter
count = OVFCount; // Let local counter mimic global counter variable
// Step 1
if(count == 1){
	PORTD = 0xE;
}
// Step 2
if(count == 2) {
	PORTD = 0xC;
}
// Step 3
if(count == 3) {
	PORTD = 0xD;
}
// Step 4
if(count == 4) {
	PORTD = 0x9;
}
// Step 5
if(count == 5) {
	PORTD = 0xB;
}
// Step 6
if(count == 6) {
	PORTD = 0x3;
}
// Step 7
if(count == 7) {
	PORTD = 0x7;
}
// Step 8
if(count == 8) {
	PORTD = 0x6;
}
// Reset Counter if gone through all steps
if(count == 8){
	OVFCount = 0; // Restart Stepper Counter
}
}

/* INIT ADC */
void adc_init(void)
{
	/** Setup and enable ADC **/
	ADMUX = (0<<REFS1)|    // Reference Selection Bits
	(1<<REFS0)|    // AVcc - external cap at AREF
	(0<<ADLAR)|    // ADC Left Adjust Result
	(0<<MUX2)|     // Analog Channel Selection Bits
	(0<<MUX1)|     // ADC0 (PC0 PIN23)
	(0<<MUX0);
	ADCSRA = (1<<ADEN)|    // ADC ENable
	(0<<ADSC)|     // ADC Start Conversion
	(0<<ADATE)|    // ADC Auto Trigger Enable
	(0<<ADIF)|     // ADC Interrupt Flag
	(0<<ADIE)|     // ADC Interrupt Enable
	(1<<ADPS2)|    // ADC Prescaler Select Bits
	(0<<ADPS1)|
	(1<<ADPS0);
}
/* READ ADC PINS */
void read_adc(void)
{
	unsigned char i = 4;
	adc_value = 0;
	while (i--)
	{
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		adc_value+= ADC;
		_delay_ms(50);
	}
	adc_value = adc_value / 4;  // Average a few samples
}