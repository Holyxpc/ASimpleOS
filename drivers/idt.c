/*
 * =====================================================================================
 *
 *       Filename:  idt.c
 *
 *    Description:  idt初始化设置
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
#include "../include/isr.h"
#include "../include/idt.h"

GATE_DESCRIPTOR idt[GATE_MAX];
IDTR idtr;

void setupGateDiscriptor(int iIDT, int offset,
  unsigned short selector, unsigned char ar)
{
  idt[iIDT].offsetLow = (unsigned short)(offset & 0x0000FFFF);
  idt[iIDT].selector = selector;
  idt[iIDT].reserved = 0x00;
  idt[iIDT].accsessRight = ar;
  idt[iIDT].offsetHigh = (unsigned short)((offset & 0xFFFF0000) >> 16);
}

void setupInterruptGate(int iIDT, uint32_t int_handler)
{
  setupGateDiscriptor(iIDT,
    int_handler,
    DEF_IDT_INT_SELECTOR,
    DEF_IDT_FLAGS_PRESENT | DEF_IDT_FLAGS_INTGATE_32BIT | DEF_IDT_FLAGS_DPL_LV0);
}

void init_idt(void)
{

  idtr.size = GATE_MAX * sizeof(GATE_DESCRIPTOR);
  idtr.base = (GATE_DESCRIPTOR*)idt;
  load_idt();

  setupInterruptGate(3, (uint32_t)isr3);
  setupInterruptGate(4, (uint32_t)isr4);
  setupInterruptGate(14, (uint32_t)isr14);
  setupInterruptGate(IRQ0, (uint32_t)irq0);
  setupInterruptGate(IRQ1, (uint32_t)irq1);
}
