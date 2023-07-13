#include "Menu.h"

extern u8g2_t u8g2; 
extern uint8_t KEY_STATE;

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

uint8_t state=0,dir=0;

void DrawPage(uint8_t pos, xpMenu Page, uint8_t LineSpacing)
{
    int8_t first_line=FirstLine;
    xpItem temp = Page->itemHead;
        for (int i = 1; i <= Page->len; i++)
        {
            u8g2_DrawStr(&u8g2,pos,first_line,Page->PageName);
            u8g2_DrawStr(&u8g2,pos,first_line + i * LineSpacing,temp->itemName);
            temp = temp->nextiTem;
        }     
}

const uint8_t Time=3;

/**
 * @brief 线性增长函数用于坐标移动
 * 
 * @param Time_Now 当前时间
 * @param Tgt 目标值
 * @param Now 当前值
 * @return uint8_t 
 */
uint8_t Line(uint8_t Time_Now,uint8_t Tgt,uint8_t Now)
{
    return (Tgt-Now)*Time_Now/Time+Now;			//return c * t / d + b;
}

void Draw_Menu(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem item,xpItem next_item)
{
    uint8_t t=0,Item_Num_Now=item->Num,Item_Num_Next=next_item->Num;
    uint8_t item_wide=strlen(item->itemName)*6+4;
    uint8_t item_line=0;
    if (Item_Num_Now>4)
    {
        Item_Num_Now=4;
    }
    if (Item_Num_Next>4)
    {
        Item_Num_Next=4;
    }
    item_line=Item_Num_Now*LineSpacing;
    #ifdef Head_To_Tail 
    Page->itemTail->nextiTem=Page->itemHead;  
    Page->itemHead->lastiTem=Page->itemTail;
    if (__fabs(next_item->Num-item->Num)==item->location->len-1)
    {
        u8g2_ClearBuffer(&u8g2);
        u8g2_SetDrawColor(&u8g2,1);  
        DrawPage(pos,Page,LineSpacing);
        u8g2_SetDrawColor(&u8g2,2);
        u8g2_DrawRBox(&u8g2,pos,(next_item->Num)*LineSpacing,strlen(next_item->itemName)*6+4,Font_Size,4);
        u8g2_SendBuffer(&u8g2);
        return ;
    }
    #else 
    Page->itemTail->nextiTem=item;
    #endif 

    do
    {
		t++;
        u8g2_ClearBuffer(&u8g2);
        u8g2_SetDrawColor(&u8g2,1);  
        item_line=Line(t,Item_Num_Next*LineSpacing,item_line);
        item_wide=Line(t,strlen(next_item->itemName)*6+4,item_wide);
        DrawPage(pos,Page,LineSpacing);
        u8g2_SetDrawColor(&u8g2,2);
        u8g2_DrawRBox(&u8g2,pos,item_line-1,item_wide,Font_Size,4);
        u8g2_SendBuffer(&u8g2);
    } while (t<Time);

}

void Show_MPU6050(void)
{
    static float pitch,roll,yaw; 		//欧拉角
    char Pi[20],Ro[20],Ya[20];
    while (1)
    {
        if (key_read()==ENTER)
        {
            KEY_STATE=RESET;
            break;
        }
        mpu_dmp_get_data(&pitch,&roll,&yaw);
        sprintf(Pi,"Pitch = %.2f",pitch);
        sprintf(Ro,"Roll  = %.2f",roll);
        sprintf(Ya,"Yaw   = %.2f",yaw);
        u8g2_ClearBuffer(&u8g2);
        u8g2_DrawStr(&u8g2,0,12,Pi);
        u8g2_DrawStr(&u8g2,0,36,Ro);
        u8g2_DrawStr(&u8g2,0,58,Ya);
        u8g2_SendBuffer(&u8g2);
    }
}

void Show_GitHub(void)
{
    const char* GitHub1="https://github.com/JF";
    const char* GitHub2="eng-Z/MY_GUI.git";
    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawStr(&u8g2,0,24,GitHub1);
    u8g2_DrawStr(&u8g2,0,36,GitHub2);
    u8g2_SendBuffer(&u8g2);
}

void App_Function_Loading(void)
{
    No3Page1item1.Item_function=Show_MPU6050;
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
    // AddItem(" -Reset All", &Mainitem4, &MainPage, &Page4);
    AddItem(" -About", &Mainitem5, &MainPage, &Page5);

        AddPage("[Application]", &Page1);
        AddItem(" -Data", &Page1item1, &Page1, &No3Page1);
        AddItem(" -Games", &Page1item2, &Page1, &No3Page2);
        AddItem(" -Altium", &Page1item3, &Page1, &No3Page3);
        AddItem(" -Return", &Page1item4, &Page1, &MainPage);

            AddPage("[Data]", &No3Page1);
            AddItem(" -MPU6050", &No3Page1item1, &No3Page1, NULL);
            AddItem(" -File2", &No3Page1item2, &No3Page1, NULL);
            AddItem(" -File3", &No3Page1item3, &No3Page1, NULL);
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

        // AddPage("[Reset All]", &Page4);
        // AddItem(" -Reset Name", &Page4item1, &Page4, NULL);
        // AddItem(" -Reset Time", &Page4item2, &Page4, NULL);
        // AddItem(" -Reset Setting", &Page4item3, &Page4, NULL);
        // AddItem(" -Return", &Page4item4, &Page4, &MainPage);

        AddPage("[About]", &Page5);
        AddItem(" -Github", &Page5item1, &Page5, NULL);
        AddItem(" -Bilibili", &Page5item2, &Page5, NULL);
        AddItem(" -ReadME", &Page5item3, &Page5, NULL);
        AddItem(" -Return", &Page5item4, &Page5, &MainPage);
}

xItem temp;
void (*App_Function)();

static void Menu_Task(void* parameter)
{
	static uint8_t MENU_STATE=MENU_RUN;
	temp=Mainitem1;
	while (1)
	{
        u8g2_SetFont(&u8g2,u8g2_font_profont12_mf);
		if (key_read()==ENTER)
		{
            KEY_STATE=RESET;
			if (temp.JumpPage==NULL)
			{
				App_Function=temp.Item_function;
				MENU_STATE=APP_RUN;
			}
			else 
			{
				MENU_STATE=MENU_RUN;
				Draw_Menu(FirstPos,temp.JumpPage,Font_Size,&temp,temp.JumpPage->itemHead);
				temp=*(temp.JumpPage->itemHead);
			}
		}
		if (key_read()==UP)
		{
            KEY_STATE=RESET;
			Draw_Menu(FirstPos,temp.location,Font_Size,&temp,temp.lastiTem);
			temp=*(temp.lastiTem);
			MENU_STATE=MENU_RUN;
		}
		if (key_read()==DOWN)
		{
            KEY_STATE=RESET;
			Draw_Menu(FirstPos,temp.location,Font_Size,&temp,temp.nextiTem);
			temp=*(temp.nextiTem); 
			MENU_STATE=MENU_RUN;
		}
		if (MENU_STATE==APP_RUN)
		{MENU_STATE=MENU_RUN;(*App_Function)();}
	}
}

void Menu_Init(void)
{
  u8g2Init(&u8g2);
  Menu_Team();
  DrawProcess(&u8g2);
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