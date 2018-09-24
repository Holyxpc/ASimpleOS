/*
 * =====================================================================================
 *
 *       Filename:  keyboard.c
 *
 *    Description:  键盘中断初始化以及服务子程序
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
#include "../include/keyboard.h"
#include "../include/isr.h"
#include "../include/printk.h"
#include "../include/stdio.h"
#include "../include/main.h"
void keyboard_callback(void *regs)
{
	int data;

	io_out8(0x61, PIC0_OCW2); /* 把IRQ-01接收信号结束的信息通知给PIC */
	data = io_in8(PORT_KEYDAT);
	clear_screen_down();
	printk("hello: %d\n",data);
	scope_keyboard();
	kernel_thread(rttask,10,NULL);
	//printf("hello: %d\n",data);
	return;
}

void init_keyboard()
{
	// 注册时间相关的处理函数
	register_int_handler(IRQ1, keyboard_callback);
}
