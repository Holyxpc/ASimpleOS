/*
 * =====================================================================================
 *
 *       Filename:  video.c
 *
 *    Description:  显卡图像模式相关设置
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
 #include "../include/sys/video.h"
#include "../include/proto.h"
#include "../include/common.h"
#include "../include/printk.h"
video_t video;
#define		PAGE_RW		7

extern uint16_t x_cursor;
extern uint16_t y_cursor;
static unsigned char Vga8x16_Font[4096];

void init_palette()
{
	static unsigned char table_rgb[16*3] = {
		0x00, 0x00, 0x00,	/*  0:黑 */
		0xff, 0x00, 0x00,	/*  1:梁红 */
		0x00, 0xff, 0x00,	/*  2:亮绿 */
		0xff, 0xff, 0x00,	/*  3:亮黄 */
		0x00, 0x00, 0xff,	/*  4:亮蓝 */
		0xff, 0x00, 0xff,	/*  5:亮紫 */
		0x00, 0xff, 0xff,	/*  6:浅亮蓝 */
		0xff, 0xff, 0xff,	/*  7:白 */
		0xc6, 0xc6, 0xc6,	/*  8:亮灰 */
		0x84, 0x00, 0x00,	/*  9:暗红 */
		0x00, 0x84, 0x00,	/* 10:暗绿 */
		0x84, 0x84, 0x00,	/* 11:暗黄 */
		0x00, 0x00, 0x84,	/* 12:暗青 */
		0x84, 0x00, 0x84,	/* 13:暗紫 */
		0x00, 0x84, 0x84,	/* 14:浅暗蓝 */
		0x84, 0x84, 0x84	/* 15:暗灰 */
	};
	set_palette(0, 15, table_rgb);
}
void set_palette(int start, int end, unsigned char *rgb)
{
	int i;

	io_out8(0x03c8, start);
	for (i = start; i <= end; i++) {

		io_out8(0x03c9, rgb[0]/4 );
		io_out8(0x03c9, rgb[1]/4 );
		io_out8(0x03c9, rgb[2]/4 );
		rgb += 3;
	}

	return;
}
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++)
			vram[y * xsize + x] = c;
	}
	return;
}

void init_screen8(unsigned char *vram, int x, int y)
{
	boxfill8(vram, x, COL8_008484,  0, 0, x , y );
	return;
}

void init_video(){
	if (video.mode == TEXT_MODE)
		return;
	unsigned long a =
	(unsigned long)(ioremap((unsigned long)video.fb, \
		(unsigned long)(video.width * video.height * (video.color / 8)),\
		PAGE_RW));
	flush_tlb();
	init_palette();
	unsigned char *s = (unsigned char *)video.fb;
	init_screen8(s,video.width,video.height);
}

void putfont8(char *vram, int xsize, int x, int y, char c, unsigned char *font)
{
	int i;
	char *p, d /* data */;
	for (i = 0; i < 16; i++) {
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
	}
	return;
}

void sprint(char *vram, int xsize, int x, int y, char c, unsigned char *s)
{

	/* C语言中，字符串都是以0x00结尾 */
	int len = strlen(s);
	int flag = 0;
	for (; *s != 0x00; s++) {
		if(*s == 0x0a) 
		{
			flag = 1;
			*s = '\0';
			x_cursor = 0;
			y_cursor = y_cursor + 16;
			continue;
		}
		putfont8(vram, xsize, x, y, c,  Vga8x16_Font + *s * 16);
		x += 8;
	}
	if(flag == 0)
	{x_cursor = 8*len + x_cursor;}	
	if( x_cursor > 1016 )
	{
		x_cursor = 0;
		y_cursor = 16  + y_cursor;
	}
	if(y_cursor > 752)
	{
		scroll_screen_down();
	}

	return;
}

