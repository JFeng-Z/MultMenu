/*
 * @Author: JFeng 2834294740@qq.com
 * @Date: 2023-07-02 23:52:10
 * @LastEditors: JFeng 2834294740@qq.com
 * @LastEditTime: 2023-07-14 19:55:56
 * @FilePath: \MY_GUI_RTOS\HARDWARE\menu\menu.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __MENU_H__
#define __MENU_H__

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>
#include "U8g2_User.h"
#include "u8g2.h"
#include "GUI.h"
#include "inv_mpu.h"
#include "DinoGame.h"
#include "AirPlane.h"

#define FirstLine   9
#define FirstPos    0
#define Font_Size   12
#define Menu_Up     0
#define Menu_Down   1

#define Time 8

#define MENU_RUN    0
#define APP_RUN     1

#define Head_To_Tail     //首尾链接开关

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
    uint8_t Num;
    xpMenu location;
    xpMenu JumpPage;
    xpItem lastiTem;
    xpItem nextiTem;
    void(*Item_function)();
} xItem;

// Page
extern xMenu MainPage, Page1, Page2, Page3, Page4, Page5, No3Page1, No3Page2;
extern xpMenu NowPage;
// item
extern xpItem Selectitm;
extern xItem Mainitem1, Mainitem2, Mainitem3, Mainitem4, Mainitem5, Mainitem6;
extern xItem Page1item1, Page1item2, Page1item3;
extern xItem Page2item1, Page2item2, Page2item3;
extern xItem No3Pageitem1, No3Pageitem2, No3Pageitem3;
void Menu_Team(void);
void AddPage(const char *name, xpMenu page);
void AddItem(const char *Name, xpItem item, xpMenu LocalPage, xpMenu NextPage);

uint8_t Line(uint8_t t,uint8_t Tgt,uint8_t Now);

void DrawPage(uint8_t pos, xpMenu Page, uint8_t LineSpacing);
void Draw_Menu(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem item,xpItem next_item);

static void Menu_Task(void* parameter);
void Menu_Init(void);
BaseType_t Menu_Task_Create(void);

#endif
