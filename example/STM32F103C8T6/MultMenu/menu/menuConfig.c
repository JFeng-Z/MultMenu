#include "menuConfig.h"
#include "menu.h"
#include "dispDirver.h"
#include "image.h"
#include "application.h"

/* Page*/
xPage Home_Page, System_Page;
/* item */
xItem HomeHead_Item, SystemHead_Item, System_Item, Image_Item, Github_Item, Bilibili_Item;
xItem Contrast_Item, Power_Item;
xItem Wave_Item;

extern int test;

/**
 * 在此建立所需显示或更改的数据
 * 无参数
 * 无返回值
 */
void Create_Parameter(void)
{
    static int Contrast = 255;
    static data_t Contrast_data;
    Contrast_data.name = "Contrast";
    Contrast_data.ptr = &Contrast;
    Contrast_data.function = OLED_SetContrast;
    Contrast_data.Function_Type = STEP_EXECUTE;
    Contrast_data.Data_Type = DATA_INT;
    Contrast_data.Operate_Type = READ_WRITE;
    Contrast_data.max = 255;
    Contrast_data.min = 0;
    Contrast_data.step = 2;
    static element_t Contrast_element;
    Contrast_element.data = &Contrast_data;
    Create_element(&Contrast_Item, &Contrast_element);

    static uint8_t power = true;
    static data_t Power_switch_data;
    Power_switch_data.ptr = &power;
    Power_switch_data.function = OLED_SetPowerSave;
    Power_switch_data.Data_Type = DATA_SWITCH;
    Power_switch_data.Operate_Type = READ_WRITE;
    static element_t Power_element;
    Power_element.data = &Power_switch_data;
    Create_element(&Power_Item, &Power_element);

    static data_t Wave_data;
    Wave_data.name = "Wave";
    Wave_data.ptr = &test;
    static element_t Wave_element;
    Wave_element.data = &Wave_data;
    Create_element(&Wave_Item, &Wave_element);
}

/**
 * 在此建立所需显示或更改的文本
 * 无参数
 * 无返回值
 */
void Create_Text(void)
{
    static text_t github_text;
    github_text.font = MENU_FONT;
    github_text.font_hight = Font_Hight;
    github_text.font_width = Font_Width;
    github_text.ptr = "https://github.com/JFeng-Z/MultMenu";
    static element_t github_element;
    github_element.text = &github_text;
    Create_element(&Github_Item, &github_element);

    static text_t bilibili_text;
    bilibili_text.font = MENU_FONT;
    bilibili_text.font_hight = Font_Hight;
    bilibili_text.font_width = Font_Width;
    bilibili_text.ptr = "https://www.bilibili.com/video/BV1d4421Q7kD?vd_source=11fa79768e087179635ff2a439abe018";
    static element_t bilibili_element;
    bilibili_element.text = &bilibili_text;
    Create_element(&Bilibili_Item, &bilibili_element);
}

/*
 * 菜单构建函数
 * 该函数不接受参数，也不返回任何值。
 * 功能：静态地构建一个菜单系统。
 */
void Create_MenuTree(xpMenu Menu)
{
    AddPage("[HomePage]", &Home_Page, IMAGE);
        AddItem("[HomePage]", LOOP_FUNCTION, NULL, &HomeHead_Item, &Home_Page, NULL, Draw_Home);
        AddItem(" +System", PARENTS, logo_allArray[8], &System_Item, &Home_Page, &System_Page, NULL);
            AddPage("[System]", &System_Page, TEXT);
                AddItem("[System]", RETURN, NULL, &SystemHead_Item, &System_Page, &Home_Page, NULL);
                AddItem(" -Contrast", DATA, NULL, &Contrast_Item, &System_Page, NULL, NULL);
                AddItem(" -Power", DATA, NULL, &Power_Item, &System_Page, NULL, NULL);              
        AddItem(" -Image", LOOP_FUNCTION, logo_allArray[6], &Image_Item, &Home_Page, NULL, Show_Logo);
        AddItem(" -Github", _TEXT_, logo_allArray[5], &Github_Item, &Home_Page, NULL, NULL);
        AddItem(" -Bilibili", _TEXT_, logo_allArray[7], &Bilibili_Item, &Home_Page, NULL, NULL);
        AddItem(" -Wave", WAVE, logo_allArray[9], &Wave_Item, &Home_Page, NULL, NULL);
}

void Menu_Init(xpMenu Menu)
{
    Disp_Init();
    Create_Menu(Menu, &HomeHead_Item);
    Draw_Home(NULL);
}
