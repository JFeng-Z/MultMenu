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
#include "menu.h"
#include "SEGGER_RTT.h"

int main()
{
    USART_Config();
    Menu_Init();
    for(;;)
    {
        Menu_Task();
    }
}
