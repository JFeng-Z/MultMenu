#ifndef _dsp_key_H
#define _dsp_key_H

#include "stm32f10x.h"
#include "bsp_led.h"

#define KEY_ON  1
#define KEY_OFF 0

#define UP 3
#define DOWN 2
#define ENTER 1

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
unsigned char Key_Scan(GPIO_TypeDef *GPIOx,unsigned int GPIO_PIN);
unsigned char Key_Scan_WK(GPIO_TypeDef *GPIOx,unsigned int GPIO_PIN);
void LED_TOGGLE(void);
uint8_t key_read(void);
void KEY_EXTI_Config(void);
void KeyEXTI_Open(void);
void Key_Open(void);

#endif