#ifndef _GUI_H_
#define _GUI_H_

#include "stm32_u8g2.h"
#include <stdio.h>
#include "u8g2.h"
#include "stm32f10x.h"
#include "test.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_key.h"
#include <string.h>

#define Enter 1
#define Up 2
#define Down 3

typedef struct GUI
{
    uint8_t now;
    uint8_t up;
    uint8_t down;
    uint8_t enter;
    uint8_t menu_num;
    uint8_t cursor_num;
    void(*now_function)();
}key_table;

BaseType_t GUI_Task_Create(void);
void GUI_init(void);
void fun_Cover(void);
void fun_menu(void);
void fun_appA_21(void);
void fun_appA_22(void);
void fun_appA_23(void);
void fun_appA_24(void);
void fun_appB_31(void);
void fun_appB_32(void);
void fun_appB_33(void);
void fun_appC_31(void);
void fun_appC_32(void);
void fun_appC_33(void);
void fun_appD_31(void);
void fun_appD_32(void);
void fun_appD_33(void);

#endif 