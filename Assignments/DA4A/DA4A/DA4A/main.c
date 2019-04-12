/*
 * DA4A.c
 *
 * Created: 4/11/2019 11:35:52 AM
 * Author : clayt
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Define Baudrate for UART
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

// Function Declarations
void read_adc(void);        
void adc_init(void);
void USART_init( unsigned int ubrr );
void USART_tx_string( char *data );
volatile unsigned int adc_temp;
char outs[20];

int main(void)
{
	adc_init();             // Initialize the ADC
	USART_init(BAUD_PRESCALLER);  // Initialize the USART
	USART_tx_string("Connected!\r\n");    // we're alive!
	_delay_ms(125);         // wait a bit
	DDRD = 0xFF; //DDRD = (1<<3); // Set Port D as an output direction THIS IS REQUIRED TO OUTPUT THE PWM
	// In the next line of code, we:
	// 1. Set the compare output mode to clear OC2A and OC2B on compare match.
	//    To achieve this, we set bits COM2A1 and COM2B1 to high.
	// 2. Set the waveform generation mode to fast PWM (mode 3 in datasheet).
	//    To achieve this, we set bits WGM21 and WGM20 to high.
	TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
	// In the next line of code, we:
	// 1. Set the waveform generation mode to fast PWM mode 7 —reset counter on
	//    OCR2A value instead of the default 255. To achieve this, we set bit
	//    WGM22 to high.
	// 2. Set the prescaler divisor to 1, so that our counter will be fed with
	//    the clock's full frequency (16MHz). To achieve this, we set CS20 to
	//    high (and keep CS21 and CS22 to low by not setting them).
	TCCR2B = _BV(WGM22) | _BV(CS20) | _BV(CS21) | _BV(CS22);
	// OCR2A holds the top value of our counter, so it acts as a divisor to the
	// clock. When our counter reaches this, it resets.
	OCR2A = 255;
	// This is the duty cycle. Think of it as the last value of the counter our
	// output will remain high for. Can't be greater than OCR2A of course.
	OCR2B = 0;
	PCMSK1 |= (1<<PCINT10);
	PCICR |= (1<<PCIE1);
	sei();
	while (1)
	{	
		read_adc();
		_delay_ms(250); // Why delay?
		adc_temp = adc_temp/4;
		snprintf(outs,sizeof(outs),"%3d\r\n", adc_temp);  // print it
		USART_tx_string(outs); // Print random integer number
		OCR2B = adc_temp; // Replace with adc value
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
	(0<<MUX1)|     // ADC0 (PC0 PIN27)
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
	adc_temp = 0;
	while (i--)
	{
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		adc_temp+= ADC;
		_delay_ms(50);
	}
	adc_temp = adc_temp / 4;  // Average a few samples
}
/* INIT USART (RS-232)  */
void USART_init( unsigned int ubrr )
{
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << TXEN0);     // Enable RX, TX & RX interrupt
	UCSR0C = (3 << UCSZ00);    //asynchronous 8 N 1
}
/* SEND A STRING TO THE RS-232 */
void USART_tx_string( char *data )
{
	while ((*data != '\0'))
	{
		while (!(UCSR0A & (1 <<UDRE0)));
		UDR0 = *data;
		data++;
	}
}

ISR(PCINT1_vect){
	DDRD ^= 0xFF;
	_delay_ms(1000); //Ground Bounce
	char check = "Pin Change ";
	USART_tx_string(check); // Print random integer number
	// Check if I bit is set in IREG
}

// use header - util/atomic.h
// specify as an atomic function and it will not be interrupted. 

