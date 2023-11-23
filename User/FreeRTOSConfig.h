/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

*/


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "stm32f10x.h"
#include "bsp_usart.h"


//��Բ�ͬ�ı��������ò�ͬ��stdint.h�ļ�
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
    #include <stdint.h>
    extern uint32_t SystemCoreClock;
#endif

//����
#define vAssertCalled(char,int) printf("Error:%s,%d\r\n",char,int)
#define configASSERT(x) if((x)==0) vAssertCalled(__FILE__,__LINE__)

/************************************************************************
 *               FreeRTOS������������ѡ�� 
 *********************************************************************/
/* ��1��RTOSʹ����ռʽ����������0��RTOSʹ��Э��ʽ��������ʱ��Ƭ��
 * 
 * ע���ڶ�������������ϣ�����ϵͳ���Է�Ϊ��ռʽ��Э��ʽ���֡�
 * Э��ʽ����ϵͳ�����������ͷ�CPU���л�����һ������
 * �����л���ʱ����ȫȡ�����������е�����
 */
#define configUSE_PREEMPTION					  1

//1ʹ��ʱ��Ƭ����(Ĭ��ʽʹ�ܵ�)
#define configUSE_TIME_SLICING					1		

/* ĳЩ����FreeRTOS��Ӳ�������ַ���ѡ����һ��Ҫִ�е�����
 * ͨ�÷������ض���Ӳ���ķ��������¼�ơ����ⷽ��������
 * 
 * ͨ�÷�����
 *      1.configUSE_PORT_OPTIMISED_TASK_SELECTION Ϊ 0 ����Ӳ����֧���������ⷽ����
 *      2.������������FreeRTOS֧�ֵ�Ӳ��
 *      3.��ȫ��Cʵ�֣�Ч���Ե������ⷽ����
 *      4.��ǿ��Ҫ���������������ȼ���Ŀ
 * ���ⷽ����
 *      1.���뽫configUSE_PORT_OPTIMISED_TASK_SELECTION����Ϊ1��
 *      2.����һ�������ض��ܹ��Ļ��ָ�һ�������Ƽ���ǰ����[CLZ]ָ���
 *      3.��ͨ�÷�������Ч
 *      4.һ��ǿ���޶����������ȼ���ĿΪ32
 * һ����Ӳ������ǰ����ָ������ʹ�õģ�MCUû����ЩӲ��ָ��Ļ��˺�Ӧ������Ϊ0��
 */
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	        1                       
                                                                        
/* ��1��ʹ�ܵ͹���ticklessģʽ����0������ϵͳ���ģ�tick���ж�һֱ����
 * ���迪���͹��ĵĻ����ܻᵼ�����س������⣬��Ϊ������˯����,�������°취���
 * 
 * ���ط�����
 *      1.���������������Ӻ�
 *      2.��ס��λ�������������˲���ɿ���λ����
 *     
 *      1.ͨ������ñ�� BOOT 0 �Ӹߵ�ƽ(3.3V)
 *      2.�����ϵ磬����
 *    
 * 			1.ʹ��FlyMcu����һ��оƬ��Ȼ���������
 *			STMISP -> ���оƬ(z)
 */
#define configUSE_TICKLESS_IDLE													0   

/*
 * д��ʵ�ʵ�CPU�ں�ʱ��Ƶ�ʣ�Ҳ����CPUָ��ִ��Ƶ�ʣ�ͨ����ΪFclk
 * FclkΪ����CPU�ں˵�ʱ���źţ�������˵��cpu��ƵΪ XX MHz��
 * ����ָ�����ʱ���źţ���Ӧ�ģ�1/Fclk��Ϊcpuʱ�����ڣ�
 */
#define configCPU_CLOCK_HZ						  (SystemCoreClock)

//RTOSϵͳ�����жϵ�Ƶ�ʡ���һ���жϵĴ�����ÿ���ж�RTOS��������������
#define configTICK_RATE_HZ						  (( TickType_t )1000)

//��ʹ�õ�������ȼ�
#define configMAX_PRIORITIES					  (32)

//��������ʹ�õĶ�ջ��С
#define configMINIMAL_STACK_SIZE				((unsigned short)128)
  
//���������ַ�������
#define configMAX_TASK_NAME_LEN					(16)

 //ϵͳ���ļ����������������ͣ�1��ʾΪ16λ�޷������Σ�0��ʾΪ32λ�޷�������
#define configUSE_16_BIT_TICKS					0                      

//�����������CPUʹ��Ȩ������ͬ���ȼ����û�����
#define configIDLE_SHOULD_YIELD					1           

//���ö���
#define configUSE_QUEUE_SETS					  0    

//��������֪ͨ���ܣ�Ĭ�Ͽ���
#define configUSE_TASK_NOTIFICATIONS    1   

//ʹ�û����ź���
#define configUSE_MUTEXES						    1    

//ʹ�õݹ黥���ź���                                            
#define configUSE_RECURSIVE_MUTEXES			0   

//Ϊ1ʱʹ�ü����ź���
#define configUSE_COUNTING_SEMAPHORES		0

/* ���ÿ���ע����ź�������Ϣ���и��� */
#define configQUEUE_REGISTRY_SIZE				10                                 
                                                                       
#define configUSE_APPLICATION_TASK_TAG		  0                       
                      

/*****************************************************************
              FreeRTOS���ڴ������й�����ѡ��                                               
*****************************************************************/
//֧�ֶ�̬�ڴ�����
#define configSUPPORT_DYNAMIC_ALLOCATION        1    
//֧�־�̬�ڴ�
#define configSUPPORT_STATIC_ALLOCATION					0					
//ϵͳ�����ܵĶѴ�С
#define configTOTAL_HEAP_SIZE					((size_t)(12*1024))    


/***************************************************************
             FreeRTOS�빳�Ӻ����йص�����ѡ��                                            
**************************************************************/
/* ��1��ʹ�ÿ��й��ӣ�Idle Hook�����ڻص�����������0�����Կ��й���
 * 
 * ������������һ������������������û���ʵ�֣�
 * FreeRTOS�涨�˺��������ֺͲ�����void vApplicationIdleHook(void )��
 * ���������ÿ�������������ڶ��ᱻ����
 * �����Ѿ�ɾ����RTOS���񣬿�����������ͷŷ�������ǵĶ�ջ�ڴ档
 * ��˱��뱣֤����������Ա�CPUִ��
 * ʹ�ÿ��й��Ӻ�������CPU����ʡ��ģʽ�Ǻܳ�����
 * �����Ե��û������������������API����
 */
#define configUSE_IDLE_HOOK						0      

/* ��1��ʹ��ʱ��Ƭ���ӣ�Tick Hook������0������ʱ��Ƭ����
 * 
 * 
 * ʱ��Ƭ������һ������������������û���ʵ�֣�
 * FreeRTOS�涨�˺��������ֺͲ�����void vApplicationTickHook(void )
 * ʱ��Ƭ�жϿ��������Եĵ���
 * ��������ǳ���С�����ܴ���ʹ�ö�ջ��
 * ���ܵ����ԡ�FromISR" �� "FROM_ISR����β��API����
 */
 /*xTaskIncrementTick��������xPortSysTickHandler�жϺ����б����õġ���ˣ�vApplicationTickHook()����ִ�е�ʱ�����̲ܶ���*/
#define configUSE_TICK_HOOK						0           

//ʹ���ڴ�����ʧ�ܹ��Ӻ���
#define configUSE_MALLOC_FAILED_HOOK			0 

/*
 * ����0ʱ���ö�ջ�����⹦�ܣ����ʹ�ô˹��� 
 * �û������ṩһ��ջ������Ӻ��������ʹ�õĻ�
 * ��ֵ����Ϊ1����2����Ϊ������ջ�����ⷽ�� */
#define configCHECK_FOR_STACK_OVERFLOW			0   


/********************************************************************
          FreeRTOS������ʱ�������״̬�ռ��йص�����ѡ��   
**********************************************************************/
//��������ʱ��ͳ�ƹ���
#define configGENERATE_RUN_TIME_STATS	        0             
 //���ÿ��ӻ����ٵ���
#define configUSE_TRACE_FACILITY				      0    
/* ���configUSE_TRACE_FACILITYͬʱΪ1ʱ���������3������
 * prvWriteNameToBuffer()
 * vTaskList(),
 * vTaskGetRunTimeStats()
*/
#define configUSE_STATS_FORMATTING_FUNCTIONS	1                       
                                                                        
                                                                        
/********************************************************************
                FreeRTOS��Э���йص�����ѡ��                                                
*********************************************************************/
//����Э�̣�����Э���Ժ���������ļ�croutine.c
#define configUSE_CO_ROUTINES 			          0                 
//Э�̵���Ч���ȼ���Ŀ
#define configMAX_CO_ROUTINE_PRIORITIES       ( 2 )                   


/***********************************************************************
                FreeRTOS��������ʱ���йص�����ѡ��      
**********************************************************************/
 //����������ʱ��
#define configUSE_TIMERS				            0                              
//������ʱ�����ȼ�
#define configTIMER_TASK_PRIORITY		        (configMAX_PRIORITIES-1)        
//������ʱ�����г���
#define configTIMER_QUEUE_LENGTH		        10                               
//������ʱ�������ջ��С
#define configTIMER_TASK_STACK_DEPTH	      (configMINIMAL_STACK_SIZE*2)    

/************************************************************
            FreeRTOS��ѡ��������ѡ��                                                     
************************************************************/
#define INCLUDE_xTaskGetSchedulerState       1                       
#define INCLUDE_vTaskPrioritySet		         1
#define INCLUDE_uxTaskPriorityGet		         1
#define INCLUDE_vTaskDelete				           1
#define INCLUDE_vTaskCleanUpResources	       1
#define INCLUDE_vTaskSuspend			           1
#define INCLUDE_vTaskDelayUntil			         1
#define INCLUDE_vTaskDelay				           1
#define INCLUDE_eTaskGetState			           1
#define INCLUDE_xTimerPendFunctionCall	     0
//#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     1
//#define INCLUDE_xTaskGetIdleTaskHandle          0


/******************************************************************
            FreeRTOS���ж��йص�����ѡ��                                                 
******************************************************************/
#ifdef __NVIC_PRIO_BITS
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4                  
#endif
//�ж�������ȼ�
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			15     

//ϵͳ�ɹ���������ж����ȼ�
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5 

#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )	/* 240 */

#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )


/****************************************************************
            FreeRTOS���жϷ������йص�����ѡ��                         
****************************************************************/
#define xPortPendSVHandler 	PendSV_Handler
#define vPortSVCHandler 	SVC_Handler


/* ����Ϊʹ��Percepio Tracealyzer��Ҫ�Ķ���������Ҫʱ�� configUSE_TRACE_FACILITY ����Ϊ 0 */
#if ( configUSE_TRACE_FACILITY == 1 )
#include "trcRecorder.h"
#define INCLUDE_xTaskGetCurrentTaskHandle               1   // ����һ����ѡ�������ú����� TraceԴ��ʹ�ã�Ĭ�ϸ�ֵΪ0 ��ʾ���ã�
#endif


#endif /* FREERTOS_CONFIG_H */

