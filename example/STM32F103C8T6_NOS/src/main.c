#include "stm32f10x.h"
#include "bsp_usart.h"
#include "TimerTick.h"
#include "menu.h"
int main()
{
    USART_Config();
    Timer_Init();
    Menu_Init();
    for(;;)
    {
        printf("Hello World!\r\n");
    }
}
