/*
 * ************************************************
 * 
 * STM32 blink demo
 * 
 * CPU:     STM32F103C8
 * PIN:     PA1
 * 
 * ************************************************
*/

#include "stm32f10x.h"
#include "bsp_usart.h"
#include "rotary_encoder.h"
#include "menu.h"

int main()
{
    USART_Config();
    Rotary_Encoder_Init();
    Menu_Init();
    for(;;)
    {
        Menu_Task();
    }
}
