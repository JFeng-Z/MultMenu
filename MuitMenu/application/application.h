#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "menu.h"
#include "menuConfig.h"

void DialogScaleShow(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void Draw_Scrollbar(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t r, double min, double max, uint16_t NowValue);
void AppBreak(void);
void White_Dark_Day(xpItem item);
void Show_MPU6050(xpItem item);
void Show_GitHub(xpItem item);
void Show_Bilibili(xpItem item);
void Setting_Pid(xpItem item);
void Show_Log(xpItem item);
void Setting_Contrast(xpItem item);
void PowerSave(xpItem item);

#endif
