#include "bsp_key.h"

void KEY_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(KEY1_CLK,ENABLE);
    RCC_APB2PeriphClockCmd(KEY2_CLK,ENABLE);
    RCC_APB2PeriphClockCmd(KEY3_CLK,ENABLE);

    GPIO_InitStruct.GPIO_Pin=KEY1_PIN;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(KEY1_PORT,&GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin=KEY2_PIN;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(KEY2_PORT,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin=KEY3_PIN;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;
    GPIO_Init(KEY3_PORT,&GPIO_InitStruct);
}

void KEY_EXTI_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
    GPIO_Init(GPIOC,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
    GPIO_Init(GPIOC,&GPIO_InitStruct);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource8);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource9);

    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line=EXTI_Line0|EXTI_Line8|EXTI_Line9;
    EXTI_InitStruct.EXTI_LineCmd=ENABLE;
    EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
    NVIC_Init(&NVIC_InitStruct);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

    NVIC_InitStruct.NVIC_IRQChannel=EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
    NVIC_Init(&NVIC_InitStruct);
}

uint8_t KEY_STATE;

void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0)==SET)
    {
        EXTI->IMR&=~(EXTI_Line0);
        KEY_STATE=ENTER;
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line8)==SET)
    {
        EXTI->IMR&=~(EXTI_Line8);
        KEY_STATE=DOWN;
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
    if(EXTI_GetITStatus(EXTI_Line9)==SET)
    {
        EXTI->IMR&=~(EXTI_Line9);
        KEY_STATE=UP;
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}

void KeyEXTI_Open(void)
{
    EXTI->IMR|=EXTI_Line0;
    EXTI->IMR|=EXTI_Line8;
    EXTI->IMR|=EXTI_Line9;
}

unsigned char Key_Scan(GPIO_TypeDef *GPIOx,unsigned int GPIO_PIN)
{
    if (GPIO_ReadInputDataBit(GPIOx,GPIO_PIN)==0)
    {
        while (GPIO_ReadInputDataBit(GPIOx,GPIO_PIN)==0);
        return KEY_ON;
    }
    return KEY_OFF;
}

unsigned char Key_Scan_WK(GPIO_TypeDef *GPIOx,unsigned int GPIO_PIN)
{
    if (GPIO_ReadInputDataBit(GPIOx,GPIO_PIN)==1)
    {
        while (GPIO_ReadInputDataBit(GPIOx,GPIO_PIN)==1);
        return KEY_ON;
    }
    return KEY_OFF;
}

uint8_t key_read(void)
{
    if (KEY_STATE==ENTER){return ENTER;}
    if (KEY_STATE==DOWN){return DOWN;}
    if (KEY_STATE==UP){return UP;}
    return 0;
}

void Key_Open(void)
{
    KEY_STATE=RESET;
    KeyEXTI_Open();
}

void LED_TOGGLE(void)
{
    LED_PORT->ODR^=LED_PIN;
}
