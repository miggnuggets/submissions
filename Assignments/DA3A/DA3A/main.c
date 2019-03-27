/*
 * DA3A.c
 *
 * Created: 3/26/2019 12:25:53 PM
 * Author : Miggy
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <math.h>
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)
// Function Declarations
void USART_init( unsigned int ubrr );
void USART_tx_string( char *data );
char outs[25];
int mynumber;
char str[] = "SUP";
char empty[] = " ";
float hernumber = 27.3653;
int main(void)
{
	// Initialize three desired values
	USART_init(BAUD_PRESCALLER);  // Initialize the USART
	USART_tx_string("Connected!\r\n");    // Send initial connection print for verification
	_delay_ms(125);         // Delay first print commands
	while(1)
	{
		// Get three desired values
		// First Desired Value, Print a string
		USART_tx_string(str); // Print SUP
		USART_tx_string(empty); // Print empty space
		
		// Second Desire value, Print a random integer
		mynumber = rand();
		snprintf(outs,sizeof(outs),"%3d\r\n", mynumber);  // print it
		USART_tx_string(outs); // Print random integer number
		USART_tx_string(empty); // Print empty space
		
		// Third desired Value, Print a floating point number
		sprintf(outs, "%g", hernumber);
		//snprintf(outs,sizeof(outs),"%3d\r\n", hernumber);  // print it
		USART_tx_string(outs); // Print random integer number
		USART_tx_string(empty); // Print empty space
		_delay_ms(1000);        // wait a bit
	}
}

/* INIT USART (RS-232)  */
void USART_init( unsigned int ubrr )
{
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << TXEN0);     // Enable RX, TX & RX interrupt
	UCSR0C = (3 << UCSZ00);    //asynchronous 8 N 1
}

/* SEND A STRING THROUGH RS-232  BY FTDI BOARD*/
void USART_tx_string( char *data )
{
	while ((*data != '\0'))
	{
		while (!(UCSR0A & (1 <<UDRE0)));
		UDR0 = *data;
		data++;
	}
}