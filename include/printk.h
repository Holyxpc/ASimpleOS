#ifndef __PRINT_H__
#define __PRINT_H__
#include "type.h"
#include "vargs.h"
#include "string.h"
//printk.c
#define is_digit(c)	((c) >= '0' && (c) <= '9')
#define ZEROPAD		1	// pad with zero
#define SIGN	 	2   	// unsigned/signed long
#define PLUS    	4	// show plus
#define SPACE	  	8   	// space if plus
#define LEFT	 	16  	// left justified
#define SPECIAL		32  	// 0x
#define SMALL	  	64  	// use 'abcdef' instead of 'ABCDEF'


void printk(const char *format, ...);
//console.c


void console_write(unsigned char *cstr);
void scroll_screen_down();
void clear_screen_up();
void clear_screen_down();
void clear_screen_all();	
// struct SHEET {
// 	unsigned char *buf;
// 	int bxsize, bysize, vx0, vy0, col_inv, height, flags;
// 	struct SHTCTL *ctl;
// };
#endif