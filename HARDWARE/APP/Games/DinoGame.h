#ifndef _DinoGame_H
#define _DinoGame_H

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "u8g2.h"
#include "bsp_key.h"
#include <stdlib.h>  // rand
#include "Menu.h"

void DinoGame_Run(void);

#endif