#ifndef __SYSTICK_DELAY_H__
#define __SYSTICK_DELAY_H__
#include "TM4C123.h"                    // Device header
#include <stdint.h>
#include <stdbool.h>

#define ONESEC 16000000-1
#define ONEMILLISEC 16000-1
#define COUNTER_IS_ZERO 0x10000
#define COUNTER_USES_INTERNAL_CLOCK 1U<<2 //tells SYSTICK to use system clock
#define COUNTER_ENABLED 1U //enables counter in SYSTICK_CTRL register

void SysTick_Init(uint32_t delay_time);
bool timeout(void);
#endif