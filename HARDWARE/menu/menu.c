#include "Menu.h"

u8g2_t u8g2; 

uint8_t Page_State=0;
//1为白天模式，0为黑夜模式
uint8_t BgColor=0x00;  
//选项缓动动画持续时间（次数）
uint8_t Options_Time=8;
//对话框缓动动画持续时间（次数）
uint8_t Dialog_Time=10;

typedef struct Error
{
    float error;
    float sum_srror;
    float last_error;
}Error;

Error Cursor;

/* Page*/
xpMenu NowPage;
xMenu
    MainPage,                          // 一级Page
    Page1, Page2, Page3, Page4, Page5, // 二级Page
    No3Page1, No3Page2, No3Page3;      // 三级Page

/* item */
// 一级Page的item
xpItem Selectitm;
xItem Mainitem1, Mainitem2, Mainitem3, Mainitem4, Mainitem5, Mainitem6,Mainitem7;
// 二级Page的item
xItem Page1item1, Page1item2, Page1item3, Page1item4;
xItem Page2item1, Page2item2, Page2item3, Page2item4;
xItem Page3item1, Page3item2, Page3item3, Page3item4;
xItem Page4item1, Page4item2, Page4item3, Page4item4;
xItem Page5item1, Page5item2, Page5item3, Page5item4;
// 三级Page的item
xItem No3Page1item1, No3Page1item2, No3Page1item3, No3Page1item4, No3Page1item5,
      No3Page2item1, No3Page2item2, No3Page2item3, No3Page2item4;

void AddPage(const char *name, xpMenu page)
{
    page->PageName = name;
    page->itemHead = NULL;
    page->itemTail = NULL;
}

void AddItem(const char *Name, xpItem item, xpMenu LocalPage, xpMenu nextpage, Itemfunction function)
{
    item->itemName = Name;
    item->location = LocalPage;
    item->JumpPage = nextpage;
    item->Itemfunction = function;
    /* 新建item的下一个肯定是null */
    item->nextiTem = NULL;
    /* 如果可以跳转，那么此item是跳转页面的父级 */
    if (nextpage != NULL)
        nextpage->ParentiTem = item;
    else nextpage = LocalPage;
    /* 链式结构创建item */
    if (LocalPage->itemHead == NULL) // 如果是第一个iTem
    {
        item->lastiTem = item;
        LocalPage->itemHead = item;
        LocalPage->itemTail = item;
        LocalPage->len = 1;
    }
    else // 不是第一个item
    {
        item->lastiTem = LocalPage->itemTail;                // 新item的last指向Local的tailitem
        LocalPage->itemTail->nextiTem = item;                // 让尾巴的next指向新的item，连接起来
        LocalPage->itemTail = LocalPage->itemTail->nextiTem; // 让尾巴指向新的item
        LocalPage->len++;
    }
    item->Number=LocalPage->len;
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
int8_t Line(uint8_t AllTime,uint8_t Time_Now,int8_t Targrt,int8_t Now)
{
    return (Targrt-Now)*Time_Now/AllTime+Now;		
}

uint8_t PID(int8_t Targrt, int8_t Now, Error *Obj)
{
    int x=Now;
    float Kp=0.5,ki=0.1,kd=0.25;
    Obj->error = Targrt - x;
    Obj->sum_srror += Obj->error;
    float delta_error = Obj->error - Obj->last_error;
    float velocity = Kp * Obj->error + ki * Obj->sum_srror + kd * delta_error;
    x += velocity;
    Obj->last_error = Obj->error;
    return x;
}

void Draw_Process(void)
{
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2,u8g2_font_profont15_mf);
    u8g2_DrawStr(&u8g2,32,16,"Mr.JFeng");
    u8g2_SetFont(&u8g2,MENU_FONT);
    
    for(size_t i=10; i<=80; i+=2)
    {
        char buff[6];
        int percentage = (int)(i/80.0*100);
        sprintf(buff, "%02d%%", percentage);
        
        u8g2_DrawStr(&u8g2,100,41,buff);
        u8g2_DrawRBox(&u8g2,16,32,i,10,4);
        u8g2_DrawRFrame(&u8g2,16,32,80,10,4);
        u8g2_SendBuffer(&u8g2);
    }
}

void Draw_DialogBox(u8g2_t *u8g2,u8g2_uint_t x,u8g2_uint_t y,u8g2_uint_t w,u8g2_uint_t h)
{
    u8g2_SetDrawColor(u8g2,BgColor^0x01);
    u8g2_DrawFrame(u8g2,x,y,w,h);
    u8g2_SetDrawColor(u8g2,BgColor);
    u8g2_DrawBox(u8g2,x+1,y+1,w-2,h-2);
    u8g2_SetDrawColor(u8g2,BgColor^0x01);
}

void Draw_DialogRBox(u8g2_t *u8g2,u8g2_uint_t x,u8g2_uint_t y,u8g2_uint_t w,u8g2_uint_t h,u8g2_uint_t r)
{
    u8g2_SetDrawColor(u8g2,BgColor^0x01);
    u8g2_DrawRFrame(u8g2,x,y,w,h,r);
    u8g2_SetDrawColor(u8g2,BgColor);
    u8g2_DrawRBox(u8g2,x+1,y+1,w-2,h-2,r);
    u8g2_SetDrawColor(u8g2,BgColor^0x01);
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
void DialogScale_Show(u8g2_t *u8g2,uint16_t x,uint16_t y,uint16_t Targrt_w,uint16_t Targrt_h)
{
    uint8_t t=0;
    uint16_t Init_w=0,Init_h=0;

    do
    {
        t++;
        Init_w=Line(Dialog_Time,t,Targrt_w,Init_w);
        Init_h=Line(Dialog_Time,t,Targrt_h,Init_h);
        Draw_DialogBox(u8g2,x,y,Init_w,Init_h);
        u8g2_SendBuffer(u8g2);
    } while (t<Dialog_Time);

}
/**
 * @brief 渐变消失函数
 * 
 * 
 */
uint8_t ui_disapper(uint8_t disapper)
{ 
    short disapper_temp = 0;
    int len = 8 * u8g2_GetBufferTileHeight(&u8g2) * u8g2_GetBufferTileWidth(&u8g2);
    u8 *p = u8g2_GetBufferPtr(&u8g2); 
    if(BgColor==0)
    {  for( int i = 0;i< len ;i++) 
    { p[i] = p[i] & (rand()%0xff) >> disapper; } }
    else
    {  for( int i = 0;i< len ;i++) 
    { p[i] = p[i] | (rand()%0xff) >> disapper; } }
    disapper +=2; 
    if(disapper >= 8) 
    {disapper = 0; } 
    u8g2_SendBuffer(&u8g2);
    disapper_temp=disapper;
    return disapper_temp;
}
/**
 * @brief 选项栏
 * 
 * @param now_time 
 * @param now_item 
 * @param next_item 
 */
void Draw_OptionPlace(uint8_t now_time, xpItem now_item, xpItem next_item)
{
    static uint8_t now_Y=0;
    static uint8_t next_Y=0;
    next_Y=(next_item->Number-1)*(64/next_item->location->len);
    u8g2_DrawVLine(&u8g2,122,2,64);
    for (size_t i = 0; i < next_item->location->len; i++)
    {
        u8g2_DrawHLine(&u8g2,119,i*(64/next_item->location->len)+2,6);
    }
    now_Y=Line(Options_Time,now_time,next_Y,now_Y);
    u8g2_DrawBox(&u8g2,118,now_Y,8,4);
}

void Draw_Page(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item)
{
    static int8_t first_line=FirstLine;
    xpItem temp = Page->itemHead;
    
    if(next_item==now_item->JumpPage->itemHead&&next_item!=now_item)    //切换页面时变量初始化
    first_line=FirstLine;
    if ((next_item->Number-now_item->Number>0)&&Page_State==CURSOR_STATIC)
    {
        Page_State=MENU_MOVE;
        if((next_item->Number-now_item->Number)>(Page->len-MaxVisible_Number))
        first_line-=((Page->len-MaxVisible_Number)*Font_Size);  //除去不移动时的项目数
        else first_line-=Font_Size;
    }
    else if ((next_item->Number-now_item->Number<0)&&Page_State==CURSOR_STATIC)
    {
        Page_State=MENU_MOVE;
        if((now_item->Number-next_item->Number)>(Page->len-MaxVisible_Number))
        first_line+=((Page->len-MaxVisible_Number)*Font_Size);  //除去不移动时的项目数
        else first_line+=Font_Size;
    }
    for (size_t i = 1; i <= Page->len; i++)
    {
        if((first_line + i * LineSpacing)<=FirstLine);      //菜单移动时不让标题移动
        else u8g2_DrawStr(&u8g2,pos,first_line + i * LineSpacing,temp->itemName);
        temp = temp->nextiTem;
    }
    u8g2_DrawStr(&u8g2,pos,FirstLine,Page->PageName); 
  
}

void Draw_Menu(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item)
{
    uint8_t t=0;
    uint8_t item_wide=strlen(now_item->itemName)*6+4;
    static uint8_t item_line=LINE_MIN;
    static int8_t Targrt_line=0;
    static uint8_t first=0;     //初始状态

    u8g2_SetMaxClipWindow(&u8g2);
    u8g2_SetFont(&u8g2,MENU_FONT);
    
    if(next_item==now_item->JumpPage->itemHead&&next_item!=now_item)        //切换页面时变量初始化
    {
        item_line=LINE_MIN;
        Targrt_line=0;
        first=0;
        Page_State=0;
    }
    if ((next_item->Number-now_item->Number==0&&first==0)||next_item==now_item->JumpPage->itemHead)
    {
        Targrt_line=LINE_MIN;
        first=1;
    }
    else if (next_item->Number-now_item->Number>0)
    {
        Targrt_line+=((next_item->Number-now_item->Number)*Font_Size);
        if (Targrt_line>LINE_MAX)  //防止光标溢出可视范围
        {
            Page_State=CURSOR_STATIC;
            Targrt_line=LINE_MAX;
        }
    }
    else if(next_item->Number-now_item->Number<0)
    {
        Targrt_line-=((now_item->Number-next_item->Number)*Font_Size);
        if (Targrt_line<LINE_MIN)  //防止光标溢出可视范围
        {
            Page_State=CURSOR_STATIC;
            Targrt_line=LINE_MIN;
        }
    }
    Page->itemTail->nextiTem=Page->itemHead;  
    Page->itemHead->lastiTem=Page->itemTail;
    do
    {
        u8g2_ClearBuffer(&u8g2);
        t++;
        u8g2_SetDrawColor(&u8g2,BgColor);
        u8g2_DrawBox(&u8g2,0,0,128,64);
        u8g2_SetDrawColor(&u8g2,BgColor^0x01);
        Draw_OptionPlace(t,now_item,next_item);
        Draw_Page(pos,Page,LineSpacing,now_item,next_item);
        u8g2_SetDrawColor(&u8g2,2);
        item_line = PID(Targrt_line,item_line,&Cursor);
        if(t>=Options_Time)item_line=Targrt_line;
        item_wide = PID(strlen(next_item->itemName)*6+4,item_wide,&Cursor);
        if(t>=Options_Time)item_wide=strlen(next_item->itemName)*6+4;
        u8g2_DrawRBox(&u8g2,pos+1,item_line-1,item_wide,Font_Size,4);
        u8g2_SendBuffer(&u8g2);
    } while (t<Options_Time);
}

void Menu_Team(void)
{
    NowPage = &MainPage;
    Selectitm = &Mainitem1;
    MainPage.ParentiTem = NULL;

    AddPage("[MainPage]", &MainPage);
    AddItem(" -Application", &Mainitem1, &MainPage, &Page1, NULL);
    AddItem(" -Files", &Mainitem2, &MainPage, &Page2, NULL);
    AddItem(" -Image", &Mainitem3, &MainPage, &Page3, NULL);
    AddItem(" -Reset All", &Mainitem4, &MainPage, &Page4, NULL);
    AddItem(" -About", &Mainitem5, &MainPage, &Page5, NULL);

        AddPage("[Application]", &Page1);
        AddItem(" -System", &Page1item1, &Page1, &No3Page1, NULL);
        AddItem(" -Games", &Page1item2, &Page1, &No3Page2, NULL);
        AddItem(" -Screen", &Page1item3, &Page1, NULL, Screen);
        AddItem(" -Return", &Page1item4, &Page1, &MainPage, NULL);

            AddPage("[System]", &No3Page1);
            AddItem(" -MPU6050", &No3Page1item1, &No3Page1, NULL, Show_MPU6050);
            AddItem(" -Speed", &No3Page1item2, &No3Page1, NULL, Setting_Speed);
            AddItem(" -Mode", &No3Page1item3, &No3Page1, NULL, White_Dark_Day);
            AddItem(" -Return", &No3Page1item5, &No3Page1, &Page1, NULL);

            AddPage("[Games]", &No3Page2);
            AddItem(" -Dino Game", &No3Page2item1, &No3Page2, NULL, DinoGame_Run);
            AddItem(" -AirPlane Game", &No3Page2item2, &No3Page2, NULL, AirPlane_Run);
            AddItem(" -Return", &No3Page2item4, &No3Page2, &Page1, NULL);

        AddPage("[Files]", &Page2);
        AddItem(" -New Project", &Page2item1, &Page2, NULL, NULL);
        AddItem(" -New Project", &Page2item2, &Page2, NULL, NULL);
        AddItem(" -New Project", &Page2item3, &Page2, NULL, NULL);
        AddItem(" -Return", &Page2item4, &Page2, &MainPage, NULL);

        AddPage("[Image]", &Page3);
        AddItem(" -New Project", &Page3item1, &Page3, NULL, NULL);
        AddItem(" -New Project", &Page3item2, &Page3, NULL, NULL);
        AddItem(" -New Project", &Page3item3, &Page3, NULL, NULL);
        AddItem(" -Return", &Page3item4, &Page3, &MainPage, NULL);

        AddPage("[Reset All]", &Page4);
        AddItem(" -Reset Name", &Page4item1, &Page4, NULL, NULL);
        AddItem(" -Reset Time", &Page4item2, &Page4, NULL, NULL);
        AddItem(" -Reset Setting", &Page4item3, &Page4, NULL, NULL);
        AddItem(" -Return", &Page4item4, &Page4, &MainPage, NULL);

        AddPage("[About]", &Page5);
        AddItem(" -Github", &Page5item1, &Page5, NULL, Show_GitHub);
        AddItem(" -Bilibili", &Page5item2, &Page5, NULL, Show_Bilibili);
        AddItem(" -ReadME", &Page5item3, &Page5, NULL, NULL);
        AddItem(" -Return", &Page5item4, &Page5, &MainPage, NULL);
}

uint8_t MENU_STATE=MENU_RUN;
uint8_t disapper = 1;
xpItem temp_item=&Mainitem1;
Itemfunction App_Function;

void Switch_Menu_State(uint8_t state)
{
    MENU_STATE = state;
}

void Process_Menu_Run(uint8_t Dir)
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
            if (MENU_STATE == MENU_RUN)
            {
                if (temp_item->Itemfunction != NULL)
                {
                    ui_disapper(disapper);
                    App_Function = temp_item->Itemfunction;
                    Switch_Menu_State(APP_RUN);
                }
                else
                {
                    Switch_Menu_State(MENU_RUN);
                    for (size_t i = 0; i < 8; i++)
                    {
                        disapper = ui_disapper(disapper);
                    }
                    Draw_Menu(FirstPos, temp_item->JumpPage, Font_Size, temp_item, temp_item->JumpPage->itemHead);
                    temp_item = temp_item->JumpPage->itemHead;
                }
            }
            if(MENU_STATE == APP_BREAK)
            {
                Switch_Menu_State(MENU_RUN);
                for (size_t i = 0; i < 8; i++)
                {
                    disapper = ui_disapper(disapper);
                }
                Draw_Menu(FirstPos, temp_item->location, Font_Size, temp_item, temp_item);
            }
            break;
        default:
            break;
    }
}

void Process_App_Run(void)
{
    (*App_Function)();
}

TaskHandle_t Menu_TaskHandle;

void Menu_Task(void *parameter)
{
    while (1)
    {
        uint8_t Dir = Get_Key_Pressed();
        
        switch (MENU_STATE)
        {
            case MENU_RUN:
                Process_Menu_Run(Dir);
                break;
                
            case APP_RUN:
                Process_App_Run();
                break;
                
            case APP_BREAK:
                Process_Menu_Run(Dir);
                break;
                
            default:
                break;
        }
        vTaskDelay(5);
    }
}

void Menu_Task_Create(void)
{
    xTaskCreate((TaskFunction_t)Menu_Task,"Menu_Task",128,NULL,6,Menu_TaskHandle);
}

void U8G2_Init(void)
{
    u8g2Init(&u8g2);    
}

void Menu_Init(void)
{
    Menu_Team();
    Draw_Process();
    Draw_Menu(FirstPos,&MainPage,Font_Size,&Mainitem1,&Mainitem1);
}