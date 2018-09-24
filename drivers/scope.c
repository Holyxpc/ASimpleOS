/*
 * =====================================================================================
 *
 *       Filename:  scope.c
 *
 *    Description:  虚拟示波器相关设置
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
#include "../include/scope.h"
#define JIANGE 30
#define JIANGE2 50
extern video_t video;
static int mark = 0;	// 
void dotfill(unsigned char *vram, unsigned char c, int x, int y)
{
	vram[y * XSIZE + x] = c;
	return;
}
void dotfill2(unsigned char *vram, unsigned char c, int x, int y)
{
	vram[y * XSIZE + x] = c;
	vram[y * XSIZE + x+1] = c;
	return;
}

void vLine(unsigned char *vram, unsigned char c, int x, int y1, int y2)
{
	int y = 0;
	for(y = y1; y < y2; y++)
		vram[y * XSIZE + x] = c;
	return;
}

void vNet(int x1, int x2, int y1, int y2, int kuan)
{
	int x = x1;
	for(x = x1; x < x2; x += kuan)
		vLine(VRAM, COL8_848484, x, y1, y2);
	return;
}

void init_osc()
{
	start_row = 30;
	end_row = 353;
	start_col = 50;
	end_col = 974;
	scopeApid = 0;
	scopeBpid = 0;
	scopeCpid = 0;
	y_A = start_row + 40;
	y_B = y_A + JIANGE + JIANGE2;
	y_C = y_B + JIANGE + JIANGE2;
	x_p = start_col + 1;
	y_Int = end_row - JIANGE - 10;
	XSIZE = video.width;
	VRAM = (unsigned char *) video.fb;
	boxfill8(VRAM, video.width, COL8_C6C6C6,  start_col - 40, start_row - 20, end_col + 40 , end_row + 20 );
	putfont8(VRAM, video.width, start_col - 20, y_A + 10, COL8_848484,  Vga8x16_Font + 'A' * 16);
	putfont8(VRAM, video.width, start_col - 20, y_B + 10, COL8_848484,  Vga8x16_Font + 'B' * 16);
	putfont8(VRAM, video.width, start_col - 20, y_C + 10, COL8_848484,  Vga8x16_Font + 'C' * 16);
	putfont8(VRAM, video.width, start_col - 20, y_Int + 10, COL8_848484,  Vga8x16_Font + 'I' * 16);
	boxfill8(VRAM, video.width, COL8_000000,  start_col, start_row, end_col , end_row );
	vNet(start_col + JIANGE, end_col - JIANGE, start_row , end_row, JIANGE);
	// boxfill8(vram, video.width, COL8_FFFFFF,  cross_col, start_row, cross_col , end_row );
	// boxfill8(vram, video.width, COL8_FFFFFF,  start_col, cross_row, end_col, cross_row );

	// putfont8(vram, video.width, cross_col, cross_row, 7,  Vga8x16_Font + '0' * 16);
	// for(temp = cross_col; temp<end_col; temp = temp + 5)
	// {
		
	// }
}

void scopeSet(int A, int B, int C)
{
	scopeApid = A;
	scopeBpid = B;
	scopeCpid = C;
}

void scope_task(int current_pid, int scope_pid, int y, unsigned char c)
{
	// if(current_pid == scope_pid)
	// {
	// 	dotfill2(VRAM, c, x_p, y);
	// }
	// else {
	// 	dotfill2(VRAM, c, x_p, y + JIANGE);
	// }
	if(current_pid == scope_pid)
	{
		dotfill(VRAM, c, x_p, y);
	}
	else {
		dotfill(VRAM, c, x_p, y + JIANGE);
	}
}

void scope_draw(int current_pid)
{
	if(mark)
	{
		mark = 0;	
		return;
	}
	scope_task(current_pid, -1, y_Int, COL8_FFFFFF);
	scope_task(current_pid, scopeApid, y_A, COL8_FFFF00);
	scope_task(current_pid, scopeBpid, y_B, COL8_00FFFF);
	scope_task(current_pid, scopeCpid, y_C, COL8_FF00FF);
	x_p++;
	int i=0;
	int a_1 = 70*1024;
	int a_2 = 100*1024;
	int b_1 = 150*1024;
	int b_2 = 180*1024;
	int c_1 = 230*1024;
	int c_2 = 260*1024;
	int i_1 = 313*1024;
	int i_2 = 343*1024;
	char *vram = video.fb;
	if(x_p >= end_col-1)
	{
		x_p = start_col + 1;
		for(i = 0; i < 1024; i++)
		{
			vram[a_1+i]=vram[a_1+i+1024];
			vram[a_2+i]=vram[a_2+i+1024];
			vram[b_1+i]=vram[b_1+i+1024];
			vram[b_2+i]=vram[b_2+i+1024];			
			vram[c_1+i]=vram[c_1+i+1024];
			vram[c_2+i]=vram[c_2+i+1024];
			vram[i_1+i]=vram[i_1+i+1024];
			vram[i_2+i]=vram[i_2+i+1024];						
		}
		//boxfill8(VRAM, video.width, COL8_000000, start_col, start_row, end_col , end_row);
		//vNet(start_col + JIANGE, end_col - JIANGE, start_row , end_row, JIANGE);
	}
	// scope_task(current_pid, -1, y_Int, COL8_FFFFFF);
	// scope_task(current_pid, scopeApid, y_A, COL8_FFFF00);
	// scope_task(current_pid, scopeBpid, y_B, COL8_00FFFF);
	// scope_task(current_pid, scopeCpid, y_C, COL8_FF00FF);
	// x_p+=2;
	// if(x_p >= end_col-1)
	// {
	// 	x_p = start_col + 1;
	// 	boxfill8(VRAM, video.width, COL8_000000, start_col, start_row, end_col , end_row);
	// 	vNet(start_col + JIANGE, end_col - JIANGE, start_row , end_row, JIANGE);
	// }
}

void scope_keyboard()
{
	// dotfill2(VRAM, COL8_FFFFFF, x_p, y_Int);
	// dotfill2(VRAM, COL8_FFFF00, x_p, y_A+JIANGE);
	// dotfill2(VRAM, COL8_00FFFF, x_p, y_B+JIANGE);
	// dotfill2(VRAM, COL8_FF00FF, x_p, y_C+JIANGE);
	// x_p+=2;
	// if(x_p >= end_col-1)
	// {
	// 	x_p = start_col + 1;
	// 	boxfill8(VRAM, video.width, COL8_000000, start_col, start_row, end_col , end_row);
	// 	vNet(start_col + JIANGE, end_col - JIANGE, start_row , end_row, JIANGE);
	// }
	dotfill(VRAM, COL8_FFFFFF, x_p, y_Int);
	dotfill(VRAM, COL8_FFFF00, x_p, y_A+JIANGE);
	dotfill(VRAM, COL8_00FFFF, x_p, y_B+JIANGE);
	dotfill(VRAM, COL8_FF00FF, x_p, y_C+JIANGE);
	x_p++;

	mark = 1;
}