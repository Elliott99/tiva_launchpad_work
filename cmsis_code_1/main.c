#include "switches_colors.h"
#include "delay.h"
#include "io.h"


int main(){
	uint32_t switch_state = SW1 | SW2;
	init_port_f();
	GPIOF->LOCK = GPIO_LOCK_KEY;
	GPIOF->CR = 0xFF; //unlock the PF4 and PF0 switches. Certain bits of the Port F register are typically reserved for JTAG debugging, PF0 and PF4 being these special cases
	GPIOF->DIR = BLUE | RED | GREEN;
	GPIOF->PUR = SW1 | SW2; //"pull-up" the state of switches 1 and 2 to high. When pressed, they're low (0)
	GPIOF->DEN = SW1 | SW2 | BLUE | RED | GREEN; //digitally enable pin f4,f0 and pins 1 2 and 3
	while(1){
		switch_state &= GPIOF->DATA;
		switch (switch_state){
			case SW2: //this is what happens if you press SW1
				set_data_bit(GREEN);
				delay(5000000);
				clear_data_bit(GREEN);
				set_data_bit(BLUE|RED);
				delay(5000000);
				clear_data_bit(BLUE|RED);
				break;
			case SW1://what happens if you press switch 2
					set_data_bit(BLUE);
					delay(5000000);
					clear_data_bit(BLUE);
					set_data_bit(BLUE|RED|GREEN);
					delay(5000000);
					clear_data_bit(RED|GREEN|BLUE);
					break;
		}
	}

}



