#include <Arduino.h>
#include <U8g2lib.h>
#include "menu.h"
#include "application.h"
#include "button.h"

xMenu menu;
int test;

void setup()
{
    Serial.begin(115200);
    buttonInit();
    Menu_Init(&menu);
}

void loop()
{
    test = rand()%360;
    Menu_Loop(&menu);
}
