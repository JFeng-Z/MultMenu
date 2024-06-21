#include "stm32f10x.h"
#include "bsp_usart.h"
#include "TimerTick.h"
#include "menu.h"

xMenu menu;

int main()
{
    USART_Config();
    Timer_Init();
    Menu_Init(&menu);
    for(;;)
    {
        Menu_Loop(&menu);
    }
}
