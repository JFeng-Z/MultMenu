#ifndef __MENU_H__
#define __MENU_H__

#include "main.h"
#include "Other_Function.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>
#include "DinoGame.h"
#include "AirPlane.h"

#define HOR_RES     128
#define VER_RES     64

#define FirstLine   9
#define FirstPos    0
#define Font_Size   12
#define LINE_MAX    48
#define LINE_MIN    12 
#define MaxVisible_Number  4
#define MENU_FONT   u8g2_font_profont12_mf

enum MenuState
{
    MENU_RUN,
    APP_RUN,
    APP_BREAK,
    MENU_MOVE,
    CURSOR_STATIC,

    MENU_UP,
    MENU_DOWN,
    MENU_ENTER
};

typedef void (*Itemfunction)(void);
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
    uint8_t id;
    xpMenu location;
    xpMenu JumpPage;
    xpItem lastiTem;
    xpItem nextiTem;
    void(*Itemfunction)();
} xItem;

void Menu_Team(void);
void AddPage(const char *name, xpMenu page);
void AddItem(const char *Name, xpItem item, xpMenu LocalPage, xpMenu nextpage, Itemfunction function);
int8_t Line(uint8_t AllTime,uint8_t Time_Now,int8_t Tgt,int8_t Now);
void Draw_Process(void);
void Draw_OptionPlace1(uint8_t now_time, xpItem now_item, xpItem next_item);
void Draw_OptionPlace2(uint8_t now_time, xpItem now_item, xpItem next_item);
void Draw_DialogBox(u8g2_t *u8g2,u8g2_uint_t x,u8g2_uint_t y,u8g2_uint_t w,u8g2_uint_t h);
void Draw_DialogRBox(u8g2_t *u8g2,u8g2_uint_t x,u8g2_uint_t y,u8g2_uint_t w,u8g2_uint_t h,u8g2_uint_t r);
void DialogScale_Show(u8g2_t *u8g2,uint16_t x,uint16_t y,uint16_t Targrt_w,uint16_t Targrt_h);
void Draw_Page(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item);
void Draw_Menu(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item);
uint8_t ui_disapper(uint8_t disapper);
void Switch_Menu_State(uint8_t state);
void Menu_Task_Create(void);
void U8G2_Init(void);
void Menu_Init(void);

#endif
