#include "dispDirver.h"
#include "u8g2.h"
#include "stm32_u8g2.h"
#include "menuConfig.h"
#include "bsp_usart.h"

u8g2_t u8g2;

/**
 * 初始化显示设备。
 * 该函数负责初始化OLED显示器，并设置默认字体。
 * 
 * @无参数
 * @无返回值
 */
void Disp_Init(void)
{
    // 初始化U8g2库，为OLED显示做准备
    u8g2Init(&u8g2);
}

/**
 * 清除OLED显示缓冲区
 * 
 * 该函数用于清空OLED显示器的缓冲区，为新的显示内容做准备。
 * 
 * 参数: 
 * 无
 * 
 * 返回值: 
 * 无
 */
void OLED_ClearBuffer(void)
{
    u8g2_ClearBuffer(&u8g2);  // 清除OLED显示缓冲区的具体实现，使用u8g2库提供的函数。
}

/**
 * 向OLED发送缓冲区数据
 * 该函数无参数。
 * 该函数无返回值。
 */
void OLED_SendBuffer(void)
{
    /* 将U8G2实例的缓冲区数据发送到OLED设备 */
    u8g2_SendBuffer(&u8g2);
}

void OLED_UpdateDisplayArea(uint8_t tx, uint8_t ty, uint8_t tw, uint8_t th)
{
    u8g2_UpdateDisplayArea(&u8g2, tx, ty, tw, th);
}

/**
 * 获取字符串在OLED显示设备上的宽度。
 * 
 * @param s 指向要测量的字符串的指针。
 * @return 返回字符串在OLED显示设备上的宽度，单位为像素。
 */
uint16_t OLED_GetStrWidth(const char *s)
{
    // 调用u8g2库的函数，获取字符串s的宽度
    return u8g2_GetStrWidth(&u8g2, s);
}

/**
 * 设置OLED的最大剪辑窗口
 * 该函数无参数。
 * 该函数无返回值。
 */
void OLED_SetMaxClipWindow(void)
{
    u8g2_SetMaxClipWindow(&u8g2);  // 调用u8g2库的函数设置最大的剪辑窗口
}

/**
 * 设置OLED显示器的字体。
 * 
 * @param font 指向要使用的字体的指针。该字体必须是提前定义并可用的。
 * 
 * 说明：这个函数通过调用u8g2的设置函数，来更改当前OLED显示的字体。
 */
void OLED_SetFont(const uint8_t  *font)
{
    u8g2_SetFont(&u8g2, font); // 设置U8g2实例的字体
}

/**
 * 在OLED屏幕上绘制一个像素点
 * 
 * @param x 像素点的x坐标
 * @param y 像素点的y坐标
 * 
 * 该函数调用u8g2的DrawPixel函数，来在指定位置绘制一个像素点。
 */
void OLED_DrawPixel(uint16_t x, uint16_t y)
{
    u8g2_DrawPixel(&u8g2, x, y); // 调用u8g2库的绘制像素点函数
}

/**
 * 在OLED显示器上绘制一条线。
 * 
 * @param x1 起始点的X坐标。
 * @param y1 起始点的Y坐标。
 * @param x2 终点的X坐标。
 * @param y2 终点的Y坐标。
 * 
 * 该函数调用u8g2的DrawLine函数，使用给定的起始点和终点坐标在OLED显示器上绘制一条线。
 */
void OLED_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    u8g2_DrawLine(&u8g2, x1, y1, x2, y2);
}

/**
 * 在OLED屏幕上绘制字符串
 * 
 * @param x 字符串起始绘制的x坐标
 * @param y 字符串起始绘制的y坐标
 * @param str 要绘制的字符串
 * @return 绘制后的光标位置（一般为字符串的结束位置），具体返回值意义可能依赖于u8g2库的实现
 */
uint16_t OLED_DrawStr(uint16_t x, uint16_t y, const char *str)
{
    // 调用u8g2库的DrawStr函数，在指定位置绘制字符串
    return u8g2_DrawStr(&u8g2, x, y, str);
}

/**
 * 设置OLED显示设备的绘制颜色。
 * 
 * @param color 指定的绘制颜色，使用uint8_t类型表示。颜色的具体编码取决于OLED驱动库的实现。
 * 
 * 该函数通过调用u8g2库的u8g2_SetDrawColor函数，来设置当前绘制的颜色。绘制颜色会影响后续所有绘制操作。
 */
void OLED_SetDrawColor(uint8_t color)
{
    u8g2_SetDrawColor(&u8g2, color); // 调用u8g2库的函数设置绘制颜色
}

/**
 * 在OLED屏幕上绘制一个框架。
 * 
 * @param x 框架的起始x坐标。
 * @param y 框架的起始y坐标。
 * @param w 框架的宽度。
 * @param h 框架的高度。
 * 
 * 该函数调用u8g2的绘制框架函数，用于在指定位置和尺寸上绘制一个框架。
 */
void OLED_DrawFrame(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    u8g2_DrawFrame(&u8g2, x, y, w, h);
}

/**
 * 在OLED屏幕上绘制一个矩形边框。
 * 
 * @param x 矩形左上角x坐标。
 * @param y 矩形左上角y坐标。
 * @param w 矩形的宽度。
 * @param h 矩形的高度。
 * @param r 矩形圆角的半径。
 * 
 * 该函数调用u8g2的绘制函数，用于在指定位置以指定尺寸和圆角半径绘制一个矩形边框。
 */
void OLED_DrawRFrame(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r)
{
    u8g2_DrawRFrame(&u8g2, x, y, w, h, r);
}

/**
 * 在OLED显示器上绘制一个矩形框。
 * 
 * @param x 矩形框左上角的x坐标。
 * @param y 矩形框左上角的y坐标。
 * @param w 矩形框的宽度。
 * @param h 矩形框的高度。
 * 
 * 该函数调用u8g2的绘制矩形框函数，传入指定的坐标和尺寸，在OLED屏幕上绘制矩形。
 */
void OLED_DrawBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    u8g2_DrawBox(&u8g2, x, y, w, h); // 调用u8g2库的绘制矩形函数
}

/**
 * 在OLED显示器上绘制一个带圆角的矩形。
 * 
 * @param x 矩形左上角的x坐标。
 * @param y 矩形左上角的y坐标。
 * @param w 矩形的宽度。
 * @param h 矩形的高度。
 * @param r 矩形圆角的半径。
 * 
 * 该函数调用u8g2的绘制函数，以指定的坐标和尺寸，在OLED显示器上绘制一个带圆角的矩形。
 */
void OLED_DrawRBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r)
{
    u8g2_DrawRBox(&u8g2, x, y, w, h, r);
}

/**
 * 在OLED显示器上绘制一个XBM图像。
 * 
 * @param x 图像在显示器上的起始x坐标。
 * @param y 图像在显示器上的起始y坐标。
 * @param w 图像的宽度。
 * @param h 图像的高度。
 * @param bitmap 指向包含XBM图像数据的缓冲区的指针。
 * 
 * 此函数使用u8g2库的u8g2_DrawXBMP函数来绘制图像。图像数据必须是XBM格式的。
 */
void OLED_DrawXBMP(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *bitmap)
{
    u8g2_DrawXBMP(&u8g2, x, y, w, h, bitmap);
}

/**
 * 设置OLED显示器的对比度。
 * 
 * @param value 对比度值，有效范围通常为0到255，具体取决于硬件的限制。
 * 
 * 该函数通过调用u8g2库中的u8g2_SetContrast函数，来设置OLED显示器的对比度。
 * 使用者需要根据具体的OLED显示器和u8g2配置来选择合适的对比度值。
 */
void OLED_SetContrast(uint8_t value)
{
    u8g2_SetContrast(&u8g2, value); // 调用u8g2库函数设置对比度
}

/**
 * 设置OLED电源节省模式
 * 
 * 本函数用于根据输入参数启用或禁用OLED的电源节省模式。当is_enable为1时，启用电源节省模式；
 * 当is_enable为0时，禁用电源节省模式。
 * 
 * @param is_enable 一个无符号字符(uint8_t)，用来控制是否启用电源节省模式。
 *                  当其值为1时，启用电源节省模式；当其值为0时，禁用电源节省模式。
 */
void OLED_SetPowerSave(uint8_t is_enable)
{
    u8g2_SetPowerSave(&u8g2, is_enable); // 调用u8g2库的函数，设置OLED的电源节省模式状态
}

/**
 * 获取OLED缓冲区的 tile 高度。
 * 
 * 该函数不接受参数。
 * 
 * @return 返回OLED缓冲区的tile高度。返回值为uint8_t类型，即无符号8位整数。
 */
uint8_t OLED_GetBufferTileHeight(void)
{
    return u8g2_GetBufferTileHeight(&u8g2); // 调用u8g2库函数，获取当前OLED缓冲区的tile高度
}

/**
 * 获取OLED缓冲区的单个瓦片宽度。
 * 
 * 该函数查询U8G2图形库当前配置的OLED缓冲区的单个瓦片宽度。瓦片宽度是指OLED屏幕
 * 在显示更新时，一次可以处理的像素宽度。这个值通常取决于OLED屏幕的物理分辨率
 * 和U8G2库的内部处理方式。
 * 
 * @return 返回OLED缓冲区单个瓦片的宽度，单位为字节。
 */
uint8_t OLED_GetBufferTileWidth(void)
{
    return u8g2_GetBufferTileWidth(&u8g2);
}

/**
 * 获取OLED显示缓冲区的指针
 * 
 * 该函数用于获取当前OLED显示设备的显示缓冲区的指针。该缓冲区是一个uint8_t类型的数组，
 * 用于存储即将显示在OLED屏幕上的图像数据。
 * 
 * @return 返回类型为uint8_t*，指向OLED显示缓冲区的起始位置。
 */
uint8_t *OLED_GetBufferPtr(void)
{
    // 调用u8g2库的函数获取显示缓冲区指针
    return u8g2_GetBufferPtr(&u8g2);
}
