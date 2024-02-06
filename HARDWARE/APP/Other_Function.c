#include "Other_Function.h"
#include "disp_dirver.h"

//1为白天模式，0为黑夜模式
extern uint8_t BgColor;  

bool IntoState = false;

void DialogScaleShow(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    if(IntoState==false)
    {
        DialogScale_Show(x,y,w,h);
        IntoState=true;
    }
    Draw_DialogBox(x,y,w,h);
}

//绘制滚动条函数
void Draw_Scrollbar(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t r, double min, double max, uint16_t NowValue)
{
    double value = NowValue * ((w - 6)/(max - min)) + 6;
    OLED_DrawRBox( x, y, (uint16_t)value, h, r);
    OLED_DrawRFrame( x, y, w, h, r);
}

bool Quit_Inspect(void)
{
    if(BtnScan() == MENU_ENTER)
    {
        Switch_Menu_State(APP_BREAK);
        if(IntoState == true)
        IntoState = false;
        return true;
    }
    return false;
}

void AppBreak(void)
{
    Switch_Menu_State(APP_BREAK);
    if(IntoState == true)
    IntoState = false;
}

void White_Dark_Day(xpItem item)
{
    BgColor = !item->SwitchState;
}

void Show_MPU6050(xpItem item)
{
    static float pitch, roll, yaw; 		//欧拉角
    char Pi[20], Ro[20], Ya[20];

    DialogScaleShow(1, 2, 100, 62);

    // mpu_dmp_get_data(&pitch, &roll, &yaw);

    sprintf(Pi, "Pitch = %.2f", pitch);
    sprintf(Ro, "Roll  = %.2f", roll);
    sprintf(Ya, "Yaw   = %.2f", yaw);

    OLED_DrawStr(12, 14, Pi);
    OLED_DrawStr(12, 36, Ro);
    OLED_DrawStr(12, 58, Ya);

    OLED_SendBuffer();

}

void Show_GitHub(xpItem item)
{
    const char* GitHub1="https://github.com/";
    const char* GitHub2="JFeng-Z/MultMenu";

    DialogScaleShow(2, 12, 125, 32);
    OLED_DrawStr(8, 25, GitHub1);
    OLED_DrawStr(8, 37, GitHub2);
    OLED_SendBuffer();
}

void Show_Bilibili(xpItem item)
{
    const char* Bilibili = "BV1Xh4y1N7By";

    DialogScaleShow(22, 24, 82, 20);

    OLED_DrawStr(28, 37, Bilibili);
    OLED_SendBuffer();
}

void Setting_Pid(xpItem item)
{
    char value[20] = {0};
    DialogScaleShow(4, 12, 120, 32);

    if((Get_Key(Key1) == PRESS_UP))(*item->data)+=10;
    if((Get_Key(Key2) == PRESS_UP))(*item->data)-=10;
    sprintf(value, "Value: %d", (*item->data));
    OLED_DrawStr(8, 25, value);
    Draw_Scrollbar(8, 30, 90, 6, 3, -1000, 1000, (*item->data));
    OLED_SendBuffer();
}

const unsigned char Log[] = { 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0x00,0x00,0x00,0x00,0x00,
0x00,0xF0,0xFF,0x01,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0x07,0x00,0x00,0x00,0x00,
0x80,0x3F,0xFF,0x1F,0x00,0x00,0x00,0x00,0xC0,0x03,0xFE,0x3F,0x00,0x00,0x00,0x00,
0x00,0x00,0xFE,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0x01,0x00,0x00,0x00,
0x00,0xF0,0xFF,0xFF,0x07,0x00,0x00,0x00,0x80,0xFF,0xFF,0xFF,0x0F,0x00,0x00,0x00,
0x00,0xFF,0xFF,0xFD,0x3F,0x00,0x00,0x00,0x00,0x00,0xFC,0xF8,0xFF,0x00,0x00,0x00,
0x00,0x00,0x7C,0xE0,0xFF,0x01,0x00,0x00,0x00,0x00,0x3C,0xC0,0xFF,0x07,0x00,0x00,
0x00,0x00,0x08,0x00,0xFF,0xEF,0x1C,0x00,0x00,0x00,0x00,0x00,0xFC,0x3F,0x60,0x00,
0x00,0x00,0x00,0x00,0xF8,0xFF,0xC3,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0x9F,0x00,
0x00,0x00,0x00,0x00,0xC0,0xFF,0x3F,0x01,0x00,0x00,0x00,0x00,0x00,0xFF,0x3F,0x01,
0x00,0x00,0x00,0x00,0x00,0xFE,0x7F,0x02,0x00,0x00,0x00,0x00,0x00,0xFE,0x7F,0x02,
0x00,0x00,0x00,0x00,0x80,0xFF,0x7F,0x02,0x00,0x00,0x00,0x00,0xE0,0xFF,0x7F,0x02,
0x00,0x00,0x00,0x00,0xF8,0xFF,0x7F,0x02,0x00,0x00,0x00,0x00,0xFE,0xFF,0x3F,0x01,
0x00,0x00,0x00,0x80,0xFF,0xFF,0x1F,0x01,0x00,0x00,0x00,0xE0,0xFF,0xFF,0x8F,0x00,
0x00,0x00,0x00,0xF0,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0xFC,0xFF,0x3F,0x20,0x00,
0x00,0x80,0x3F,0xFF,0xFF,0xEF,0x1C,0x00,0x00,0xE0,0xFF,0xFF,0xFF,0x03,0x00,0x00,
0x00,0xF0,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0xF0,0xFF,0xFF,0x3F,0x00,0x00,0x00,
0x00,0xF8,0xFF,0xFF,0x0F,0x00,0x00,0x00,0x00,0xF8,0xFF,0xFF,0x03,0x00,0x00,0x00,
0x00,0xF8,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0x7F,0x00,0x00,0x00,0x00,
0x00,0xF8,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0x7F,0x00,0x00,0x00,0x00,
0x00,0xF0,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0x7F,0x00,0x00,0x00,0x00,
0x00,0xE0,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x7F,0x00,0x00,0x00,0x00,
0x00,0x80,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x7F,0x00,0x00,0x00,0x00,
0x00,0x80,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x7F,0x00,0x00,0x00,0x00,
0x00,0xF8,0xFF,0xFF,0x03,0x00,0x00,0x00,0x00,0xF8,0xFF,0xFF,0x03,0x00,0x00,0x00,
0x00,0xF8,0xFF,0xFF,0x03,0x00,0x00,0x00,0x00,0xF8,0xFF,0xFF,0x03,0x00,0x00,0x00,
0x00,0xE0,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0x7F,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

void Show_Log(xpItem item)
{
    OLED_ClearBuffer();
    OLED_DrawXBMP(32,0,64,64,Log);
    OLED_SendBuffer();
}

void Setting_Contrast(xpItem item)
{
    char contrast[20];
    DialogScaleShow(4, 12, 120, 32);

    if((Get_Key(Key1) == PRESS_UP) && (*item->data) < 255)(*item->data)++;
    if((Get_Key(Key2) == PRESS_UP) && (*item->data) > 0)(*item->data)--;

    sprintf(contrast, "Contrast = %d", (*item->data));
    OLED_DrawStr(8, 25, contrast);

    Draw_Scrollbar(8, 30, 90, 6, 3, 0, 255, (*item->data));
    OLED_SetContrast((*item->data));
    OLED_SendBuffer();
}

void PowerSave(xpItem item)
{
    OLED_SetPowerSave(!item->SwitchState);
}
