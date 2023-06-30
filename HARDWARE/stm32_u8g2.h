#ifndef __STM32_U8G2_H
#define __STM32_U8G2_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "u8g2.h"
#include "MyI2C.h"
#include "Tim_Tick.h"
/* USER CODE BEGIN Includes */
 
/* USER CODE END Includes */
 
 
 
/* USER CODE BEGIN Private defines */

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
void draw(u8g2_t *u8g2);
void testDrawPixelToFillScreen(u8g2_t *u8g2);

#endif