#ifndef _dsp_led_H
#define _dsp_led_H

#include "stm32f10x.h"

#define LED_PIN    GPIO_Pin_8
#define LED_PORT   GPIOA
#define LED_CLK   RCC_APB2Periph_GPIOA

#define ON 1
#define OFF 0
#define LED(a)  if(a) GPIO_ResetBits(LED_PORT,LED_PIN);\
                else GPIO_SetBits(LED_PORT,LED_PIN);

void LED_GPIO_Config(void);

#endif
