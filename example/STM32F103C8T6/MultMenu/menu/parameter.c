#include "parameter.h"
#include "stdio.h"
#include "menu.h"
#include "dispDirver.h"

void ParameterSetting_Widget(xpMenu Menu)
{
    if(Menu->now_item->element->data->Data_Type == DATA_SWITCH)
    {
        if(Menu->now_item->element->data->function != NULL)Menu->now_item->element->data->function(Menu->now_item->element->data->ptr);
    }
    else
    {
        char value[20] = {0};
        int x = 4, y = 12, w = HOR_RES - 8, h = VER_RES - 32;
        float Step = Menu->now_item->element->data->step;
        
        if (DialogScale_Show(Menu, x, y, w, h))
        {
            switch (Menu->dir)
            {
            case MENU_UP:
                Draw_Scrollbar(x + 4, y + 18, w - 30, 6, 3, Step, Menu->now_item->element->data);
                if(Menu->now_item->element->data->function != NULL && Menu->now_item->element->data->Function_Type == STEP_EXECUTE)Menu->now_item->element->data->function(Menu->now_item->element->data->ptr);
                break;
            case MENU_DOWN:
                Step = -Step;
                Draw_Scrollbar(x + 4, y + 18, w - 30, 6, 3, Step, Menu->now_item->element->data);
                if(Menu->now_item->element->data->function != NULL && Menu->now_item->element->data->Function_Type == STEP_EXECUTE)Menu->now_item->element->data->function(Menu->now_item->element->data->ptr);
                break;
            default:
                Draw_Scrollbar(x + 4, y + 18, w - 30, 6, 3, 0, Menu->now_item->element->data);
                break;
            }
            if(Menu->now_item->element->data->name == NULL)Menu->now_item->element->data->name = "Null name";
            switch (Menu->now_item->element->data->Data_Type)
            {
            case DATA_INT:
                sprintf(value, "%s:%d ", Menu->now_item->element->data->name, *(int *)(Menu->now_item->element->data->ptr));
                break;
            case DATA_FLOAT:
                sprintf(value, "%s:%.2f ", Menu->now_item->element->data->name, *(float *)(Menu->now_item->element->data->ptr));
            default:
                break;
            }
            OLED_DrawStr(x + 4, y + 13, value);
            OLED_SendBuffer();
        }
    }
}
