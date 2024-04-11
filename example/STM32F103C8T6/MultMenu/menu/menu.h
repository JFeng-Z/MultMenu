#ifndef __MENU_H__
#define __MENU_H__

#include "stm32f10x.h"
#include "stdbool.h"
#include "menuConfig.h"
#include "bsp_usart.h"

void Draw_DialogBox(uint16_t x,uint16_t y,uint16_t w,uint16_t h);
void Draw_DialogRBox(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t r);
bool DialogScale_Show(uint8_t x,uint8_t y,uint8_t Targrt_w,uint8_t Targrt_h);
void Set_BgColor(uint8_t color);
void Draw_Scrollbar(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t r, double min, double max, uint16_t NowValue);
void Menu_Task(void);
void Menu_Init(void);

#endif
