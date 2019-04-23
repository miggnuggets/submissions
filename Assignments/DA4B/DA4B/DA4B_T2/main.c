/*
 * DA4B_T2.c
 *
 * Created: 4/22/2019 3:37:49 PM
 * Author : johnsm17
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

void read_adc(void);        // Function Declarations
void adc_init(void);
void USART_init( unsigned int ubrr );
void USART_tx_string( char *data );
volatile unsigned int adc_value;
volatile double	float_value;
volatile int PWMvalue;
float c = 97; // Needed offset for Affine Transformation
char outs[20];
int main(void)
{
	// Configure Timer 1
	TCCR1A |= (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);
	/*
	Non inverted PWM
	Prescalar = 64
	WGM Mode 14 - Fast PWM
	*/
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10);
	ICR1 = 4999;
	/*
	PWM Frequency = 50Hz, Period = 20ms standard for servo motor operation
	*/
	DDRB |= (1<<PORTB1);
	adc_init();             // Initialize the ADC
	USART_init(BAUD_PRESCALLER);  // Initialize the USART
	USART_tx_string("Connected!\r\n");    // we're alive!
	_delay_ms(125);         // wait a bit
	while(1)
	{
		read_adc();
		/*
		Affine Transformation
		PWM output = adc_value*(535-97)/(1024-0) + 97
		*/
		float_value = adc_value;
		PWMvalue = (float_value*0.4277343) + c;
		snprintf(outs,sizeof(outs),"%3d\r\n", PWMvalue);  // print it
		USART_tx_string(outs);
		OCR1A = PWMvalue; 
		_delay_ms(125);        // wait a bit
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
