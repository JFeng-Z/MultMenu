#include "stm32f10x.h"
#include "bsp_usart.h"
#include "TimerTick.h"
#include "menu.h"
#include "stdlib.h"
#include "rotary_encoder.h"

xMenu menu;
int test;

int main()
{
    USART_Config();
    Timer_Init();
    Rotary_Encoder_Init();
    Menu_Init(&menu);
    for(;;)
    {
        test = rand();
        Menu_Loop(&menu);
    }
}
