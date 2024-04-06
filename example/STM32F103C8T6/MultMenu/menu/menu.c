#include "menu.h"
#include "dispDirver.h"
#include "stdlib.h"
#include "application.h"
#include "DinoGame.h"
#include "AirPlane.h"

Menu_State Page_State;
//1为白天模式，0为黑夜模式
uint8_t BgColor = 0x00;  
//对话框缓动动画持续时间（次数）
uint8_t Dialog_Time = 5;

Pid_Error Cursor_Line = {900, 370, 30, 0, 0, 0}, Cursor_Wide = {450, 370, 30, 0, 0, 0};

/* Page*/
xPage
    Home_Page, 
    System_Page,
    Games_Page;
/* item */
xItem HomeHead_Item, SystemHead_Item, GamesHead_Item, System_Item, Games_Item, ShowLogo_Item, Github_Item, Bilibili_Item, ReadME_Item;
xItem MPU6050_Item, LineKp_Item, LineKi_Item, LineKd_Item, WideKp_Item, WideKi_Item, WideKd_Item, Mode_Item, Contrast_Item, Power_Item, SystemReturn_Item;
xItem Dino_Item, AirPlane_Item, GamesReturn_Item;

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
}

static void AddPage(const char *name, xpPage page, xpItem item, xpPage LocalPage, xpPage nextpage, ItemFunction function)
{
    static uint8_t ID = 0;
    page->pageName = name;
    page->itemHead = NULL;
    page->itemTail = NULL;
    page->id = ID++;
    if (page->id == 0)AddItem(name, LOOP_FUNCTION, NULL, item, LocalPage, nextpage, function);
    else AddItem(name, PARENTS, NULL, item, LocalPage, nextpage, function);
}

/**
 * @brief 线性增长函数用于坐标移动
 * 
 * @param AllTime 总时长
 * @param Time_Now 当前时间
 * @param Targrt 目标值
 * @param Now 当前值
 * @return uint8_t 
 */
static int8_t Line(uint8_t AllTime, uint8_t Time_Now, int8_t Targrt, int8_t Now)
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
//首页
void Draw_Home(xpItem item)
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
 * @param u8g2 U8G2
 * @param x 初始位置x
 * @param y 初始位置y
 * @param Targrt_w 目标宽度
 * @param Targrt_h 目标高度
 */
void DialogScale_Show(uint8_t x,uint8_t y,uint8_t Targrt_w,uint8_t Targrt_h)
{
    uint8_t t = 0;
    uint8_t Init_w = 0,Init_h = 0;

    do
    {
        t++;
        Init_w=Line(Dialog_Time, t, Targrt_w, Init_w);
        Init_h=Line(Dialog_Time, t, Targrt_h, Init_h);
        Draw_DialogBox(x, y, Init_w, Init_h);
        OLED_SendBuffer();
    } while (t < Dialog_Time);

}

void Draw_DialogRBox(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t r)
{
    OLED_SetDrawColor(BgColor^0x01);
    OLED_DrawRFrame(x, y, w, h, r);
    OLED_SetDrawColor(BgColor);
    OLED_DrawRBox(x+1, y+1, w-2, h-2, r);
    OLED_SetDrawColor(BgColor^0x01);
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
    Now_Lenght = Line(Dialog_Time, t, Next_Lenght, Now_Lenght);
    OLED_DrawLine(HOR_RES - 7, 0, HOR_RES - 7, 64);
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
    //切换页面时变量初始化
    if(next_item->location != now_item->location)first_line = FirstLine;

    if (Page_State == CURSOR_STATIC)
    {
        if ((next_item->id - now_item->id) > 0)first_line -= ((next_item->id - now_item->id) > (Page->length - MaxVisible_Number)) ? ((Page->length - MaxVisible_Number) * Font_Size) : Font_Size;
        else first_line += ((now_item->id - next_item->id) > (Page->length - MaxVisible_Number)) ? ((Page->length - MaxVisible_Number) * Font_Size) : Font_Size;
        Page_State = MENU_MOVE;
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
    int item_wide = strlen(now_item->ItemName)*6 + 4;
    int item_line = 0;
    static int Targrt_line, Targrt_wide;
    static bool first = true;     //初始状态

    OLED_SetMaxClipWindow();

    Page->itemTail->nextiTem = Page->itemHead;  
    Page->itemHead->lastiTem = Page->itemTail;
    
    if(next_item->location != now_item->location)        //切换页面时变量初始化
    {
        item_line = 0;
        Targrt_line = 0;
        first = true;
        Page_State = MENU_RUN;
    }
    if ((next_item->id == now_item->id && first == true) || next_item->location != now_item->location)
    {
        Targrt_line = 0;
        first = false;
    }
    else if (next_item->id > now_item->id)
    {
        Targrt_line += ((next_item->id - now_item->id)*Font_Size);
        if (Targrt_line > LINE_MAX)  //防止光标溢出可视范围
        {
            Page_State = CURSOR_STATIC;
            Targrt_line = LINE_MAX;
        }
    }
    else if(next_item->id < now_item->id)
    {
        Targrt_line -= ((now_item->id - next_item->id)*Font_Size);
        if (Targrt_line < 0)  //防止光标溢出可视范围
        {
            Page_State = CURSOR_STATIC;
            Targrt_line = 0;
        }
    }
    Targrt_wide = OLED_GetStrWidth(next_item->ItemName) + 3;
    bool OpState = false;
    while (item_line != Targrt_line || OpState == false || item_wide != Targrt_wide)
    {       
        OLED_ClearBuffer();
        OLED_SetDrawColor(BgColor);
        OLED_DrawBox(0, 0, 128, 64);
        OLED_SetDrawColor(BgColor^0x01);
        OpState = Draw_OptionPlace(now_item, next_item);
        Draw_Page(pos, Page, LineSpacing, now_item, next_item);
        OLED_SetDrawColor(2);
        item_line = PID(Targrt_line, item_line, &Cursor_Line);
        item_wide = PID(Targrt_wide, item_wide, &Cursor_Wide);
        OLED_DrawRBox(pos+1, item_line+1, item_wide, Font_Size, 4);
        OLED_SendBuffer();
    }
}

int Contrast = 255;

static void Menu_Team(void)
{
    AddPage("[HomePage]", &Home_Page, &HomeHead_Item, &Home_Page, NULL, Draw_Home);
        AddItem(" +System", PARENTS, NULL, &System_Item, &Home_Page, &System_Page, NULL);
            AddPage("[System]", &System_Page, &SystemHead_Item, &System_Page, &Home_Page, NULL);
                AddItem(" -MPU6050", LOOP_FUNCTION, NULL, &MPU6050_Item, &System_Page, NULL, Show_MPU6050);
                AddItem(" -Line Kp", DATA, &Cursor_Line.kp, &LineKp_Item, &System_Page, NULL, Setting_Pid);
                AddItem(" -Line Ki", DATA, &Cursor_Line.ki, &LineKi_Item, &System_Page, NULL, Setting_Pid);
                AddItem(" -Line Kd", DATA, &Cursor_Line.kd, &LineKd_Item, &System_Page, NULL, Setting_Pid);
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
}

Menu_State MENU_STATE = MENU_RUN;
//初始化为开始菜单项
xpItem temp_item = &HomeHead_Item;

void Switch_Menu_State(Menu_State state)
{
    MENU_STATE = state;
}
/* 填入按键扫描程序 */
static Menu_State BtnScan(void)
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

void Process_Menu_Run(Menu_State Dir)
{
    uint8_t disapper = 1;
    if (MENU_STATE == MENU_RUN)
    {
        switch (Dir)
        {
        case MENU_UP:
            Draw_Menu(FirstPos, temp_item->location, Font_Size, temp_item, temp_item->lastiTem);
            temp_item = temp_item->lastiTem;
            break;
        case MENU_DOWN:
            Draw_Menu(FirstPos, temp_item->location, Font_Size, temp_item, temp_item->nextiTem);
            temp_item = temp_item->nextiTem;
            break;
        case MENU_ENTER:
            if(temp_item->ItemType == PARENTS)
            {
                Switch_Menu_State(MENU_RUN);
                for (size_t i = 0; i < 8; i++)
                {
                    disapper = ui_disapper(disapper);
                }
                Draw_Menu(FirstPos, temp_item->JumpPage, Font_Size, temp_item, temp_item->JumpPage->itemHead);
                temp_item = temp_item->JumpPage->itemHead;
            }
            else
            {
                ui_disapper(disapper);
                Switch_Menu_State(APP_RUN);
            }
        default:
            break;
        }
    }
    else if(MENU_STATE == APP_BREAK)
    {
        if(Dir == MENU_ENTER)
        {
            Switch_Menu_State(MENU_RUN);
            for (size_t i = 0; i < 8; i++)
            {
                disapper = ui_disapper(disapper);
            }
            Draw_Menu(FirstPos, temp_item->location, Font_Size, temp_item, temp_item);
        }
    }
}

void Process_App_Run(xpItem item, Menu_State State)
{
    item->state = State;
    switch (item->ItemType)
    {
    case DATA:
    case LOOP_FUNCTION:
        (item->itemFunction)(item);
        if(item->state == MENU_ENTER)AppBreak();
        break;
    case SWITCH:
        item->SwitchState = ! item->SwitchState;
    case ONCE_FUNCTION:
        (item->itemFunction)(item);
        AppBreak();
        break;
    default:
        break;
    }
}

void Menu_Task(void)
{
    static bool Start;
    Menu_State Dir = BtnScan();
    if (Start == true)
    {
        switch (MENU_STATE)
        {
            case APP_RUN:
                Process_App_Run(temp_item, Dir);
            case MENU_RUN:case APP_BREAK:
                Process_Menu_Run(Dir);
                break;
            default:
                break;
        }
    }
    if (Dir != MENU_NONE && Start == false)
    {
        Draw_Menu(FirstPos, temp_item->location, Font_Size, temp_item, temp_item);
        Start = true;
    }
}

void Menu_Init(void)
{
    disp_init();
    Menu_Team();
    Draw_Home(NULL);
}
