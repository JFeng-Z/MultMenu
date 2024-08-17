#include "wave.h"
#include "stdio.h"
#include "dispDirver.h"

uint8_t buffer[HOR_RES];

void Wave_Widget(xpMenu Menu)
{
    uint8_t time;
    char str[30];
    for (time = HOR_RES - 1; time > 0; time--)
    {
        buffer[time] = buffer[time - 1];
    }
    buffer[0] = *(int *)(Menu->now_item->element->data->ptr) % (VER_RES - 20);
    OLED_ClearBuffer();
    uint8_t color = 1;
    OLED_SetDrawColor(&color);
    for (time = 0; time < HOR_RES - 1; time++)
    {
        OLED_DrawLine(time, buffer[time], time + 1, buffer[time + 1]);
    }
    sprintf(str, "%s:%d ", Menu->now_item->element->data->name, *(int *)(Menu->now_item->element->data->ptr));
    OLED_DrawStr(0, VER_RES - 5, str);
    OLED_SendBuffer();
}
