#ifndef _DISP_DRIVER_H_
#define _DISP_DRIVER_H_

#include "stdint.h"

void Disp_Init(void);
void OLED_ClearBuffer(void);
void OLED_SendBuffer(void);
void OLED_SetFont(const uint8_t  *font);
void OLED_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
uint16_t OLED_DrawStr(uint16_t x, uint16_t y, const char *str);
void OLED_SetDrawColor(uint8_t color);
void OLED_DrawFrame(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void OLED_DrawRFrame(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r);
void OLED_DrawBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void OLED_DrawRBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r);
void OLED_DrawXBMP(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *bitmap);
void OLED_SetContrast(uint8_t value);
void OLED_SetPowerSave(uint8_t is_enable);
uint8_t OLED_GetBufferTileHeight(void);
uint8_t OLED_GetBufferTileWidth(void);
uint8_t *OLED_GetBufferPtr(void);

#endif
