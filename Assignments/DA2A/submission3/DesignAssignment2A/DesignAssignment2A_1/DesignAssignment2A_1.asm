/*
 * DesignAssignment2A_1.asm
 *
 *  Created: 3/2/2019 11:14:39 AM
 *   Author: johnsm17
 */ 

 .ORG 0
	LDI R16, 4
	LDI R22, 0x02
	OUT DDRB, R22
	LDI R17, 0 ; Register needed to toggle the LED
	OUT PORTB, R17
	LDI R20, 5 ; To set the prescalar
	STS TCCR1B, R20 ; Prescalar: 1024

BEGIN:
	LDI R25, 0x00 ; Reset counter
	STS TCNT1H, R25
	STS TCNT1L, R25
	EOR R16, R17 ; XOR to toggle LED
	OUT PORTB, R16

DELAYHIGH:
	LDS R29, TCNT1H ; Loading upper bit of counter to R29
	LDS R28, TCNT1L ; Loading lower bit of counter to R28
	CPI R28, 0x8C ; Compare with Immediate 
	BRSH BODYHIGH ; Branch if same or higher to BODY label
	RJMP DELAYHIGH

BODYHIGH:
	CPI R29, 0x1A ; Compare to immediate
	BRSH DONE
	RJMP DELAYHIGH

DONE:
	; Reset HIGH and LOW bytes for counter
	LDI R20, 0x00
	STS TCNT1H, R20
	STS TCNT1L, R20
	EOR R16, R17 ; XOR to toggle LED
	OUT PORTB, R17

DELAYLOW:
	LDS R29, TCNT1H ; Loading upper bit of counter to R29
	LDS R28, TCNT1L ; Loading lower bit of counter to R28
	CPI R28, 0xB2 ; Compare with Immediate 
	BRSH BODYLOW ; Branch if same or higher to BODY label
	RJMP DELAYLOW

BODYLOW:
	CPI R29, 0x11 ; Compare to immediate
	BRSH BEGIN
	RJMP DELAYLOW
	RET


