#ifndef _KEYDRIVER_H_
#define _KEYDRIVER_H_

#include "stm32f10x.h"
#include "multi_button.h"

#define Key1    1
#define Key2    2
#define key3    3

void KEY_GPIO_Config(void);
void Key_Loading(void);
PressEvent Get_Key(uint8_t Key_Number);
void KeyScan_Task_Create(void);

#endif 
