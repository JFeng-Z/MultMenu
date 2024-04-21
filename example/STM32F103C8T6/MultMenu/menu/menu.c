#include "menu.h"
#include "dispDirver.h"
#include "stdlib.h"
#include "application.h"
#include "DinoGame.h"
#include "AirPlane.h"

/* pid动画参数 */
Pid_Error Cursor_Line = {600, 370, 30, 0, 0, 0}, Cursor_Wide = {250, 370, 30, 0, 0, 0};
/* 线性动画参数 */
Animation_Param AnimationParam = {10, 0, 0, 0, 0, 0, 0, 0, false};
/* Page*/
xPage Home_Page, System_Page, Games_Page;
/* item */
xItem HomeHead_Item, SystemHead_Item, GamesHead_Item, System_Item, Games_Item, ShowLogo_Item, Github_Item, Bilibili_Item, ReadME_Item, SWITCH1_Item, SWITCH2_Item, SWITCH3_Item, SWITCH4_Item, SWITCH5_Item;
xItem MPU6050_Item, LineKp_Item, LineKi_Item, LineKd_Item, WideKp_Item, WideKi_Item, WideKd_Item, Mode_Item, Contrast_Item, Power_Item, SystemReturn_Item;
xItem Dino_Item, AirPlane_Item, GamesReturn_Item;
/* menu */
xMenu Menu = {0, &HomeHead_Item, &HomeHead_Item, MENU_INIT, PAGE_STATIC, 0};

/**
 * 向指定页面添加一个项目。
 * 
 * @param Name 项目名称。
 * @param Type 项目类型。
 * @param Data 项目关联的数据指针。
 * @param item 待添加的项目结构体。
 * @param LocalPage 项目所属的页面。
 * @param nextpage 项目跳转到的下一个页面。
 * @param function 项目关联的函数指针。
 * 
 * 项目会被添加到指定页面的链表中，并且会更新页面的相关信息。
 */
static void AddItem(const char *Name, Item_Type Type, int *Data, xpItem item, xpPage LocalPage, xpPage nextpage, ItemFunction function)
{
    // 参数检验
    if (!Name || !item || !LocalPage || (Type == DATA && !Data)) {
        printf("Invalid parameter(s)\n");
        return; // 早期返回以避免进一步错误
    }

    item->itemName = Name; // 设置项目名称
    item->itemType = Type; // 设置项目类型
    // 如果项目类型为DATA，则设置数据指针
    if(Type == DATA)item->data.ptr = Data; 

    item->page.location = LocalPage; // 设置项目所在页面
    item->itemFunction= function; // 设置项目关联的函数

    /* 初始化项目下一个项为NULL */
    item->nextItem = NULL;

    /* 设置跳转页面的父级项目 */
    if (nextpage != NULL)
        nextpage->item.parent = item;
    else // 如果没有下一个页面，则设置为当前页面
        nextpage = LocalPage;
    item->page.jumpPage = nextpage; // 设置项目跳转页面

    /* 链式结构创建项目 */
    if (LocalPage->item.head == NULL) // 如果是首个项目
    {
        // 初始化项目链表头部和尾部
        item->lastItem = item;
        LocalPage->item.head = item;
        LocalPage->item.tail = item;
        LocalPage->length = 0; // 初始化项目计数
    }
    else // 如果不是首个项目
    {
        // 连接新项目到链表尾部
        item->lastItem = LocalPage->item.tail;                // 新项目上一个项指向当前尾项目
        LocalPage->item.tail->nextItem = item;                // 尾项目下一个项指向新项目
        LocalPage->item.tail = LocalPage->item.tail->nextItem; // 更新尾项目为新项目
        LocalPage->length++; // 项目计数加一
    }
    item->id = LocalPage->length; // 设置项目ID为当前页面项目计数
    // 关联页面的头尾项目互相指向，用于快速遍历
    LocalPage->item.tail->nextItem = LocalPage->item.head;  
    LocalPage->item.head->lastItem = LocalPage->item.tail;
}

/**
 * @brief 向页面中添加一个新页面。
 * 
 * @param name 新页面的名称。
 * @param Menu 新页面所在菜单。
 * @param page 指向新页面结构体的指针。
 * @param item 新页面对应的项。
 * @param LocalPage 当前局部页面的指针。
 * @param nextpage 下一个页面的指针。
 * @param function 项的功能函数指针。
 */
static void AddPage(const char *name, xpMenu Menu, xpPage page, xpItem item, xpPage LocalPage, xpPage nextpage, ItemFunction function)
{
    // 初始化新页面的基本信息
    page->pageName = name;
    page->item.head = NULL;
    page->item.tail = NULL;
    page->menu = Menu;
    Menu->page_length++;
    page->id = Menu->page_length; // 分配ID，并递增ID值
    // 如果是第一个页面，将其头跳转至首页；否则，将其父项设置为上一页
    if (page->id == 1)AddItem(name, LOOP_FUNCTION, NULL, item, LocalPage, nextpage, function);  
    else AddItem(name, PARENTS, NULL, item, LocalPage, nextpage, function);
}

/**
 * 使用线性插值公式计算当前时间点对应的插值结果。
 * 
 * @param AllTime 总时间长度，单位为秒。表示从开始到结束的总时间长度。
 * @param Time_Now 当前时间点，单位为秒。表示在总时间长度中的当前位置。
 * @param Targrt 目标值。在给定总时间长度和当前时间点的情况下，此函数将计算出达到此目标值的插值。
 * @param Now 当前值。表示在当前时间点的值。
 * @return 返回计算出的插值结果。此结果是根据线性插值公式，将目标值和当前值结合当前时间点与总时间长度计算得出的。
 */
static int8_t Linear(uint8_t AllTime, uint8_t Time_Now, int8_t Targrt, int8_t Now)
{
    // 根据线性插值公式，计算并返回当前时间点对应的插值结果
    return (Targrt - Now)*Time_Now/AllTime + Now;		
}

/**
 * PID 插值控制器计算函数
 * 
 * 本函数用于根据目标值、当前值及已设定的PID参数，计算出下一时刻的控制量。
 * 
 * @param Targrt 目标值，即期望的控制结果。
 * @param Now 当前值，即当前的控制结果。
 * @param Obj PID控制器的结构体对象，包含比例、积分、微分系数及误差相关信息。
 * @return 计算后得到的控制量，用于调整系统状态。
 */
static int PID(int Targrt, int Now, Pid_Error *Obj)
{
    int x = Now;
    // 将PID系数从毫单位转换为浮点数
    float Kp = (float)(Obj->kp)/1000.00, Ki = (float)(Obj->ki)/1000.00, Kd = (float)(Obj->kd)/1000.00;
    
    // 计算误差
    Obj->error = Targrt - x;
    // 积分环节，累加误差
    Obj->sum_srror += Obj->error;
    // 微分环节，计算误差变化率
    float delta_error = Obj->error - Obj->last_error;

    // 计算控制量
    float velocity = Kp * Obj->error + Ki * Obj->sum_srror + Kd * delta_error;
    
    // 更新状态
    x += velocity;
    Obj->last_error = Obj->error;

    return x;
}

static void Change_MenuState(Menu_State state)
{
    Menu.menu_state = state;
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

/**
 * 绘制对话框
 * @param x 对话框左上角x坐标
 * @param y 对话框左上角y坐标
 * @param w 对话框宽度
 * @param h 对话框高度
 * 该函数首先绘制对话框的边框，然后绘制对话框的背景盒。
 * 使用了OLED显示设备的相关函数来完成绘制操作。
 */
void Draw_DialogBox(uint16_t x,uint16_t y,uint16_t w,uint16_t h)
{
    // 设置绘制边框的颜色，并绘制边框
    OLED_SetDrawColor(Menu.BgColor^0x01);
    OLED_DrawFrame(x, y, w, h);
    
    // 设置绘制背景的颜色，并绘制背景盒
    OLED_SetDrawColor(Menu.BgColor);
    OLED_DrawBox(x+1, y+1, w-2, h-2);
    
    // 设置边框高亮颜色（通常与背景色异或得到），用于强调边框
    OLED_SetDrawColor(Menu.BgColor^0x01);
}

/**
 * @brief 显示一个按指定尺寸缩放的对话框。
 * 
 * 此函数用于在应用绘制状态时，通过线性动画效果展示一个对话框的缩放过程。函数首先检查当前是否处于应用绘制状态，
 * 如果是，则根据设定的时间和目标尺寸计算当前对话框的宽度和高度，并进行绘制。当动画时间达到预设的对话框显示时间
 * 后，将状态切换到应用运行状态，并返回true。整个过程通过OLED发送缓冲区来更新显示。
 * 
 * @param x 对话框的x坐标。
 * @param y 对话框的y坐标。
 * @param Targrt_w 目标对话框的宽度。
 * @param Targrt_h 目标对话框的高度。
 * @return bool 如果动画时间达到预设的对话框显示时间，则返回true；否则返回false。
 */
bool DialogScale_Show(uint8_t x,uint8_t y,uint8_t Targrt_w,uint8_t Targrt_h)
{
    // 当前处于应用绘制状态时，处理对话框的缩放动画
    if (Menu.menu_state == APP_DRAWING)
    {
        AnimationParam.DialogScale_time++; // 动画时间递增
        // 根据当前时间和目标尺寸计算对话框的当前宽度
        AnimationParam.DialogScale_InitWide = Linear(AnimationParam.Dialog_AllTime, AnimationParam.DialogScale_time, Targrt_w, AnimationParam.DialogScale_InitWide);
        // 根据当前时间和目标尺寸计算对话框的当前高度
        AnimationParam.DialogScale_InitHigh = Linear(AnimationParam.Dialog_AllTime, AnimationParam.DialogScale_time, Targrt_h, AnimationParam.DialogScale_InitHigh);
        // 绘制当前尺寸的对话框
        Draw_DialogBox(x, y, AnimationParam.DialogScale_InitWide, AnimationParam.DialogScale_InitHigh);
    }
    // 当动画时间达到预设的对话框显示时间时，切换到应用运行状态
    if (AnimationParam.DialogScale_time == AnimationParam.Dialog_AllTime)
    {
        Change_MenuState(APP_RUN);
        return true;
    }
    // 更新OLED显示缓冲区
    OLED_SendBuffer();
    return false;
}

/**
 * 绘制一个带有圆角的对话框背景框
 * @param x 对话框左上角x坐标
 * @param y 对话框左上角y坐标
 * @param w 对话框宽度
 * @param h 对话框高度
 * @param r 对话框圆角半径
 */
void Draw_DialogRBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r)
{
    // 设置绘制边框的颜色，先绘制外边框
    OLED_SetDrawColor(Menu.BgColor^0x01);
    OLED_DrawRFrame(x, y, w, h, r);
    
    // 设置绘制填充颜色，绘制内框，即填充部分
    OLED_SetDrawColor(Menu.BgColor);
    OLED_DrawRBox(x + 1, y + 1, w - 2, h - 2, r);
    
    // 设置边框颜色，用于强调边框，这里使用与背景色异或的方式
    OLED_SetDrawColor(Menu.BgColor^0x01);
}

/**
 * 设置背景颜色
 * @param color 要设置的颜色值，类型为uint8_t
 * 该函数用于将全局背景颜色设置为指定的颜色值。
 */
void Set_BgColor(uint8_t color)
{
    Menu.BgColor = color; // 设置背景颜色
}

/**
 * 绘制一个滚动条
 * 该函数根据给定的参数，在OLED屏幕上绘制一个滚动条。滚动条的可见部分的长度
 * 会根据当前值在最小值和最大值之间动态调整。
 * @param x 滚动条左上角的x坐标
 * @param y 滚动条左上角的y坐标
 * @param w 滚动条的宽度
 * @param h 滚动条的高度
 * @param r 滚动条圆角的半径
 * @param min 滚动条代表的最小值
 * @param max 滚动条代表的最大值
 * @param NowValue 滚动条当前的值
 */
void Draw_Scrollbar(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t r, double min, double max, uint16_t NowValue)
{
    // 根据当前值计算滚动条可见部分的长度
    double value = NowValue * ((w - 6) / (max - min)) + 6;
    // 绘制滚动条的填充部分
    OLED_DrawRBox(x, y, (uint16_t)value, h, r);
    // 绘制滚动条的边框
    OLED_DrawRFrame(x, y, w, h, r);
}

/**
 * @brief 使屏幕上的内容逐渐消失的函数
 * 
 * @param disapper 一个用于控制消失程度的参数，初始值建议为0，每次调用本函数会自动增加
 * @return uint8_t 返回调整后的disapper值，用于下次调用时继续消失过程
 */
static uint8_t ui_disapper(uint8_t disapper)
{ 
    short disapper_temp = 0;
    // 计算屏幕缓冲区的总长度
    int length = 8 * OLED_GetBufferTileHeight() * OLED_GetBufferTileWidth();
    u8 *p = OLED_GetBufferPtr(); // 获取屏幕缓冲区的指针

    // 如果背景色为黑色，那么执行与操作，让像素点逐渐变暗从而消失
    if(Menu.BgColor==0)
    { 
        for( int i = 0; i < length; i++) 
        { 
            p[i] = p[i] & (rand()%0xff) >> disapper; // 通过与操作使像素点变暗
        } 
    }
    else // 如果背景色不是黑色，执行或操作，让像素点逐渐变亮从而消失
    { 
        for( int i = 0; i < length; i++) 
        { 
            p[i] = p[i] | (rand()%0xff) >> disapper; // 通过或操作使像素点变亮
        } 
    }
    
    disapper += 2; // 每次调用使消失程度增加，以便逐渐完成消失过程
    if(disapper >= 8) // 当消失程度达到最大值时重置为0，准备下一次调用
    { 
        disapper = 0; 
    } 
    
    OLED_SendBuffer(); // 将更新后的缓冲区内容发送到OLED显示设备
    disapper_temp = disapper;
    return disapper_temp; // 返回调整后的disapper值，供外部调用者使用
}

/**
 * @brief 绘制选项位置
 * 
 * 该函数用于在OLED屏幕上动态绘制选项的位置，实现类似选项滑动选择的效果。
 * 根据当前项和下一个项的信息，计算并绘制出选项的滑动过程。
 * 
 * @param now_item 当前选项项的指针
 * @param next_item 下一个选项项的指针
 * @return bool 如果绘制过程完成（达到设定的时间），返回true；否则返回false。
 */
static bool Draw_OptionPlace(xpItem now_item, xpItem next_item)
{
    static uint8_t t; // 控制绘制过程的时间变量
    static uint8_t Now_Lenght; // 当前绘制长度
    static uint8_t Next_Lenght; // 下一个选项的理论绘制长度
    // 根据下一个项的id和位置长度，计算其理论绘制长度
    Next_Lenght = (VER_RES / (float)(next_item->page.location->length)) * next_item->id;
    t++; // 时间变量递增
    // 使用线性插值计算当前的绘制长度
    Now_Lenght = Linear(AnimationParam.Dialog_AllTime, t, Next_Lenght, Now_Lenght);
    // 绘制选项移动的指示线
    OLED_DrawLine(HOR_RES - 7, 0, HOR_RES - 7, VER_RES);
    // 根据计算出的长度，绘制当前选项的高亮框
    OLED_DrawBox(HOR_RES - 10, 0, 6, Now_Lenght);
    // 如果绘制时间达到预设的Dialog_Time，重置时间变量并返回true
    if(t == AnimationParam.Dialog_AllTime)
    {
        t = 0;
        return true;
    }
    return false;
}

/**
 * @brief 绘制指定位置的页面
 * 
 * @param pos 页面绘制的起始位置
 * @param Page 指向当前页面结构体的指针
 * @param LineSpacing 行间距
 * @param now_item 当前选中的项目
 * @param next_item 下一个将要选中的项目
 */
static void Draw_Page(uint8_t pos, xpPage Page, xpItem now_item, xpItem next_item)
{
    char Data[10] = {0}; // 用于临时存储数据字符串
    static int16_t first_line = FirstLine; // 页面中第一行的起始位置

    xpItem temp = Page->item.head; // 从页面的头部开始遍历

    // 初始化或计算页面滚动时的起始绘制位置
    if(Menu.menu_state == MENU_RUN)
    {
        // 页面切换时重置第一行位置
        if(next_item->page.location != now_item->page.location)first_line = FirstLine;

        // 计算页面滚动的距离
        if (Menu.page_state == PAGE_MOVING)
        {
            if ((next_item->id - now_item->id) > 0)first_line -= ((next_item->id - now_item->id) > (Page->length - MaxVisible_Number)) ? ((Page->length - MaxVisible_Number) * Font_Size) : Font_Size;
            else first_line += ((now_item->id - next_item->id) > (Page->length - MaxVisible_Number)) ? ((Page->length - MaxVisible_Number) * Font_Size) : Font_Size;
            Menu.page_state = PAGE_STATIC; // 更新页面状态为静态显示
        }
    }

    // 遍历页面中的所有项目并绘制
    for (uint16_t i = 0; i <= Page->length; i++)
    {
        OLED_DrawStr(pos, first_line + i * Font_Size, temp->itemName); // 绘制项目名称

        // 根据项目类型进行特殊绘制处理
        if(temp->itemType == SWITCH)
        {
            // 绘制开关状态
            if(temp->switchState == false)OLED_DrawFrame(pos + 95, first_line + i * Font_Size - Font_Size + 3, 10, 10);
            else OLED_DrawBox(pos + 95, first_line + i * Font_Size - Font_Size + 3, 10, 10);
        }
        if(temp->itemType == DATA)
        {
            // 格式化并绘制数据
            sprintf(Data, "%d", *temp->data.ptr);
            OLED_DrawStr(pos + 95, first_line + i * Font_Size, Data);
        }

        temp = temp->nextItem; // 移动到下一个项目
    }
}

/**
 * 绘制菜单界面
 * 该函数负责在 OLED 屏幕上绘制菜单界面，包括计算各项的位置、绘制背景、选项及其高亮显示。
 * 根据不同的菜单状态（如页面移动、选项切换），动态调整选项的显示位置，实现平滑的动画效果。
 * @param pos 当前页面位置
 * @param Page 当前页面结构体
 * @param LineSpacing 行间距
 * @param now_item 当前选中项
 * @param next_item 下一个将要选中的项
 */
static void Draw_Menu(uint8_t pos, xpPage Page, xpItem now_item,xpItem next_item)
{
    // 初始化或更新动画参数及页面状态
    if(Menu.menu_state == MENU_RUN)
    {
        // 计算当前项名称的宽度
        AnimationParam.Item_NowWide  = strlen(now_item->itemName)*6 + 4;

        // 切换页面时进行初始化并设置页面状态为静态
        if(next_item->page.location != now_item->page.location)        
        {
            Item_AnimationParam_Init();
            Menu.page_state = PAGE_STATIC;
        }
        else // 在同一页面内切换选项时的处理
        {
            // 根据选项的id差值计算目标行位置，并限制在可视范围内
            if (next_item->id > now_item->id)
            {
                AnimationParam.Item_TargrtLine += ((next_item->id - now_item->id)*Font_Size);
                if (AnimationParam.Item_TargrtLine > LINE_MAX)  // 防止光标溢出可视范围
                {
                    Menu.page_state = PAGE_MOVING;
                    AnimationParam.Item_TargrtLine = LINE_MAX;
                }
            }
            else if(next_item->id < now_item->id)
            {
                AnimationParam.Item_TargrtLine -= ((now_item->id - next_item->id)*Font_Size);
                if (AnimationParam.Item_TargrtLine < 0)  // 防止光标溢出可视范围
                {
                    Menu.page_state = PAGE_MOVING;
                    AnimationParam.Item_TargrtLine = 0;
                }
            }
        }
    }

    // 计算下一个将要选中项的名称宽度
    AnimationParam.Item_TargrtWide = OLED_GetStrWidth(next_item->itemName) + 3;

    // 开始绘制菜单界面
    OLED_ClearBuffer(); // 清除屏幕缓冲区
    OLED_SetDrawColor(Menu.BgColor); // 设置背景颜色
    OLED_DrawBox(0, 0, 128, 64); // 绘制屏幕背景框
    OLED_SetDrawColor(Menu.BgColor^0x01); // 设置绘制颜色为高亮或低亮
    AnimationParam.OptionState = Draw_OptionPlace(now_item, next_item); // 绘制选项及其位置
    Draw_Page(pos, Page, now_item, next_item); // 绘制页面内容
    OLED_SetDrawColor(2); // 设置特定的颜色，通常用于高亮显示
    // 根据目标位置和当前位置，以及PID算法计算并更新当前选项的位置和宽度
    AnimationParam.Item_NowLine = PID(AnimationParam.Item_TargrtLine, AnimationParam.Item_NowLine, &Cursor_Line);
    AnimationParam.Item_NowWide = PID(AnimationParam.Item_TargrtWide, AnimationParam.Item_NowWide, &Cursor_Wide);
    // 绘制选中项的高亮边框
    OLED_DrawRBox(pos+1, AnimationParam.Item_NowLine+1, AnimationParam.Item_NowWide, Font_Size, 4);
    OLED_SendBuffer(); // 将缓冲区的内容发送到OLED屏幕显示

    // 更新菜单状态为绘制中
    Change_MenuState(MENU_DRAWING);
    // 如果动画参数达到目标值且选项绘制完成，则更新菜单状态为运行中
    if ((AnimationParam.Item_NowLine == AnimationParam.Item_TargrtLine) && (AnimationParam.Item_NowWide == AnimationParam.Item_TargrtWide) && (AnimationParam.OptionState == true))
    {
        Change_MenuState(MENU_RUN);
    }
}

int Contrast = 255;

/*
 * 菜单构建函数
 * 该函数不接受参数，也不返回任何值。
 * 功能：静态地构建一个菜单系统。
 */
static void Menu_Team(void)
{
    AddPage("[HomePage]", &Menu, &Home_Page, &HomeHead_Item, &Home_Page, NULL, Draw_Home);
        AddItem(" +System", PARENTS, NULL, &System_Item, &Home_Page, &System_Page, NULL);
            AddPage("[System]", &Menu, &System_Page, &SystemHead_Item, &System_Page, &Home_Page, NULL);
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
            AddPage("[Games]", &Menu, &Games_Page, &GamesHead_Item, &Games_Page, &Home_Page, NULL);
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

/* 在此填入按键扫描程序
 * 功能：执行按键扫描，根据接收到的数据确定按键方向
 * 参数：无
 * 返回值：Menu_Direction 枚举类型，表示按键的方向，没有按键按下时返回MENU_NONE
 */
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

/**
 * @brief 根据菜单方向运行处理菜单逻辑
 * 
 * @param Dir 菜单操作方向，包括上、下、进入等操作
 */
void Process_Menu_Run(Menu_Direction Dir)
{
    uint8_t disapper = 0;
    switch (Menu.menu_state) // 根据当前菜单状态进行不同的操作
    {
        case MENU_RUN:
            switch (Dir) // 根据操作方向更新菜单项
            {
                case MENU_UP:
                    // 向上移动菜单项，确保当前项和上一项非空
                    if (Menu.now_item != NULL && Menu.now_item->lastItem != NULL) 
                    {
                        Draw_Menu(FirstPos, Menu.now_item->page.location, Menu.now_item, Menu.now_item->lastItem);
                        Menu.old_item = Menu.now_item;
                        Menu.now_item = Menu.now_item->lastItem;
                    }
                    break;
                case MENU_DOWN:
                    // 向下移动菜单项，确保当前项和下一项非空
                    if (Menu.now_item != NULL && Menu.now_item->nextItem != NULL) 
                    {
                        Draw_Menu(FirstPos, Menu.now_item->page.location, Menu.now_item, Menu.now_item->nextItem);
                        Menu.old_item = Menu.now_item;
                        Menu.now_item = Menu.now_item->nextItem;
                    }
                    break;
                case MENU_ENTER:
                    // 当前项非空且为父项时，进入下一级菜单
                    if(Menu.now_item != NULL && Menu.now_item->itemType == PARENTS)
                    {
                        // 避免重复状态改变
                        if(Menu.menu_state != MENU_ENTER) Change_MenuState(MENU_RUN);

                        for (size_t i = 0; i < 8; i++) // 执行UI淡出操作
                        {
                            disapper = ui_disapper(disapper);
                        }
                        
                        // 如果存在跳转页面且不为空，则绘制跳转页面并更新当前项
                        if(Menu.now_item->page.jumpPage != NULL && Menu.now_item->page.jumpPage->item.head != NULL) 
                        {
                            Draw_Menu(FirstPos, Menu.now_item->page.jumpPage, Menu.now_item, Menu.now_item->page.jumpPage->item.head);
                            Menu.old_item = Menu.now_item;
                            Menu.now_item = Menu.now_item->page.jumpPage->item.head;
                        }
                    }
                    else // 如果当前项为空或非父项，则进入应用绘制状态
                    {
                        ui_disapper(1);
                        Change_MenuState(APP_DRAWING);
                    }
                    break;
                default:
                    break;
            }
            break;
        case APP_QUIT:
            // 退出应用时的初始化操作，并返回菜单运行状态
            DialogScale_AnimationParam_Init();
            Change_MenuState(MENU_RUN);
            for (size_t i = 0; i < 8; i++)
            {
                disapper = ui_disapper(disapper); // 执行UI淡出操作
            }
            if(Menu.now_item != NULL) // 确保now_item非空
            {
                Draw_Menu(FirstPos, Menu.now_item->page.location, Menu.now_item, Menu.now_item);
            }
            break;
        case MENU_DRAWING:
            // 如果当前正在绘制菜单，则根据方向进行状态更新和菜单重绘
            if(Menu.now_item != NULL && Menu.old_item != NULL)
            {
                Draw_Menu(FirstPos, Menu.now_item->page.location, Menu.old_item, Menu.now_item);
                if (Dir != MENU_NONE)
                {
                    Change_MenuState(MENU_RUN);
                    Process_Menu_Run(Dir);
                }
            }
            break;
        default:
            break;
    }
}

/**
 * @brief 处理应用程序的运行状态。
 * 对给定的项目(item)根据其类型(ItemType)和当前的菜单状态(State)执行相应的操作。
 * @param item 指向要处理的项目的指针。
 * @param State 当前菜单的方向或状态。
 */
void Process_App_Run(xpItem item, Menu_Direction State)
{
    if (item == NULL) return; // 检查空指针，避免未定义行为

    item->state = State; // 设置项目的状态为当前菜单状态

    switch (item->itemType) // 根据项目类型执行不同的操作
    {
    case DATA:
    case LOOP_FUNCTION:
        (item->itemFunction)(item); // 执行项目的函数
        if(item->state == MENU_ENTER)Change_MenuState(APP_QUIT); // 如果项目状态为进入菜单，则改变菜单状态为函数退出
        break;
    case SWITCH:
        item->switchState = ! item->switchState; // 切换开关状态
    case ONCE_FUNCTION:
        if (item->itemFunction != NULL)(item->itemFunction)(item); // 如果项目有函数，则执行该函数
        Change_MenuState(APP_QUIT); // 改变菜单状态为函数退出
        break;
    default:
        break; // 对未知类型不执行任何操作
    }
}

/**
 * 菜单任务处理函数
 * 该函数负责根据当前按钮扫描结果更新菜单状态，并进行相应的绘制或处理操作。
 * 无参数
 * 无返回值
 */
void Menu_Task(void)
{
    Menu_Direction Dir = BtnScan(); // 扫描按钮方向，确定菜单操作方向
    if (Menu.menu_state == MENU_INIT && Dir != MENU_NONE)
    {
        // 初始化状态下，非空检查now_item并绘制当前菜单项
        if (Menu.now_item != NULL)
        {
            Draw_Menu(FirstPos, Menu.now_item->page.location, Menu.now_item, Menu.now_item);
        }
        else
        {
            printf("Menu.now_item is NULL\n"); // 打印错误信息，now_item为空
        }
    }
    else
    {
        switch (Menu.menu_state)
        {
            case APP_RUN:
            case APP_DRAWING:
                // 在这两个状态下，处理应用程序运行逻辑
                Process_App_Run(Menu.now_item, Dir);
                if (Menu.menu_state == APP_DRAWING)
                {
                    break; // 在绘制状态下，终止进一步流程执行
                }
            // 故意不写 break，以允许流程进入下方的共同逻辑处理（如果存在）

            case MENU_RUN:
            case APP_QUIT:
            case MENU_DRAWING:
                // 处理菜单运行、应用退出和菜单绘制状态
                Process_Menu_Run(Dir);
                break;

            default:
                // 处理未知的menu_state，可能是记录错误或默认行为
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
