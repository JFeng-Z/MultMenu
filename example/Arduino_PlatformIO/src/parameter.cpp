#include "parameter.h"
#include "menu.h"
#include "disp_dirver.h"
#include "application.h"

extern xMenu menu;

int Contrast = 255; 
data_t Contrast_data = {    
    .name = "Contrast",
    .ptr = &Contrast,
    .function = OLED_SetContrast,
    .Function_Type = STEP_EXECUTE,
    .Data_Type = DATA_INT,
    .Operate_Type = READ_WRITE,
    .max = 255,
    .min = 0,
    .step = 2
};
data_t Cursorkp_data = {
    .name = "Cursor kp",
    .ptr = &menu._animation.TextPage_Cursor.kp,
    .function = NULL,
    .Data_Type = DATA_FLOAT,
    .Operate_Type = READ_WRITE,
    .max = 1,
    .min = -1,
    .step = 0.05
};
data_t Cursorki_data = {
    .name = "Cursor ki",
    .ptr = &menu._animation.TextPage_Cursor.ki,
    .function = NULL,
    .Data_Type = DATA_FLOAT,
    .Operate_Type = READ_WRITE,
    .max = 1,
    .min = -1,
    .step = 0.05
};
data_t Cursorkd_data = {
    .name = "Cursor kd",
    .ptr = &menu._animation.TextPage_Cursor.kd,
    .function = NULL,
    .Data_Type = DATA_FLOAT,
    .Operate_Type = READ_WRITE,
    .max = 1,
    .min = -1,
    .step = 0.05
};
data_t image_space_data = {
    .name = "image space",
    .ptr = &menu.image_space,
    .function = Application::Menu_to_Home,
    .Function_Type = EXIT_EXECUTE,
    .Data_Type = DATA_INT,
    .Operate_Type = READ_WRITE,
    .max = 100,
    .min = 0,
    .step = 2
};
data_t text_space_data = {
    .name = "text space",
    .ptr = &menu.text_space,
    .function = Application::Menu_to_Home,
    .Function_Type = EXIT_EXECUTE,
    .Data_Type = DATA_INT,
    .Operate_Type = READ_WRITE,
    .max = 100,
    .min = 0,
    .step = 2
};
data_t MenuColor_switch_data = {
    .ptr = &menu.bgColor,
    .function = OLED_SetDrawColor,
    .Data_Type = DATA_SWITCH,
    .Operate_Type = READ_WRITE
};
uint8_t power = true;
data_t Power_switch_data = {
    .ptr = &power,
    .function = OLED_SetPowerSave,
    .Data_Type = DATA_SWITCH,
    .Operate_Type = READ_WRITE
};

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
            switch (Menu->now_item->state)
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
