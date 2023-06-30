#ifndef _TIM_TICK_H
#define _TIM_TICK_H

#include "stm32f10x.h"   

void Tim_Tick_Init(void);
void Tims_delay_us(uint16_t nus);
void Tims_delay_ms(uint32_t xms);

#endif