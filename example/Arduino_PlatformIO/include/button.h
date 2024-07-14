#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Arduino.h"

typedef enum BtnEnum
{
    Up,
    Down,
    Center,
    Lift,
    Right,
    NONE_PRESS
}BtnState;

BtnState ButtonScan(void);
BtnState Button_Scan(void);
void buttonInit(void);
void clearButtonstate(void);

#endif