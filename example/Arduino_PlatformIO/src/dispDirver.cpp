#include "dispDirver.h"
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SCL, SDA);

void Disp_Init(void)
{
    u8g2.begin();
    u8g2.enableUTF8Print();
}

void OLED_ClearBuffer(void)
{
    u8g2.clearBuffer();
}

void OLED_SendBuffer(void)
{
    u8g2.sendBuffer();
}

uint16_t OLED_GetStrWidth(const char *s)
{
    return u8g2.getStrWidth(s);
}

void OLED_SetMaxClipWindow(void)
{
    u8g2.setMaxClipWindow();
}

void OLED_SetFont(const uint8_t  *font)
{
    u8g2.setFont(font);
}

void OLED_DrawPixel(uint16_t x, uint16_t y)
{
    u8g2.drawPixel(x, y);
}

void OLED_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    u8g2.drawLine(x1, y1, x2, y2);
}

uint16_t OLED_DrawStr(uint16_t x, uint16_t y, const char *str)
{
    return u8g2.drawStr(x, y, str);
}

void OLED_SetDrawColor(void *color)
{
    u8g2.setDrawColor(*(uint8_t *)color);
}

void OLED_DrawFrame(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    u8g2.drawFrame(x, y, w, h);
}

void OLED_DrawRFrame(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r)
{
    u8g2.drawRFrame(x, y, w, h, r);
}

void OLED_DrawBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    u8g2.drawBox(x, y, w, h);
}

void OLED_DrawRBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r)
{
    u8g2.drawRBox(x, y, w, h, r);
}

void OLED_SetCursor(uint16_t x, uint16_t y)
{
    u8g2.setCursor(x, y);
}

void OLED_DrawXBMP(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *bitmap)
{
    u8g2.drawXBMP(x, y, w, h, bitmap);
}

void OLED_SetContrast(void *value)
{
    u8g2.setContrast(*(uint8_t *)value);
}

void OLED_SetPowerSave(void *is_enable)
{
    u8g2.setPowerSave(*(uint8_t *)is_enable);
}

uint8_t OLED_GetBufferTileHeight(void)
{
    return u8g2.getBufferTileHeight();
}

uint8_t OLED_GetBufferTileWidth(void)
{
    return u8g2.getBufferTileWidth();
}

uint8_t *OLED_GetBufferPtr(void)
{
    return u8g2.getBufferPtr();
}
