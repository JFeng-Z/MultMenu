/*
 * @Author: JFeng 2834294740@qq.com
 * @Date: 2023-07-02 23:52:10
 * @LastEditors: JFeng 2834294740@qq.com
 * @LastEditTime: 2023-07-24 22:33:24
 * @FilePath: \MY_GUI_RTOS\HARDWARE\menu\menu.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __MENU_H__
#define __MENU_H__

#include "main.h"
#include "Other_Function.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>
#include "DinoGame.h"
#include "AirPlane.h"

#define FirstLine   9
#define FirstPos    0
#define Font_Size   12

#define LINE_MAX    48
#define LINE_MIN    12 
#define Max_Visible_Number  4

#define Menu_Up     0
#define Menu_Down   1
#define MENU_RUN    0
#define APP_RUN     1

#define MENU_MOVE 0
//光标静止
#define CURSOR_STATIC 1

typedef struct MenuPage *xpMenu;
typedef struct Item *xpItem;
typedef struct MenuPage
{
    const char *PageName;
    uint8_t len;
    xpItem ParentiTem;
    xpItem itemHead;
    xpItem itemTail;
} xMenu;

typedef struct Item
{
    const char *itemName;
    uint8_t Number;
    xpMenu location;
    xpMenu JumpPage;
    xpItem lastiTem;
    xpItem nextiTem;
    void(*Item_function)();
} xItem;

void Menu_Team(void);
void AddPage(const char *name, xpMenu page);
void AddItem(const char *Name, xpItem item, xpMenu LocalPage, xpMenu NextPage);

int8_t Line(uint8_t AllTime,uint8_t Time_Now,int8_t Tgt,int8_t Now);

void Draw_Process(void);
void Draw_OptionPlace(uint8_t now_time, xpItem now_item, xpItem next_item);
void Draw_DialogBox(u8g2_t *u8g2,u8g2_uint_t x,u8g2_uint_t y,u8g2_uint_t w,u8g2_uint_t h);
void Draw_DialogRBox(u8g2_t *u8g2,u8g2_uint_t x,u8g2_uint_t y,u8g2_uint_t w,u8g2_uint_t h,u8g2_uint_t r);
void DialogScale_Show(u8g2_t *u8g2,uint16_t x,uint16_t y,uint16_t Targrt_w,uint16_t Targrt_h);
void Draw_Page(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item);
void Draw_Menu(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item);
uint8_t ui_disapper(uint8_t disapper);

void Menu_Task_Create(void);
void Menu_Init(void);

#endif
