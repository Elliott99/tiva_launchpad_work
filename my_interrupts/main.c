#include "TM4C123.h"                    // Device header

void Timer_1A_10HZ_init(void);



int main(){
	SYSCTL->RCGCGPIO |= 0x20;
	GPIOF->DIR = 0x0E;
	GPIOF->DEN = 0x0E;
	__disable_irq();
	Timer_1A_10HZ_init();
	__enable_irq();
	while (1){
		GPIOF->DATA ^= 0x08;
	}
}
	

void Timer_1A_10HZ_init(void){
	SYSCTL->RCGCTIMER = 0x02;
	TIMER1->CTL = 0; //timer is disabled
	TIMER1->CFG |= 0x04; //timer is timer A of a 32 bit timer
	TIMER1->TAMR |= 0x02; //timer is in periodic mode
	TIMER1->TAPR = 250; //16,000,000 / 250 =  64,000.
	TIMER1->TAILR = 6400; //6400 * 250 = 1,600,000 16,000,000 / 1,600,000 = 10 so we have an interupt occuring 10 times a second here
	TIMER1->IMR |= 0x01; //time-out interrupts are enabled for Timer 1A
	TIMER1->ICR = 0x1; //disable interrupt flags in RIS and MIS just in case
	TIMER1->CTL |= 0x01; //enable the timer
	NVIC->ISER[0] |= 0x00200000; //enable the interrupt for Timer1A
}
void TIMER1A_Handler(void){
	volatile uint32_t clear;
	if (TIMER1->MIS & 0x01){ //if the interrupt occurred. MIS 0 bit is set
		GPIOF->DATA ^= 0x04; //toggle blue
		TIMER1->ICR = 0x1; //clear interrupt bit in MIS/RIS so we can do this again (periodic mode timer)
		clear = TIMER1->ICR; //clear is a precaution. Reading the value of ICR clears the bit, so we're clearing again just in case
	}
	else{ //if for some reason the timeout never occurs,clear ICR anyway as a precaution
		TIMER1->ICR = 0x1;
		clear = TIMER1->ICR;
	}
}

	
	
	

