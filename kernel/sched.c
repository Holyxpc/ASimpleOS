/*
 * =====================================================================================
 *
 *       Filename:  sched.c
 *
 *    Description:  线程调度函数
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */

#include "../include/sched.h"
#include "../include/heap.h"
#include "../include/proto.h"
#include "../include/printk.h"
#include "../include/scope.h"
extern unsigned int stack_end;
extern unsigned int stack_init;
// 可调度进程链表
struct task_struct *running_proc_head = NULL;

// 等待进程链表
struct task_struct *wait_proc_head = NULL;

// 当前运行的任务
struct task_struct *current = NULL;

void init_sched()
{
	// 为当前执行流创建信息结构体 该结构位于当前执行流的栈最低端

	current = (struct task_struct *)kmalloc(STACK_SIZE);

	current->state = TASK_RUNNABLE;
	current->pid = now_pid++;
	current->stack = current; 	// 该成员指向栈低地址
	current->mm = NULL; 		// 内核线程不需要该成员

	current->priority = 1;
	current->count = current->priority;

	// 单向循环链表
	current->next = current;

	running_proc_head = current;
}

void schedule()
{

	if (current) {
		//printk("i am here%d\n",current->count);
		if(current->count>0)
			current->count = current->count-1;
		else{
			current->count = current->priority;
			change_task_to(current->next);	
		}
	scope_draw(current->pid);
	}
}



void change_task_to(struct task_struct *next)
{
	if (current != next) {
		struct task_struct *prev = current;
		current = next;

		switch_to(&(prev->context), &(current->context));
	}

}

