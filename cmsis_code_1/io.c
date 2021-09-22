#include "io.h"
void init_port_f(){
	SYSCTL_RCGCGPIO_R |=0x20U;
}

void clear_data_bit(uint32_t color){
	GPIOF->DATA &= ~color;
}

void set_data_bit(uint32_t color){
	GPIOF->DATA |= color;
}

void toggle_data_bit(uint32_t color){
	GPIOF->DATA ^= color;
}
