#ifndef __MENU_H__
#define __MENU_H__

#include "stm32f10x.h"
#include "stdbool.h"
#include "menuConfig.h"
#include "bsp_usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void Draw_DialogBox(uint16_t x,uint16_t y,uint16_t w,uint16_t h);
void Draw_DialogRBox(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t r);
bool DialogScale_Show(uint8_t x,uint8_t y,uint8_t Targrt_w,uint8_t Targrt_h);
void Set_BgColor(uint8_t color);
int Draw_Scrollbar(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, double min, double max, int step, int NowValue);
void Menu_Task(void* parameter);
void Menu_Init(void);
BaseType_t Menu_Task_Create(void);

#endif
