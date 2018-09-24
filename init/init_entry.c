/*
 * =====================================================================================
 *
 *       Filename:  init_entry.c
 *
 *    Description:  内核初始化相关函数
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
#include "../include/main.h"

int wait(int n)
{
	int m = 10000;

	while(n)
	{
		while(m)
		{
			m--;
		}
		m = 10000;
		n--;
	}
	return 0;
}
void init_all()
{
	io_cli();
	init_video();
	printk("\n");
	printk("\n");
	printk("\n");
	printk("\n");
	printk("          System is starting...\n\n");
	wait(5000);
	printk("          video parameters: \n\n");	
	printk("          video.fb: 0x%X\n\n", (unsigned long)video.fb);
	printk("          video.width: 0x%X\n\n", (unsigned long)video.width);
	printk("          video.height: 0x%X\n\n", (unsigned long)video.height);
	printk("          video.color: 0x%X\n\n", (unsigned long)video.color);
	//wait(5000);
	init_gdt();	
	printk("          GDT is prepared.            OK\n\n");
	//wait(5000);
	init_idt();	
	printk("          IDT is prepared.            OK\n\n");
	//wait(5000);
	init_keyboard();
	printk("          Keyboard is prepared.       OK\n\n");	
	//wait(5000);
	init_pic();
	printk("          PIC is prepared.            OK\n\n");	
	//wait(5000);	
	setup_memory();
	printk("          Physical memory is prepared.OK\n\n");
	//wait(5000);
	init_vmm();
	printk("          Virtual memory is prepared. OK\n\n");
	//wait(5000);
	init_heap();
	printk("          Heap memory is prepared.    OK\n\n");
	//wait(5000);
	//test_heap();
	init_timer(200);
	printk("          Timer is prepared.          OK\n\n");
	//wait(5000);
	io_sti();
	printk("          INT is prepared.            OK\n\n");
	wait(10000);

	clear_screen_all();
	wait(5000);
	printk("\n");
	printk("\n");
	printk("\n");
	printk("\n");
	printk("          WELCOME!\n\n");
	wait(10000);
	init_osc();
	clear_screen_down();
	init_sched();
}