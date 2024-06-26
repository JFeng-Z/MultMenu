#include "application.h"
#include "dispDirver.h"
#include "bsp_usart.h"
#include "image.h"

void Show_MPU6050(xpMenu Menu)
{
    float pitch = 0, roll = 0, yaw = 0; 		//欧拉角
    char Pi[20], Ro[20], Ya[20];

    if (DialogScale_Show(Menu, 1, 2, 100, 62))
    {
        // mpu_dmp_get_data(&pitch, &roll, &yaw);

        sprintf(Pi, "Pitch = %.2f", pitch);
        sprintf(Ro, "Roll  = %.2f", roll);
        sprintf(Ya, "Yaw   = %.2f", yaw);

        OLED_DrawStr(12, 14, Pi);
        OLED_DrawStr(12, 36, Ro);
        OLED_DrawStr(12, 58, Ya);

        OLED_SendBuffer();
    }

}

void Show_GitHub(xpMenu Menu)
{
    const char* GitHub1="https://github.com/";
    const char* GitHub2="JFeng-Z/MultMenu";

    if(DialogScale_Show(Menu, 2, 12, 125, 32))
    {
        OLED_DrawStr(8, 25, GitHub1);
        OLED_DrawStr(8, 37, GitHub2);
        OLED_SendBuffer();
    }
}

void Show_Bilibili(xpMenu Menu)
{
    const char* Bilibili = "BV1Xh4y1N7By";

    if(DialogScale_Show(Menu, 22, 24, 82, 20))
    {
        OLED_DrawStr(28, 37, Bilibili);
        OLED_SendBuffer();
    }
}

const uint8_t logo[] = { 
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

void Show_Log(xpMenu Menu)
{
    OLED_ClearBuffer();
    OLED_DrawXBMP(32, 0, 64, 64, logo);
    OLED_SendBuffer();
}
