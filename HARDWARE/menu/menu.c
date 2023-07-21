#include "Menu.h"

u8g2_t u8g2; 
uint8_t Page_State=0;
uint8_t Time=6;
uint8_t BgColor=0x00;       //1为白天模式，0为黑夜模式

/* Page*/
xpMenu NowPage;
xMenu
    MainPage,                          // 一级Page
    Page1, Page2, Page3, Page4, Page5, // 二级Page
    No3Page1, No3Page2, No3Page3;      // 三级Page

/* item */
// 一级Page的item
xpItem Selectitm;
xItem Mainitem1, Mainitem2, Mainitem3, Mainitem4, Mainitem5, Mainitem6,Mainitem7;
// 二级Page的item
xItem Page1item1, Page1item2, Page1item3, Page1item4;
xItem Page2item1, Page2item2, Page2item3, Page2item4;
xItem Page3item1, Page3item2, Page3item3, Page3item4;
xItem Page4item1, Page4item2, Page4item3, Page4item4;
xItem Page5item1, Page5item2, Page5item3, Page5item4;
// 三级Page的item
xItem No3Page1item1, No3Page1item2, No3Page1item3, No3Page1item4,
      No3Page2item1, No3Page2item2, No3Page2item3, No3Page2item4;

void AddPage(const char *name, xpMenu page)
{
    page->PageName = name;
    page->itemHead = NULL;
    page->itemTail = NULL;
}

void AddItem(const char *Name, xpItem item, xpMenu LocalPage, xpMenu nextpage)
{
    item->itemName = Name;
    item->location = LocalPage;
    item->JumpPage = nextpage;
    /* 新建item的下一个肯定是null */
    item->nextiTem = NULL;
    /* 如果可以跳转，那么此item是跳转页面的父级 */
    if (nextpage != NULL)
        nextpage->ParentiTem = item;
    /* 链式结构创建item */
    if (LocalPage->itemHead == NULL) // 如果是第一个iTem
    {
        item->lastiTem = item;
        LocalPage->itemHead = item;
        LocalPage->itemTail = item;
        LocalPage->len = 1;
    }
    else // 不是第一个item
    {
        item->lastiTem = LocalPage->itemTail;                // 新item的last指向Local的tailitem
        LocalPage->itemTail->nextiTem = item;                // 让尾巴的next指向新的item，连接起来
        LocalPage->itemTail = LocalPage->itemTail->nextiTem; // 让尾巴指向新的item
        LocalPage->len++;
    }
    item->Num=LocalPage->len;
}

void Draw_Process(void)
{
	for(int i=10;i<=80;i=i+=2)
	{
		u8g2_ClearBuffer(&u8g2); 
			
		char buff[6];
		int hundred = (int)(i/80.0*100)/100%10;   
		int quotient = (int)(i/80.0*100)/10%10;   
		int remainder = (int)(i/80.0*100)%10;
		buff[0] = hundred + '0';  
		buff[1] = quotient + '0';   
		buff[2] = remainder + '0';   
		buff[3] = '%'; 
		buff[4] = '\0';   
		
		u8g2_SetFont(&u8g2,u8g2_font_profont15_mf);
		u8g2_DrawStr(&u8g2,32,16,"Mr.JFeng");
		
		u8g2_SetFont(&u8g2,u8g2_font_profont12_mf);
		u8g2_DrawStr(&u8g2,100,41,buff);
		
		u8g2_DrawRBox(&u8g2,16,32,i,10,4);
		u8g2_DrawRFrame(&u8g2,16,32,80,10,4);
		
		u8g2_SendBuffer(&u8g2);
	}
}

void Draw_Page(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item)
{
    static int8_t first_line=FirstLine;
    xpItem temp = Page->itemHead;
    
    if(next_item==now_item->JumpPage->itemHead&&next_item!=now_item)    //切换页面时变量初始化
    {first_line=FirstLine;}  

    if ((next_item->Num-now_item->Num>0)&&Page_State==CURSOR_STATIC)
    {
        Page_State=MENU_MOVE;
        first_line-=Font_Size;
    }
    else if ((next_item->Num-now_item->Num<0)&&Page_State==CURSOR_STATIC)
    {
        Page_State=MENU_MOVE;
        first_line+=Font_Size;
    }
    for (int i = 1; i <= Page->len; i++)
    {
        u8g2_DrawStr(&u8g2,pos,first_line + i * LineSpacing,temp->itemName);
        temp = temp->nextiTem;
    }
    u8g2_DrawStr(&u8g2,pos,FirstLine,Page->PageName); 
  
}

/**
 * @brief 线性增长函数用于坐标移动
 * 
 * @param Time_Now 当前时间
 * @param Tgt 目标值
 * @param Now 当前值
 * @return uint8_t 
 */
int8_t Line(uint8_t Time_Now,int8_t Tgt,int8_t Now)
{
    return (Tgt-Now)*Time_Now/Time+Now;			//return c * t / d + b;
}

void Draw_OptionPlace(xpItem next_item)
{
    u8g2_DrawVLine(&u8g2,122,2,64);
    for (uint8_t i = 0; i < next_item->location->len; i++)
    {
        u8g2_DrawHLine(&u8g2,119,i*(64/next_item->location->len)+2,6);
    }
    u8g2_DrawBox(&u8g2,118,(next_item->Num-1)*(64/next_item->location->len),8,4);
}

void Draw_Menu(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item)
{
    uint8_t t=0;
    uint8_t item_wide=strlen(now_item->itemName)*6+4;
    static uint8_t item_line=LINE_MIN;
    static int8_t Tgt_line=0;
    static uint8_t first=0;     //初始状态

    u8g2_SetMaxClipWindow(&u8g2);
    u8g2_SetFont(&u8g2,u8g2_font_profont12_mf);
    
    if(next_item==now_item->JumpPage->itemHead&&next_item!=now_item)        //切换页面时变量初始化
    {item_line=LINE_MIN;Tgt_line=0;first=0;Page_State=0;}

    if ((next_item->Num-now_item->Num==0&&first==0)||next_item==now_item->JumpPage->itemHead)
    {
        Tgt_line=LINE_MIN;first=1;
    }
    else if (next_item->Num-now_item->Num>0)
    {
        Tgt_line+=Font_Size;
        if (Tgt_line>LINE_MAX)
        {
            Page_State=CURSOR_STATIC;
            Tgt_line=LINE_MAX;
        }
    }
    else if(next_item->Num-now_item->Num<0)
    {
        Tgt_line-=Font_Size;
        if (Tgt_line<LINE_MIN)
        {
            Page_State=CURSOR_STATIC;
            Tgt_line=LINE_MIN;
        }
    }
    #ifdef Head_To_Tail 
    Page->itemTail->nextiTem=Page->itemHead;  
    Page->itemHead->lastiTem=Page->itemTail;
    if (__fabs(next_item->Num-now_item->Num)==now_item->location->len-1)
    {
        u8g2_ClearBuffer(&u8g2);
        u8g2_SetDrawColor(&u8g2,1);  
        Draw_OptionPlace(next_item);
        Draw_Page(pos,Page,LineSpacing,now_item,next_item);
        u8g2_SetDrawColor(&u8g2,2);
        u8g2_DrawRBox(&u8g2,pos,(next_item->Num)*LineSpacing,strlen(next_item->itemName)*6+4,Font_Size,4);
        u8g2_SendBuffer(&u8g2);
        return ;
    }
    #else 
    Page->itemTail->nextiTem=now_item;
    #endif 
    do
    {
        u8g2_ClearBuffer(&u8g2);
        t++;
        u8g2_SetDrawColor(&u8g2,BgColor);
        u8g2_DrawBox(&u8g2,0,0,128,64);
        u8g2_SetDrawColor(&u8g2,BgColor^0x01);
        Draw_OptionPlace(next_item);
        Draw_Page(pos,Page,LineSpacing,now_item,next_item);
        u8g2_SetDrawColor(&u8g2,2);
        item_line=Line(t,Tgt_line,item_line);
        item_wide=Line(t,strlen(next_item->itemName)*6+4,item_wide);
        u8g2_DrawRBox(&u8g2,pos+1,item_line-1,item_wide,Font_Size,4);
        u8g2_SendBuffer(&u8g2);
    } while (t<Time);

}

void White_Dark_Day(void)
{
    BgColor=BgColor^0x01;
}

void Show_MPU6050(void)
{
    static float pitch,roll,yaw; 		//欧拉角
    char Pi[20],Ro[20],Ya[20];
    while (1)
    {
        if (key_read()==ENTER)
        {
            Key_Open();
            break;
        } 
        mpu_dmp_get_data(&pitch,&roll,&yaw);
        sprintf(Pi,"Pitch = %.2f",pitch);
        sprintf(Ro,"Roll  = %.2f",roll);
        sprintf(Ya,"Yaw   = %.2f",yaw);
        u8g2_ClearBuffer(&u8g2);
        u8g2_SetDrawColor(&u8g2,BgColor);
        u8g2_DrawBox(&u8g2,0,0,128,64);
        u8g2_SetDrawColor(&u8g2,BgColor^0x01);
        u8g2_DrawStr(&u8g2,12,12,Pi);
        u8g2_DrawStr(&u8g2,12,36,Ro);
        u8g2_DrawStr(&u8g2,12,58,Ya);
        u8g2_SendBuffer(&u8g2);
    }
}

void Show_GitHub(void)
{
    const char* GitHub1="https://github.com/";
    const char* GitHub2="JFeng-Z/MY_GUI.git";
    u8g2_SetDrawColor(&u8g2,BgColor^0x01);
    u8g2_DrawRBox(&u8g2,0,12,127,32,8);
    u8g2_SetDrawColor(&u8g2,BgColor);
    u8g2_DrawStr(&u8g2,8,25,GitHub1);
    u8g2_DrawStr(&u8g2,8,37,GitHub2);
    u8g2_SendBuffer(&u8g2);
}

void Setting_System(void)
{
    static char Speed[1],Spd[10];
    while (1)
    {
        Speed[0]=Time;
        if (key_read()!=0)
        {
            switch (key_read())
            {
            case UP:
                Time++;
                break;
            case DOWN:
                Time--;
                break;
            case ENTER:
                Key_Open();
                return;
            default:
                break;
            }
            Key_Open();
        }
        u8g2_SetDrawColor(&u8g2,BgColor^0x01);
        u8g2_DrawRBox(&u8g2,0,12,127,32,8);
        u8g2_SetDrawColor(&u8g2,BgColor);
        sprintf(Spd,"Speed = %d",Speed[0]);
        u8g2_DrawStr(&u8g2,8,25,Spd);
        u8g2_SendBuffer(&u8g2);
    }
}

void App_Function_Loading(void)
{
    No3Page1item1.Item_function=Show_MPU6050;
    No3Page1item2.Item_function=Setting_System;
    No3Page1item3.Item_function=White_Dark_Day;

    No3Page2item1.Item_function=DinoGame_Run;
    No3Page2item2.Item_function=AirPlane_Run;

    Page5item1.Item_function=Show_GitHub;
}

void Menu_Team(void)
{
    NowPage = &MainPage;
    Selectitm = &Mainitem1;
    MainPage.ParentiTem = NULL;

    AddPage("[MainPage]", &MainPage);
    AddItem(" -Application", &Mainitem1, &MainPage, &Page1);
    AddItem(" -Text", &Mainitem2, &MainPage, &Page2);
    AddItem(" -Image", &Mainitem3, &MainPage, &Page3);
    AddItem(" -Reset All", &Mainitem4, &MainPage, &Page4);
    AddItem(" -About", &Mainitem5, &MainPage, &Page5);
    AddItem(" -test1", &Mainitem6, &MainPage, &Page5);
    AddItem(" -test2", &Mainitem7, &MainPage, &Page5);

        AddPage("[Application]", &Page1);
        AddItem(" -Data", &Page1item1, &Page1, &No3Page1);
        AddItem(" -Games", &Page1item2, &Page1, &No3Page2);
        AddItem(" -Altium", &Page1item3, &Page1, &No3Page3);
        AddItem(" -Return", &Page1item4, &Page1, &MainPage);

            AddPage("[Data]", &No3Page1);
            AddItem(" -MPU6050", &No3Page1item1, &No3Page1, NULL);
            AddItem(" -System", &No3Page1item2, &No3Page1, NULL);
            AddItem(" -Mode", &No3Page1item3, &No3Page1, NULL);
            AddItem(" -Return", &No3Page1item4, &No3Page1, &Page1);

            AddPage("[Games]", &No3Page2);
            AddItem(" -Dino Game", &No3Page2item1, &No3Page2, NULL);
            AddItem(" -AirPlane Game", &No3Page2item2, &No3Page2, NULL);
            AddItem(" -Game3", &No3Page2item3, &No3Page2, NULL);
            AddItem(" -Return", &No3Page2item4, &No3Page2, &Page1);

        AddPage("[Text]", &Page2);
        AddItem(" -New Project", &Page2item1, &Page2, NULL);
        AddItem(" -New Project", &Page2item2, &Page2, NULL);
        AddItem(" -New Project", &Page2item3, &Page2, NULL);
        AddItem(" -Return", &Page2item4, &Page2, &MainPage);

        AddPage("[Image]", &Page3);
        AddItem(" -New Project", &Page3item1, &Page3, NULL);
        AddItem(" -New Project", &Page3item2, &Page3, NULL);
        AddItem(" -New Project", &Page3item3, &Page3, NULL);
        AddItem(" -Return", &Page3item4, &Page3, &MainPage);

        AddPage("[Reset All]", &Page4);
        AddItem(" -Reset Name", &Page4item1, &Page4, NULL);
        AddItem(" -Reset Time", &Page4item2, &Page4, NULL);
        AddItem(" -Reset Setting", &Page4item3, &Page4, NULL);
        AddItem(" -Return", &Page4item4, &Page4, &MainPage);

        AddPage("[About]", &Page5);
        AddItem(" -Github", &Page5item1, &Page5, NULL);
        AddItem(" -Bilibili", &Page5item2, &Page5, NULL);
        AddItem(" -ReadME", &Page5item3, &Page5, NULL);
        AddItem(" -Return", &Page5item4, &Page5, &MainPage);
}

xItem temp_item;
void (*App_Function)();

static void Menu_Task(void* parameter)
{
	static uint8_t MENU_STATE=MENU_RUN;
	temp_item=Mainitem1;
	while (1)
	{
        if (key_read()!=0)
        {
            switch (key_read())
            {
            case ENTER:
                if (temp_item.JumpPage==NULL)
                {
                    App_Function=temp_item.Item_function;
                    MENU_STATE=APP_RUN;
                }
                else 
                {
                    MENU_STATE=MENU_RUN;
                    Draw_Menu(FirstPos,temp_item.JumpPage,Font_Size,&temp_item,temp_item.JumpPage->itemHead);
                    temp_item=*(temp_item.JumpPage->itemHead);
                }
                break;
            case UP:
                Draw_Menu(FirstPos,temp_item.location,Font_Size,&temp_item,temp_item.lastiTem);
                temp_item=*(temp_item.lastiTem);
                MENU_STATE=MENU_RUN;
                break;
            case DOWN:
                Draw_Menu(FirstPos,temp_item.location,Font_Size,&temp_item,temp_item.nextiTem);
                temp_item=*(temp_item.nextiTem); 
                MENU_STATE=MENU_RUN;
                break;
            default:
                break;
            }
            Key_Open();
        }
		if (MENU_STATE==APP_RUN)
		{MENU_STATE=MENU_RUN;(*App_Function)();}
	}
}

void Menu_Init(void)
{
  u8g2Init(&u8g2);
  Menu_Team();
  Draw_Process();
  Draw_Menu(FirstPos,&MainPage,Font_Size,&Mainitem1,&Mainitem1);
  App_Function_Loading();
}

TaskHandle_t Menu_Task_Handle;

BaseType_t Menu_Task_Create(void)
{
    BaseType_t xReturn=pdPASS;
    xReturn=xTaskCreate((TaskFunction_t)Menu_Task,"Menu_Task",100,NULL,5,&Menu_Task_Handle);
    if (pdPASS==xReturn)
    {
        return pdPASS;
    }
    else return pdFAIL;
}