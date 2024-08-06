#include "application.h"
#include "dispDirver.h"
#include "image.h"

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

void Show_Logo(xpMenu Menu)
{
    OLED_ClearBuffer();
    OLED_DrawXBMP(32, 0, 64, 64, logo);
    OLED_SendBuffer();
}
