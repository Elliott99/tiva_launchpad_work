#include "TM4C123.h"                    // Device header

void delay_milliseconds(int num_ms);

int main(){
	SYSCTL->RCGCGPIO |= 0x20; //enable clock to Port F
	GPIOF->LOCK = 0x4C4F434B;
	GPIOF->CR = 0x01;
	GPIOF->LOCK = 0;
	GPIOF->DIR &= ~0x11; //make PF0 and PF1 input pins by disabling them
	GPIOF->DIR |= 0x0E;
	GPIOF->DEN |= 0x1F;
	GPIOF->PUR = 0x11; //enable the pull up resistor for PF1
	
	__disable_irq();
	GPIOF->IS &= ~0x11; //cearing these bits means we are edge-detecting bits 5 and bits 0, corresponding to the bits in data that are brought down when a switch is pressed
	GPIOF->IBE  &=~0x11; //both edges (rising/falling) can be detected for the correspoding bits to trigger an interrupt
	GPIOF->IEV &= ~0x11; //detect falling edges to trigger interrupts
	GPIOF->ICR |= 0x11; //clear any prior interrupts (for edge detected interripts, we write a 1 to the corresponding bit)
	GPIOF->IM |= 0x11; //writing 1 to the corresponding bits we are detectting interrupts on indicates the microcontroller to send interrupt signals to the interrupt handler
	NVIC->IP[30] = 3<<5; //Interrupt vector[30] is the interrupt for port F. We are setting it's priority value to 3
	NVIC->ISER[0] = 0x40000000; //Enable IRQ30
	__enable_irq();
	while (1){
		GPIOF->DATA |= 0x02;
		delay_milliseconds(2000000);
		GPIOF->DATA &= ~0x02;
		delay_milliseconds(2000000);
	}
}
		

void GPIOF_Handler(void){
	int i=0;
	volatile int readback;
	if ((GPIOF->DATA & 0x02) != 0){
		GPIOF->DATA &= ~0x02;
	}
	//toggle green LED
	while (GPIOF->MIS !=0){
		if ((GPIOF->MIS & 0x10)){ //PF4 If PF4 (SW1) triggered the interrupt, 
			for (i=0;i<3;i++){
				GPIOF->DATA |= 0x08;
				delay_milliseconds(2000000);
				GPIOF->DATA &= ~0x08;
				delay_milliseconds(2000000);
			}
			GPIOF->ICR |= 0x11; //clear interrupt flags out of an abundance of caution even if the interrupt doesn't occur
			readback = GPIOF->ICR;
		}
		else if (GPIOF->MIS & 0x01){ //if PF0/SW2 triggered the interrupt, blink green
			if (GPIOF->DATA & 0x08){
				GPIOF->DATA &= ~0x08;
			}
				for (i=0;i<3;i++){
					GPIOF->DATA |= 0x04;
					delay_milliseconds(2000000);
					GPIOF->DATA &= ~0x04;
					delay_milliseconds(2000000);
				}
				GPIOF->ICR |= 0x11;
				readback = GPIOF->ICR;			}

	}
}

	
void delay_milliseconds(int num_mseconds_delay){
	int j;
	SYSCTL->RCGCTIMER |= 0x01; //enable clock to timer 0
	TIMER0->CTL = 0x0; //disable timer during configuration
	TIMER0->CFG |= 0x04; //timer A, 16 bit timer of a 32-bit timer
	TIMER0->TAMR |= 0x02; //enable timer, and enable periodic timer
	TIMER0->TAILR = 16000 - 1; //countdown from 16000-1
	TIMER0->ICR = 0x01; //clears bit 0 in TIMER0->RIS, indicating that we've not timed out yet
	TIMER0->CTL = 0x01; //enable timer
	for (j=0;j<num_mseconds_delay;j++){
		while (TIMER0->RIS && 0x01 == 0); //wait until RIS becomes 0x01, indicating that the timer has counted down all the way
		TIMER0->ICR = 0x01;
	}
}
