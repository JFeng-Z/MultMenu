#include "text.h"
#include "menu.h"
#include "dispDirver.h"
#include "string.h"

void Text_Widget(xpMenu Menu) 
{
    OLED_SetFont(Menu->now_item->element->text->font);
    int char_cnt = strlen(Menu->now_item->element->text->ptr);
    int lineMaxCnt = HOR_RES / Menu->now_item->element->text->font_width - 1;
    int line = 0;
    int x = 2;
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
                x = 2;
            }
            OLED_DrawStr(x, Menu->now_item->element->text->font_hight + line*(Menu->now_item->element->text->font_hight), &c);
        }
        OLED_SendBuffer();
    }
}
