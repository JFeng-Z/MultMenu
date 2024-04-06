#ifndef __MENU_H__
#define __MENU_H__

#include "stm32f10x.h"
#include "stdbool.h"
#include "menuConfig.h"
#include "bsp_usart.h"

void DialogScale_Show(uint8_t x,uint8_t y,uint8_t Targrt_w,uint8_t Targrt_h);
void Draw_DialogRBox(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t r);
void Switch_Menu_State(Menu_State state);
void Menu_Task(void);
void Menu_Init(void);

#endif
