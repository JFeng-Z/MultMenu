#include "stm32f10x.h"
#include "bsp_usart.h"
#include "menu.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

TaskHandle_t AppTaskCreate_Handle;
TaskHandle_t Test_Task_Handle;

static void Test_Task(void* parameter)
{
    while(1)
    {
        printf("Hello World!\n");
    }
}

static void AppTaskCreate(void* parameter)
{
  taskENTER_CRITICAL();           //进入临界区
  Menu_Task_Create();
  xTaskCreate(Test_Task,"Test_Task",128,NULL,4,Test_Task_Handle);
  vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
  taskEXIT_CRITICAL();            //退出临界区
}

int main()
{
    USART_Config();
    Menu_Init();
    BaseType_t xReturn = pdPASS;
    xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,"AppTaskCreate",128,NULL,3,&AppTaskCreate_Handle);	
    if (pdPASS==xReturn)
    {
      printf("/******Menu is Successfully loaded!******/\n");
    }
    vTaskStartScheduler(); 
    while(1); 
}
