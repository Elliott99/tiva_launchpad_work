#include "TM4C123.h"
#include "../../Downloads/tm4c123gh6pm.h"
#include "delay.h"
#define RED 1U<<1
#define BLUE 1U<<2
#define GREEN 1U<<3


void init_port_f(void);
void clear_data_bit(uint32_t color);
void set_data_bit(uint32_t color);


int main(){
	init_port_f();
	GPIOF->DIR |=0xEU;
	GPIOF->DEN |=0xEU;
	while(1){
		set_data_bit(RED);
		delay(5000000);
		clear_data_bit(RED);
		set_data_bit(BLUE);
		delay(5000000);
		clear_data_bit(BLUE);
		set_data_bit(GREEN);
		delay(5000000);
		clear_data_bit(GREEN);
	}
}

void init_port_f(){
	SYSCTL_RCGCGPIO_R |=0x20U;
}

void clear_data_bit(uint32_t color){
	GPIOF->DATA &= ~color;
}

void set_data_bit(uint32_t color){
	GPIOF->DATA |= color;
}