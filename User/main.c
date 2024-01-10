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
      printf("/******Menu is Successfully loaded!******/\r\n");
    }
														
    vTaskStartScheduler();  

    while(1);
}

void Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    USART_Config();
    // Timer_Init();
    LED_GPIO_Config();
    U8G2_Init();
    Menu_Init();
    KEY_GPIO_Config();
    Key_Loading();
}

TaskHandle_t test_handle;

void test_(void *parameter)
{
  while (1)
  {
    printf("test\r\n");
  }
}

static void AppTaskCreate(void)
{
  taskENTER_CRITICAL();           //进入临界区

  /* 创建任务 */
  xTaskCreate((TaskFunction_t)test_,"test",128,NULL,5,test_handle);
  Menu_Task_Create();
  KeyScan_Task_Create();
  vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
  
  taskEXIT_CRITICAL();            //退出临界区
}
