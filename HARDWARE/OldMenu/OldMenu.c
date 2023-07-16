#include "OldMenu.h"

extern u8g2_t u8g2;

uint8_t func_index=0;
uint8_t last_index=0;
void (*now_func)();

TaskHandle_t GUI_Task_Handle;
static void GUI_Task(void* parameter);

/**
 * @brief 菜单名称定义
 * 
 */
const char *Menu[][4]= 
{

    {"-[1]Function",
    "-[2]Game",
    "-[3]Time",
    "<--"},

    {"-[1]PID",
    "-[2]State",
    "-[3]Speed",
    "<--"},

    {"-[1]Game1",
    "-[2]Game2",
    "-[3]Game3",
    "<--"}

};

const char *Menu2[]= 
{
    "-[1]Function",
    "-[2]Game",
    "-[3]Time",
    "<--",
    "-[1]PID",
    "-[2]State",
    "-[3]Speed",
    "<--",
    "-[1]Game1",
    "-[2]Game2",
    "-[3]Game3",
    "<--"

};

/**
 * @brief 行数及坐标
 * 
 */
const uint8_t line_num[4]={15,30,45,60};

key_table table[30]=
{
    {0,0,0,1,0,0,(*fun_Cover)},

    {1,4,2,5,0,0,(*fun_menu)},
    {2,1,3,9,0,1,(*fun_menu)},
    {3,2,4,13,0,2,(*fun_menu)},
    {4,3,1,0,0,3,(*fun_menu)},

    {5,8,6,17,1,0,(*fun_menu)},
    {6,5,7,18,1,1,(*fun_menu)},
    {7,6,8,19,1,2,(*fun_menu)},
    {8,7,5,1,1,3,(*fun_menu)},

    {9,12,10,20,2,0,(*fun_menu)},
    {10,9,11,21,2,1,(*fun_menu)},
    {11,10,12,22,2,2,(*fun_menu)},
    {12,11,9,2,2,3,(*fun_menu)},

    {13,16,14,23,3,0,(*fun_menu)},
    {14,13,15,24,3,1,(*fun_menu)},
    {15,14,16,25,3,2,(*fun_menu)},
    {16,15,13,3,3,3,(*fun_menu)},

    {17,17,17,5,4,0,(*fun_appA_21)},
    {18,18,18,6,4,1,(*fun_appA_22)},
    {19,19,19,7,4,2,(*fun_appA_23)},

    {20,20,20,9,4,0,(*fun_appC_31)},
    {21,21,21,10,4,1,(*fun_appC_32)},
    {22,22,22,11,4,2,(*fun_appB_33)},

    {23,23,23,13,4,0,(*fun_appC_31)},
    {24,24,24,14,4,1,(*fun_appC_32)},
    {25,25,25,15,4,2,(*fun_appC_33)},
};

static void GUI_Task(void* parameter)
{
  while (1)
  {
    if (Key_Scan_WK(KeyWkUp_PORT,KeyWkUp_PIN)==KEY_ON)
	{
		func_index=table[func_index].enter;
	}
	if (Key_Scan(KEY1_PORT,KEY1_PIN)==KEY_ON)
	{
        func_index=table[func_index].up;
	}
	if (Key_Scan(KEY2_PORT,KEY2_PIN)==KEY_ON)
	{
        func_index=table[func_index].down;
	}
	if (func_index!=last_index)
	{
        now_func=table[func_index].now_function;
        if (__fabs(last_index-func_index)!=3)
        {
            last_index=func_index;
        }
        else func_index=last_index;
        u8g2_ClearBuffer(&u8g2);
        (*now_func)();
        u8g2_SendBuffer(&u8g2);
	}
  }
}

void GUI_init(void)
{
  u8g2Init(&u8g2);
  DrawProcess(&u8g2);
  fun_Cover();
}

BaseType_t GUI_Task_Create(void)
{
    BaseType_t xReturn=pdPASS;
    xReturn=xTaskCreate((TaskFunction_t)GUI_Task,"GUI_Task",100,NULL,4,&GUI_Task_Handle);
    if (pdPASS==xReturn)
    {
        return pdPASS;
    }
    else return pdFAIL;
}

uint8_t x,y=3,w;
const uint8_t tim=3;

/**
 * @brief 线性增长函数用于坐标移动
 * 
 * @param Tgt 目标值
 * @param Now 当前值
 * @return uint8_t 
 */
uint8_t Linear(uint8_t Tgt,uint8_t Now)
{
    uint8_t x=0;
    static uint8_t t=0;
    // if (__fabs(last_index-func_index)==3)
    // {
    //     t=0;
    //     return Now;
    // }
    t++;
    x=(Tgt-Now)*t/tim+Now;
    if (t>=tim)
    {
        t=0;
    }
    return x;
}

/**
 * @brief 线性增长函数2用于方框变化
 * 
 * @param Tgt 目标值
 * @param Now 当前值
 * @return uint8_t 
 */
uint8_t Linear2(uint8_t Tgt,uint8_t Now)
{
    uint8_t x=0;
    static uint8_t t=0;
    // if (__fabs(last_index-func_index)==3)
    // {
    //     t=0;
    //     return Now;
    // }
    t++;
    x=(Tgt-Now)*t/tim+Now;
    if (t>=tim)
    {
        t=0;
    }
    return x;
}

uint8_t Quadratic(uint8_t Tgt,uint8_t Now)
{
    uint8_t x=0;
    static float t=0;
    // if (__fabs(last_index-func_index)==3)
    // {
    //     t=0;
    //     return Tgt;
    // }
    t++;
    x=(float)(Tgt-Now)*(t/=(float)tim)*(t*tim)+Now;
    if (t>=tim)
    {
        t=0;
    }
    return x;
}

uint8_t Quadratic2(uint8_t Tgt,uint8_t Now)
{
    uint8_t x=0;
    static float t=0;
    // if (__fabs(last_index-func_index)==3)
    // {
    //     t=0;
    //     return Tgt;
    // }
    t++;
    x=(float)(Tgt-Now)*(t/=(float)tim)*(t*tim)+Now;
    if (t>=tim)
    {
        t=0;
    }
    return x;
}

/**
 * @brief 首页
 * 
 */
void fun_Cover(void)
{
    draw_(&u8g2);
    u8g2_SetFont(&u8g2,u8g2_font_profont12_mf);
}

/**
 * @brief 菜单函数
 * 
 */
void fun_menu(void)
{
    uint8_t y1=y,t=0,w1=w;
    uint8_t w2=strlen(Menu[table[func_index].menu_num][table[func_index].cursor_num])*6+4;
    uint8_t Line_Num=line_num[table[func_index].cursor_num];
    do
    {
        u8g2_ClearBuffer(&u8g2);
        u8g2_SetDrawColor(&u8g2,1);    
        for (uint8_t i = 0; i < 4; i++)
        {
            u8g2_DrawStr(&u8g2,0,line_num[i],Menu[table[func_index].menu_num][i]);
        }
        y1=Linear(Line_Num,y1);
        w1=Linear2(w2,w1);
        u8g2_SetDrawColor(&u8g2,2);
        u8g2_DrawRBox(&u8g2,0,y1-12,w1,14,4);
        t++;
        u8g2_SendBuffer(&u8g2);
    } while (t<tim);
    y=Line_Num;w=w2;
}

/******************APP功能函数*******************/
void fun_appA_21(void)
{
    
}

void fun_appA_22(void)
{
    
}

void fun_appA_23(void)
{
    
}

void fun_appA_24(void)
{
    
}

void fun_appB_31(void)
{
    
}

void fun_appB_32(void)
{
    
}

void fun_appB_33(void)
{
    
}

void fun_appC_31(void)
{
    
}

void fun_appC_32(void)
{
    
}

void fun_appC_33(void)
{
    
}

void fun_appD_31(void)
{
    
}

void fun_appD_32(void)
{
    
}

void fun_appD_33(void)
{
    
}
