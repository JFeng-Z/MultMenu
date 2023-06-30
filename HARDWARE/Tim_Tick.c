#include "Tim_Tick.h"

void Tim_Tick_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimebaseInitStruct;
    TIM_TimebaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimebaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimebaseInitStruct.TIM_Period=65535;
    TIM_TimebaseInitStruct.TIM_Prescaler=71;
    TIM_TimebaseInitStruct.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM3,&TIM_TimebaseInitStruct);
    TIM_Cmd(TIM3,DISABLE);
}

void Tims_delay_us(uint16_t nus)
{
    TIM_SetCounter(TIM3,0);
	TIM_Cmd(TIM3,ENABLE);
	while (TIM_GetCounter(TIM3) < nus)
	{
	}
	TIM_Cmd(TIM3,DISABLE);
}

void Tims_delay_ms(uint32_t xms)
{
	while(xms--)
	{
		Tims_delay_us(1000);
	}
}