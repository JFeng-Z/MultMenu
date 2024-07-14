#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "MenuConfig.h"

class Application {
public:
    static void Show_MPU6050(xpMenu Menu);
    static void Show_GitHub(xpMenu Menu);
    static void Show_Bilibili(xpMenu Menu);
    static void Show_Log(xpMenu Menu);
    static void Menu_to_Home(void *value);
};

#endif