#include "parameter.h"
#include "menu.h"
#include "dispDirver.h"

int Contrast = 255; 
data_t Contrast_data = {    
    .ptr = &Contrast,
    .name = "Contrast",
    .max = 255,
    .min = 0,
    .step = 2
};

void ParameterSetting_Widget(xpMenu Menu)
{
    char value[20] = {0};
    int x = 4, y = 12, w = 120, h = 32;
    
    if (DialogScale_Show(Menu, x, y, w, h))
    {

        switch (Menu->now_item->state)
        {
        case MENU_UP:
            (*Menu->now_item->element->data->ptr) = Draw_Scrollbar(x + 4, y + 18, w - 30, 6, 3, Menu->now_item->element->data->min, Menu->now_item->element->data->max, Menu->now_item->element->data->step, (*Menu->now_item->element->data->ptr));
            break;
        case MENU_DOWN:
            (*Menu->now_item->element->data->ptr) = Draw_Scrollbar(x + 4, y + 18, w - 30, 6, 3, Menu->now_item->element->data->min, Menu->now_item->element->data->max, -Menu->now_item->element->data->step, (*Menu->now_item->element->data->ptr));
        default:
            Draw_Scrollbar(x + 4, y + 18, w - 30, 6, 3, Menu->now_item->element->data->min, Menu->now_item->element->data->max, 0, (*Menu->now_item->element->data->ptr));
            break;
        }

        sprintf(value, "%d ", (*Menu->now_item->element->data->ptr));
        OLED_DrawStr(x + 4, y + 13, Menu->now_item->element->data->name);
        OLED_DrawStr(x + w - 40, y + 13, value);
        OLED_SendBuffer();
    }

}
