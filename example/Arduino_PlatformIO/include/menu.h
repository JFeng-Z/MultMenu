#ifndef __MENU_H__
#define __MENU_H__

#include "MenuConfig.h"

void AddItem(const char *Name, Item_Type Type, element_t *Element, const uint8_t *Image, xpItem item, xpPage LocalPage, xpPage nextpage, ItemFunction function);
void Draw_DialogBox(xpMenu Menu, uint16_t x,uint16_t y,uint16_t w,uint16_t h);
void Draw_DialogRBox(xpMenu Menu, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r);
uint8_t DialogScale_Show(xpMenu Menu, int16_t x,int16_t y,int16_t Targrt_w,int16_t Targrt_h);
void Set_BgColor(xpMenu Menu, uint8_t color);
void Draw_Scrollbar(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, float step, data_t *data);
void Menu_Loop(xpMenu menu);
void Menu_Init(xpMenu Menu);

#endif
