/*
MidtermII.c
Created: 5/11/2019 3:37:39 PM
Author : johnsm17
*/

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "i2c_master.h"
#include "uart.h"
#include "apds.h"
#define BAUD 9600
#define BRGVAL (F_CPU/16/BAUD) - 1

#ifndef APDS_H
#define APDS_H

#include <avr/io.h>
#include "i2c_master.h"
#include "apds.h"

#define APDS_WRITE	(0x39 << 1) | 0
#define APDS_READ	(0x39 << 1) | 1

/* Debug */

/* APDS-9960 I2C address */
#define APDS9960_I2C_ADDR       0x39

/* Error code for returned values */
#define ERROR                   0xFF

/* Acceptable device IDs */
#define APDS9960_ID_1           0xAB
#define APDS9960_ID_2           0x9C

/* Misc parameters */
#define FIFO_PAUSE_TIME         30      // Wait period (ms) between FIFO reads

/* APDS-9960 register addresses */
#define APDS9960_ENABLE         0x80
#define APDS9960_ATIME          0x81
#define APDS9960_WTIME          0x83
#define APDS9960_PERS           0x8C
#define APDS9960_CONFIG1        0x8D
#define APDS9960_PPULSE         0x8E
#define APDS9960_CONFIG2        0x90
#define APDS9960_ID             0x92
#define APDS9960_RDATAL         0x96
#define APDS9960_RDATAH         0x97
#define APDS9960_GDATAL         0x98
#define APDS9960_GDATAH         0x99
#define APDS9960_BDATAL         0x9A
#define APDS9960_BDATAH         0x9B
#define APDS9960_POFFSET_UR     0x9D
#define APDS9960_POFFSET_DL     0x9E
#define APDS9960_CONFIG3        0x9F

/* Bit fields */
#define APDS9960_PON            0b00000001
#define APDS9960_AEN            0b00000010
#define APDS9960_PEN            0b00000100
#define APDS9960_WEN            0b00001000
#define APSD9960_AIEN           0b00010000
#define APDS9960_PIEN           0b00100000
#define APDS9960_GEN            0b01000000
#define APDS9960_GVALID         0b00000001

/* On/Off definitions */
#define OFF                     0
#define ON                      1

/* Acceptable parameters for setMode */
#define POWER                   0
#define AMBIENT_LIGHT           1
/*#define PROXIMITY               2*/
#define WAIT                    3
#define AMBIENT_LIGHT_INT       4
#define ALL                     7

/* LED Drive values */
#define LED_DRIVE_100MA         0
#define LED_DRIVE_50MA          1
#define LED_DRIVE_25MA          2
#define LED_DRIVE_12_5MA        3

/* LED Boost values */
#define LED_BOOST_100           0
#define LED_BOOST_150           1
#define LED_BOOST_200           2
#define LED_BOOST_300           3

/* Default values */
#define DEFAULT_ATIME           219     // 103ms
#define DEFAULT_WTIME           246     // 27ms
#define DEFAULT_PROX_PPULSE     0x87    // 16us, 8 pulses
#define DEFAULT_POFFSET_UR      0       // 0 offset
#define DEFAULT_POFFSET_DL      0       // 0 offset
#define DEFAULT_CONFIG1         0x60    // No 12x wait (WTIME) factor
#define DEFAULT_LDRIVE          LED_DRIVE_100MA
#define DEFAULT_PGAIN           PGAIN_4X
#define DEFAULT_AGAIN           AGAIN_4X
#define DEFAULT_AILT            0xFFFF  // Force interrupt for calibration
#define DEFAULT_AIHT            0
#define DEFAULT_PERS            0x11    // 2 consecutive prox or ALS for int.
#define DEFAULT_CONFIG2         0x01    // No saturation interrupts or LED boost
#define DEFAULT_CONFIG3         0       // Enable all photodiodes, no SAI
#define DEFAULT_GLDRIVE         LED_DRIVE_100MA
#define DEFAULT_GWTIME          GWTIME_2_8MS


void apds_init();
void readColor();

#endif


FILE str_uart = FDEV_SETUP_STREAM(uart_putchar, NULL , _FDEV_SETUP_WRITE);
char results[256];

void init_UART();
int uart_putchar( char c, FILE *stream);


int main(void){
	uint16_t red = 0, green = 0, blue = 0;
	
	i2c_init();
	init_UART();
	stdout = &str_uart;
	apds_init();
	
 	_delay_ms(2000);
	printf("AT\r\n");	
	_delay_ms(5000);
	printf("AT+CWMODE=1\r\n"); 	
	_delay_ms(5000);
	printf("AT+CWJAP=\"Wifi\",\"Password\"\r\n");	
    while (1) {
		_delay_ms(5000);
		printf("AT+CIPMUX=0\r\n");
		
		_delay_ms(5000);
		printf("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
		
		_delay_ms(5000);
		readColor(&red, &green, &blue);
		printf("AT+CIPSEND=104\r\n");
		printf("GET https://api.thingspeak.com/update?api_key=4GOY7N6MQ2MKJOS9&field1=%05u&field2=%05u&field3=%05u\r\n", red, green, blue);
		_delay_ms(5000);
    }
}

void apds_init(){
	uint8_t setup;
	i2c_readReg(APDS_WRITE, APDS9960_ID, &setup,1);
	if(setup != APDS9960_ID_1) while(1);
	setup = 1 << 1 | 1<<0 | 1<<3 | 1<<4;
	i2c_writeReg(APDS_WRITE, APDS9960_ENABLE, &setup, 1);
	setup = DEFAULT_ATIME;
	i2c_writeReg(APDS_WRITE, APDS9960_ATIME, &setup, 1);
	setup = DEFAULT_WTIME;
	i2c_writeReg(APDS_WRITE, APDS9960_WTIME, &setup, 1);
	setup = DEFAULT_PROX_PPULSE;
	i2c_writeReg(APDS_WRITE, APDS9960_PPULSE, &setup, 1);
	setup = DEFAULT_POFFSET_UR;
	i2c_writeReg(APDS_WRITE, APDS9960_POFFSET_UR, &setup, 1);
	setup = DEFAULT_POFFSET_DL;
	i2c_writeReg(APDS_WRITE, APDS9960_POFFSET_DL, &setup, 1);
	setup = DEFAULT_CONFIG1;
	i2c_writeReg(APDS_WRITE, APDS9960_CONFIG1, &setup, 1);
	setup = DEFAULT_PERS;
	i2c_writeReg(APDS_WRITE, APDS9960_PERS, &setup, 1);
	setup = DEFAULT_CONFIG2;
	i2c_writeReg(APDS_WRITE, APDS9960_CONFIG2, &setup, 1);
	setup = DEFAULT_CONFIG3;
	i2c_writeReg(APDS_WRITE, APDS9960_CONFIG3, &setup, 1);
}

void readColor(uint16_t *red, uint16_t *green, uint16_t *blue){
	uint8_t redl, redh;
	uint8_t greenl, greenh;
	uint8_t bluel, blueh;
	i2c_readReg(APDS_WRITE, APDS9960_RDATAL, &redl, 1);
	i2c_readReg(APDS_WRITE, APDS9960_RDATAH, &redh, 1);
	i2c_readReg(APDS_WRITE, APDS9960_GDATAL, &greenl, 1);
	i2c_readReg(APDS_WRITE, APDS9960_GDATAH, &greenh, 1);
	i2c_readReg(APDS_WRITE, APDS9960_BDATAL, &bluel, 1);
	i2c_readReg(APDS_WRITE, APDS9960_BDATAH, &blueh, 1);
	*red = redh << 8 | redl;
	*green = greenh << 8 | greenl;
	*blue = blueh << 8 | bluel;
}

void init_UART(void){
	//Set baud rate
	uint16_t baud_rate = BRGVAL;
	UBRR0H = baud_rate >> 8;
	UBRR0L = baud_rate & 0xFF;
	
	//Enable receiver and transmitter
	UCSR0B = ( 1 <<RXEN0)|( 1 <<TXEN0);
	
	// Set frame format: 8data, 1stop bit
	UCSR0C = (3 <<UCSZ00);
}

int uart_putchar(char c, FILE *stream){
	//wait until buffer empty
	while ( !( UCSR0A & ( 1 <<UDRE0)) );
	
	//Put data into buffer
	UDR0 = c;
	return 0;
}

