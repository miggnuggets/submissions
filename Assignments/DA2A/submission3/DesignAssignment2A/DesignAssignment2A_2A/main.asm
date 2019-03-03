;
; DesignAssignment2A_2A.asm
;
; Created: 3/2/2019 2:11:25 PM
; Author : johnsm17
;
 .ORG 0
	LDI R22, 0
	LDI R16, 0xFF
	OUT DDRB, R16
	OUT DDRC, R22
	OUT PORTC, R16
	LDI R17, 0 ; Register needed to toggle the LED
	OUT PORTB, R16
	LDI R20, 5 ; To set the prescalar
	STS TCCR1B, R20 ; Prescalar: 1024
	
CHECK:
	IN R26, PINC
	COM R26
	ANDI R26, 0x04
	CPI R26, 0x04
	BRNE CHECK

BEGIN:
	LDI R25, 0x00 ; Reset Counter
	STS TCNT1H, R25
	STS TCNT1L, R25
	OUT PORTB, R22

DELAYHIGH:
	LDS R29, TCNT1H ; Loading upper bit of counter to R29
	LDS R28, TCNT1L ; Loading lower bit of counter to R28
	CPI R28, 0x4A ; Compare with Immediate 
	BRSH BODYHIGH ; Branch if same or higher to BODY label
	RJMP DELAYHIGH

BODYHIGH:
	CPI R29, 0x4C ; Compare to immediate
	BRSH DONE
	RJMP DELAYHIGH

DONE:
	; Reset HIGH and LOW bytes for counter
	LDI R25, 0x00
	STS TCNT1H, R25
	STS TCNT1L, R25
	OUT PORTB, R16
	RJMP CHECK

