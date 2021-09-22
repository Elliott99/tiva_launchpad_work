#include "switches_colors.h"
#include "SysTick_Delay.h"
#include "io.h"



int main(){
	uint32_t switch_state = SW1 | SW2;
	init_port_f();
	GPIOF->LOCK = GPIO_LOCK_KEY;
	GPIOF->CR = 0xFF; //unlock the PF4 and PF0 switches. Certain bits of the Port F register are typically reserved for JTAG debugging, PF0 and PF4 being these special cases
	GPIOF->DIR = BLUE | RED | GREEN;
	GPIOF->PUR = SW1 | SW2; //"pull-up" the state of switches 1 and 2 to high. When pressed, they're low (0)
	GPIOF->DEN = SW1 | SW2 | BLUE | RED | GREEN; //digitally enable pin f4,f0 and pins 1 2 and 3
	SysTick_Init(ONESEC);	
	//to get an action occur at 1 second we have to put 16,000,000-1: why?
	//our clock speed: 16 MHZ: 16,000,000 clock cycles every 1 second
	//why the minus -1? It starts counting from 0. So we have to account for that, like we would for string length vs the actual char buffer size
	//if 16,000,000 cycles = 1second, 16,000 cycles every 1 millisecond (16,000-1)
	
	//when this countdown is complete, the 16th bit in the CTRL register will become 1 
	while(1){
		switch_state &= GPIOF->DATA;
		switch (switch_state){
			case SW2: //this is what happens if you press SW1
				if (timeout())
					toggle_data_bit(GREEN); //toggle Green LED on/off every second using XOR
				break;
			case SW1://what happens if you press switch 2
					if (timeout())
						toggle_data_bit(GREEN | RED); //toggle orange on/off every second using XOR
					break;
			case 0x0: //if both switches are pressed, blink white (SW1 and SW2 both become low, so the logical AND gives back all 0's)
				if (timeout())
					toggle_data_bit(RED|GREEN|BLUE);
		}
	}
}

void SysTick_Init(uint32_t delay_time){
	SysTick->LOAD = delay_time;
	SysTick->CTRL = COUNTER_ENABLED | COUNTER_USES_INTERNAL_CLOCK; //set bits 0 and bits 2 in the STCTRL register, enabling timer and having timer use internal clock
}

bool timeout(void){
	return SysTick->CTRL & COUNTER_IS_ZERO; //if true, we've counted down one second
}



