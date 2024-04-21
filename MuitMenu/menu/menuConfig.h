#ifndef _MENUCONFIG_H_
#define _MENUCONFIG_H_

#include "stm32f10x.h"
#include "u8g2.h"
#include "stdbool.h"

// 屏幕分辨率定义
#define HOR_RES     128 // 水平分辨率
#define VER_RES     64 // 垂直分辨率

// 菜单字体定义
#define MENU_FONT   u8g2_font_profont12_mf

// 字体尺寸定义
#define Font_Size   12 // 字体高度

// 起始Y坐标，基于字体大小设定的第一行位置
#define FirstLine   Font_Size

// 起始X坐标
#define FirstPos    0

// 最大可见项目数，除去标题栏
#define MaxVisible_Number  4 // 可见菜单项数量

// 最大Y坐标，根据字体大小和最大可见项目数计算得出
#define LINE_MAX    Font_Size*MaxVisible_Number

// 最小Y坐标，即第一行的位置
#define LINE_MIN    Font_Size

/**
 * Pid_Error 结构体定义
 * 用于存储PID控制器的误差及调整参数
 */
typedef struct PidError
{
    int kp;         // 比例增益
    int ki;         // 积分增益
    int kd;         // 微分增益
    float error;    // 当前误差
    float sum_srror; // 积累的误差
    float last_error; // 上一次的误差
}Pid_Error;

// 菜单状态枚举: 定义了菜单及应用程序的不同运行状态
typedef enum MenuState
{
    MENU_INIT,       // 菜单初始化状态
    MENU_DRAWING,    // 菜单绘制状态
    MENU_RUN,        // 菜单运行状态
    APP_RUN,         // 应用程序运行状态
    APP_DRAWING,     // 应用程序绘制状态
    APP_QUIT         // 应用程序退出状态
} Menu_State;

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
    // 开关切换项: 代表菜单中的开关选项，可用于启用或禁用某个功能或设置。
    SWITCH,
    // 数据项: 代表菜单中用于显示或设置数据的项，可以是数值、文本等各种形式的数据。
    DATA
} Item_Type;

//菜单页面类型枚举
typedef enum
{
    FIGURE,
    TEXT
}Page_Type;

// 动画参数
typedef struct AnimationParam
{
    // 对话框总时间（单位：毫秒）
    uint8_t Dialog_AllTime;
    // 对话框缩放时间（单位：毫秒）
    uint8_t DialogScale_time;
    // 对话框初始宽度（单位：像素）
    uint8_t DialogScale_InitWide;
    // 对话框初始高度（单位：像素）
    uint8_t DialogScale_InitHigh;
    // 当前项宽度（单位：像素）
    int Item_NowWide;
    // 当前项高度（单位：行数）
    int Item_NowLine;
    // 目标项高度（单位：行数）
    int Item_TargrtLine;
    // 目标项宽度（单位：像素）
    int Item_TargrtWide;
    // 选项条状态
    bool OptionState;
} Animation_Param;

typedef struct Page *xpPage;
typedef struct Item *xpItem;
typedef struct Menu *xpMenu;
typedef void (*ItemFunction)(xpItem);

/**
 * 定义一个名为Page的结构体类型
 * 用于表示页面信息
 */
typedef struct Page {
    const char *pageName;      // 页面名称，使用指针存储
    uint8_t length;            // 页面长度
    uint8_t id;                // 页面ID
    // 使用内联函数封装成员变量，以提供更高的数据封装性和保护性
    struct {
        xpItem parent;         // 父级项
        xpItem head;           // 头部项
        xpItem tail;           // 尾部项
    } item;
    Page_Type pageType;        // 页面类型
    xpMenu menu;               // 页面关联的菜单
} xPage;

// 定义一个名为Item的结构体类型
typedef struct Item {
    const char *itemName; // 结构体成员变量：项目名称，使用const char*类型
    Item_Type itemType; // 结构体成员变量：项目类型，自定义的Item_Type类型
    bool switchState; // 结构体成员变量：开关状态，使用bool类型
    // 封装data指针，以确保数据的完整性
    struct {
        int *ptr; // 指向整型数据的指针
        size_t size; // 指针指向数据的大小，使用size_t类型
    } data;
    uint8_t id; // 结构体成员变量：项目ID，使用uint8_t类型
    // 使用内联函数封装page结构体，以保护成员变量
    struct {
        xpPage location; // 页面位置
        xpPage jumpPage; // 跳转页面
    } page;
    xpItem lastItem, nextItem; // 上一个项目和下一个项目的指针
    ItemFunction itemFunction; // 函数指针，指向一个接受xpItem类型参数的函数
    Menu_Direction state; // 菜单方向状态
    uint8_t *logo; // 结构体成员变量：项目关联的图片指针
} xItem;

/**
 * xMenu结构体定义
 * 用于表示一个菜单的相关信息
 */
typedef struct Menu {
    size_t page_length;       // 菜单页面item的数量
    xpItem now_item;          // 当前选中的item
    xpItem old_item;          // 上一个选中的item
    Menu_State menu_state;    // 菜单的状态
    Page_State page_state;    // 页面的状态
    uint8_t BgColor;          // 菜单的背景颜色
} xMenu;

#endif
