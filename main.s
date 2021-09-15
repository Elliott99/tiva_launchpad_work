SYSCTL_RCGC_GPIO_R EQU 0x400FE608
GPIO_PORTF_DIR_R EQU 0x40025400
GPIO_PORTF_DEN_R EQU 0x4002551C
GPIO_PORTF_DATA_R EQU 0x400253FC
	

	AREA	|.text|,CODE,READONLY,ALIGN=2	
				THUMB
				EXPORT Main
Main
	BL GPIOF_init ; initialize GPIO port F

loop
	LDR R1, =GPIO_PORTF_DATA_R
	MOV R0, #0x02 ;;mov bit 1 into the Data register for GPIO Portf
	STR R0, [R1]
	B loop


GPIOF_init
	LDR R1, =SYSCTL_RCGC_GPIO_R ; R1 points to contents of rcgc register
	LDR R0, [R1]; load contents of R0 into
	ORR R0,R0,#0x20; set 5th bit of register (set clock for PORTF)
	STR R0, [R1]
	LDR R1, =GPIO_PORTF_DIR_R
	MOV R0, #0x02 ;set pin F1 as output, which controls the red LED
	STR R0, [R1]
	LDR R1, =GPIO_PORTF_DEN_R
	MOV R0, #0x02
	STR R0, [R1] ;digitally enable Pin F2 to send voltage to the red LED
	BX LR

	
	ALIGN
	END
