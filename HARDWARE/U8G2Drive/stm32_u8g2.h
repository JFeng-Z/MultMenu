/*
 * @Author: JFeng 2834294740@qq.com
 * @Date: 2023-06-30 12:08:24
 * @LastEditors: JFeng 2834294740@qq.com
 * @LastEditTime: 2023-07-14 20:12:55
 * @FilePath: \MY_GUI\HARDWARE\stm32_u8g2.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __STM32_U8G2_H
#define __STM32_U8G2_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */
 
/* USER CODE END Includes */
 
 
 
/* USER CODE BEGIN Private defines */

#define HARDWARE_I2C
// #define SOFTWARE_I2C

#define SEND_BUFFER_DISPLAY_MS(u8g2, ms)\
  do {\
    u8g2_SendBuffer(u8g2); \
    Tims_delay_ms(ms);\
  }while(0);

/* USER CODE END Private defines */
#define u8         unsigned char  // ?unsigned char ????
#define MAX_LEN    128  //
#define OLED_ADDRESS  0x78 // oled
#define OLED_CMD   0x00  // 
#define OLED_DATA  0x40  // 
 
/* USER CODE BEGIN Prototypes */
uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void u8g2Init(u8g2_t *u8g2);
void draw_(u8g2_t *u8g2);
void testDrawPixelToFillScreen(u8g2_t *u8g2);

#endif