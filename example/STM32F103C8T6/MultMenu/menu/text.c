#include "text.h"
#include "menu.h"
#include "dispDirver.h"
#include "string.h"

text_t github_text = {"https://github.com/JFeng-Z/MultMenu", u8g2_font_7x13_tr, 13, 7};
text_t bilibili_text = {"https://www.bilibili.com/video/BV1d4421Q7kD?vd_source=11fa79768e087179635ff2a439abe018", MENU_FONT, Font_Hight, Font_Width};

void Text_Widget(xpMenu Menu) 
{
    OLED_SetFont(Menu->now_item->element->text->font);
    int char_cnt = strlen(Menu->now_item->element->text->ptr);
    int lineMaxCnt = HOR_RES / Menu->now_item->element->text->font_width;
    int line = 0;
    int x = 0;
    char c;
    int cnt = 0;
    if(DialogScale_Show(Menu, 0, 0, HOR_RES, VER_RES))
    {
        for (int i = 0; i < char_cnt; i++)
        {
            c = Menu->now_item->element->text->ptr[i];
            x += Menu->now_item->element->text->font_width;
            cnt ++;
            if(cnt == lineMaxCnt)
            {
                line ++;
                cnt = 0;
                x = 0;
            }
            OLED_DrawStr(x, Menu->now_item->element->text->font_hight + line*(Menu->now_item->element->text->font_hight), &c);
        }
        OLED_SendBuffer();
    }
}
