#include "menu.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "dispDirver.h"
#include "application.h"
#include "image.h"
#include "parameter.h"
#include "text.h"
#include "wave.h"
#include "bsp_usart.h"

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
 * 添加项目到页面
 * 
 * 该函数用于向指定的页面添加一个新的项目。项目包括名称、类型、元素、图像等信息，
 * 并且可以设置项目的功能回调。添加的项目将按照链表的形式管理，同时维护页面上项目的数量。
 *
 * @param Name 项目名称，不可为NULL。
 * @param Type 项目类型，如数据、开关、文本等。
 * @param Element 项目关联的元素，根据项目类型不同，包含不同的数据。
 * @param Image 项目显示的图像，如果为NULL，则使用预设图像。
 * @param item 待添加的项目结构体。
 * @param LocalPage 项目所在的页面结构体。
 * @param nextpage 项目跳转的下一个页面结构体，如果无跳转，则为NULL。
 * @param function 项目对应的函数指针，用于处理项目特定的功能。
 */
void AddItem(const char *Name, Item_Type Type, const uint8_t *Image, xpItem item, xpPage LocalPage, xpPage nextpage, ItemFunction function)
{
    // 检查输入参数的有效性
    // 参数检验
    if (!Name || !item || !LocalPage) {
        printf("Invalid parameter(s)\n");
        return; // 早期返回以避免进一步错误
    }
    item->x = Init_x;
    item->y = Init_y;
    // 初始化项目的基本信息
    item->itemName = Name; // 设置项目名称
    item->itemType = Type; // 设置项目类型
    item->logo = Image;
    // 如果页面类型为图像且没有指定图像，则使用预设图像
    if(LocalPage->type == IMAGE && Image == NULL)
    {
        item->logo = Presets_Logo;
        printf("%s : Image is null!\n", item->itemName);
    }
    item->page.location = LocalPage; // 设置项目所在页面
    item->itemFunction= function; // 设置项目关联的函数

    // 初始化项目链接信息
    /* 初始化项目下一个项为NULL */
    item->nextItem = NULL;

    // 设置跳转页面的父项目为当前项目
    /* 设置跳转页面的父级项目 */
    if (nextpage != NULL)
        nextpage->item.parent = item;
    else // 如果没有下一个页面，则设置为当前页面
        nextpage = LocalPage;
    item->page.jumpPage = nextpage; // 设置项目跳转页面

    // 如果页面为空，则初始化项目链表
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
        // 将新项目连接到链表尾部
        // 连接新项目到链表尾部
        item->lastItem = LocalPage->item.tail;                // 新项目上一个项指向当前尾项目
        LocalPage->item.tail->nextItem = item;                // 尾项目下一个项指向新项目
        LocalPage->item.tail = LocalPage->item.tail->nextItem; // 更新尾项目为新项目
        LocalPage->length++; // 项目计数加一
    }
    // 设置项目ID为当前页面项目数量
    item->id = LocalPage->length; // 设置项目ID为当前页面项目计数
    // 关联页面的头尾项目互相指向，用于快速遍历
    // 关联页面的头尾项目互相指向，用于快速遍历
    LocalPage->item.tail->nextItem = LocalPage->item.head;  
    LocalPage->item.head->lastItem = LocalPage->item.tail;
}

/**
 * @brief 向页面中添加一个新页面。
 * 
 * @param name 新页面的名称。
 * @param page 指向新页面结构体的指针。
 * @param Type 新页面的类型。
 */
void AddPage(const char *name, xpPage page, PageType Type)
{
    // 初始化新页面的基本信息
    page->pageName = name;
    page->item.head = NULL;
    page->item.tail = NULL;
    page->type = Type;
    page->page_state = PAGE_STATIC;
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
    
    // 计算误差
    Obj->error = Targrt - x;
    // 积分环节，累加误差
    Obj->sum_srror += Obj->error;
    // 微分环节，计算误差变化率
    float delta_error = Obj->error - Obj->last_error;

    // 计算控制量
    float velocity = Obj->kp * Obj->error + Obj->ki * Obj->sum_srror + Obj->kd * delta_error;
    
    // 更新状态
    x += velocity;
    Obj->last_error = Obj->error;

    return x;
}

static void Change_MenuState(xpMenu Menu, MenuState state)
{
    Menu->menu_state = state;
}

static void CursorAnimationParam_Init(xpMenu Menu, xpItem next_item)
{
    Menu->now_item->page.location->cursorLastColumn = Menu->_cursor.TargrtColumn;
    Menu->now_item->page.location->cursorLastRow = Menu->_cursor.TargrtRow;
    Menu->now_item->page.location->cursorLastWide = Menu->_cursor.TargrtWide;
    Menu->_cursor.TargrtColumn = next_item->x;
    Menu->_cursor.TargrtRow = next_item->y - Font_Hight;
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
    uint8_t bgC = Menu->bgColor;
    bgC = bgC^0x01;
    // 设置绘制边框的颜色，并绘制边框
    OLED_SetDrawColor(&bgC);
    OLED_DrawBox(x, y, w, h);
    // 设置绘制背景的颜色，并绘制背景盒
    bgC = bgC^0x01;
    OLED_SetDrawColor(&bgC);
    OLED_DrawBox(x+1, y+1, Menu->_dialogScale.now_wide-2, Menu->_dialogScale.now_high-2);
    bgC = bgC^0x01;
    // 设置边框高亮颜色（通常与背景色异或得到），用于强调边框
    OLED_SetDrawColor(&bgC);
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
 * @return uint8_t 如果动画时间达到预设的对话框显示时间，则返回true；否则返回false。
 */
uint8_t DialogScale_Show(xpMenu Menu, int16_t x,int16_t y,int16_t Targrt_w,int16_t Targrt_h)
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
    if ((Menu->_dialogScale.now_wide >= (Targrt_w - 2) && Menu->_dialogScale.now_wide <= (Targrt_w + 2)) && (Menu->_dialogScale.now_high >= Targrt_h - 2 && Menu->_dialogScale.now_high <= Targrt_h + 2))
    {
        Change_MenuState(Menu, APP_RUN);
        return true;
    }
    
    OLED_SendBuffer();
    return false;
}

uint8_t Notifications(xpMenu Menu, int16_t x,int16_t y,int16_t Targrt_w,int16_t Targrt_h)
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
        char value[20] = {0};
        sprintf(value, "%s OK!", Menu->now_item->itemName);
        OLED_DrawStr(x + 5, y + Targrt_h/2, value);
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
    uint8_t bgC = Menu->bgColor;
    // 设置绘制边框的颜色，先绘制外边框
    bgC = bgC^0x01;
    OLED_SetDrawColor(&bgC);
    OLED_DrawRFrame(x, y, w, h, r);
    bgC = bgC^0x01;
    // 设置绘制填充颜色，绘制内框，即填充部分
    OLED_SetDrawColor(&bgC);
    OLED_DrawRBox(x + 1, y + 1, w - 2, h - 2, r);
    bgC = bgC^0x01;
    // 设置边框颜色，用于强调边框，这里使用与背景色异或的方式
    OLED_SetDrawColor(&bgC);
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
 * @param data->min 滚动条代表的最小值
 * @param data->max 滚动条代表的最大值
 * @param data->step 滚动条的步进值
 * @param NowValue 滚动条当前的值
 */
void Draw_Scrollbar(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, float step, data_t *data)
{
    uint8_t color = 0;
    switch (data->Data_Type)
    {
    case DATA_INT:
        // 根据当前值计算滚动条可见部分的长度
        if (((*(int *)(data->ptr)) <= data->max) && ((*(int *)(data->ptr)) >= data->min))
        {
            (*(int *)(data->ptr)) += step;
            if ((*(int *)(data->ptr)) > data->max)
            {
                (*(int *)(data->ptr)) = data->max;
            }
            if ((*(int *)(data->ptr)) < data->min)
            {
                (*(int *)(data->ptr)) = data->min;
            }
            float value = (float)(abs((*(int *)(data->ptr)) - data->min) * ((w - 6)) / (float)((data->max - data->min)) + 6);
            // 绘制滚动条的填充部分
            color = 0;
            OLED_SetDrawColor(&color);
            OLED_DrawBox(x + (uint16_t)value, y, w-(uint16_t)value, h);
            color = 1;
            OLED_SetDrawColor(&color);
            OLED_DrawBox(x, y, (uint16_t)value, h);
        }
        break;
    case DATA_FLOAT:
        // 根据当前值计算滚动条可见部分的长度
        if (((*(float *)(data->ptr)) <= data->max) && ((*(float *)(data->ptr)) >= data->min))
        {
            (*(float *)(data->ptr)) += step;
            if ((*(float *)(data->ptr)) > data->max)
            {
                (*(float *)(data->ptr)) = data->max;
            }
            if ((*(float *)(data->ptr)) < data->min)
            {
                (*(float *)(data->ptr)) = data->min;
            }
            float value = (float)(abs((*(float *)(data->ptr)) - data->min) * ((w - 6)) / (float)((data->max - data->min)) + 6);
            // 绘制滚动条的填充部分
            color = 0;
            OLED_SetDrawColor(&color);
            OLED_DrawBox(x + (uint16_t)value, y, w-(uint16_t)value, h);
            color = 1;
            OLED_SetDrawColor(&color);
            OLED_DrawBox(x, y, (uint16_t)value, h);
        }
    default:
        break;
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
    uint8_t *p = OLED_GetBufferPtr(); // 获取屏幕缓冲区的指针

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
 * @return uint8_t 如果绘制过程完成（达到设定的时间），返回true；否则返回false。
 */
static uint8_t Draw_TextPage_OptionPlace(xpMenu Menu, xpItem now_item, xpItem next_item)
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

static uint8_t Draw_ImagePage_OptionPlace(xpMenu Menu, xpItem now_item, xpItem next_item)
{
    // 根据下一个项的id和位置长度，计算其理论绘制长度
    Menu->_optionPlace.ImagePage_OptionPlace.target_lenght = (HOR_RES / (float)(next_item->page.location->length)) * next_item->id;
    // 使用线性插值计算当前的绘制长度
    Menu->_optionPlace.ImagePage_OptionPlace.now_lenght = PID_Animation(Menu->_optionPlace.ImagePage_OptionPlace.target_lenght, Menu->_optionPlace.ImagePage_OptionPlace.now_lenght, &Menu->_animation.OptionPlace);
    // 绘制选项移动的指示线
    OLED_DrawLine(0, 2, HOR_RES, 2);
    // 根据计算出的长度，绘制当前选项的高亮框
    OLED_DrawBox(0, 0, Menu->_optionPlace.ImagePage_OptionPlace.now_lenght, 6);

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
        if (Page->page_state == PAGE_MOVING)
        {
            if ((next_item->id - now_item->id) > 0)Menu->headY -= ((next_item->id - now_item->id) > (Page->length - VER_RES/Menu->text_space)) ? ((Page->length + 1 - VER_RES/Menu->text_space) * Menu->text_space) : Menu->text_space;
            else Menu->headY += ((now_item->id - next_item->id) > (Page->length - VER_RES/Menu->text_space)) ? ((Page->length + 1 - VER_RES/Menu->text_space) * Menu->text_space) : Menu->text_space;
            Page->page_state = PAGE_STATIC; // 更新页面状态为静态显示
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
        if (Page->page_state == PAGE_MOVING)
        {
            if ((next_item->id - now_item->id) > 0)Menu->headX -= ((next_item->id - now_item->id) > (Page->length - HOR_RES/Menu->image_space)) ? ((Page->length + 1 - HOR_RES/Menu->image_space) * Menu->image_space) : Menu->image_space;
            else Menu->headX += ((now_item->id - next_item->id) > (Page->length - HOR_RES/Menu->image_space)) ? ((Page->length + 1 - HOR_RES/Menu->image_space) * Menu->image_space) : Menu->image_space;
            Page->page_state = PAGE_STATIC; // 更新页面状态为静态显示
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
        Calculate_Item_xy(Menu, Page);
    }
}

static void Calculate_PageState(xpMenu Menu, xpPage Page, xpItem now_item,xpItem next_item)
{
    if(next_item->page.location != now_item->page.location)        
    {
        Page->page_state = PAGE_STATIC;
        return ;
    }
    switch (Page->type)
    {
    case TEXT:
        if (next_item->y - Font_Hight > (Menu->text_space*(VER_RES/Menu->text_space) - Menu->text_space) || next_item->y - Font_Hight < 0)  // 防止光标溢出可视范围
        {
            Page->page_state = PAGE_MOVING;
        }
        break;
    case IMAGE:
        if (next_item->x > (Menu->image_space*(HOR_RES/Menu->image_space) - Menu->image_space) || next_item->x < 0)  // 防止光标溢出可视范围
        {
            Page->page_state = PAGE_MOVING;
        }
        break;
    default:
        break;
    }
}

static void Calculate_Cursor(xpMenu Menu, xpPage Page, xpItem now_item,xpItem next_item)
{
    if(next_item->page.location != now_item->page.location)        
    {
        if(now_item->itemType == PARENTS)CursorAnimationParam_Init(Menu, next_item);
        else if (now_item->itemType == RETURN)CursorAnimationParam_Change(Menu);
        return ;
    }
    switch (Page->type)
    {
    case TEXT:
        // 计算当前项名称的宽度
        Menu->_cursor.NowWide  = strlen(now_item->itemName)*Font_Width + 4;
        Menu->_cursor.TargrtRow = next_item->y - Font_Hight;
        if (Menu->_cursor.TargrtRow > (Menu->text_space*(VER_RES/Menu->text_space) - Menu->text_space) || Menu->_cursor.TargrtRow < 0)  // 防止光标溢出可视范围
        {
            if(abs(now_item->id - next_item->id) == 1)Menu->_cursor.TargrtRow = now_item->y - Font_Hight;
        }
        break;
    case IMAGE:
        // 根据选项的id差值计算目标行位置，并限制在可视范围内
        Menu->_cursor.TargrtColumn = next_item->x;
        if (Menu->_cursor.TargrtColumn > (Menu->image_space*(HOR_RES/Menu->image_space) - Menu->image_space) || Menu->_cursor.TargrtColumn < 0)  // 防止光标溢出可视范围
        {
            if(abs(now_item->id - next_item->id) == 1)Menu->_cursor.TargrtColumn = now_item->x;
        }
        break;
    default:
        break;
    }
}

static void Draw_TextPage(xpMenu Menu, xpPage Page, xpItem now_item, xpItem next_item)
{
    xpItem temp_item = Page->item.head; // 从页面的头部开始遍历

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
        if(temp_item->Animation_y >= -Font_Hight && temp_item->Animation_y <= VER_RES + Font_Hight) //超出屏幕范围则不绘制
        {
            OLED_DrawStr(temp_item->x, temp_item->Animation_y, temp_item->itemName); // 绘制项目名称
            if(temp_item->itemType == DATA)
            {
                char Data[20] = {0}; // 用于临时存储数据字符串
                // 格式化并绘制数据
                switch (temp_item->element->data->Data_Type)
                {
                case DATA_INT:
                    sprintf(Data, "%d", *(int *)(temp_item->element->data->ptr));
                    break;
                case DATA_FLOAT:
                    sprintf(Data, "%.2f", *(float *)(temp_item->element->data->ptr));
                    break;
                case DATA_SWITCH:
                OLED_DrawFrame(temp_item->x + HOR_RES - 33, temp_item->Animation_y - Font_Hight, Font_Hight, Font_Hight);
                    // 绘制开关状态
                    if(*(uint8_t *)temp_item->element->data->ptr == true)OLED_DrawBox(temp_item->x + HOR_RES - 33 + 2, temp_item->Animation_y - Font_Hight + 2, Font_Hight - 4, Font_Hight - 4);
                    break;
                default:
                    break;
                }
                OLED_DrawStr(temp_item->x + HOR_RES - 36, temp_item->Animation_y, Data);
            }
        }
        temp_item = temp_item->nextItem;
    }
    uint8_t color = 2;
    OLED_SetDrawColor(&color); // 设置特定的颜色，通常用于高亮显示
    // 根据目标位置和当前位置，以及PID算法计算并更新当前选项的位置和宽度
    Menu->_cursor.NowRow = PID_Animation(Menu->_cursor.TargrtRow, Menu->_cursor.NowRow, &Menu->_animation.TextPage_Cursor);
    Menu->_cursor.NowWide = PID_Animation(Menu->_cursor.TargrtWide, Menu->_cursor.NowWide, &Menu->_animation.TextPage_Cursor);
    // 绘制选中项的高亮边框
    OLED_DrawRBox(Menu->headX + 1, Menu->_cursor.NowRow + 1, Menu->_cursor.NowWide, Font_Hight, 4);
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
        if(temp_item->Animation_x >= -IMG_WIDTH && temp_item->Animation_x <= HOR_RES + IMG_WIDTH) //超出屏幕范围则不绘制
        OLED_DrawXBMP(temp_item->Animation_x, 12, IMG_WIDTH, IMG_HEIGHT, temp_item->logo);
        temp_item = temp_item->nextItem;
    }
    OLED_DrawStr(0, VER_RES - 2, next_item->itemName);
    uint8_t color = 2;
    OLED_SetDrawColor(&color); // 设置特定的颜色，通常用于高亮显示
    // 根据目标位置和当前位置，以及PID算法计算并更新当前选项的位置和宽度
    Menu->_cursor.NowColumn = PID_Animation(Menu->_cursor.TargrtColumn, Menu->_cursor.NowColumn, &Menu->_animation.ImagePage_Cursor);
    // 绘制选中项的高亮边框
    OLED_DrawFrame(Menu->_cursor.NowColumn, VER_RES - 17, IMG_WIDTH, 2);
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
    OLED_SetFont(MENU_FONT);
    OLED_ClearBuffer(); // 清除屏幕缓冲区

    // 计算页面状态
    if(Menu->menu_state == MENU_RUN)
    {
        Calculate_PageState(Menu, Page, now_item, next_item);
    }

    // 计算下一个将要选中项的名称宽度
    Menu->_cursor.TargrtWide = strlen(next_item->itemName)*Font_Width;
    uint8_t color = Menu->bgColor;
    // 开始绘制菜单界面
    OLED_SetDrawColor(&color); // 设置背景颜色
    OLED_DrawBox(0, 0, 128, 64); // 绘制屏幕背景框
    color = color^0x01;
    OLED_SetDrawColor(&color); // 设置绘制颜色为高亮或低亮
    Calculate_Coordinate(Menu, Page, now_item, next_item); // 绘制前计算最终坐标
    // 初始化或更新动画参数及页面状态
    if(Menu->menu_state == MENU_RUN)
    {
        Calculate_Cursor(Menu, Page, now_item, next_item);
    }

    if(Page->type == TEXT)
    {
        Draw_TextPage_OptionPlace(Menu, now_item, next_item); // 绘制选项及其位置
        Draw_TextPage(Menu, Page, now_item, next_item);
    }
    else if(Page->type == IMAGE)
    {
        Draw_ImagePage_OptionPlace(Menu, now_item, next_item); // 绘制选项及其位置
        Draw_ImagePage(Menu, Page, now_item, next_item);
    }

    OLED_SendBuffer(); // 将缓冲区的内容发送到OLED屏幕显示

    // 更新菜单状态为绘制中
    Change_MenuState(Menu, MENU_DRAWING);
}

void Create_element(xpItem item, element_t *element)
{
    if(item == NULL || element == NULL)return ;

    item->element = element;
}

/* 在此填入按键扫描程序
 * 功能：执行按键扫描，根据接收到的数据确定按键方向
 * 参数：无
 * 返回值：Menu_Direction 枚举类型，表示按键的方向，没有按键按下时返回MENU_NONE
 */
static Menu_Direction BtnScan(void)
{
    
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
                        if(Menu->dir != MENU_ENTER) Change_MenuState(Menu, MENU_RUN);

                        for (size_t i = 0; i < 8; i++) // 执行UI淡出操作
                        {
                            disapper = ui_disapper(Menu ,disapper);
                        }
                        
                        // 如果存在跳转页面且不为空，则绘制跳转页面并更新当前项
                        if(Menu->now_item->page.jumpPage != NULL && Menu->now_item->page.jumpPage->item.head != NULL) 
                        {
                            Draw_Menu(Menu, Menu->now_item->page.jumpPage, Menu->now_item, Menu->now_item->page.jumpPage->item.head);
                            Menu->now_item->page.location->item.lastJumpItem = Menu->now_item;
                            Menu->old_item = Menu->now_item;
                            Menu->now_item = Menu->now_item->page.jumpPage->item.head;
                        }
                    }
                    else if (Menu->now_item != NULL && Menu->now_item->itemType == RETURN)
                    {
                        // 避免重复状态改变
                        if(Menu->dir != MENU_ENTER) Change_MenuState(Menu, MENU_RUN);

                        for (size_t i = 0; i < 8; i++) // 执行UI淡出操作
                        {
                            disapper = ui_disapper(Menu ,disapper);
                        }
                        
                        // 如果存在跳转页面，则绘制跳转页面并更新当前项
                        if(Menu->now_item->page.jumpPage != NULL) 
                        {
                            Draw_Menu(Menu, Menu->now_item->page.jumpPage->item.lastJumpItem->page.location, Menu->now_item, Menu->now_item->page.jumpPage->item.lastJumpItem);
                            Menu->old_item = Menu->now_item;
                            Menu->now_item = Menu->now_item->page.jumpPage->item.lastJumpItem;
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
 * 对给定的项目(item)根据其类型(ItemType)和当前的菜单状态(Direction)执行相应的操作。
 * @param item 指向要处理的项目的指针。
 * @param Direction 当前菜单的方向或状态。
 */
static void Process_App_Run(xpMenu Menu, xpItem item, Menu_Direction Direction)
{
    if (item == NULL) return; // 检查空指针，避免未定义行为

    Menu->dir = Direction; // 设置项目的状态为当前菜单状态

    switch (item->itemType) // 根据项目类型执行不同的操作
    {
    case DATA:
        if(item->element->data->Operate_Type == READ_ONLY)
        {
            Change_MenuState(Menu, APP_QUIT); // 如果项目状态为进入菜单，则改变菜单状态为函数退出
            break;
        }
        else
        {
            if (item->itemFunction == NULL)ParameterSetting_Widget(Menu);
            else (item->itemFunction)(Menu); // 执行项目的函数

            if(item->element->data->Data_Type == DATA_SWITCH)
            {
                *(uint8_t *)item->element->data->ptr = ! *(uint8_t *)item->element->data->ptr; // 切换开关状态
                Change_MenuState(Menu, APP_QUIT); // 改变菜单状态为函数退出
            }
            else
            {
                if(Menu->dir == MENU_ENTER)
                {
                    if(Menu->now_item->element->data->function != NULL && Menu->now_item->element->data->Function_Type == EXIT_EXECUTE)Menu->now_item->element->data->function(Menu->now_item->element->data->ptr);
                    Change_MenuState(Menu, APP_QUIT); // 如果项目状态为进入菜单，则改变菜单状态为函数退出
                }
            }
        }
        break;
    case LOOP_FUNCTION:
        if (item->itemFunction != NULL)(item->itemFunction)(Menu); // 执行项目的函数
        if(Menu->dir == MENU_ENTER)Change_MenuState(Menu, APP_QUIT); // 如果项目状态为进入菜单，则改变菜单状态为函数退出
        break;
    case WAVE:
        if (item->itemFunction == NULL)Wave_Widget(Menu);
        else (item->itemFunction)(Menu); // 执行项目的函数
        if(Menu->dir == MENU_ENTER)Change_MenuState(Menu, APP_QUIT); // 如果项目状态为进入菜单，则改变菜单状态为函数退出
        break;
    case _TEXT_:
        if (item->itemFunction == NULL)Text_Widget(Menu);
        else (item->itemFunction)(Menu);
        if(Menu->dir == MENU_ENTER)Change_MenuState(Menu, APP_QUIT); // 如果项目状态为进入菜单，则改变菜单状态为函数退出
        break;
    case ONCE_FUNCTION:
        if (item->itemFunction != NULL && item->function_state == false)
        {
            (item->itemFunction)(Menu); // 如果项目有函数，则执行该函数
            item->function_state = true;
        }
        if (Notifications(Menu, 4, 12, HOR_RES - 8, VER_RES - 32))
        {
            OLED_SendBuffer();
        }
        if(Menu->dir != MENU_NONE)
        {
            item->function_state = false;
            Change_MenuState(Menu, APP_QUIT); // 改变菜单状态为函数退出
        }
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
        Menu->menu_state = MENU_RUN;
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
    Ani->OptionPlace.kp = 0.35f;
    Ani->OptionPlace.ki = 0.0f;
    Ani->OptionPlace.kd = 0.03f;

    Ani->Dialog_Scale.kp = 0.5f;
    Ani->Dialog_Scale.ki = 0.0f;
    Ani->Dialog_Scale.kd = 0.0f;

    Ani->TextPage_Cursor.kp = 0.5f;
    Ani->TextPage_Cursor.ki = 0.2f;
    Ani->TextPage_Cursor.kd = 0.17f;

    Ani->ImagePage_X.kp = 0.25f;
    Ani->ImagePage_X.ki = 0.0f;
    Ani->ImagePage_X.kd = 0.03f;

    Ani->ImagePage_Cursor.kp = 0.45f;
    Ani->ImagePage_Cursor.ki = 0.0f;
    Ani->ImagePage_Cursor.kd = 0.1f;

    Ani->TextPage_Y.kp = 0.35f;
    Ani->TextPage_Y.ki = 0.0f;
    Ani->TextPage_Y.kd = 0.1f;
}

void Create_Menu(xpMenu Menu, xpItem item)
{
    Create_MenuTree(Menu);
    Create_Parameter();
    Create_Text();
    Menu->text_space = TEXT_SPACE;
    Menu->image_space = IMG_SPACE;
    Menu->headX = Init_x;
    Menu->headY = Init_y;
    Menu->now_item = item;
    Menu->old_item = item;
    Menu->menu_state = MENU_INIT;
    Menu->dir = MENU_NONE;
    Menu->bgColor = 0;
    AnimationParam_Init(&Menu->_animation);
}
