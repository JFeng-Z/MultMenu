#ifndef _MENUCONFIG_H_
#define _MENUCONFIG_H_

#include "stdint.h"
#include "u8g2.h"

// 屏幕分辨率定义
#define HOR_RES     128 // 水平分辨率
#define VER_RES     64 // 垂直分辨率

// 菜单字体定义
#define MENU_FONT   u8g2_font_profont12_mf

// 字体尺寸定义
#define Font_Hight   12 // 字体高度

// 字体宽度
#define Font_Width  6

// 文本间距
#define TEXT_SPACE  16

// 起始Y坐标，基于字体大小设定的第一行位置
#define Init_y   Font_Hight

// 起始X坐标
#define Init_x    0

// 图片大小
#define IMG_WIDTH   30
#define IMG_HEIGHT  30

// 图片间距
#define IMG_SPACE   39

#define true 1
#define false 0

typedef struct Page *xpPage;
typedef struct Item *xpItem;
typedef struct Menu *xpMenu;
typedef void (*ItemFunction)(xpMenu);
typedef void (*DataFunction)(void *);

typedef enum DataFunction_Type
{
    EXIT_EXECUTE,   // 退出执行
    STEP_EXECUTE    // 步进执行
} DataFunction_Type;

typedef enum data_type
{
    DATA_INT,      // 整型数据
    DATA_FLOAT,     // 浮点型数据
    DATA_SWITCH    // 开关类型数据
} data_type;

typedef enum operate_type
{
    READ_ONLY,     // 只读
    READ_WRITE     // 读写
} operate_type;

typedef struct data_t {
    const char *name;
    void *ptr; // 指向整型数据的指针
    DataFunction function;
    DataFunction_Type Function_Type;
    data_type Data_Type;
    operate_type Operate_Type;
    int max;
    int min;
    float step;
} data_t;

typedef struct text_t {
    const char *ptr;
    const uint8_t *font;
    int font_hight;
    int font_width;
} text_t;

typedef struct element_t {
    data_t *data;
    text_t *text;
} element_t;

/**
 * AnimationParam 结构体定义
 * 用于存储PID控制器的误差及调整参数
 */
typedef struct Animation_Param
{
    float kp;         // 比例增益
    float ki;         // 积分增益
    float kd;         // 微分增益
    float error;    // 当前误差
    float sum_srror; // 积累的误差
    float last_error; // 上一次的误差
}AnimationParam;

typedef struct Animation
{
    AnimationParam OptionPlace;
    AnimationParam Dialog_Scale;
    AnimationParam TextPage_Cursor;
    AnimationParam TextPage_Y;
    AnimationParam ImagePage_X;
    AnimationParam ImagePage_Cursor;
}Animation;

// 菜单状态枚举: 定义了菜单及应用程序的不同运行状态
typedef enum MenuState
{
    MENU_INIT,       // 菜单初始化状态
    MENU_DRAWING,    // 菜单绘制状态
    MENU_RUN,        // 菜单运行状态
    APP_RUN,         // 应用程序运行状态
    APP_DRAWING,     // 应用程序绘制状态
    APP_QUIT         // 应用程序退出状态
} MenuState;

// 页面状态枚举
// 用于表示页面的两种状态：移动和静止
typedef enum PageState
{
    PAGE_MOVING,    // 页面正在移动
    PAGE_STATIC     // 页面处于静止状态
}Page_State;

// 菜单运动方向枚举
// 这个枚举定义了菜单系统中可能的方向和操作，包括向上、向下、进入和退出等操作。
typedef enum MenuDirection
{
    MENU_NONE,    // 无操作
    MENU_UP,      // 向上
    MENU_DOWN,    // 向下
    MENU_ENTER   // 进入或选择
} Menu_Direction;

// 菜单项目类型枚举
typedef enum ItemType
{
    // 父菜单项: 代表菜单中的顶级项或父级项，通常不直接与用户交互，而是作为其他菜单项的容器。
    PARENTS = 0,
    // 循环功能项: 代表一种循环执行的功能，在菜单系统中用于循环播放、轮询等操作。
    LOOP_FUNCTION,
    // 一次性功能项: 代表仅执行一次的功能，执行后即被标记为完成，不再重复执行。
    ONCE_FUNCTION,
    // 数据项: 代表菜单中用于显示或设置数据的项，可以是数值、文本等各种形式的数据。
    DATA,
    // 文本类型: 代表菜单中的文本项，用于显示提示信息或提示用户操作。
    _TEXT_,
    // 波形类型: 代表菜单中的波形显示项，用于显示波形数据。
    WAVE,
    // 返回键: 代表菜单中的返回键，用于返回上一级菜单。
    RETURN
} Item_Type;

// 页面类型枚举
typedef enum PageType
{
    // 文本类型
    TEXT,
    // 图像类型
    IMAGE
} PageType;

// 光标运动参数
typedef struct CursorParam
{
    // 当前宽度
    int NowWide;
    // 当前行坐标
    int NowRow;
    // 当前列坐标
    int NowColumn;
    // 目标行坐标
    int TargrtRow;
    // 目标列坐标
    int TargrtColumn;
    // 目标宽度
    int TargrtWide;
} CursorParam;

// 对话框运动参数
typedef struct DialogScaleParam
{
    // 对话框初始宽度
    uint16_t now_wide;
    // 对话框初始高度
    uint16_t now_high;
} DialogScaleParam;

// 滚动条运动参数
typedef struct OptionPlaceParam
{
    // 滚动条当前长度
    uint16_t now_lenght;
    // 滚动条目标长度
    uint16_t target_lenght;
} OptionPlaceParam;

// 滚动条
typedef struct OptionPlace
{
    OptionPlaceParam TextPage_OptionPlace;
    OptionPlaceParam ImagePage_OptionPlace;
} OptionPlace;

/**
 * 定义一个名为Page的结构体类型
 * 用于表示页面信息
 */
typedef struct Page {
    const char *pageName;      // 页面名称，使用指针存储
    uint8_t length;            // 页面长度
    // 使用内联函数封装成员变量，以提供更高的数据封装性和保护性
    struct {
        xpItem parent;         // 父级项
        xpItem head;           // 头部项
        xpItem tail;           // 尾部项
    } item;
    PageType type;         // 页面类型
    int16_t jumpX;
    int16_t jumpY;
    xpItem lastJumpItem;
    int cursorLastColumn;
    int cursorLastRow;
    int cursorLastWide;
} xPage;

// 定义一个名为Item的结构体类型
typedef struct Item {
    const char *itemName; // 结构体成员变量：项目名称，使用const char*类型
    Item_Type itemType; // 结构体成员变量：项目类型，自定义的Item_Type类型
    element_t *element;
    int id; // 结构体成员变量：项目ID，使用uint8_t类型
    // 使用内联函数封装page结构体，以保护成员变量
    struct {
        xpPage location; // 页面位置
        xpPage jumpPage; // 跳转页面
    } page;
    xpItem lastItem, nextItem; // 上一个项目和下一个项目的指针
    ItemFunction itemFunction; // 函数指针，指向一个接受xpItem类型参数的函数
    Menu_Direction state; // 菜单方向状态
    int16_t x;
    int16_t y;
    int16_t Animation_x;
    int16_t Animation_y;
    const uint8_t *logo;
    uint8_t function_state;
} xItem;

/**
 * xMenu结构体定义
 * 用于表示一个菜单的相关信息
 */
typedef struct Menu {
    int text_space;        // 文本间距
    int image_space;      // 图片间距
    int headX;                // 整个菜单的起始x坐标
    int headY;                // 整个菜单的起始y坐标
    xpItem now_item;          // 当前选中的item
    xpItem old_item;          // 上一个选中的item
    xItem lastJumpItem;
    MenuState menu_state;    // 菜单的状态
    Page_State page_state;    // 页面的状态
    uint8_t bgColor;          // 菜单的背景颜色
    Animation _animation;
    CursorParam _cursor;
    DialogScaleParam _dialogScale;
    OptionPlace _optionPlace;
} xMenu;

#endif
