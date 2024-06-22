#include "menu.h"
#include "dispDirver.h"
#include "application.h"
#include "image.h"
#include "DinoGame.h"
#include "AirPlane.h"

/* Page*/
xPage Home_Page, System_Page, CursorAnimation_Page, SettingTextPage_Page, SettingImagePage_Page, Games_Page;
/* item */
xItem HomeHead_Item, SystemHead_Item, GamesHead_Item, System_Item, Games_Item, Image_Item, Github_Item, Bilibili_Item, ReadME_Item;
xItem CursorAnimationHead_Item, CursorKp_Item, CursorKi_Item, CursorKd_Item;
xItem SettingTextPageHead_Item, TextSpace_Item;
xItem SettingImagePageHead_Item, ImageSpace_Item;
xItem MPU6050_Item, CursorAnimation_Item, SettingTextPage_Item, SettingImagePage_Item, Mode_Item, Contrast_Item, Power_Item;
xItem Dino_Item, AirPlane_Item;

const uint8_t Presets_Logo [] = {
	0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0x3f, 
	0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0x3f, 0xcf, 0xff, 0xff, 0x3f, 
	0xcf, 0xff, 0xff, 0x3f, 0xcf, 0x3f, 0xff, 0x3f, 0xcf, 0x1f, 0xff, 0x3f, 0xcf, 0x8f, 0xff, 0x3f, 
	0xcf, 0xe7, 0xff, 0x3f, 0xcf, 0xf3, 0xff, 0x3f, 0xcf, 0x01, 0x00, 0x3c, 0xcf, 0x01, 0x00, 0x3c, 
	0xcf, 0xf3, 0xff, 0x3f, 0xcf, 0xe7, 0xff, 0x3f, 0xcf, 0x8f, 0xff, 0x3f, 0xcf, 0x1f, 0xff, 0x3f, 
	0xcf, 0x3f, 0xff, 0x3f, 0xcf, 0xff, 0xff, 0x3f, 0xcf, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0x3f, 
	0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0x3f, 
	0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0x3f
};

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
static void AddItem(const char *Name, Item_Type Type, void *Data, const uint8_t *Image, xpItem item, xpPage LocalPage, xpPage nextpage, ItemFunction function)
{
    // 参数检验
    if (!Name || !item || !LocalPage || (Type == DATA && !Data)) {
        printf("Invalid parameter(s)\n");
        return; // 早期返回以避免进一步错误
    }

    item->itemName = Name; // 设置项目名称
    item->itemType = Type; // 设置项目类型
    // 如果项目类型为DATA，则设置数据指针
    if(Type == DATA)item->data.ptr = (int *)Data; 
    if(LocalPage->type == IMAGE)item->image = Image;
    if(LocalPage->type == IMAGE && Image == NULL)
    {
        item->image = Presets_Logo;
        printf("%s : Image is null!\n", item->itemName);
    }
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
 * @param menu 新页面所在菜单。
 * @param page 指向新页面结构体的指针。
 * @param item 新页面对应的项。
 * @param LocalPage 当前局部页面的指针。
 * @param nextpage 下一个页面的指针。
 * @param function 项的功能函数指针。
 */
static void AddPage(const char *name, xpPage page, Page_Type Type)
{
    // 初始化新页面的基本信息
    page->pageName = name;
    page->item.head = NULL;
    page->item.tail = NULL;
    page->type = Type;
}

/**
 * PID_Animation 插值控制器计算函数
 * 
 * 本函数用于根据目标值、当前值及已设定的PID参数，计算出下一时刻的控制量。
 * 
 * @param Targrt 目标值，即期望的控制结果。
 * @param Now 当前值，即当前的控制结果。
 * @param Obj PID控制器的结构体对象，包含比例、积分、微分系数及误差相关信息。
 * @return 计算后得到的控制量，用于调整系统状态。
 */
static int PID_Animation(int Targrt, int Now, AnimationParam *Obj)
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

static void Change_MenuState(xpMenu Menu, Menu_State state)
{
    Menu->menu_state = state;
}

void Switch_Widget(xpMenu Menu)
{
    
}

void ParamSet_Widget(xpMenu Menu)
{
    
}

static void CursorAnimationParam_Init(xpMenu Menu)
{
    Menu->now_item->page.location->cursorLastColumn = Menu->_cursor.TargrtColumn;
    Menu->now_item->page.location->cursorLastRow = Menu->_cursor.TargrtRow;
    Menu->now_item->page.location->cursorLastWide = Menu->_cursor.TargrtWide;
    Menu->_cursor.TargrtColumn = 0;
    Menu->_cursor.TargrtRow = 0;
    Menu->_cursor.TargrtWide = 0;
}

static void CursorAnimationParam_Change(xpMenu Menu)
{
    Menu->_cursor.TargrtColumn = Menu->now_item->page.jumpPage->cursorLastColumn;
    Menu->_cursor.TargrtRow = Menu->now_item->page.jumpPage->cursorLastRow;
    Menu->_cursor.TargrtWide = Menu->now_item->page.jumpPage->cursorLastWide;
}

static void DialogScale_AnimationParam_Init(xpMenu Menu)
{
    Menu->_dialogScale.now_high = 0;
    Menu->_dialogScale.now_wide = 0;
}

//首页
static void Draw_Home(xpMenu Menu)
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
void Draw_DialogBox(xpMenu Menu, uint16_t x,uint16_t y,uint16_t w,uint16_t h)
{
    // 设置绘制边框的颜色，并绘制边框
    OLED_SetDrawColor(Menu->bgColor^0x01);
    OLED_DrawBox(x, y, w, h);
    // 设置绘制背景的颜色，并绘制背景盒
    OLED_SetDrawColor(Menu->bgColor);
    OLED_DrawBox(x+1, y+1, Menu->_dialogScale.now_wide-2, Menu->_dialogScale.now_high-2);
    
    // 设置边框高亮颜色（通常与背景色异或得到），用于强调边框
    OLED_SetDrawColor(Menu->bgColor^0x01);
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
bool DialogScale_Show(xpMenu Menu, int16_t x,int16_t y,int16_t Targrt_w,int16_t Targrt_h)
{
    // 当前处于应用绘制状态时，处理对话框的缩放动画
    if (Menu->menu_state == APP_DRAWING)
    {
        // 根据当前时间和目标尺寸计算对话框的当前宽度
        Menu->_dialogScale.now_wide = PID_Animation(Targrt_w, Menu->_dialogScale.now_wide, &Menu->_animation.Dialog_Scale);
        // 根据当前时间和目标尺寸计算对话框的当前高度
        Menu->_dialogScale.now_high = PID_Animation(Targrt_h, Menu->_dialogScale.now_high, &Menu->_animation.Dialog_Scale);
        // 绘制当前尺寸的对话框
        Draw_DialogBox(Menu, x, y, Menu->_dialogScale.now_wide, Menu->_dialogScale.now_high);
    }

    // 当动画时间达到预设的对话框显示时间时，切换到应用运行状态
    if (Menu->_dialogScale.now_wide >= (Targrt_w - 2) && Menu->_dialogScale.now_high >= (Targrt_h - 2))
    {
        Change_MenuState(Menu, APP_RUN);
        return true;
    }
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
void Draw_DialogRBox(xpMenu Menu, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r)
{
    // 设置绘制边框的颜色，先绘制外边框
    OLED_SetDrawColor(Menu->bgColor^0x01);
    OLED_DrawRFrame(x, y, w, h, r);
    
    // 设置绘制填充颜色，绘制内框，即填充部分
    OLED_SetDrawColor(Menu->bgColor);
    OLED_DrawRBox(x + 1, y + 1, w - 2, h - 2, r);
    
    // 设置边框颜色，用于强调边框，这里使用与背景色异或的方式
    OLED_SetDrawColor(Menu->bgColor^0x01);
}

/**
 * 设置背景颜色
 * @param color 要设置的颜色值，类型为uint8_t
 * 该函数用于将全局背景颜色设置为指定的颜色值。
 */
void Set_BgColor(xpMenu Menu, uint8_t color)
{
    Menu->bgColor = color; // 设置背景颜色
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
 * @param step 滚动条的步进值
 * @param NowValue 滚动条当前的值
 */
int Draw_Scrollbar(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, double min, double max, int step, int NowValue)
{
    int value = NowValue;
    // 根据当前值计算滚动条可见部分的长度
    if ((value <= max) && (value >= min))
    {
        value += step;
        if (value > max)
        {
            value = max;
        }
        if (value < min)
        {
            value = min;
        }
        double value = (double)(abs(value - min) * ((w - 6)) / (double)((max - min)) + 6);
        // 绘制滚动条的填充部分
        OLED_SetDrawColor(0);
        OLED_DrawBox(x + (uint16_t)value, y, w-(uint16_t)value, h);
        OLED_SetDrawColor(1);
        OLED_DrawBox(x, y, (uint16_t)value, h);
    }
    return value;
}

static void Setting_AnimationParam(xpMenu Menu)
{
    char value[20] = {0};
    
    if (DialogScale_Show(Menu, 4, 12, 120, 32))
    {

        switch (Menu->now_item->state)
        {
        case MENU_UP:
            (*Menu->now_item->data.ptr) = Draw_Scrollbar(8, 30, 90, 6, 3, -1000, 1000, 10, (*Menu->now_item->data.ptr));
            break;
        case MENU_DOWN:
            (*Menu->now_item->data.ptr) = Draw_Scrollbar(8, 30, 90, 6, 3, -1000, 1000, -10, (*Menu->now_item->data.ptr));
        default:
            Draw_Scrollbar(8, 30, 90, 6, 3, -1000, 1000, 0, (*Menu->now_item->data.ptr));
            break;
        }

        sprintf(value, "Value = %d ", (*Menu->now_item->data.ptr));
        OLED_DrawStr(8, 25, value);
        OLED_SendBuffer();
    }

}

static void Setting_Space(xpMenu Menu)
{
    char value[20] = {0};
    
    if (DialogScale_Show(Menu, 4, 12, 120, 32))
    {

        switch (Menu->now_item->state)
        {
        case MENU_UP:
            (*Menu->now_item->data.ptr) = Draw_Scrollbar(8, 30, 90, 6, 3, 0, 100, 2, (*Menu->now_item->data.ptr));
            break;
        case MENU_DOWN:
            (*Menu->now_item->data.ptr) = Draw_Scrollbar(8, 30, 90, 6, 3, 0, 100, -2, (*Menu->now_item->data.ptr));
        default:
            Draw_Scrollbar(8, 30, 90, 6, 3, 0, 100, 0, (*Menu->now_item->data.ptr));
            break;
        }

        sprintf(value, "Space = %d ", (*Menu->now_item->data.ptr));
        OLED_DrawStr(8, 25, value);
        OLED_SendBuffer();
    }
}

static void Setting_Contrast(xpMenu Menu)
{
    char contrast[20];
    if(DialogScale_Show(Menu, 4, 12, 120, 32))
    {

        switch (Menu->now_item->state)
        {
        case MENU_UP:
            (*Menu->now_item->data.ptr) = Draw_Scrollbar(8, 30, 90, 6, 3, 0, 255, 5, (*Menu->now_item->data.ptr));
            break;
        case MENU_DOWN:
            (*Menu->now_item->data.ptr) = Draw_Scrollbar(8, 30, 90, 6, 3, 0, 255, -5, (*Menu->now_item->data.ptr));
            break;
        default:
            Draw_Scrollbar(8, 30, 90, 6, 3, 0, 255, 0, (*Menu->now_item->data.ptr));
            break;
        }

        sprintf(contrast, "Contrast = %d", (*Menu->now_item->data.ptr));
        OLED_DrawStr(8, 25, contrast);

        OLED_SetContrast((*Menu->now_item->data.ptr));
        OLED_SendBuffer();
    }
}

/**
 * @brief 使屏幕上的内容逐渐消失的函数
 * 
 * @param disapper 一个用于控制消失程度的参数，初始值建议为0，每次调用本函数会自动增加
 * @return uint8_t 返回调整后的disapper值，用于下次调用时继续消失过程
 */
static uint8_t ui_disapper(xpMenu Menu, uint8_t disapper)
{ 
    short disapper_temp = 0;
    // 计算屏幕缓冲区的总长度
    int length = 8 * OLED_GetBufferTileHeight() * OLED_GetBufferTileWidth();
    u8 *p = OLED_GetBufferPtr(); // 获取屏幕缓冲区的指针

    // 如果背景色为黑色，那么执行与操作，让像素点逐渐变暗从而消失
    if(Menu->bgColor==0)
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
static bool Draw_TextPage_OptionPlace(xpMenu Menu, xpItem now_item, xpItem next_item)
{
    // 根据下一个项的id和位置长度，计算其理论绘制长度
    Menu->_optionPlace.TextPage_OptionPlace.target_lenght = (VER_RES / (float)(next_item->page.location->length)) * next_item->id;
    // 使用线性插值计算当前的绘制长度
    Menu->_optionPlace.TextPage_OptionPlace.now_lenght = PID_Animation(Menu->_optionPlace.TextPage_OptionPlace.target_lenght, Menu->_optionPlace.TextPage_OptionPlace.now_lenght, &Menu->_animation.OptionPlace);
    // 绘制选项移动的指示线
    OLED_DrawLine(HOR_RES - 7, 0, HOR_RES - 7, VER_RES);
    // 根据计算出的长度，绘制当前选项的高亮框
    OLED_DrawBox(HOR_RES - 10, 0, 6, Menu->_optionPlace.TextPage_OptionPlace.now_lenght);

    if(Menu->_optionPlace.TextPage_OptionPlace.now_lenght == Menu->_optionPlace.TextPage_OptionPlace.target_lenght)
    {
        return true;
    }
    return false;
}

static bool Draw_ImagePage_OptionPlace(xpMenu Menu, xpItem now_item, xpItem next_item)
{
    // 根据下一个项的id和位置长度，计算其理论绘制长度
    Menu->_optionPlace.ImagePage_OptionPlace.target_lenght = (HOR_RES / (float)(next_item->page.location->length)) * next_item->id;
    // 使用线性插值计算当前的绘制长度
    Menu->_optionPlace.ImagePage_OptionPlace.now_lenght = PID_Animation(Menu->_optionPlace.ImagePage_OptionPlace.target_lenght, Menu->_optionPlace.ImagePage_OptionPlace.now_lenght, &Menu->_animation.OptionPlace);
    // 绘制选项移动的指示线
    OLED_DrawLine(0, 2, HOR_RES, 2);
    // 根据计算出的长度，绘制当前选项的高亮框
    OLED_DrawBox(0, 0, Menu->_optionPlace.ImagePage_OptionPlace.now_lenght, 6);
    // 如果绘制时间达到预设的Dialog_Time，重置时间变量并返回true
    if(Menu->_optionPlace.ImagePage_OptionPlace.now_lenght == Menu->_optionPlace.ImagePage_OptionPlace.target_lenght)
    {
        return true;
    }
    return false;
}

static void Calculate_Menu_xy(xpMenu Menu, xpPage Page, xpItem now_item, xpItem next_item)
{
    switch (Page->type)
    {
    case TEXT:
        // 计算页面滚动的距离
        if (Menu->page_state == PAGE_MOVING)
        {
            if ((next_item->id - now_item->id) > 0)Menu->headY -= ((next_item->id - now_item->id) > (Page->length - VER_RES/Menu->text_space)) ? ((Page->length + 1 - VER_RES/Menu->text_space) * Menu->text_space) : Menu->text_space;
            else Menu->headY += ((now_item->id - next_item->id) > (Page->length - VER_RES/Menu->text_space)) ? ((Page->length + 1 - VER_RES/Menu->text_space) * Menu->text_space) : Menu->text_space;
            Menu->page_state = PAGE_STATIC; // 更新页面状态为静态显示
        }

        // 页面切换时重置第一行位置
        if(next_item->page.location != now_item->page.location)
        {
            if(now_item->itemType == PARENTS)
            {
                now_item->page.location->jumpX = Menu->headX;
                now_item->page.location->jumpY = Menu->headY;
                Menu->headY = Init_y;
                Menu->headX = Init_x;
            }
            else if (now_item->itemType == RETURN)
            {
                Menu->headY = next_item->page.location->jumpY;
                Menu->headX = next_item->page.location->jumpX;
            }
        }
        break;
    case IMAGE:
        // 计算页面滚动的距离
        if (Menu->page_state == PAGE_MOVING)
        {
            if ((next_item->id - now_item->id) > 0)Menu->headX -= ((next_item->id - now_item->id) > (Page->length - HOR_RES/Menu->image_space)) ? ((Page->length + 1 - HOR_RES/Menu->image_space) * Menu->image_space) : Menu->image_space;
            else Menu->headX += ((now_item->id - next_item->id) > (Page->length - HOR_RES/Menu->image_space)) ? ((Page->length + 1 - HOR_RES/Menu->image_space) * Menu->image_space) : Menu->image_space;
            Menu->page_state = PAGE_STATIC; // 更新页面状态为静态显示
        }

        // 页面切换时重置第一行位置
        if(next_item->page.location != now_item->page.location)
        {
            if(now_item->itemType == PARENTS)
            {
                now_item->page.location->jumpX = Menu->headX;
                now_item->page.location->jumpY = Menu->headY;
                Menu->headX = Init_x;
                Menu->headY = Init_y;
            }
            else if (now_item->itemType == RETURN)
            {
                Menu->headX = next_item->page.location->jumpX;
                Menu->headY = next_item->page.location->jumpY;
            }
        }
        break;
    default:
        break;
    }
}

static void Calculate_Item_xy(xpMenu Menu, xpPage Page)
{
    xpItem Item = Page->item.head; // 从页面的头部开始遍历
    for (uint16_t i = 0; i <= Page->length; i++)
    {
        switch (Page->type)
        {
        case TEXT:
            Item->y = Menu->headY + i * Menu->text_space;
            break;
        case IMAGE:
            Item->x = Menu->headX + i * Menu->image_space;
            break;
        default:
            break;
        }
        Item = Item->nextItem;
    }
}

static void Calculate_Coordinate(xpMenu Menu, xpPage Page, xpItem now_item, xpItem next_item)
{
    // 初始化或计算页面滚动时的起始绘制位置
    if(Menu->menu_state == MENU_RUN)
    {
        Calculate_Menu_xy(Menu, Page, now_item, next_item);
    }

    Calculate_Item_xy(Menu, Page);
}

static void Calculate_Cursor(xpMenu Menu, xpPage Page, xpItem now_item,xpItem next_item)
{
    switch (Page->type)
    {
    case TEXT:
        // 计算当前项名称的宽度
        Menu->_cursor.NowWide  = strlen(now_item->itemName)*6 + 4;

        // 切换页面时进行初始化并设置页面状态为静态
        if(next_item->page.location != now_item->page.location)        
        {
            if(now_item->itemType == PARENTS)CursorAnimationParam_Init(Menu);
            else if (now_item->itemType == RETURN)CursorAnimationParam_Change(Menu);
            Menu->page_state = PAGE_STATIC;
        }
        else // 在同一页面内切换选项时的处理
        {
            // 根据选项的id差值计算目标行位置，并限制在可视范围内
            if (next_item->id > now_item->id)
            {
                Menu->_cursor.TargrtRow += ((next_item->id - now_item->id)*Menu->text_space);
                if (Menu->_cursor.TargrtRow > (Menu->text_space*(VER_RES/Menu->text_space) - Menu->text_space))  // 防止光标溢出可视范围
                {
                    Menu->page_state = PAGE_MOVING;
                    Menu->_cursor.TargrtRow = (Menu->text_space*(VER_RES/Menu->text_space) - Menu->text_space);
                }
            }
            else if(next_item->id < now_item->id)
            {
                Menu->_cursor.TargrtRow -= ((now_item->id - next_item->id)*Menu->text_space);
                if (Menu->_cursor.TargrtRow < 0)  // 防止光标溢出可视范围
                {
                    Menu->page_state = PAGE_MOVING;
                    Menu->_cursor.TargrtRow = 0;
                }
            }
        }
        break;
    case IMAGE:
        // 切换页面时进行初始化并设置页面状态为静态
        if(next_item->page.location != now_item->page.location)        
        {
            if(now_item->itemType == PARENTS)CursorAnimationParam_Init(Menu);
            else if (now_item->itemType == RETURN)CursorAnimationParam_Change(Menu);
            Menu->page_state = PAGE_STATIC;
        }
        else // 在同一页面内切换选项时的处理
        {
            // 根据选项的id差值计算目标行位置，并限制在可视范围内
            if (next_item->id > now_item->id)
            {
                Menu->_cursor.TargrtColumn += ((next_item->id - now_item->id)*Menu->image_space);
                if (Menu->_cursor.TargrtColumn > (Menu->image_space*(HOR_RES/Menu->image_space) - Menu->image_space))  // 防止光标溢出可视范围
                {
                    Menu->page_state = PAGE_MOVING;
                    Menu->_cursor.TargrtColumn = (Menu->image_space*(HOR_RES/Menu->image_space) - Menu->image_space);
                }
            }
            else if(next_item->id < now_item->id)
            {
                Menu->_cursor.TargrtColumn -= ((now_item->id - next_item->id)*Menu->image_space);
                if (Menu->_cursor.TargrtColumn < 0)  // 防止光标溢出可视范围
                {
                    Menu->page_state = PAGE_MOVING;
                    Menu->_cursor.TargrtColumn = 0;
                }
            }
        }
        break;
    default:
        break;
    }
}

static void Draw_TextPage(xpMenu Menu, xpPage Page, xpItem now_item, xpItem next_item)
{
    xpItem temp_item = Page->item.head; // 从页面的头部开始遍历

    char Data[10] = {0}; // 用于临时存储数据字符串
    if(Menu->now_item->page.location != next_item->page.location)
    {
        for (uint16_t i = 0; i <= Page->length; i++)
        {
            temp_item->Animation_y = 0;
            temp_item = temp_item->nextItem;
        }
    }
    for (uint16_t i = 0; i <= Page->length; i++)
    {
        temp_item->Animation_y = PID_Animation(temp_item->y, temp_item->Animation_y, &Menu->_animation.TextPage_Y);
        temp_item = temp_item->nextItem;
    }
    for (uint16_t i = 0; i <= Page->length; i++)
    {
        OLED_DrawStr(temp_item->x, temp_item->Animation_y, temp_item->itemName); // 绘制项目名称
        // 根据选项类型绘制开关或数据
        if(temp_item->itemType == SWITCH)
        {
            OLED_DrawFrame(temp_item->x + HOR_RES - 33, temp_item->Animation_y - Font_Size, Font_Size, Font_Size);
            // 绘制开关状态
            if(temp_item->switchState == true)OLED_DrawBox(temp_item->x + HOR_RES - 33 + 2, temp_item->Animation_y - Font_Size + 2, Font_Size - 4, Font_Size - 4);
        }
        if(temp_item->itemType == DATA)
        {
            // 格式化并绘制数据
            sprintf(Data, "%d", *temp_item->data.ptr);
            OLED_DrawStr(temp_item->x + HOR_RES - 33, temp_item->Animation_y, Data);
        }
        temp_item = temp_item->nextItem;
    }

    OLED_SetDrawColor(2); // 设置特定的颜色，通常用于高亮显示
    // 根据目标位置和当前位置，以及PID算法计算并更新当前选项的位置和宽度
    Menu->_cursor.NowRow = PID_Animation(Menu->_cursor.TargrtRow, Menu->_cursor.NowRow, &Menu->_animation.TextPage_Cursor);
    Menu->_cursor.NowWide = PID_Animation(Menu->_cursor.TargrtWide, Menu->_cursor.NowWide, &Menu->_animation.TextPage_Cursor);
    // 绘制选中项的高亮边框
    OLED_DrawRBox(Menu->headX + 1, Menu->_cursor.NowRow + 1, Menu->_cursor.NowWide, Font_Size, 4);
}

static void Draw_ImagePage(xpMenu Menu, xpPage Page, xpItem now_item, xpItem next_item)
{
    xpItem temp_item = Page->item.head; // 从页面的头部开始遍历

    Draw_ImagePage_OptionPlace(Menu, now_item, next_item);
    if(Menu->now_item->page.location != next_item->page.location)
    {
        for (uint16_t i = 0; i <= Page->length; i++)
        {
            temp_item->Animation_x = 0;
            temp_item = temp_item->nextItem;
        }
    }
    for (uint16_t i = 0; i <= Page->length; i++)
    {
        temp_item->Animation_x = PID_Animation(temp_item->x, temp_item->Animation_x, &Menu->_animation.ImagePage_X);
        temp_item = temp_item->nextItem;
    }
    for (uint16_t i = 0; i <= Page->length; i++)
    {
        OLED_DrawXBMP(temp_item->Animation_x, 12, IMG_WIDTH, IMG_HEIGHT, temp_item->image);
        temp_item = temp_item->nextItem;
    }
    OLED_DrawStr(0, VER_RES - 3, next_item->itemName);
    OLED_SetDrawColor(2); // 设置特定的颜色，通常用于高亮显示
    // 根据目标位置和当前位置，以及PID算法计算并更新当前选项的位置和宽度
    Menu->_cursor.NowColumn = PID_Animation(Menu->_cursor.TargrtColumn, Menu->_cursor.NowColumn, &Menu->_animation.ImagePage_Cursor);
    // 绘制选中项的高亮边框
    OLED_DrawFrame(Menu->_cursor.NowColumn, 47, 28, 2);
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
static void Draw_Menu(xpMenu Menu, xpPage Page, xpItem now_item,xpItem next_item)
{
    bool OptionState = false;

    OLED_ClearBuffer(); // 清除屏幕缓冲区

    // 初始化或更新动画参数及页面状态
    if(Menu->menu_state == MENU_RUN)
    {
        Calculate_Cursor(Menu, Page, now_item, next_item);
    }

    // 计算下一个将要选中项的名称宽度
    Menu->_cursor.TargrtWide = OLED_GetStrWidth(next_item->itemName) + 3;

    // 开始绘制菜单界面
    OLED_SetDrawColor(Menu->bgColor); // 设置背景颜色
    OLED_DrawBox(0, 0, 128, 64); // 绘制屏幕背景框
    OLED_SetDrawColor(Menu->bgColor^0x01); // 设置绘制颜色为高亮或低亮
    Calculate_Coordinate(Menu, Page, now_item, next_item); // 绘制前计算最终坐标

    if(Page->type == TEXT)
    {
        OptionState = Draw_TextPage_OptionPlace(Menu, now_item, next_item); // 绘制选项及其位置
        Draw_TextPage(Menu, Page, now_item, next_item);
    }
    else if(Page->type == IMAGE)
    {
        OptionState = Draw_ImagePage_OptionPlace(Menu, now_item, next_item); // 绘制选项及其位置
        Draw_ImagePage(Menu, Page, now_item, next_item);
    }

    OLED_SendBuffer(); // 将缓冲区的内容发送到OLED屏幕显示

    // 更新菜单状态为绘制中
    Change_MenuState(Menu, MENU_DRAWING);
    // 如果动画参数达到目标值且选项绘制完成，则更新菜单状态为运行中
    if ((Menu->_cursor.NowRow == Menu->_cursor.TargrtRow) && (Menu->_cursor.NowWide == Menu->_cursor.TargrtWide) && (OptionState == true))
    {
        Change_MenuState(Menu, MENU_RUN);
    }
}

int Contrast = 255; 
/*
 * 菜单构建函数
 * 该函数不接受参数，也不返回任何值。
 * 功能：静态地构建一个菜单系统。
 */
static void Craete_MenuTree(xpMenu Menu)
{
    AddPage("[HomePage]", &Home_Page, IMAGE);
        AddItem("[HomePage]", LOOP_FUNCTION, NULL, NULL, &HomeHead_Item, &Home_Page, NULL, Draw_Home);
        AddItem(" +System", PARENTS, NULL, logo_allArray[8], &System_Item, &Home_Page, &System_Page, NULL);
            AddPage("[System]", &System_Page, TEXT);
                AddItem("[System]", RETURN, NULL, NULL, &SystemHead_Item, &System_Page, &Home_Page, NULL);
                AddItem(" -MPU6050", LOOP_FUNCTION, NULL, NULL, &MPU6050_Item, &System_Page, NULL, Show_MPU6050);
                AddItem(" +Cursor Program", PARENTS, NULL, NULL, &CursorAnimation_Item, &System_Page, &CursorAnimation_Page, NULL);
                    AddPage("[Cursor Program]", &CursorAnimation_Page, TEXT);
                        AddItem("[Cursor Program]", RETURN, NULL, NULL, &CursorAnimationHead_Item, &CursorAnimation_Page, &System_Page, NULL);
                        AddItem(" -Kp", DATA, &Menu->_animation.TextPage_Cursor.kp, NULL, &CursorKp_Item, &CursorAnimation_Page, NULL, Setting_AnimationParam);
                        AddItem(" -Ki", DATA, &Menu->_animation.TextPage_Cursor.ki, NULL, &CursorKi_Item, &CursorAnimation_Page, NULL, Setting_AnimationParam);
                        AddItem(" -Kd", DATA, &Menu->_animation.TextPage_Cursor.kd, NULL, &CursorKd_Item, &CursorAnimation_Page, NULL, Setting_AnimationParam);
                AddItem(" -Mode", SWITCH, NULL, NULL, &Mode_Item, &System_Page, NULL, White_Dark_Day);
                AddItem(" -Contrast", DATA, &Contrast, NULL, &Contrast_Item, &System_Page, NULL, Setting_Contrast);
                AddItem(" +Setting Image", PARENTS, NULL, NULL, &SettingImagePage_Item, &System_Page, &SettingImagePage_Page, NULL);
                    AddPage("[Setting Image]", &SettingImagePage_Page, TEXT);
                        AddItem("[Setting Image]", RETURN, NULL, NULL, &SettingImagePageHead_Item, &SettingImagePage_Page, &System_Page, NULL);
                        AddItem(" -Image Space", DATA, &Menu->image_space, NULL, &ImageSpace_Item, &SettingImagePage_Page, NULL, Setting_Space);
                AddItem(" -Power", SWITCH, NULL, NULL, &Power_Item, &System_Page, NULL, PowerSave);
        AddItem(" +Games", PARENTS, NULL, logo_allArray[4], &Games_Item, &Home_Page, &Games_Page, NULL);
            AddPage("[Games]", &Games_Page, IMAGE);
                AddItem("[Games]", RETURN, NULL, NULL, &GamesHead_Item, &Games_Page, &Home_Page, NULL);
                AddItem(" -DinoGame", LOOP_FUNCTION, NULL, logo_allArray[3], &Dino_Item, &Games_Page, NULL, DinoGame_Run);
                AddItem(" -AirPlane", LOOP_FUNCTION, NULL, logo_allArray[0], &AirPlane_Item, &Games_Page, NULL, AirPlane_Run);
        AddItem(" +Setting Text", PARENTS, NULL, logo_allArray[1], &SettingTextPage_Item, &Home_Page, &SettingTextPage_Page, NULL);
            AddPage("[Setting Text]", &SettingTextPage_Page, IMAGE);
                AddItem("[Setting Text]", RETURN, NULL, NULL, &SettingTextPageHead_Item, &SettingTextPage_Page, &Home_Page, NULL);
                AddItem(" -Text Space", DATA, &Menu->text_space, logo_allArray[9], &TextSpace_Item, &SettingTextPage_Page, NULL, Setting_Space);                
        AddItem(" -Image", LOOP_FUNCTION, NULL, logo_allArray[6], &Image_Item, &Home_Page, NULL, Show_Log);
        AddItem(" -Github", LOOP_FUNCTION, NULL, logo_allArray[5], &Github_Item, &Home_Page, NULL, Show_GitHub);
        AddItem(" -Bilibili", LOOP_FUNCTION, NULL, logo_allArray[7], &Bilibili_Item, &Home_Page, NULL, Show_Bilibili);
        AddItem(" -ReadME", LOOP_FUNCTION, NULL, logo_allArray[9], &ReadME_Item, &Home_Page, NULL, Show_Bilibili);
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
static void Process_Menu_Run(xpMenu Menu, Menu_Direction Dir)
{
    uint8_t disapper = 0;
    switch (Menu->menu_state) // 根据当前菜单状态进行不同的操作
    {
        case MENU_RUN:
            switch (Dir) // 根据操作方向更新菜单项
            {
                case MENU_UP:
                    // 向上移动菜单项，确保当前项和上一项非空
                    if (Menu->now_item != NULL && Menu->now_item->lastItem != NULL) 
                    {
                        Draw_Menu(Menu, Menu->now_item->page.location, Menu->now_item, Menu->now_item->lastItem);
                        Menu->old_item = Menu->now_item;
                        Menu->now_item = Menu->now_item->lastItem;
                    }
                    break;
                case MENU_DOWN:
                    // 向下移动菜单项，确保当前项和下一项非空
                    if (Menu->now_item != NULL && Menu->now_item->nextItem != NULL) 
                    {
                        Draw_Menu(Menu, Menu->now_item->page.location, Menu->now_item, Menu->now_item->nextItem);
                        Menu->old_item = Menu->now_item;
                        Menu->now_item = Menu->now_item->nextItem;
                    }
                    break;
                case MENU_ENTER:
                    // 当前项非空且为父项时，进入下一级菜单
                    if(Menu->now_item != NULL && Menu->now_item->itemType == PARENTS)
                    {
                        // 避免重复状态改变
                        if(Menu->menu_state != MENU_ENTER) Change_MenuState(Menu, MENU_RUN);

                        for (size_t i = 0; i < 8; i++) // 执行UI淡出操作
                        {
                            disapper = ui_disapper(Menu ,disapper);
                        }
                        
                        // 如果存在跳转页面且不为空，则绘制跳转页面并更新当前项
                        if(Menu->now_item->page.jumpPage != NULL && Menu->now_item->page.jumpPage->item.head != NULL) 
                        {
                            Draw_Menu(Menu, Menu->now_item->page.jumpPage, Menu->now_item, Menu->now_item->page.jumpPage->item.head);
                            Menu->now_item->page.location->lastJumpItem = Menu->now_item;
                            Menu->old_item = Menu->now_item;
                            Menu->now_item = Menu->now_item->page.jumpPage->item.head;
                        }
                    }
                    else if (Menu->now_item != NULL && Menu->now_item->itemType == RETURN)
                    {
                        // 避免重复状态改变
                        if(Menu->menu_state != MENU_ENTER) Change_MenuState(Menu, MENU_RUN);

                        for (size_t i = 0; i < 8; i++) // 执行UI淡出操作
                        {
                            disapper = ui_disapper(Menu ,disapper);
                        }
                        
                        // 如果存在跳转页面，则绘制跳转页面并更新当前项
                        if(Menu->now_item->page.jumpPage != NULL) 
                        {
                            Draw_Menu(Menu, Menu->now_item->page.jumpPage->lastJumpItem->page.location, Menu->now_item, Menu->now_item->page.jumpPage->lastJumpItem);
                            Menu->old_item = Menu->now_item;
                            Menu->now_item = Menu->now_item->page.jumpPage->lastJumpItem;
                        }
                    }
                    else // 如果当前项为空或非父项，则进入应用绘制状态
                    {
                        ui_disapper(Menu, 1);
                        Change_MenuState(Menu, APP_DRAWING);
                    }
                    break;
                default:
                    Draw_Menu(Menu, Menu->now_item->page.location, Menu->now_item, Menu->now_item);
                    break;
            }
            break;
        case APP_QUIT:
            // 退出应用时的初始化操作，并返回菜单运行状态
            DialogScale_AnimationParam_Init(Menu);
            Change_MenuState(Menu, MENU_RUN);
            for (size_t i = 0; i < 8; i++)
            {
                disapper = ui_disapper(Menu, disapper); // 执行UI淡出操作
            }
            if(Menu->now_item != NULL) // 确保now_item非空
            {
                Draw_Menu(Menu, Menu->now_item->page.location, Menu->now_item, Menu->now_item);
            }
            break;
        case MENU_DRAWING:
            // 如果当前正在绘制菜单，则根据方向进行状态更新和菜单重绘
            if(Menu->now_item != NULL && Menu->old_item != NULL)
            {
                Draw_Menu(Menu, Menu->now_item->page.location, Menu->old_item, Menu->now_item);
                if (Dir != MENU_NONE)
                {
                    Change_MenuState(Menu, MENU_RUN);
                    Process_Menu_Run(Menu, Dir);
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
static void Process_App_Run(xpMenu Menu, xpItem item, Menu_Direction State)
{
    if (item == NULL) return; // 检查空指针，避免未定义行为

    item->state = State; // 设置项目的状态为当前菜单状态

    switch (item->itemType) // 根据项目类型执行不同的操作
    {
    case DATA:
    case LOOP_FUNCTION:
        if (item->itemFunction != NULL)(item->itemFunction)(Menu); // 执行项目的函数
        if(item->state == MENU_ENTER)Change_MenuState(Menu, APP_QUIT); // 如果项目状态为进入菜单，则改变菜单状态为函数退出
        break;
    case SWITCH:
        item->switchState = ! item->switchState; // 切换开关状态
    case ONCE_FUNCTION:
        if (item->itemFunction != NULL)(item->itemFunction)(Menu); // 如果项目有函数，则执行该函数
        Change_MenuState(Menu, APP_QUIT); // 改变菜单状态为函数退出
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
void Menu_Loop(xpMenu Menu)
{
    Menu_Direction Dir = BtnScan(); // 扫描按钮方向，确定菜单操作方向
    if (Menu->menu_state == MENU_INIT && Dir != MENU_NONE)
    {
        // 初始化状态下，非空检查now_item并绘制当前菜单项
        if (Menu->now_item != NULL)
        {
            Draw_Menu(Menu, Menu->now_item->page.location, Menu->now_item, Menu->now_item);
        }
        else
        {
            printf("Menu->now_item is NULL\n"); // 打印错误信息，now_item为空
        }
    }
    else
    {
        switch (Menu->menu_state)
        {
            case MENU_INIT:break;
            case APP_RUN:
            case APP_DRAWING:
                // 在这两个状态下，处理应用程序运行逻辑
                Process_App_Run(Menu, Menu->now_item, Dir);
                if (Menu->menu_state == APP_DRAWING)
                {
                    break; // 在绘制状态下，终止进一步流程执行
                }
            // 故意不写 break，以允许流程进入下方的共同逻辑处理（如果存在）

            case MENU_RUN:
            case APP_QUIT:
            case MENU_DRAWING:
                // 处理菜单运行、应用退出和菜单绘制状态
                Process_Menu_Run(Menu, Dir);
                break;

            default:
                break;
        }
    }
}

static void AnimationParam_Init(Animation *Ani)
{
    Ani->OptionPlace.kp = 350;
    Ani->OptionPlace.ki = 0;
    Ani->OptionPlace.kd = 30;

    Ani->Dialog_Scale.kp = 400;
    Ani->Dialog_Scale.ki = 0;
    Ani->Dialog_Scale.kd = 0;

    Ani->TextPage_Cursor.kp = 500;
    Ani->TextPage_Cursor.ki = 100;
    Ani->TextPage_Cursor.kd = 170;

    Ani->ImagePage_X.kp = 250;
    Ani->ImagePage_X.ki = 0;
    Ani->ImagePage_X.kd = 30;

    Ani->ImagePage_Cursor.kp = 450;
    Ani->ImagePage_Cursor.ki = 100;
    Ani->ImagePage_Cursor.kd = 100;

    Ani->TextPage_Y.kp = 350;
    Ani->TextPage_Y.ki = 0;
    Ani->TextPage_Y.kd = 100;
}

static void Create_Menu(xpMenu Menu, xpItem item)
{
    Craete_MenuTree(Menu);
    Menu->text_space = TEXT_SPACE;
    Menu->image_space = IMG_SPACE;
    Menu->headX = Init_x;
    Menu->headY = Init_y;
    Menu->now_item = item;
    Menu->old_item = item;
    Menu->menu_state = MENU_INIT;
    Menu->page_state = PAGE_STATIC;
    Menu->bgColor = 0;
    AnimationParam_Init(&Menu->_animation);
}

void Menu_Init(xpMenu Menu)
{
    Disp_Init();
    Create_Menu(Menu, &HomeHead_Item);
    Draw_Home(NULL);
    printf("MultMenu demo\n");
}
