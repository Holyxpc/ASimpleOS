/*
 * =====================================================================================
 *
 *       Filename:  pic.c
 *
 *    Description:  pic设置
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
#include "../include/pic.h"
#include "../include/common.h"
#include "../include/isr.h"

void timer_interrupt(void);


void init_pic(void)
/* PIC初始化 */
{
  io_out8(PIC_MASTER_IMR, 0xff); /* 禁用主片 */
  io_out8(PIC_SLAVE_IMR, 0xff); /* 禁用从片 */

  io_out8(PIC_MASTER_ICW1, 0x11); /* 初始化主片 */
  io_out8(PIC_MASTER_ICW2, 0x20); /* 设置主片从IRQ从0x20号中断开始 */
  io_out8(PIC_MASTER_ICW3, 0x04); /* 设置主片IR2引脚链接从片*/
  io_out8(PIC_MASTER_ICW4, 0x01); /* 设置主片按8086方式工作 */

  io_out8(PIC_SLAVE_ICW1, 0x11); /* 初始化从片 */
  io_out8(PIC_SLAVE_ICW2, 0x28); /* 设置从片从IRQ从0x28号中断开始 */
  io_out8(PIC_SLAVE_ICW3, 0x02); /* 告诉从片输出引脚和主片IR2相连*/
  io_out8(PIC_SLAVE_ICW4, 0x01); /* 设置从片按8086方式工作 */

  io_out8(PIC_MASTER_IMR,  0xf8  ); /* 11111000 使能时钟、键盘、从片2 */
  io_out8(PIC_SLAVE_IMR,  0xff  ); /* 11111111 禁止中断 */


  return;
}

