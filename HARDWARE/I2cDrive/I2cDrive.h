#ifndef _I2cDrive_H
#define _I2cDrive_H

#include "stm32f10x.h"
#include "TimerTick.h"

#define I2C_PORT GPIOB
#define I2C_SCL GPIO_Pin_11
#define I2C_SDA GPIO_Pin_10
#define I2C_RCC RCC_APB2Periph_GPIOB

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