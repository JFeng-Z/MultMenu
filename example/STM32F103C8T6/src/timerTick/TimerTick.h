#ifndef _TimerTick_H
#define _TimerTick_H

#include "stm32f10x.h"   
#include "multi_button.h"

void Timer_Init(void);
void Tims_delay_us(uint16_t nus);
void Tims_delay_ms(uint32_t xms);

#endif
