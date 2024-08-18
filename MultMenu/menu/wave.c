#include "wave.h"
#include "stdio.h"
#include "dispDirver.h"

uint8_t buffer[HOR_RES];

void Wave_Widget(xpMenu Menu)
{
    uint8_t time;
    char str[30];
    float value = 0;
    switch (Menu->now_item->element->data->Data_Type)
    {
    case DATA_INT:
        value = (float)(*(int *)(Menu->now_item->element->data->ptr));
        break;
    case DATA_FLOAT:
        value = *(float *)(Menu->now_item->element->data->ptr);
        break;
    default:
        break;
    }
    for (time = HOR_RES - 1; time > 0; time--)
    {
        buffer[time] = buffer[time - 1];
    }
    buffer[0] = ((value) / (float)((Menu->now_item->element->data->max - Menu->now_item->element->data->min))) * (VER_RES - 20);
    OLED_ClearBuffer();
    OLED_SetDrawColor(&Menu->bgColor);
    OLED_DrawBox(0, 0, HOR_RES, VER_RES);
    uint8_t color = Menu->bgColor^0x01;
    OLED_SetDrawColor(&color);
    for (time = 0; time < HOR_RES - 1; time++)
    {
        OLED_DrawLine(time, buffer[time], time + 1, buffer[time + 1]);
    }
    switch (Menu->now_item->element->data->Data_Type)
    {
    case DATA_INT:
        sprintf(str, "%s:%d ", Menu->now_item->element->data->name, *(int *)(Menu->now_item->element->data->ptr));
        break;
    case DATA_FLOAT:
        sprintf(str, "%s:%.2f ", Menu->now_item->element->data->name, *(float *)(Menu->now_item->element->data->ptr));
        break;
    default:
        break;
    }
    OLED_DrawStr(0, VER_RES - 5, str);
    OLED_SendBuffer();
}
