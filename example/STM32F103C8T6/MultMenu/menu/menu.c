#include "menu.h"
#include "dispDirver.h"
#include "stdlib.h"
#include "application.h"
#include "DinoGame.h"
#include "AirPlane.h"

//1为白天模式，0为黑夜模式
uint8_t BgColor;  
//对话框缓动动画持续时间（次数）
uint8_t Dialog_Time = 10;

Pid_Error Cursor_Line = {600, 370, 30, 0, 0, 0}, Cursor_Wide = {250, 370, 30, 0, 0, 0};
Animation_Param AnimationParam = {0, 0, 0, 0, 0, 0, 0, false};

/* Page*/
xPage
    Home_Page, 
    System_Page,
    Games_Page;
/* item */
xItem HomeHead_Item, SystemHead_Item, GamesHead_Item, System_Item, Games_Item, ShowLogo_Item, Github_Item, Bilibili_Item, ReadME_Item, SWITCH1_Item, SWITCH2_Item, SWITCH3_Item, SWITCH4_Item, SWITCH5_Item;
xItem MPU6050_Item, LineKp_Item, LineKi_Item, LineKd_Item, WideKp_Item, WideKi_Item, WideKd_Item, Mode_Item, Contrast_Item, Power_Item, SystemReturn_Item;
xItem Dino_Item, AirPlane_Item, GamesReturn_Item;
Menu_State MENU_STATE = MENU_INIT;
Page_State PAGE_STATE = PAGE_STATIC;
//初始化为开始菜单项
xpItem temp_item = &HomeHead_Item, old_item = &HomeHead_Item;

static void AddItem(const char *Name, Item_Type Type, int *Data, xpItem item, xpPage LocalPage, xpPage nextpage, ItemFunction function)
{
    item->ItemName = Name;
    item->ItemType = Type;
    if(Type == DATA)item->data = Data;
    item->location = LocalPage;
    item->itemFunction = function;
    /* 新建item的下一个肯定是null */
    item->nextiTem = NULL;
    /* 如果可以跳转，那么此item是跳转页面的父级 */
    if (nextpage != NULL)
    nextpage->parentITem = item;
    else nextpage = LocalPage;
    item->JumpPage = nextpage;
    /* 链式结构创建item */
    if (LocalPage->itemHead == NULL) // 如果是第一个iTem
    {
        item->lastiTem = item;
        LocalPage->itemHead = item;
        LocalPage->itemTail = item;
        LocalPage->length = 0;
    }
    else // 不是第一个item
    {
        item->lastiTem = LocalPage->itemTail;                // 新item的last指向Local的tailitem
        LocalPage->itemTail->nextiTem = item;                // 让尾巴的next指向新的item，连接起来
        LocalPage->itemTail = LocalPage->itemTail->nextiTem; // 让尾巴指向新的item
        LocalPage->length++;
    }
    item->id = LocalPage->length;
    LocalPage->itemTail->nextiTem = LocalPage->itemHead;  
    LocalPage->itemHead->lastiTem = LocalPage->itemTail;
}

static void AddPage(const char *name, xpPage page, xpItem item, xpPage LocalPage, xpPage nextpage, ItemFunction function)
{
    static uint8_t ID;
    page->pageName = name;
    page->itemHead = NULL;
    page->itemTail = NULL;
    page->id = ID++;
    if (page->id == 0)AddItem(name, LOOP_FUNCTION, NULL, item, LocalPage, nextpage, function);  //第一页的头跳转至首页
    else AddItem(name, PARENTS, NULL, item, LocalPage, nextpage, function);
}

/**
 * @brief 线性增长函数用于坐标移动
 * 
 * @param AllTime 总时长
 * @param Time_Now 当前时间
 * @param Targrt 目标值
 * @param Now 当前值
 * @return int8_t 
 */
static int8_t Linear(uint8_t AllTime, uint8_t Time_Now, int8_t Targrt, int8_t Now)
{
    return (Targrt - Now)*Time_Now/AllTime + Now;		
}

static int PID(int Targrt, int Now, Pid_Error *Obj)
{
    int x = Now;
    float Kp = (float)(Obj->kp)/1000.00, Ki = (float)(Obj->ki)/1000.00, Kd = (float)(Obj->kd)/1000.00;
    Obj->error = Targrt - x;
    Obj->sum_srror += Obj->error;
    float delta_error = Obj->error - Obj->last_error;
    float velocity = Kp * Obj->error + Ki * Obj->sum_srror + Kd * delta_error;
    x += velocity;
    Obj->last_error = Obj->error;
    return x;
}

static void Change_MenuState(Menu_State state)
{
    MENU_STATE = state;
}

void Switch_Widget(xpItem item)
{
    
}

void ParamSet_Widget(xpItem item)
{
    
}

static void Item_AnimationParam_Init(void)
{
    AnimationParam.Item_NowLine = 0;
    AnimationParam.Item_NowWide = 0;
    AnimationParam.Item_TargrtLine = 0;
    AnimationParam.Item_TargrtWide = 0;
    AnimationParam.OptionState = false;
}

static void PID_Param_Init(void)
{
    Cursor_Line.error = 0;
    Cursor_Line.sum_srror = 0;
    Cursor_Line.last_error = 0;
    Cursor_Wide.error = 0;
    Cursor_Wide.sum_srror = 0;
    Cursor_Wide.last_error = 0;
}

static void DialogScale_AnimationParam_Init(void)
{
    AnimationParam.DialogScale_InitHigh = 0;
    AnimationParam.DialogScale_InitWide = 0;
    AnimationParam.DialogScale_time = 0;
}

//首页
static void Draw_Home(xpItem item)
{
    OLED_ClearBuffer();
    OLED_DrawStr(0, Font_Size, "MultMenu");
    OLED_DrawStr(0, Font_Size*2, "Author:ZhangJianFeng");
    OLED_DrawStr(0, Font_Size*3, "Wait button...");
    OLED_DrawStr(50, Font_Size*5, "Version:1.0.0");
    OLED_SendBuffer();
}

void Draw_DialogBox(uint16_t x,uint16_t y,uint16_t w,uint16_t h)
{
    OLED_SetDrawColor(BgColor^0x01);
    OLED_DrawFrame(x, y, w, h);
    OLED_SetDrawColor(BgColor);
    OLED_DrawBox(x+1, y+1, w-2, h-2);
    OLED_SetDrawColor(BgColor^0x01);
}

/**
 * @brief 对话框出现函数
 * 
 * @param x 初始位置x
 * @param y 初始位置y
 * @param Targrt_w 目标宽度
 * @param Targrt_h 目标高度
 */
bool DialogScale_Show(uint8_t x,uint8_t y,uint8_t Targrt_w,uint8_t Targrt_h)
{
    if (MENU_STATE == APP_DRAWING)
    {
        AnimationParam.DialogScale_time++;
        AnimationParam.DialogScale_InitWide = Linear(Dialog_Time, AnimationParam.DialogScale_time, Targrt_w, AnimationParam.DialogScale_InitWide);
        AnimationParam.DialogScale_InitHigh = Linear(Dialog_Time, AnimationParam.DialogScale_time, Targrt_h, AnimationParam.DialogScale_InitHigh);
        Draw_DialogBox(x, y, AnimationParam.DialogScale_InitWide, AnimationParam.DialogScale_InitHigh);
    }
    if (AnimationParam.DialogScale_time == Dialog_Time)
    {
        Change_MenuState(APP_RUN);
        return true;
    }
    OLED_SendBuffer();
    return false;
}

void Draw_DialogRBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r)
{
    OLED_SetDrawColor(BgColor^0x01);
    OLED_DrawRFrame(x, y, w, h, r);
    OLED_SetDrawColor(BgColor);
    OLED_DrawRBox(x + 1, y + 1, w - 2, h - 2, r);
    OLED_SetDrawColor(BgColor^0x01);
}

void Set_BgColor(uint8_t color)
{
    BgColor = color;
}

//绘制滚动条函数
void Draw_Scrollbar(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t r, double min, double max, uint16_t NowValue)
{
    double value = NowValue * ((w - 6)/(max - min)) + 6;
    OLED_DrawRBox( x, y, (uint16_t)value, h, r);
    OLED_DrawRFrame( x, y, w, h, r);
}

/**
 * @brief 渐变消失函数
 * 
 * 
 */
static uint8_t ui_disapper(uint8_t disapper)
{ 
    short disapper_temp = 0;
    int length = 8 * OLED_GetBufferTileHeight() * OLED_GetBufferTileWidth();
    u8 *p = OLED_GetBufferPtr(); 
    if(BgColor==0)
    {  for( int i = 0;i < length ;i++) 
    { p[i] = p[i] & (rand()%0xff) >> disapper; } }
    else
    {  for( int i = 0;i < length ;i++) 
    { p[i] = p[i] | (rand()%0xff) >> disapper; } }
    disapper += 2; 
    if(disapper >= 8) 
    {disapper = 0; } 
    OLED_SendBuffer();
    disapper_temp = disapper;
    return disapper_temp;
}

/**
 * @brief 导航栏
 * 
 * @param now_time 
 * @param now_item 
 * @param next_item 
 */
static bool Draw_OptionPlace(xpItem now_item, xpItem next_item)
{
    static uint8_t t;
    static uint8_t Now_Lenght;
    static uint8_t Next_Lenght;
    Next_Lenght = (VER_RES / (float)(next_item->location->length)) * next_item->id;
    t++;
    Now_Lenght = Linear(Dialog_Time, t, Next_Lenght, Now_Lenght);
    OLED_DrawLine(HOR_RES - 7, 0, HOR_RES - 7, VER_RES);
    OLED_DrawBox(HOR_RES - 10, 0, 6, Now_Lenght);
    if(t == Dialog_Time)
    {
        t = 0;
        return true;
    }
    return false;
}

static void Draw_Page(uint8_t pos, xpPage Page, uint8_t LineSpacing, xpItem now_item, xpItem next_item)
{
    char Data[10] = {0};
    static int16_t first_line = FirstLine;
    xpItem temp = Page->itemHead;
    //初次载入时计算坐标
    if(MENU_STATE == MENU_RUN)
    {
        //切换页面时变量初始化
        if(next_item->location != now_item->location)first_line = FirstLine;

        if (PAGE_STATE == PAGE_MOVE)
        {
            if ((next_item->id - now_item->id) > 0)first_line -= ((next_item->id - now_item->id) > (Page->length - MaxVisible_Number)) ? ((Page->length - MaxVisible_Number) * Font_Size) : Font_Size;
            else first_line += ((now_item->id - next_item->id) > (Page->length - MaxVisible_Number)) ? ((Page->length - MaxVisible_Number) * Font_Size) : Font_Size;
            PAGE_STATE = PAGE_STATIC;
        }
    }
   for (uint16_t i = 0; i <= Page->length; i++)
    {
        OLED_DrawStr(pos, first_line + i * LineSpacing, temp->ItemName);
        if(temp->ItemType == SWITCH)
        {
            if(temp->SwitchState == false)OLED_DrawFrame(pos + 95, first_line + i * LineSpacing - Font_Size + 3, 10, 10);
            else OLED_DrawBox(pos + 95, first_line + i * LineSpacing - Font_Size + 3, 10, 10);
        }
        if(temp->ItemType == DATA)
        {
            sprintf(Data, "%d", *temp->data);
            OLED_DrawStr(pos + 95, first_line + i * LineSpacing, Data);
        }
        temp = temp->nextiTem;
    }
}

static void Draw_Menu(uint8_t pos, xpPage Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item)
{
    //初次载入时计算坐标
    if(MENU_STATE == MENU_RUN)
    {
        AnimationParam.Item_NowWide  = strlen(now_item->ItemName)*6 + 4;
        if(next_item->location != now_item->location)        //切换页面时变量初始化
        {
            Item_AnimationParam_Init();
            PAGE_STATE = PAGE_STATIC;
        }
        else if (next_item->id > now_item->id)
        {
            AnimationParam.Item_TargrtLine += ((next_item->id - now_item->id)*Font_Size);
            if (AnimationParam.Item_TargrtLine > LINE_MAX)  //防止光标溢出可视范围
            {
                PAGE_STATE = PAGE_MOVE;
                AnimationParam.Item_TargrtLine = LINE_MAX;
            }
        }
        else if(next_item->id < now_item->id)
        {
            AnimationParam.Item_TargrtLine -= ((now_item->id - next_item->id)*Font_Size);
            if (AnimationParam.Item_TargrtLine < 0)  //防止光标溢出可视范围
            {
                PAGE_STATE = PAGE_MOVE;
                AnimationParam.Item_TargrtLine = 0;
            }
        }
    }

    AnimationParam.Item_TargrtWide = OLED_GetStrWidth(next_item->ItemName) + 3;

    //整体菜单绘制
    OLED_ClearBuffer();
    OLED_SetDrawColor(BgColor);
    OLED_DrawBox(0, 0, 128, 64);
    OLED_SetDrawColor(BgColor^0x01);
    AnimationParam.OptionState = Draw_OptionPlace(now_item, next_item);
    Draw_Page(pos, Page, LineSpacing, now_item, next_item);
    OLED_SetDrawColor(2);
    AnimationParam.Item_NowLine = PID(AnimationParam.Item_TargrtLine, AnimationParam.Item_NowLine, &Cursor_Line);
    AnimationParam.Item_NowWide = PID(AnimationParam.Item_TargrtWide, AnimationParam.Item_NowWide, &Cursor_Wide);
    OLED_DrawRBox(pos+1, AnimationParam.Item_NowLine+1, AnimationParam.Item_NowWide, Font_Size, 4);
    OLED_SendBuffer();

    Change_MenuState(MENU_DRAWING);
    if ((AnimationParam.Item_NowLine == AnimationParam.Item_TargrtLine) && (AnimationParam.Item_NowWide == AnimationParam.Item_TargrtWide) && (AnimationParam.OptionState == true))
    {
        Change_MenuState(MENU_RUN);
    }
}

int Contrast = 255;

static void Menu_Team(void)
{
    AddPage("[HomePage]", &Home_Page, &HomeHead_Item, &Home_Page, NULL, Draw_Home);
        AddItem(" +System", PARENTS, NULL, &System_Item, &Home_Page, &System_Page, NULL);
            AddPage("[System]", &System_Page, &SystemHead_Item, &System_Page, &Home_Page, NULL);
                AddItem(" -MPU6050", LOOP_FUNCTION, NULL, &MPU6050_Item, &System_Page, NULL, Show_MPU6050);
                AddItem(" -Linear Kp", DATA, &Cursor_Line.kp, &LineKp_Item, &System_Page, NULL, Setting_Pid);
                AddItem(" -Linear Ki", DATA, &Cursor_Line.ki, &LineKi_Item, &System_Page, NULL, Setting_Pid);
                AddItem(" -Linear Kd", DATA, &Cursor_Line.kd, &LineKd_Item, &System_Page, NULL, Setting_Pid);
                AddItem(" -Wide Kp", DATA, &Cursor_Wide.kp, &WideKp_Item, &System_Page, NULL, Setting_Pid);
                AddItem(" -Wide Ki", DATA, &Cursor_Wide.ki, &WideKi_Item, &System_Page, NULL, Setting_Pid);
                AddItem(" -Wide Kd", DATA, &Cursor_Wide.kd, &WideKd_Item, &System_Page, NULL, Setting_Pid);
                AddItem(" -Mode", SWITCH, NULL, &Mode_Item, &System_Page, NULL, White_Dark_Day);
                AddItem(" -Contrast", DATA, &Contrast, &Contrast_Item, &System_Page, NULL, Setting_Contrast);
                AddItem(" -Power", SWITCH, NULL, &Power_Item, &System_Page, NULL, PowerSave);
        AddItem(" +Games", PARENTS, NULL, &Games_Item, &Home_Page, &Games_Page, NULL);
            AddPage("[Games]", &Games_Page, &GamesHead_Item, &Games_Page, &Home_Page, NULL);
                AddItem(" -DinoGame", LOOP_FUNCTION, NULL, &Dino_Item, &Games_Page, NULL, DinoGame_Run);
                AddItem(" -AirPlane", LOOP_FUNCTION, NULL, &AirPlane_Item, &Games_Page, NULL, AirPlane_Run);
        AddItem(" -ShowLogo", LOOP_FUNCTION, NULL, &ShowLogo_Item, &Home_Page, NULL, Show_Log);
        AddItem(" -Github", LOOP_FUNCTION, NULL, &Github_Item, &Home_Page, NULL, Show_GitHub);
        AddItem(" -Bilibili", LOOP_FUNCTION, NULL, &Bilibili_Item, &Home_Page, NULL, Show_Bilibili);
        AddItem(" -ReadME", LOOP_FUNCTION, NULL, &ReadME_Item, &Home_Page, NULL, Show_Bilibili);
        AddItem(" -SWITCH1", SWITCH, NULL, &SWITCH1_Item, &Home_Page, NULL, NULL);
        AddItem(" -SWITCH2", SWITCH, NULL, &SWITCH2_Item, &Home_Page, NULL, NULL);
        AddItem(" -SWITCH3", SWITCH, NULL, &SWITCH3_Item, &Home_Page, NULL, NULL);
        AddItem(" -SWITCH4", SWITCH, NULL, &SWITCH4_Item, &Home_Page, NULL, NULL);
        AddItem(" -SWITCH5", SWITCH, NULL, &SWITCH5_Item, &Home_Page, NULL, NULL);
}

/* 填入按键扫描程序 */
static Menu_Direction BtnScan(void)
{
    if(RXD_GetReceiveFlag() == 1)
    {
        uint8_t data = RXD_GetReceiveData();
        switch (data)
        {
        case MENU_UP:
            return MENU_UP;
        case MENU_DOWN:
            return MENU_DOWN;
        case MENU_ENTER:
            return MENU_ENTER;
        default:
            break;
        }
    }
    return MENU_NONE;
}

void Process_Menu_Run(Menu_Direction Dir)
{
    uint8_t disapper = 0;
    if (MENU_STATE == MENU_RUN)
    {
        switch (Dir)
        {
        case MENU_UP:
            Draw_Menu(FirstPos, temp_item->location, Font_Size, temp_item, temp_item->lastiTem);
            old_item = temp_item;
            temp_item = temp_item->lastiTem;
            break;
        case MENU_DOWN:
            Draw_Menu(FirstPos, temp_item->location, Font_Size, temp_item, temp_item->nextiTem);
            old_item = temp_item;
            temp_item = temp_item->nextiTem;
            break;
        case MENU_ENTER:
            if(temp_item->ItemType == PARENTS)
            {
                Change_MenuState(MENU_RUN);
                for (size_t i = 0; i < 8; i++)
                {
                    disapper = ui_disapper(disapper);
                }
                Draw_Menu(FirstPos, temp_item->JumpPage, Font_Size, temp_item, temp_item->JumpPage->itemHead);
                old_item = temp_item;
                temp_item = temp_item->JumpPage->itemHead;
            }
            else
            {
                ui_disapper(1);
                Change_MenuState(APP_DRAWING);
            }
        default:
            break;
        }
    }
    else if(MENU_STATE == APP_QUIT)
    {
        DialogScale_AnimationParam_Init();
        Change_MenuState(MENU_RUN);
        for (size_t i = 0; i < 8; i++)
        {
            disapper = ui_disapper(disapper);
        }
        Draw_Menu(FirstPos, temp_item->location, Font_Size, temp_item, temp_item);
    }
    else if (MENU_STATE == MENU_DRAWING)
    {
        Draw_Menu(FirstPos, temp_item->location, Font_Size, old_item, temp_item);
        if (Dir != MENU_NONE)
        {
            Change_MenuState(MENU_RUN);
            Process_Menu_Run(Dir);
        }
    }
}

void Process_App_Run(xpItem item, Menu_Direction State)
{
    item->state = State;
    switch (item->ItemType)
    {
    case DATA:
    case LOOP_FUNCTION:
        (item->itemFunction)(item);
        if(item->state == MENU_ENTER)Change_MenuState(APP_QUIT);
        break;
    case SWITCH:
        item->SwitchState = ! item->SwitchState;
    case ONCE_FUNCTION:
        if (item->itemFunction != NULL)(item->itemFunction)(item);
        Change_MenuState(APP_QUIT);
        break;
    default:
        break;
    }
}

void Menu_Task(void)
{
    Menu_Direction Dir = BtnScan();
    if (MENU_STATE == MENU_INIT && Dir != MENU_NONE)
    {
        Draw_Menu(FirstPos, temp_item->location, Font_Size, temp_item, temp_item);
    }
    else
    {
        switch (MENU_STATE)
        {
            case APP_RUN:case APP_DRAWING:
                Process_App_Run(temp_item, Dir);
            case MENU_RUN:case APP_QUIT:case MENU_DRAWING:
                Process_Menu_Run(Dir);
                break;
            default:
                break;
        }
    }
}

void Menu_Init(void)
{
    Disp_Init();
    Menu_Team();
    Draw_Home(NULL);
    printf("MultMenu demo\n");
}
