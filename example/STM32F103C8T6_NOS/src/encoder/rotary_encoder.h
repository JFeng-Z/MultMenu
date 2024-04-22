#ifndef _rotary_encoder_h
#define _rotary_encoder_h

#include "stm32f10x.h"
#include "menuConfig.h"

#define CLK GPIO_Pin_0
#define DT  GPIO_Pin_1
#define SW  GPIO_Pin_5

void Rotary_Encoder_Init(void);
Menu_Direction Encoder_Scan(void);

#endif
