#ifndef __MAIN_H__
#define __MAIN_H__


#include "stdio.h"
#include "sys/video.h"
#include "proto.h"
#include "common.h"
#include "printk.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "keyboard.h"


#include "heap.h"
#include "sched.h"
#include "timer.h"

#include "scope.h"

extern video_t video;
extern uint16_t x_cursor;
extern uint16_t y_cursor;
int wait(int n);
void init_all();
int rttask();
#endif