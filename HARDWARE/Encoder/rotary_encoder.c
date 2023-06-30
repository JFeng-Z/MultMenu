/*
 * @Author: JFeng 2834294740@qq.com
 * @Date: 2023-06-29 20:48:08
 * @LastEditors: JFeng 2834294740@qq.com
 * @LastEditTime: 2023-06-29 21:47:39
 * @FilePath: \Projectd:\study\STM32F103_CMSIS\MY_GUI_RTOS\HARDWARE\Encoder\rotary_encoder.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "stm32f10x.h"
#include "rotary_encoder.h"

void rotary_encoder_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

    TIM_InternalClockConfig(TIM4);

    GPIO_InitTypeDef    GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin=CLK|DT|SW;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(Encoder_Port,&GPIO_InitStruct);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseSturcture;
    TIM_TimeBaseSturcture.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseSturcture.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseSturcture.TIM_Period=65536-1;
    TIM_TimeBaseSturcture.TIM_Prescaler=1-1;
    TIM_TimeBaseSturcture.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseSturcture);

    TIM_ICInitTypeDef   TIM_ICInitStruct;
    TIM_ICStructInit(&TIM_ICInitStruct);
    TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;
    TIM_ICInitStruct.TIM_ICFilter=0xF;
    TIM_ICInit(TIM4,&TIM_ICInitStruct);

    TIM_ICStructInit(&TIM_ICInitStruct);
    TIM_ICInitStruct.TIM_Channel=TIM_Channel_2;
    TIM_ICInitStruct.TIM_ICFilter=0xF;
    TIM_ICInit(TIM4,&TIM_ICInitStruct);

    TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);

    TIM_Cmd(TIM4,ENABLE);
}

int16_t GETCounter_Encoder(void) //����Encoder_Count��ֵ�������ת�ͷ���+1����ת�ͷ���-1�����㹲����������
{
    int16_t Temp=0;
    Temp=TIM_GetCounter(TIM4);       
    TIM_SetCounter(TIM4,0);       
    return Temp;
}