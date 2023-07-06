#include "menu.h"
extern u8g2_t u8g2; 
const uint8_t Time=3;
/* Page*/
xpMenu NowPage;
xMenu
    MainPage,                          // 一级Page
    Page1, Page2, Page3, Page4, Page5, // 二级Page
    No3Page1, No3Page2, No3Page3;      // 三级Page

/* item */
// 一级Page的item
xpItem Selectitm;
xItem Mainitem1, Mainitem2, Mainitem3, Mainitem4, Mainitem5, Mainitem6;
// 二级Page的item
xItem Page1item1, Page1item2, Page1item3, Page1item4;
xItem Page2item1, Page2item2, Page2item3, Page2item4;
xItem Page3item1, Page3item2, Page3item3, Page3item4;
xItem Page4item1, Page4item2, Page4item3, Page4item4;
xItem Page5item1, Page5item2, Page5item3, Page5item4;
// 三级Page的item
xItem No3Pageitem1, No3Pageitem2, No3Pageitem3, No3Pageitem4;

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
        item->lastiTem = NULL;
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
    xpItem temp = Page->itemHead;
        for (int i = 1; i <= Page->len; i++)
        {
            u8g2_DrawStr(&u8g2,pos,FirstLine,Page->PageName);
            u8g2_DrawStr(&u8g2,pos,FirstLine + i * LineSpacing,temp->itemName);
            temp = temp->nextiTem;
        }     
}

void DrawBox(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem item,xpItem next_item)
{
    uint8_t t=0;
    uint8_t item_wide=strlen(item->itemName)*6+4;
    uint8_t item_line=(item->Num)*LineSpacing;
    Page->itemTail->nextiTem=Page->itemHead;
    Page->itemHead->lastiTem=Page->itemTail;
        do
        {
            u8g2_ClearBuffer(&u8g2);
            u8g2_SetDrawColor(&u8g2,1);  
            DrawPage(pos,Page,LineSpacing);
            item_line=Linear((next_item->Num)*LineSpacing,item_line);
            item_wide=Linear2(strlen(next_item->itemName)*6+4,item_wide);
            u8g2_SetDrawColor(&u8g2,2);
            u8g2_DrawRBox(&u8g2,pos,item_line-1,item_wide,Font_Size,4);
            printf("y=%d",item_line);
            u8g2_SendBuffer(&u8g2);
            t++;
        } while (t<Time);
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
    AddItem(" -666666", &Mainitem6, &MainPage, &Page5);

        AddPage("[Application]", &Page1);
        AddItem(" -VsCode", &Page1item1, &Page1, &No3Page1);
        AddItem(" -STM32CubeMX", &Page1item2, &Page1, &No3Page2);
        AddItem(" -Altium", &Page1item3, &Page1, &No3Page3);
        AddItem(" -<<--", &Page1item4, &Page1, &MainPage);

            AddPage("[VsCode]", &No3Page1);
            AddItem(" -File1", &No3Pageitem1, &No3Page1, NULL);
            AddItem(" -File2", &No3Pageitem2, &No3Page1, NULL);
            AddItem(" -File3", &No3Pageitem3, &No3Page1, NULL);
            AddItem(" -<<--", &No3Pageitem4, &No3Page1, &Page1);

        AddPage("[Text]", &Page2);
        AddItem(" -New Project", &Page2item1, &Page2, NULL);
        AddItem(" -New Project", &Page2item2, &Page2, NULL);
        AddItem(" -New Project", &Page2item3, &Page2, NULL);
        AddItem(" -<<--", &Page2item4, &Page2, &MainPage);

        AddPage("[Image]", &Page3);
        AddItem(" -New Project", &Page3item1, &Page3, NULL);
        AddItem(" -New Project", &Page3item2, &Page3, NULL);
        AddItem(" -New Project", &Page3item3, &Page3, NULL);
        AddItem(" -<<--", &Page3item4, &Page3, &MainPage);

        AddPage("[Reset All]", &Page4);
        AddItem(" -Reset Name", &Page4item1, &Page4, NULL);
        AddItem(" -Reset Time", &Page4item2, &Page4, NULL);
        AddItem(" -Reset Setting", &Page4item3, &Page4, NULL);
        AddItem(" -<<--", &Page4item4, &Page4, &MainPage);

        AddPage("[About]", &Page5);
        AddItem(" -Github", &Page5item1, &Page5, NULL);
        AddItem(" -Bilibili", &Page5item2, &Page5, NULL);
        AddItem(" -ReadME", &Page5item3, &Page5, NULL);
        AddItem(" -<<--", &Page5item4, &Page5, &MainPage);
}

xItem temp;

static void Menu_Task(void* parameter)
{
  temp=Mainitem1;
  while (1)
  {
    if (Key_Scan_WK(KeyWkUp_PORT,KeyWkUp_PIN)==KEY_ON)
	{
		DrawBox(FirstPos,temp.JumpPage,Font_Size,&temp,temp.JumpPage->itemHead);
        temp=*(temp.JumpPage->itemHead);
	}
	if (Key_Scan(KEY1_PORT,KEY1_PIN)==KEY_ON)
	{
        DrawBox(FirstPos,temp.location,Font_Size,&temp,temp.lastiTem);
        temp=*(temp.lastiTem);
	}
	if (Key_Scan(KEY2_PORT,KEY2_PIN)==KEY_ON)
	{
        DrawBox(FirstPos,temp.location,Font_Size,&temp,temp.nextiTem);
        temp=*(temp.nextiTem);
	}
  }
}

void Menu_Init(void)
{
  u8g2Init(&u8g2);
  Menu_Team();
  u8g2_SetFont(&u8g2,u8g2_font_profont12_mf);
  testDrawProcess(&u8g2);
  DrawBox(FirstPos,&MainPage,Font_Size,&Mainitem1,&Mainitem1);
}

TaskHandle_t Menu_Task_Handle;

BaseType_t Menu_Task_Create(void)
{
    BaseType_t xReturn=pdPASS;
    xReturn=xTaskCreate((TaskFunction_t)Menu_Task,"Menu_Task",100,NULL,4,&Menu_Task_Handle);
    if (pdPASS==xReturn)
    {
        return pdPASS;
    }
    else return pdFAIL;
}