#ifndef __MENU_H__
#define __MENU_H__

#include "stm32f10x.h"
#include "stdbool.h"
#include "menuConfig.h"

void Menu_Team(void);
void AddPage(const char *name, xpPage page);
void AddItem(const char *Name, Item_Type Type, int *Data, xpItem item, xpPage LocalPage, xpPage nextpage, ItemFunction function);
int8_t Line(uint8_t AllTime,uint8_t Time_Now,int8_t Tgt,int8_t Now);
int PID(int Targrt, int Now, Pid_Error *Obj);
bool Draw_OptionPlace(xpItem now_item, xpItem next_item);
void Draw_DialogBox(uint16_t x,uint16_t y,uint16_t w,uint16_t h);
void Draw_DialogRBox(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t r);
void DialogScale_Show(uint8_t x,uint8_t y,uint8_t Targrt_w,uint8_t Targrt_h);
void Draw_Page(uint8_t pos, xpPage Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item);
void Draw_Menu(uint8_t pos, xpPage Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item);
uint8_t ui_disapper(uint8_t disapper);
void Switch_Menu_State(Menu_State state);
Menu_State BtnScan(void);
void Menu_Task(void *parameter);
void Menu_Task_Create(void);
void Menu_Init(void);

#endif
