/*
 * @Author: JFeng 2834294740@qq.com
 * @Date: 2023-06-22 22:18:02
 * @LastEditors: JFeng 2834294740@qq.com
 * @LastEditTime: 2023-06-27 01:25:41
 * @FilePath: \Projectd:\study\STM32F103_CMSIS\MY_GUI_RTOS\HARDWARE\KEY\bsp_key.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
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

#define KeyWkUp_PIN    GPIO_Pin_0
#define KeyWkUp_PORT   GPIOA
#define KeyWkUp_CLK   RCC_APB2Periph_GPIOA

void KEY_GPIO_Config(void);
unsigned char Key_Scan(GPIO_TypeDef *GPIOx,unsigned int GPIO_PIN);
unsigned char Key_Scan_WK(GPIO_TypeDef *GPIOx,unsigned int GPIO_PIN);
void LED_TOGGLE(void);
uint8_t key_read(void);
void KEY_EXTI_Config(void);

#endif