#ifndef __IO_H__
#define __IO_H__
#include <stdint.h>
#include "TM4C123.h"
#include "../../Downloads/tm4c123gh6pm.h"


void clear_data_bit(uint32_t color);
void set_data_bit(uint32_t color);
void toggle_data_bit();
void init_port_f(void);

#endif