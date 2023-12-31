#include "U8g2_User.h"

void testDrawFrame(u8g2_t *u8g2)
{
	int t = 1000;
	int x = 16;
	int y = 32;
	int w = 50;
	int h = 20;
	u8g2_ClearBuffer(u8g2);
	u8g2_DrawStr(u8g2,0, 15, "DrawFrame");

	u8g2_DrawFrame(u8g2, x, y, w, h);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawFrame(u8g2, x+w+5, y-10, w-20, h+20);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
}

void testDrawRBox(u8g2_t *u8g2)
{
	int t = 1000;
	int x = 16;
	int y = 32;
	int w = 50;
	int h = 20;
	int r = 3;
	u8g2_ClearBuffer(u8g2);
	u8g2_DrawStr(u8g2,0, 15, "DrawRBox");

	u8g2_DrawRBox(u8g2, x, y, w, h, r);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawRBox(u8g2, x+w+5, y-10, w-20, h+20, r);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
}

void testDrawCircle(u8g2_t *u8g2)
{
	int t = 600;
	int stx = 0;  
	int sty = 16; 
	int with = 16;
	int r = 15; 
	u8g2_ClearBuffer(u8g2);
	u8g2_DrawStr(u8g2, 0, 15, "DrawCircle");

	u8g2_DrawCircle(u8g2, stx, sty - 1 + with, r, U8G2_DRAW_UPPER_RIGHT); 
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawCircle(u8g2, stx + with, sty, r, U8G2_DRAW_LOWER_RIGHT); 
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawCircle(u8g2, stx - 1 + with * 3, sty - 1 + with, r, U8G2_DRAW_UPPER_LEFT); 
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawCircle(u8g2, stx - 1 + with * 4, sty, r, U8G2_DRAW_LOWER_LEFT); 
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawCircle(u8g2, stx - 1 + with * 2, sty - 1 + with * 2, r, U8G2_DRAW_ALL);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	
    u8g2_DrawCircle(u8g2, 32*3, 32, 31, U8G2_DRAW_ALL);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
}

void testDrawFilledEllipse(u8g2_t *u8g2)
{
	int t = 800;
	int with = 16;
	int rx = 27;  
	int ry = 22;  
	u8g2_ClearBuffer(u8g2);
	u8g2_DrawStr(u8g2,0, 14, "DrawFilledEllipse");

	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawFilledEllipse(u8g2, 0, with, rx, ry, U8G2_DRAW_LOWER_RIGHT);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawFilledEllipse(u8g2, with * 4 - 1, with, rx, ry, U8G2_DRAW_LOWER_LEFT); 
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawFilledEllipse(u8g2, 0, with * 4 - 1, rx, ry, U8G2_DRAW_UPPER_RIGHT); 
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawFilledEllipse(u8g2, with * 4 - 1, with * 4 - 1, rx, ry, U8G2_DRAW_UPPER_LEFT); 
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
	u8g2_DrawFilledEllipse(u8g2, with * 6, with * 2.5, rx, ry, U8G2_DRAW_ALL);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
}

void testDrawMulti(u8g2_t *u8g2)
{
	u8g2_ClearBuffer(u8g2);
	for (int j = 0; j < 64; j+=16)
	{
		for (int i = 0; i < 128; i+=16)
		{
			u8g2_DrawPixel(u8g2, i, j);
			u8g2_SendBuffer(u8g2);
		}
	}
  
    u8g2_ClearBuffer(u8g2);
	for(int i=30; i>0; i-=2)
	{
		u8g2_DrawBox(u8g2,i*2,i,128-i*4,64-2*i);
		u8g2_SendBuffer(u8g2);
	}

	u8g2_ClearBuffer(u8g2);
	for(int i=0; i<32; i+=2)
	{
		u8g2_DrawFrame(u8g2,i*2,i,128-i*4,64-2*i);
		u8g2_SendBuffer(u8g2);
	}
	
	u8g2_ClearBuffer(u8g2);
	for(int i=30; i>0; i-=2)
	{
		u8g2_DrawRBox(u8g2,i*2,i,128-i*4,64-2*i,10-i/3);
		u8g2_SendBuffer(u8g2);
	}

	u8g2_ClearBuffer(u8g2);
	for(int i=0; i<32; i+=2)
	{
		u8g2_DrawRFrame(u8g2,i*2,i,128-i*4,64-2*i,10-i/3);
		u8g2_SendBuffer(u8g2);
	}
	
	u8g2_ClearBuffer(u8g2);
	for(int i=2; i<64; i+=3)
	{
		u8g2_DrawDisc(u8g2,64,32,i, U8G2_DRAW_ALL);
		u8g2_SendBuffer(u8g2);
	}

	u8g2_ClearBuffer(u8g2);
	for(int i=64; i>0; i-=3)
	{
		u8g2_DrawCircle(u8g2,64,32,i, U8G2_DRAW_ALL);
		u8g2_SendBuffer(u8g2);
	}
	
    u8g2_ClearBuffer(u8g2);
	for(int i=2; i<32; i+=3)
	{
		u8g2_DrawFilledEllipse(u8g2,64,32, i*2, i, U8G2_DRAW_ALL);
		u8g2_SendBuffer(u8g2);
	}
	
    u8g2_ClearBuffer(u8g2);
	for(int i=32; i>0; i-=3)
	{
		u8g2_DrawEllipse(u8g2,64,32, i*2, i, U8G2_DRAW_ALL);
		u8g2_SendBuffer(u8g2);
	}
}


// width: 128, height: 48
const unsigned char bilibili[] U8X8_PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0xe0, 0x03, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0xf0, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x80, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0x01, 0xfc, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x03, 0xfc, 0x00, 0x00, 0x3c, 0xc0, 0x0f, 0x00, 0x80, 0x03, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0x07, 0xfc, 0x00, 0x00, 0x3c, 0xc0, 0x0f, 0x00, 0xc0, 0x07, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xfc, 0x00, 0x00, 0x3c, 0x80, 0x0f, 0x00, 0xc0, 0x07, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x80, 0x0f, 0xf8, 0x00, 0x00, 0x3c, 0x80, 0x0f, 0x00, 0x80, 0x07, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x78, 0x80, 0x0f, 0x00, 0x80, 0x07, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x78, 0x80, 0x0f, 0x00, 0x80, 0x07, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x80, 0x79, 0x80, 0x0f, 0x00, 0x98, 0x07, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0xe0, 0x79, 0x9f, 0x0f, 0x00, 0xbe, 0xe7, 0x01, 0xc0, 0x07, 0x10, 0x40, 0x00, 0x1f, 0xf8, 0x00, 0xe0, 0x7b, 0x1f, 0x0f, 0x00, 0xbe, 0xe7, 0x01, 0xc0, 0x87, 0x1f, 0xe0, 0x0f, 0x1f, 0xf8, 0x00, 0xe0, 0x7b, 0x1e, 0x0f, 0x00, 0x3e, 0xe7, 0x01, 0xc0, 0xe7, 0x3f, 0xe0, 0x3f, 0x1f, 0xf0, 0x00, 0xe0, 0x7b, 0x1e, 0x0f, 0x00, 0x3e, 0xe7, 0x01, 0xc0, 0xe7, 0x3f, 0xe0, 0x3f, 0x1f, 0xf0, 0x00, 0x60, 0x71, 0x1e, 0x0f, 0x00, 0x34, 0xe7, 0x01, 0xc0, 0xe7, 0x07, 0x00, 0x3f, 0x1f, 0xf0, 0x00, 0x00, 0x70, 0x00, 0x1f, 0x00, 0x00, 0x07, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0xc0, 0x73, 0x1e, 0x1f, 0x00, 0x3c, 0xc7, 0x01, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0xc0, 0x73, 0x1e, 0x1f, 0x00, 0x7c, 0xe7, 0x01, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0xc0, 0x73, 0x1e, 0x1f, 0x00, 0x7c, 0xef, 0x01, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x01, 0xc0, 0x77, 0x1e, 0x1e, 0x00, 0x7c, 0xef, 0x01, 0xc0, 0x07, 0x00, 0x03, 0x00, 0x1f, 0xf0, 0xff, 0xc1, 0xf7, 0x1e, 0xfe, 0x1f, 0x78, 0xef, 0x01, 0xc0, 0x07, 0x70, 0x37, 0x00, 0x1f, 0xe0, 0xff, 0x87, 0xf7, 0x1e, 0xfe, 0xff, 0x78, 0xee, 0x01, 0xc0, 0x07, 0xe0, 0x3f, 0x00, 0x1f, 0xe0, 0xff, 0x9f, 0xf7, 0x1e, 0xfe, 0xff, 0x79, 0xce, 0x01, 0xc0, 0x07, 0xc0, 0x18, 0x00, 0x1f, 0xe0, 0xff, 0xbf, 0xe7, 0x1e, 0xfe, 0xff, 0x7b, 0xce, 0x01, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0xc7, 0xbf, 0xe7, 0x1e, 0xfe, 0xf8, 0x77, 0xce, 0x01, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x0f, 0x3f, 0xe7, 0x1c, 0xfe, 0xf0, 0x77, 0xce, 0x03, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0xcf, 0x3f, 0xe7, 0x1c, 0xfe, 0xf8, 0xf3, 0xce, 0x03, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xe0, 0xef, 0x1f, 0xe7, 0x1c, 0xfe, 0xfe, 0xf1, 0xce, 0x03, 0x80, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xe0, 0xff, 0x0f, 0xcf, 0x1c, 0xfc, 0xff, 0xf0, 0xc0, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff, 0x07, 0xe0, 0xff, 0x03, 0x06, 0x1c, 0xfc, 0x7f, 0x60, 0xc0, 0x01, 0x00, 0xfe, 0xff, 0xff, 0xff, 0x03, 0xe0, 0xff, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// width: 128, height: 48
const unsigned char three_support[] U8X8_PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x00, 0x00, 0xfc, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x00, 0x00, 0xfe, 0x7f, 0x00, 0x00, 0x00, 0x80, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x80, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x03, 0x00, 0x00, 0x80, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0xc0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x03, 0x00, 0x00, 0x80, 0x0f, 0xf0, 0x01, 0x00, 0x00, 0xfc, 0xff, 0x01, 0x00, 0x00, 0xc0, 0xfd, 0xff, 0x00, 0x00, 0xc0, 0x7f, 0xfe, 0x01, 0x00, 0x00, 0xff, 0xff, 0x0f, 0x00, 0x00, 0xe0, 0xfd, 0xff, 0x01, 0x00, 0xc0, 0x1f, 0xf8, 0x03, 0x00, 0x00, 0xff, 0xff, 0x0f, 0x00, 0x00, 0xe0, 0xfd, 0xff, 0x01, 0x00, 0xc0, 0x0f, 0xf0, 0x03, 0x00, 0x00, 0xfe, 0xff, 0x07, 0x00, 0x00, 0xe0, 0xfd, 0xff, 0x01, 0x00, 0xc0, 0x67, 0xe6, 0x03, 0x00, 0x00, 0xfc, 0xff, 0x03, 0x00, 0x00, 0xe0, 0xfd, 0xff, 0x01, 0x00, 0xc0, 0x67, 0xe6, 0x03, 0x00, 0x00, 0xf8, 0xff, 0x01, 0x00, 0x00, 0xe0, 0xfd, 0xff, 0x00, 0x00, 0xc0, 0x67, 0xe6, 0x03, 0x00, 0x00, 0xf0, 0x7f, 0x00, 0x00, 0x00, 0xe0, 0xfd, 0xff, 0x00, 0x00, 0xc0, 0x67, 0xee, 0x03, 0x00, 0x00, 0xe0, 0x7f, 0x00, 0x00, 0x00, 0xe0, 0xfd, 0xff, 0x00, 0x00, 0x80, 0x7f, 0xfe, 0x01, 0x00, 0x00, 0xe0, 0xff, 0x00, 0x00, 0x00, 0xe0, 0xfd, 0xff, 0x00, 0x00, 0x80, 0x7f, 0xfe, 0x01, 0x00, 0x00, 0xf0, 0xff, 0x00, 0x00, 0x00, 0xe0, 0xfd, 0x7f, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x00, 0x00, 0x00, 0xe0, 0xfd, 0x7f, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x00, 0x00, 0x00, 0xf8, 0xf9, 0x01, 0x00, 0x00, 0xe0, 0xfd, 0x7f, 0x00, 0x00, 0x00, 0xfe, 0x7f, 0x00, 0x00, 0x00, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0xe0, 0xfd, 0x1f, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x30, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// width: 64, height: 64
const unsigned char CJLU_Logo[] U8X8_PROGMEM = { 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFC,0X3F,0X00,0X00,0X00,0X00,0X00,0X80,0X7F,0XFE,0X01,0X00,0X00,0X00,0X00,0XF0,0X21,0X83,0X0F,0X00,0X00,0X00,0X00,0X3C,0XBC,0XC6,0X3C,0X00,0X00,0X00,0X00,0X8E,0XB8,0XC6,0X73,0X00,0X00,0X00,0X80,0X93,0XB9,0X47,0XC9,0X01,0X00,0X00,0XC0,0XB0,0X2F,0X43,0X2D,0X03,0X00,0X00,0X60,0XA3,0XE3,0XC7,0X3D,0X06,0X00,0X00,0XB0,0X76,0X0E,0X70,0X9E,0X0C,0X00,0X00,0XD8,0X9C,0X01,0X80,0X59,0X1A,0X00,0X00,0XAC,0X6D,0X80,0X01,0X66,0X33,0X00,0X00,0X66,0X1B,0X80,0X01,0X18,0X6D,0X00,0X00,0XC3,0X04,0XC0,0X03,0X30,0XC3,0X00,0X00,0X83,0X02,0XE0,0X07,0X40,0XD8,0X00,0X80,0X81,0X01,0XE0,0X07,0X80,0XAE,0X01,0X80,0X86,0X00,0XF0,0X0F,0X00,0X1D,0X01,0XC0,0XBE,0X00,0XF0,0X0F,0X00,0X4D,0X03,0X40,0X58,0X00,0XF8,0X1F,0X00,0XFA,0X02,0X60,0X29,0X00,0XF8,0X1F,0X00,0XFC,0X06,0X60,0X27,0X00,0XFC,0X3F,0X00,0X54,0X06,0XA0,0X17,0X00,0XFE,0X7F,0X00,0X98,0X04,0XB0,0X17,0X00,0XFE,0X7F,0X00,0XE8,0X0D,0X30,0X14,0X00,0X7F,0XFE,0X00,0XE8,0X0D,0XF0,0X0F,0X00,0XBF,0XFD,0X00,0X40,0X0C,0X10,0X0F,0X80,0XDF,0XFB,0X01,0XD0,0X0B,0X50,0X08,0XC0,0XEF,0XF7,0X01,0XD0,0X0B,0XD0,0X0B,0XC0,0XF7,0XEF,0X03,0X10,0X0A,0X50,0X0B,0XE0,0XFB,0XDF,0X07,0XD0,0X0B,0XD0,0X0B,0XE0,0XFD,0XBF,0X07,0XD0,0X0B,0X10,0X08,0XF0,0XFE,0X7F,0X0F,0X10,0X0A,0XD0,0X0A,0X70,0XFF,0XFF,0X0E,0X10,0X0A,0X50,0X0A,0XB8,0XFF,0XFF,0X1D,0XD0,0X0B,0XD0,0X0B,0XDC,0XFF,0XFF,0X3B,0X90,0X0D,0X30,0X01,0XEC,0XFF,0XFF,0X37,0X08,0X0C,0X30,0X10,0X06,0X10,0X08,0X60,0X08,0X0C,0X20,0X10,0X00,0X7F,0XFF,0X00,0X08,0X04,0X20,0X20,0XFF,0XFF,0XFF,0XFF,0X04,0X04,0X60,0X20,0XFF,0XFF,0XFF,0XFF,0X04,0X06,0X60,0X80,0XFF,0XFF,0XFF,0XFF,0X01,0X02,0XC0,0X80,0X7F,0XC0,0X03,0XFE,0X01,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X80,0X01,0X06,0X00,0X00,0XE0,0X81,0X01,0X80,0XC1,0X03,0X00,0X00,0XE0,0X81,0X01,0X00,0XC3,0X07,0X00,0X00,0XE0,0XC1,0X00,0X00,0XC6,0X23,0X00,0X00,0XC1,0X63,0X00,0X00,0X66,0XF0,0X00,0X00,0XC1,0X63,0X00,0X00,0X0C,0XF8,0X38,0X0C,0X87,0X30,0X00,0X00,0X18,0XFC,0X3C,0X1E,0X03,0X18,0X00,0X00,0X30,0X7C,0X3C,0X0E,0X1E,0X0C,0X00,0X00,0XE0,0X70,0X1C,0X0F,0X0E,0X07,0X00,0X00,0XC0,0X01,0X1E,0X0E,0X83,0X03,0X00,0X00,0X00,0X07,0X18,0X0E,0XE0,0X00,0X00,0X00,0X00,0X0E,0X18,0X06,0X70,0X00,0X00,0X00,0X00,0X78,0X00,0X00,0X1E,0X00,0X00,0X00,0X00,0XE0,0X03,0XC0,0X07,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0XE0,0X07,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};

void testDrawXBM(u8g2_t *u8g2)
{
	int t = 1000;
	u8g2_ClearBuffer(u8g2);
	u8g2_DrawStr(u8g2,0, 14, "DrawXBM");

	u8g2_DrawXBM(u8g2,0, 16, 128, 48, bilibili);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);

	u8g2_ClearBuffer(u8g2);
	u8g2_DrawStr(u8g2,0, 14, "bilibili");
	u8g2_DrawXBM(u8g2,0, 16, 128, 48, three_support);
	SEND_BUFFER_DISPLAY_MS(u8g2,t);
}

void Draw_CJLU(u8g2_t *u8g2)
{
	u8g2_ClearBuffer(u8g2);
	u8g2_DrawXBMP(u8g2,32, 0, 64, 64, CJLU_Logo);
	u8g2_SendBuffer(u8g2);
}

void OLED_DisplayScrollingText(u8g2_t *u8g2,uint8_t columns,const char* text, uint16_t delay_ms) 
{
	uint8_t speed=2;
	
	for (int x = 0; x < 256; x+=speed)
	{
		u8g2_ClearBuffer(u8g2);
		u8g2_DrawStr(u8g2,x,columns,text);
		u8g2_DrawStr(u8g2,x-128,columns,text);
		Tims_delay_ms(delay_ms);
		u8g2_SendBuffer(u8g2);
	}
}
