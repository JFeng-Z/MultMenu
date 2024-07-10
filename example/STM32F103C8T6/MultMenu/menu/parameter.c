#include "parameter.h"
#include "menu.h"
#include "dispDirver.h"

extern xMenu menu;

int Contrast = 255; 
data_t Contrast_data = {    
    .ptr = &Contrast,
    .name = "Contrast",
    .type = DATA_INT,
    .max = 255,
    .min = 0,
    .step = 2
};
data_t Cursorkp_data = {
    .ptr = &menu._animation.TextPage_Cursor.kp,
    .name = "Cursor kp",
    .type = DATA_FLOAT,
    .max = 1,
    .min = -1,
    .step = 0.05
};
data_t Cursorki_data = {
    .ptr = &menu._animation.TextPage_Cursor.ki,
    .name = "Cursor ki",
    .type = DATA_FLOAT,
    .max = 1,
    .min = -1,
    .step = 0.05
};
data_t Cursorkd_data = {
    .ptr = &menu._animation.TextPage_Cursor.kd,
    .name = "Cursor kd",
    .type = DATA_FLOAT,
    .max = 1,
    .min = -1,
    .step = 0.05
};
data_t image_space_data = {
    .ptr = &menu.image_space,
    .name = "image space",
    .type = DATA_INT,
    .max = 100,
    .min = 0,
    .step = 2
};
data_t text_space_data = {
    .ptr = &menu.text_space,
    .name = "text space",
    .type = DATA_INT,
    .max = 100,
    .min = 0,
    .step = 2
};

void ParameterSetting_Widget(xpMenu Menu)
{
    char value[20] = {0};
    int x = 4, y = 12, w = 120, h = 32;
    float Step = Menu->now_item->element->data->step;
    
    if (DialogScale_Show(Menu, x, y, w, h))
    {

        switch (Menu->now_item->state)
        {
        case MENU_UP:
            Draw_Scrollbar(x + 4, y + 18, w - 30, 6, 3, Step, Menu->now_item->element->data);
            break;
        case MENU_DOWN:
            Step = -Step;
            Draw_Scrollbar(x + 4, y + 18, w - 30, 6, 3, Step, Menu->now_item->element->data);
            break;
        default:
            Draw_Scrollbar(x + 4, y + 18, w - 30, 6, 3, 0, Menu->now_item->element->data);
            break;
        }
        switch (Menu->now_item->element->data->type)
        {
        case DATA_INT:
            sprintf(value, "%d ", *(int *)(Menu->now_item->element->data->ptr));
            break;
        case DATA_FLOAT:
            sprintf(value, "%.2f ", *(float *)(Menu->now_item->element->data->ptr));
        default:
            break;
        }
        OLED_DrawStr(x + 4, y + 13, Menu->now_item->element->data->name);
        OLED_DrawStr(x + w - 40, y + 13, value);
        OLED_SendBuffer();
    }

}
