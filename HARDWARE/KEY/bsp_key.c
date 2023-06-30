/*
 * @Author: JFeng 2834294740@qq.com
 * @Date: 2023-06-22 22:18:02
 * @LastEditors: JFeng 2834294740@qq.com
 * @LastEditTime: 2023-06-27 01:26:06
 * @FilePath: \Projectd:\study\STM32F103_CMSIS\MY_GUI_RTOS\HARDWARE\KEY\bsp_key.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "bsp_key.h"

void KEY_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(KEY1_CLK,ENABLE);
    RCC_APB2PeriphClockCmd(KEY2_CLK,ENABLE);
    RCC_APB2PeriphClockCmd(KeyWkUp_CLK,ENABLE);

    GPIO_InitStruct.GPIO_Pin=KEY1_PIN;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(KEY1_PORT,&GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin=KEY2_PIN;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(KEY2_PORT,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin=KeyWkUp_PIN;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;
    GPIO_Init(KeyWkUp_PORT,&GPIO_InitStruct);
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

void LED_TOGGLE(void)
{
    LED_PORT->ODR^=LED_PIN;
}
