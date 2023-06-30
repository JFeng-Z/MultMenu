#ifndef _rotary_encoder_h
#define _rotary_encoder_h

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

#define CLK GPIO_Pin_7
#define DT  GPIO_Pin_6
#define SW  GPIO_Pin_8
#define Encoder_Port GPIOB

void rotary_encoder_Init(void);
int16_t GETCounter_Encoder(void);
BaseType_t Encoder_Task_Create(void);

#endif