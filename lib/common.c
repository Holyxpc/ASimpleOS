/*
 * =====================================================================================
 *
 *       Filename:  common.c
 *
 *    Description:  I/O函数
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
#include "../include/common.h"

unsigned char io_in8(unsigned short address)
{
  unsigned char ret;
  __asm__ __volatile__("inb %1, %0": "=a"(ret) : "Nd"(address));
  return ret;
}
void io_out8(uint16_t port, uint8_t value)
{
	
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
	//return;
}