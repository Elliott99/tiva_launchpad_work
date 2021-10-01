#include "TM4C123.h"   // Device header
#include <math.h>


volatile int temp;



void delay_5ms(void);

int main(){
	//enable clocks for ADC and Wide Timer
	SYSCTL->RCGCADC = 0x01;
	SYSCTL->RCGCWTIMER = 0x01; //enable wide (64-bit timer) 0
	//enable ADC0 to take in a timer trigger, take temp from sensor after 1 sample, and set the interrupt flag after temp is collected
	SYSCTL->RCGCGPIO |= 0x20;
	GPIOF->DIR |= 0x06;
	GPIOF->DEN |= 0x06;
	ADC0->ACTSS &= ~0x08; //disable ADC3
	ADC0->EMUX &= ~0xF000; 
	ADC0->EMUX |= 0x5000; //select timer trigger
	ADC0->SSMUX3 = 0; //getting input from chan0
	ADC0->SSCTL3 = 0x0E; //take temp from senor, only take 1 sample, set interrupt after first sample (bits 2,3, and 4)
	ADC0->ACTSS |= 0x08;
	
	//enable wide Timer0
	WTIMER0->CTL = 0; //disable during configuration
	WTIMER0->CFG = 0x04; //use Timer A of 64 bit wide timer
	WTIMER0->TAMR = 0x02; //periodic mode, down count mode
	WTIMER0->TAILR = 16000000; //1 HZ: once per second, 16,000,000 clock cycles per second
	WTIMER0->CTL |= 0x20; //enable the ADC trigger for WTimer0
	WTIMER0->CTL |= 0x01; //enable timer, config is done
	
	while (1){
		int i=0;
		while ((ADC0->RIS & 0x08) == 0); //wait for ADC0 Raw Inerrupt Status Flag to Be Set (Will be Set after every temperature reading)
		temp = 147 - (247 * ADC0->SSFIFO3) / pow(2,12); //conversion explained in data sheet

	//if internal board temp < 30 Celsius, blink red. If >= 30 Celsius, blink blue, temp taken once a second
		if (temp < 30){
			
			GPIOF->DATA |= 0x02;
			delay_5ms();
			GPIOF->DATA &= ~0x02;
			delay_5ms();
		}
		
		else if (temp >= 30){
			
			GPIOF->DATA |= 0x04;
			delay_5ms();
			GPIOF->DATA &= ~0x04;
			delay_5ms();
		}

			
		ADC0->ISC = 0x08; //clear completion flag
	}	
}

void delay_5ms(void){
	SysTick->LOAD = 800000-1;
	SysTick->CTRL |= 0x01 | 0x04;
	while ((SysTick->CTRL & 1U<<16) == 0);
	SysTick->CTRL = 0;
}

	