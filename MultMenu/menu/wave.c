#include "wave.h"
#include "dispDirver.h"

uint8_t buffer[128];
int test;
data_t Wave_data = {"Wave", &test, DATA_INT, 0, 0, 0};

void Wave_Widget(xpMenu Menu)
{
    uint8_t time;
    for (time = 127; time > 0; time--)
    {
        buffer[time] = buffer[time - 1];
    }
    buffer[0] = *(int *)(Menu->now_item->element->data->ptr) % 64;
    OLED_ClearBuffer();
    for (time = 0; time < 127; time++)
    {
        OLED_DrawLine(time, 63 - buffer[time], time + 1, 63 - buffer[time + 1]);
    }
    OLED_SendBuffer();
}
