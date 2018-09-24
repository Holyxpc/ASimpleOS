/*
 * =====================================================================================
 *
 *       Filename:  console.c
 *
 *    Description:  屏幕操作的一些实现
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
#include "../include/printk.h"
#include "../include/common.h"
#include "../include/proto.h"
#include "../include/sys/video.h"

extern video_t video;

//全屏写满128*48个字符
uint16_t x_cursor=0 * 8;
uint16_t y_cursor=0 * 16;

void console_write(unsigned char *cstr)
{
	char *vram = (char *)video.fb;
	sprint(vram, video.width, x_cursor, y_cursor, COL8_FFFFFF, cstr);
}

void clear_screen_up()
{
	char *vram = (char *)video.fb;
	boxfill8(vram, video.width, COL8_008484,  0, 0, 1024, 383  );
}

void clear_screen_down()
{
	char *vram = (char *)video.fb;
	boxfill8(vram, video.width, COL8_008484,  0, 384, 1024, 768  );
	x_cursor=0 * 8;
	y_cursor=24 * 16;
}

void clear_screen_all()
{
	char *vram = (char *)video.fb;
	boxfill8(vram, video.width, COL8_008484,  0, 0, 1024, 768  );
	x_cursor=0 * 8;
	y_cursor=0 * 16;
}


void scroll_screen_down()
{
	int start_pos = 1024*24*16;
	int end_pos = 1024*48*16;
	int next_pos = 1024*25*16;
	char *vram = (char *)video.fb;
	while(next_pos<end_pos)
	{
		vram[start_pos++]=vram[next_pos++];
	}
	boxfill8(vram, video.width, COL8_008484,  0, 47 * 16, 1024, 768  );
	x_cursor=0 * 8;
	y_cursor=47 * 16;
	// boxfill8(vram, video.width, COL8_008484,  0, 24 * 16, 1024, 768  );
	// x_cursor=0 * 8;
	// y_cursor=24 * 16;
}