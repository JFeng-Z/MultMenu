/*
 * @Author: JFeng 2834294740@qq.com
 * @Date: 2023-06-11 18:47:17
 * @LastEditors: JFeng 2834294740@qq.com
 * @LastEditTime: 2023-06-30 17:53:08
 * @FilePath: \Projectd:\study\STM32F103_CMSIS\RTOS_Trends\User\main.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

TaskHandle_t AppTaskCreate_Handle;

void Init(void);
static void AppTaskCreate(void);


int main(void)
{
    BaseType_t xReturn = pdPASS;

    Init();

    xReturn=xTaskCreate((TaskFunction_t)AppTaskCreate,"AppTaskCreate",128,NULL,3,&AppTaskCreate_Handle);	
    if (pdPASS==xReturn)
    {
      printf("/******GUI is Successfully loaded!******/");
    }
														
    vTaskStartScheduler();  

    while(1);
}

void Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    LED_GPIO_Config();
    USART_Config();
    Tim_Tick_Init();
    GUI_init();
    LED_GPIO_Config();
    KEY_GPIO_Config();
    rotary_encoder_Init();
}

static void AppTaskCreate(void)
{
  taskENTER_CRITICAL();           //进入临界区

  /* 创建任务 */
  GUI_Task_Create();

  vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
  
  taskEXIT_CRITICAL();            //退出临界区
}
