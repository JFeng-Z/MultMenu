#include "Key_Driver.h"
#include "FreeRTOS.h"
#include "task.h"

struct Button KEY1,KEY2,KEY3;

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

uint8_t READ_KEY1_GPIO()
{
	return GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN);
}

uint8_t READ_KEY2_GPIO()
{
	return GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN);
}

uint8_t READ_KEYWKUP_GPIO()
{
	return GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN);
}

PressEvent Get_Key(uint8_t Key_Number)
{
    switch (Key_Number)
    {
    case 1:
        return get_button_event(&KEY1);
    case 2: 
        return get_button_event(&KEY2);
    case 3:
        return get_button_event(&KEY3);
    default :
        return NONE_PRESS;
    }
}

void KEY1_PRESS_UP_Handler(void* btn){}
void KEY1_LONG_PRESS_HOLD_Handler(void* btn){}
void KEY1_SINGLE_CLICK_Handler(void* btn){}
void KEY2_PRESS_UP_Handler(void* btn){}
void KEY2_LONG_PRESS_HOLD_Handler(void* btn){}
void KEY2_SINGLE_CLICK_Handler(void* btn){}
void KEYWKUP_PRESS_UP_Handler(void* btn){}

void Key_Loading(void)
{
    button_init(&KEY1,READ_KEY1_GPIO,0);
    button_init(&KEY2,READ_KEY2_GPIO,0);
    button_init(&KEY3,READ_KEYWKUP_GPIO,1);
    
    button_attach(&KEY1,PRESS_UP,KEY1_PRESS_UP_Handler);
    button_attach(&KEY1,LONG_PRESS_HOLD,KEY1_LONG_PRESS_HOLD_Handler);
    button_attach(&KEY1,SINGLE_CLICK,KEY1_SINGLE_CLICK_Handler);

    button_attach(&KEY2,PRESS_UP,KEY2_PRESS_UP_Handler);
    button_attach(&KEY2,LONG_PRESS_HOLD,KEY2_LONG_PRESS_HOLD_Handler);
    button_attach(&KEY2,SINGLE_CLICK,KEY2_SINGLE_CLICK_Handler);

    button_attach(&KEY3,PRESS_UP,KEYWKUP_PRESS_UP_Handler);

	button_start(&KEY1);
    button_start(&KEY2);
    button_start(&KEY3);
}

TaskHandle_t KeyScan_TaskHandle;

void KeyScan_Task(void *parameter)
{
    while (1)
    {
        button_ticks();
        vTaskDelay(5);
    }
}

void KeyScan_Task_Create(void)
{
    xTaskCreate((TaskFunction_t)KeyScan_Task,"KeyScan_Task",128,NULL,7,KeyScan_TaskHandle);
}