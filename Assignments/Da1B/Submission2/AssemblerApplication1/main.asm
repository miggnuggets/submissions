;
; AssemblerApplication1.asm
;
; Created: 2/23/2019 11:20:35 AM
; Author : Migggy

; STORE 99 NUMBERS 11-110
;IF DIVISIBLE BY 3 STORE IN 0X0400
;IF NOT DIV/3 STORE IN 0X0600
.include<m328pdef.inc>

.equ STARTADDS = 0x0200 ; start adress
.equ counter = 99 ; storing 99 numbers
.equ startval = 12 ; starting number is 12 in this case

.ORG 0
	LDI XL, LOW($400)  ; Location for division by 3 
	LDI XH, HIGH($400)
	LDI YL, LOW($600) ;  Location  for not divisible by 3 
	LDI YH, HIGH($600)
	LDI ZL, LOW(STARTADDS) ; STARTADDS = 0x0200
	LDI ZH, HIGH(STARTADDS) 
	LDI R20, startval ; starting value increasing by 1
	LDI R24, counter ; counter of fill up loop
ZFILL: INC R20
	ST Z+, R20 ; Store at STARTADDS using Z
	DEC R24
	BREQ DIVISION
	RJMP ZFILL ; loop to fill Z

DIVISION: DEC ZL ; Decrement Z
	LDI R24, counter ; put R24 back to 99
DIFFERENT: LD R20, Z ; R20 store of the rest 
	MOV R25, R20 ; Use R25 for X
	DEC ZL ; Decreasing Z
.DEF NUM = R23
	MOV NUM, R20 ; Dividend value 
.DEF DENOMINATOR = R21
.DEF QUOTIENT = R22
	LDI QUOTIENT,0
	LDI DENOMINATOR, 3
L1: INC QUOTIENT ; Subtract until zero or negative, Increasing by one QUOTIENT for each subtraction
	SUB NUM, DENOMINATOR
	BRCC L1
	DEC QUOTIENT ; Again for adding
	MUL QUOTIENT,DENOMINATOR ; Remainder is dividend so QUOTIENT*DENOMINATOR
	SUB R20, R0 ; Store remainder in R20 
	BRNE YFILL ;If not divisible by 3 store in Y
XFILL: ST X+, R25 ; else go X_STORE
	JMP NEXT 
YFILL: ST Y+, R25
NEXT: DEC R24 
	BREQ SUM_X
	RJMP DIFFERENT

SUM_X: LDI R24, 40 ; Counter of X
	   LDI R21, 0 ; Start the higher value of R20 
SUM_X_LOOP: DEC XL
	LD R20, X ; Load X
	ADD R16, R20  ; Add for carry overflow
	ADC R17,R21
	DEC R24
	BREQ SUM_Y
	RJMP SUM_X_LOOP
SUM_Y: LDI R24, 90 ; Counter for Y
	   LDI R21, 0 ; Start the higher value of R20 
SUM_Y_LOOP: DEC YL
	LD R20, Y ; Load Y
	ADD R18, R20 ; Add for carry overflow
	ADC R19, R21
	DEC R24
	BREQ END
	RJMP SUM_Y_LOOP
END: RJMP END

