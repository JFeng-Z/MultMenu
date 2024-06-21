#ifndef __MENU_H__
#define __MENU_H__

#include "stm32f10x.h"
#include "stdbool.h"
#include "menuConfig.h"
#include "bsp_usart.h"

void Draw_DialogBox(xpMenu Menu, uint16_t x,uint16_t y,uint16_t w,uint16_t h);
void Draw_DialogRBox(xpMenu Menu, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r);
bool DialogScale_Show(xpMenu Menu, int16_t x,int16_t y,int16_t Targrt_w,int16_t Targrt_h);
void Set_BgColor(xpMenu Menu, uint8_t color);
int Draw_Scrollbar(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, double min, double max, int step, int NowValue);
void Menu_Loop(xpMenu menu);
void Menu_Init(xpMenu Menu);

#endif
