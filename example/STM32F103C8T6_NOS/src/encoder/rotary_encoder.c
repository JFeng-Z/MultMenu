#include "rotary_encoder.h"
#include "TimerTick.h"

void Rotary_Encoder_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    TIM_InternalClockConfig(TIM2);

    GPIO_InitTypeDef    GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin=CLK|DT;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin=SW;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseSturcture;
    TIM_TimeBaseSturcture.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseSturcture.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseSturcture.TIM_Period=65536-1;
    TIM_TimeBaseSturcture.TIM_Prescaler=1-1;
    TIM_TimeBaseSturcture.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseSturcture);

    TIM_ICInitTypeDef   TIM_ICInitStruct;
    TIM_ICStructInit(&TIM_ICInitStruct);
    TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;
    TIM_ICInitStruct.TIM_ICFilter=0xF;
    TIM_ICInit(TIM2,&TIM_ICInitStruct);

    TIM_ICStructInit(&TIM_ICInitStruct);
    TIM_ICInitStruct.TIM_Channel=TIM_Channel_2;
    TIM_ICInitStruct.TIM_ICFilter=0xF;
    TIM_ICInit(TIM2,&TIM_ICInitStruct);

    TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);

    TIM_SetCounter(TIM2,0);
    TIM_Cmd(TIM2,ENABLE);
}

bool EncoderButton_Scan(void)
{
    if(GPIO_ReadInputDataBit(GPIOA,SW) == 0)
    {
        Tims_delay_ms(20);
        if (GPIO_ReadInputDataBit(GPIOA,SW) == 0)
        return true;
    }
    return false;
}

Menu_Direction Encoder_Scan(void)
{
    int16_t Temp;
    Temp = TIM_GetCounter(TIM2);       
    TIM_SetCounter(TIM2,0);   
    if (Temp > 0)
    {
        return MENU_UP;
    }
    else if (Temp < 0)
    {
        return MENU_DOWN;
    }
    return MENU_NONE;
}
