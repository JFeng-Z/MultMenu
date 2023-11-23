#include "Other_Function.h"

extern u8g2_t u8g2;
//1为白天模式，0为黑夜模式
extern uint8_t BgColor;  

void White_Dark_Day(void)
{
    BgColor=BgColor^0x01;
}

void Show_MPU6050(void)
{
    static float pitch,roll,yaw; 		//欧拉角
    char Pi[20],Ro[20],Ya[20];

    DialogScale_Show(&u8g2,1,2,100,62);

    while (1)
    {
        if(Get_Key(KeyWkUp)==PRESS_UP)return;
        mpu_dmp_get_data(&pitch,&roll,&yaw);
        sprintf(Pi,"Pitch = %.2f",pitch);
        sprintf(Ro,"Roll  = %.2f",roll);
        sprintf(Ya,"Yaw   = %.2f",yaw);
        u8g2_DrawStr(&u8g2,12,14,Pi);
        u8g2_DrawStr(&u8g2,12,36,Ro);
        u8g2_DrawStr(&u8g2,12,58,Ya);
        u8g2_SendBuffer(&u8g2);
    }

}

void Show_GitHub(void)
{
    const char* GitHub1="https://github.com/";
    const char* GitHub2="JFeng-Z/MY_GUI.git";

    DialogScale_Show(&u8g2,2,12,125,32);

    while (1)
    {
        if(Get_Key(KeyWkUp)==PRESS_UP)return;
        u8g2_DrawStr(&u8g2,8,25,GitHub1);
        u8g2_DrawStr(&u8g2,8,37,GitHub2);
        u8g2_SendBuffer(&u8g2);
    }
    
}

void Show_Bilibili(void)
{
    const char* Bilibili="BV1Xh4y1N7By";

    DialogScale_Show(&u8g2,22,24,82,20);

    while (1)
    {
        if(Get_Key(KeyWkUp)==PRESS_UP)return;
        u8g2_DrawStr(&u8g2,28,37,Bilibili);
        u8g2_SendBuffer(&u8g2);
    }
    
}

extern uint8_t Options_Time;

void Setting_Speed(void)
{
    static char Speed[1],Spd[10];

    DialogScale_Show(&u8g2,4,12,120,32);
    while(1)
    {
        if(Get_Key(KeyWkUp)==PRESS_UP)return;
        if((Get_Key(Key1)==PRESS_UP)&&Options_Time<30)Options_Time++;
        if((Get_Key(Key2)==PRESS_UP)&&Options_Time>2)Options_Time--;
        Speed[0]=Options_Time;
        Draw_DialogBox(&u8g2,0,12,127,32);
        sprintf(Spd,"Speed = %d",Speed[0]);
        u8g2_DrawStr(&u8g2,8,25,Spd);
        u8g2_DrawRBox(&u8g2,8,30,Options_Time*3,6,3);
        u8g2_DrawRFrame(&u8g2,8,30,90,6,3);
        u8g2_SendBuffer(&u8g2);
    }
}

void Car_State(void)
{
    u8g2_DrawStr(&u8g2,100,45,"ON");
    u8g2_SendBuffer(&u8g2);
    printf("Car is ready!!!!\n");
    // MPU_EXTI_Init();
    vTaskDelete(NULL);
}

extern uint8_t RX_Packet[1024];
extern bool packetReceived;

void Screen(void)
{
    Tims_delay_ms(20);
    while(1)
    {
        if(Get_Key(KeyWkUp)==PRESS_UP)return;
        if(packetReceived)
        {
            u8g2_ClearBuffer(&u8g2);
            u8g2_DrawXBMP(&u8g2,0,0,128,64,RX_Packet);
            u8g2_SendBuffer(&u8g2);
            packetReceived=false;
        }
    }
}