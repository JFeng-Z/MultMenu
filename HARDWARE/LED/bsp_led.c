/*
 * @Author: JFeng 2834294740@qq.com
 * @Date: 2023-06-22 22:18:02
 * @LastEditors: JFeng 2834294740@qq.com
 * @LastEditTime: 2023-06-27 01:26:23
 * @FilePath: \Projectd:\study\STM32F103_CMSIS\MY_GUI_RTOS\HARDWARE\LED\bsp_led.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "bsp_led.h"

void LED_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(LED_CLK,ENABLE);
    GPIO_InitStruct.GPIO_Pin=LED_PIN;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT,&GPIO_InitStruct);
    
}
