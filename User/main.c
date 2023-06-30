/*
 * @Author: JFeng 2834294740@qq.com
 * @Date: 2023-06-11 18:47:17
 * @LastEditors: JFeng 2834294740@qq.com
 * @LastEditTime: 2023-06-29 23:18:41
 * @FilePath: \Projectd:\study\STM32F103_CMSIS\RTOS_Trends\User\main.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

u8g2_t u8g2;
uint8_t func_index=0;
uint8_t last_index=0;
// int16_t number;
extern key_table table[30];
void (*now_func)();

TaskHandle_t AppTaskCreate_Handle;
// TaskHandle_t LED_Task_Handle;
// TaskHandle_t Encoder_Handle;
TaskHandle_t GUI_Task_Handle;

void Init(void);
static void AppTaskCreate(void);
// static void LED_Task(void* parameter);
// static void Encoder(void* parameter);
static void GUI_Task(void* parameter);

int main(void)
{
    BaseType_t xReturn = pdPASS;

    Init();

    testDrawProcess(&u8g2);

    now_func=table[0].now_function;

    fun_Cover();

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
    u8g2Init(&u8g2);
    LED_GPIO_Config();
    KEY_GPIO_Config();
    rotary_encoder_Init();
}

static void AppTaskCreate(void)
{
  taskENTER_CRITICAL();           //进入临界区

  /* 创建LED_Task任务 */
	// xTaskCreate((TaskFunction_t)LED_Task,"LED_Task",52,NULL,4,&LED_Task_Handle);	
  // xTaskCreate((TaskFunction_t)Encoder,"Encoder",128,NULL,4,&Encoder_Handle);
  xTaskCreate((TaskFunction_t)GUI_Task,"GUI_Task",128,NULL,4,&GUI_Task_Handle);

  vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
  
  taskEXIT_CRITICAL();            //退出临界区
}

static void GUI_Task(void* parameter)
{
  while (1)
  {
    if (Key_Scan_WK(KeyWkUp_PORT,KeyWkUp_PIN)==KEY_ON)
		{
			func_index=table[func_index].enter;
		}
		if (Key_Scan(KEY1_PORT,KEY1_PIN)==KEY_ON)
		{
			func_index=table[func_index].up;
		}
		if (Key_Scan(KEY2_PORT,KEY2_PIN)==KEY_ON)
		{
			func_index=table[func_index].down;
		}
		if (func_index!=last_index)
		{
			now_func=table[func_index].now_function;
			u8g2_ClearBuffer(&u8g2);
			(*now_func)();
			u8g2_SendBuffer(&u8g2);
			last_index=func_index;
		}
  }
}

// static void LED_Task(void* parameter)
// {
//     while (1)
//     {
//         LED(ON);
//         vTaskDelay(500);
//         printf("led1_task running,LED1_ON\r\n");
//         LED(OFF);
//         vTaskDelay(500);
//         printf("led1_task running,LED1_OFF\r\n");
//         vTaskDelay(500);
//         printf("the min free stack size is %d \r\n",(uint32_t)uxTaskGetStackHighWaterMark(NULL));     //还剩多少栈空间
//     }
// }

// static void Encoder(void* parameter)
// {
//   while (1)
//   {
//     number=GETCounter_Encoder();
//     printf("number=%d",number);
//     vTaskDelay(1000);
//   }
// }