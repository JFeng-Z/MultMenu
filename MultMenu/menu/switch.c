#include "switch.h"
#include "menu.h"
#include "dispDirver.h"

uint8_t power;
switch_t Power_switch = {&power, OLED_SetPowerSave};

void Switch_Widget(xpMenu Menu)
{
    Menu->now_item->element->switch_data->function(*Menu->now_item->element->switch_data->is_enable);
}
