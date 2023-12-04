#ifndef __TIME_H
#define __TIME_H

#include "main.h"

typedef struct 
{
    uint8_t hour;
    uint8_t min;
    uint8_t sec;

    uint16_t year;
    uint8_t month;
    uint8_t date;
    uint8_t week;
}TIME;

uint8_t RTC_Init(void);
uint8_t Is_Leap_Year(uint16_t year);
uint8_t RTC_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec);
uint8_t RTC_Alarm_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec);
uint8_t RTC_Get(void);
uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day);
void Show_Time(void);

#endif