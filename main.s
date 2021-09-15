SYSCTL_RCGC_GPIO_R EQU 0x400FE608
GPIO_PORTF_DIR_R EQU 0x40025400
GPIO_PORTF_DEN_R EQU 0x4002551C
GPIO_PORTF_DATA_R EQU 0x400253FC
	
SEC_DIV_FIVE EQU 200

	AREA	|.text|,CODE,READONLY,ALIGN=2	
				THUMB
				EXPORT Main
Main
	BL GPIOF_init ; initialize GPIO port F

loop
	LDR R1, =GPIO_PORTF_DATA_R
	MOV R0, #0xE
	STR R0, [R1]
	BL delay
	LDR R1, =GPIO_PORTF_DATA_R
	MOV R0, #0x08
	STR R0, [R1]
	BL delay
	B loop

delay 
	LDR R1, =SEC_DIV_FIVE
	SUBS R1, R1, #1
	BNE delay
	BX LR


GPIOF_init
	LDR R1, =SYSCTL_RCGC_GPIO_R ; R1 points to contents of rcgc register
	LDR R0, [R1]; load contents of R0 into
	ORR R0,R0,#0x20; set 5th bit of register (set clock for PORTF)
	STR R0, [R1]
	LDR R1, =GPIO_PORTF_DIR_R
	MOV R0, #0xE
	STR R0, [R1]
	LDR R1, =GPIO_PORTF_DEN_R
	MOV R0, #0xE
	STR R0, [R1]
	BX LR

	
	ALIGN
	END
	
	