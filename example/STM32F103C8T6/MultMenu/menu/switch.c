#include "switch.h"
#include "menu.h"
#include "dispDirver.h"

extern xMenu menu;

switch_t MenuColor_switch = {&menu.bgColor, OLED_SetDrawColor};
uint8_t power;
switch_t Power_switch = {&power, OLED_SetPowerSave};

void Switch_Widget(xpMenu Menu)
{
    if(Menu->now_item->element->switch_data->function != NULL)Menu->now_item->element->switch_data->function(*Menu->now_item->element->switch_data->is_enable);
}
