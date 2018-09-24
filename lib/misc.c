/*
 * =====================================================================================
 *
 *       Filename:  misc.c
 *
 *    Description:  显卡文本模式下的打印相关函数
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
#include "../include/type.h"
#include "../include/stdio.h"

#define COLUMNS				80
#define LINES				24
#define ATTRIBUTE			7
#define VIDEO				0xB8000+0xc0000000
#define __PAGE_OFFSET		0xC000000
static int xpos = 0;
static int ypos = 0;
static volatile unsigned char *video = (unsigned char *)VIDEO;


void cls (void){
	
  int i;
  video = (unsigned char *) VIDEO;
  
  for (i = 0; i < COLUMNS * LINES * 2; i++)
    *(video + i) = 0;

  xpos = 0;
  ypos = 0;
}

void itoa (char *buf, int base, int d){

	char *p = buf;
	char *p1, *p2;
	unsigned long ud = d;
	int divisor = 10;

	if (base == 'd' && d < 0){
		*p++ = '-';
		buf++;
		ud = -d;
	}
	else if (base == 'x')
		divisor = 16;

	do{
		int remainder = ud % divisor;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
	}while (ud /= divisor);

	*p = 0;

	p1 = buf;
	p2 = p - 1;
	while (p1 < p2){
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
}

void putchar (int c){
	
	if (c == '\n' || c == '\r'){	
newline:
		xpos = 0;
		ypos++;
		
		if (ypos >= LINES)
			ypos = 0;
		return;
	}
	*(video + (xpos + ypos * COLUMNS) * 2) = c & 0xFF;
	*(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;
	xpos++;
	
	if (xpos >= COLUMNS)
		goto newline;
}

void printf (const char *format, ...){
	
	char **arg = (char **) &format;
	int c;
	char buf[20];

	arg++;

	while ((c = *format++) != 0){
		
		if (c != '%')
			putchar (c);
		else{
			char *p;
			c = *format++;
			switch (c){
				case 'd':
				case 'u':
				case 'x':
					itoa (buf, c, *((int *) arg++));
					p = buf;
					goto string;
					break;
				case 's':
					p = *arg++;
					if (! p)
						p = "(null)";
					string:
					while (*p)
						putchar (*p++);
					break;
				default:
					putchar (*((int *) arg++));
					break;
				}
		}
    }
}

unsigned sleep(unsigned seconds){
	int i = 0, j = 0, k = 0;
	for(; i < seconds * 0x1000000; ++i){
		for(; j < seconds * 0x100000; ++j){
			for(; k < seconds * 0x100000; ++k)
				;
			}
		}
	return 0;
	}

void *memset1(void *s, int c, size_t count){
	char *xs = s;
	while(count--){
		*xs++ = c;	
	}
	return s;	
}

