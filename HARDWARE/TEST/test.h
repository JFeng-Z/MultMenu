/*
 * @Author: JFeng 2834294740@qq.com
 * @Date: 2023-06-22 22:18:02
 * @LastEditors: JFeng 2834294740@qq.com
 * @LastEditTime: 2023-06-26 00:47:46
 * @FilePath: \Projectd:\study\STM32F103_CMSIS\MY_GUI_RTOS\HARDWARE\TEST\test.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __TEST_H
#define __TEST_H

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#include "main.h"
#include "u8g2.h"
#include "stm32_u8g2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Tim_Tick.h"

void testDrawProcess(u8g2_t *u8g2);
void testShowFont(u8g2_t *u8g2);
void testDrawFrame(u8g2_t *u8g2);
void testDrawRBox(u8g2_t *u8g2);
void testDrawCircle(u8g2_t *u8g2);
void testDrawFilledEllipse(u8g2_t *u8g2);
void testDrawMulti(u8g2_t *u8g2);
void testDrawXBM(u8g2_t *u8g2);
void Draw_CJLU(u8g2_t *u8g2);
void OLED_DisplayScrollingText(u8g2_t *u8g2,uint8_t columns,const char* text, uint16_t delay_ms);
void u8g2DrawTest(u8g2_t *u8g2);

#endif