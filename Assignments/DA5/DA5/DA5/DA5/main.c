/*
 * DA5.c
 *
 * Created: 4/25/2019 3:24:22 PM
 * Author : clayt
 Special note: This code is setup to work ONLY with Atmega328P not PB. PB has 2 SPI interfaces and so we have to handle two sets of registers
 */ 


#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//Set up UART for printf();
#ifndef BAUD
#define BAUD 9600
#endif
#include "inctest/STDIO_UART.h"

//Include nRF24L01+ library
#include "inctest/nrf24l01.h"
#include "inctest/nrf24l01-mnemonics.h"
#include "inctest/spi.h"
//void print_config(void);

//Used in IRQ ISR
volatile bool message_received = false;
volatile bool status = false;

//Set cliche message to send (message cannot exceed 32 characters)
char tx_message[32];// Define string array

int main(void)
{
	strcpy(tx_message, "Hello from the Other Side!");// Copy string into array
	//Initialize UART
	uart_init();
	//Initialize nRF24L01+ and print configuration info
	nrf24_init();
	//print_config();
	//Start listening to incoming messages
	nrf24_start_listening();
	status = nrf24_send_message(tx_message);
	if (status == true) 
	{
		printf("Connection Established\n");
	}
	while(1)
	{
		if(message_received)
		{
			// Message received, print it
			message_received = false; // Reset to false
			printf("Received Temperature: %s\n", nrf24_read_message());
			// Send message as response
			_delay_ms(500);
			// Perform ADC execution HERE and store in tx_message
			status = nrf24_send_message(tx_message);
			if(status == true)
			{
				printf("Temperature Transmitted\n");
			}
		}
	}
}

// Interrupt on IRQ pin
ISR(INT0_vect)
{
	message_received = true;
}