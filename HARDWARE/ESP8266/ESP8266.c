#include "ESP8266.h"

extern char RX_Packet[1024];
extern uint8_t RX_Packet_Length;

void ESP8266_Init(void)
{
    // 初始化相关设置，包括串口初始化和中断使能等
    
    printf("AT\r\n");
    while (strcmp("AT\r\n\r\nOK\r\n", RX_Packet) != 0)
    {
        // 继续执行其他任务，确保中断可以正常接收数据
    }
    RX_Packet_Length=0;
    LED(ON);
    
    printf("AT+CWMODE=3\r\n");
    while (strcmp("AT+CWMODE=3\r\n\r\nOK\r\n", RX_Packet) != 0)
    {
        // 继续执行其他任务，确保中断可以正常接收数据
    }
    RX_Packet_Length=0;
    // Tims_delay_ms(1000);
    
    printf("AT+RST\r\n");
    while (strcmp("AT+RST\r\n\r\nOK\r\n", RX_Packet) != 0)
    {
        // 继续执行其他任务，确保中断可以正常接收数据
    }
    RX_Packet_Length=0;
    // Tims_delay_ms(1000);
    
    printf("AT+CWSAP=\"ESP8266\",\"12345678\",1,3\r\n");
    while (strcmp("AT+CWSAP=\"ESP8266\",\"12345678\",1,3\r\n\r\nOK\r\n", RX_Packet) != 0)
    {
        // 继续执行其他任务，确保中断可以正常接收数据
    }
    RX_Packet_Length=0;
    // Tims_delay_ms(1000);
    
    printf("AT+CIPMUX=1\r\n");
    while (strcmp("AT+CIPMUX=1\r\n\r\nOK\r\n", RX_Packet) != 0)
    {
        // 继续执行其他任务，确保中断可以正常接收数据
    }
    RX_Packet_Length=0;
    // Tims_delay_ms(1000);
    
    printf("AT+CIPSERVER=1,8080\r\n");
    while (strcmp("AT+CIPSERVER=1,8080\r\n\r\nOK\r\n", RX_Packet) != 0)
    {
        // 继续执行其他任务，确保中断可以正常接收数据
    }
    RX_Packet_Length=0;
    // Tims_delay_ms(1000);
}