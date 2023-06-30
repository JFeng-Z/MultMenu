/*
 * @Author: JFeng 2834294740@qq.com
 * @Date: 2023-06-22 22:18:02
 * @LastEditors: JFeng 2834294740@qq.com
 * @LastEditTime: 2023-06-29 20:30:30
 * @FilePath: \Projectd:\study\STM32F103_CMSIS\MY_GUI_RTOS\HARDWARE\MY_I2C\MyI2C.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _MYI2C_H
#define _MYI2C_H

#include "stm32f10x.h"
#include "Tim_Tick.h"

#define I2C_PORT GPIOB
#define I2C_SCL GPIO_Pin_10
#define I2C_SDA GPIO_Pin_11

void MyI2C_SCL_W_SetState(uint8_t State);
void MyI2C_SDA_W_SetState(uint8_t State);
uint8_t MyI2C_SDA_R_SetState(void);
void MyI2C_Init(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ResetByte(void);
void MyI2C_SendAck(uint8_t AckBit);
uint8_t MyI2C_ReceiveAck(void);

#endif