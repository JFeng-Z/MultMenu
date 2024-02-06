#ifndef _KEYDRIVER_H_
#define _KEYDRIVER_H_

#include "main.h"

#define Key1    1
#define Key2    2
#define key3    3

#define UP 3
#define DOWN 2

#define KEY1_PIN    GPIO_Pin_9
#define KEY1_PORT   GPIOC
#define KEY1_CLK   RCC_APB2Periph_GPIOC

#define KEY2_PIN    GPIO_Pin_8
#define KEY2_PORT   GPIOC
#define KEY2_CLK   RCC_APB2Periph_GPIOC

#define KEY3_PIN    GPIO_Pin_0
#define KEY3_PORT   GPIOA
#define KEY3_CLK   RCC_APB2Periph_GPIOA

void KEY_GPIO_Config(void);
void Key_Loading(void);
PressEvent Get_Key(uint8_t Key_Number);
void KeyScan_Task_Create(void);

#endif 