#include "disp_dirver.h"
#include "u8g2.h"
#include "stm32_u8g2.h"

u8g2_t u8g2;

void disp_init(void)
{
    u8g2Init(&u8g2);
}

void OLED_ClearBuffer(void)
{
    u8g2_ClearBuffer(&u8g2);
}

void OLED_SendBuffer(void)
{
    u8g2_SendBuffer(&u8g2);
}

uint16_t OLED_GetStrWidth(const char *s)
{
    return u8g2_GetStrWidth(&u8g2, s);
}

void OLED_SetMaxClipWindow(void)
{
    u8g2_SetMaxClipWindow(&u8g2);
}

void OLED_SetFont(const uint8_t  *font)
{
    u8g2_SetFont(&u8g2, font);
}

void OLED_DrawPixel(uint16_t x, uint16_t y)
{
    u8g2_DrawPixel(&u8g2, x, y);
}

void OLED_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    u8g2_DrawLine(&u8g2, x1, y1, x2, y2);
}

uint16_t OLED_DrawStr(uint16_t x, uint16_t y, const char *str)
{
    return u8g2_DrawStr(&u8g2, x, y, str);
}

void OLED_SetDrawColor(uint8_t color)
{
    u8g2_SetDrawColor(&u8g2, color);
}

void OLED_DrawFrame(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    u8g2_DrawFrame(&u8g2, x, y, w, h);
}

void OLED_DrawRFrame(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r)
{
    u8g2_DrawRFrame(&u8g2, x, y, w, h, r);
}

void OLED_DrawBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    u8g2_DrawBox(&u8g2, x, y, w, h);
}

void OLED_DrawRBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r)
{
    u8g2_DrawRBox(&u8g2, x, y, w, h, r);
}

void OLED_DrawXBMP(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *bitmap)
{
    u8g2_DrawXBMP(&u8g2, x, y, w, h, bitmap);
}

void OLED_SetContrast(uint8_t value)
{
    u8g2_SetContrast(&u8g2, value);
}

void OLED_SetPowerSave(uint8_t is_enable)
{
    u8g2_SetPowerSave(&u8g2, is_enable);
}

uint8_t OLED_GetBufferTileHeight(void)
{
    return u8g2_GetBufferTileHeight(&u8g2);
}

uint8_t OLED_GetBufferTileWidth(void)
{
    return u8g2_GetBufferTileWidth(&u8g2);
}

uint8_t *OLED_GetBufferPtr(void)
{
    return u8g2_GetBufferPtr(&u8g2);
}
