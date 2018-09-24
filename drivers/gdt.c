/*
 * =====================================================================================
 *
 *       Filename:  gdt.c
 *
 *    Description:  gdt初始化设置
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
#include "../include/gdt.h"
SEGMENT_DESCRIPTOR gdt[GDTMAX];
GDTR gdtr;

void load_gdt(void)
{
  __asm__ __volatile__("lgdt gdtr");
  __asm__ __volatile__("mov $0x10, %ax");
  __asm__ __volatile__("mov %ax, %ds");
  __asm__ __volatile__("mov %ax, %es");
  __asm__ __volatile__("mov %ax, %fs");
  __asm__ __volatile__("mov %ax, %gs");
  __asm__ __volatile__("mov %ax, %ss");
  
  __asm__ __volatile__("ljmp $0x08, $_flush_seg");
  __asm__ __volatile__("_flush_seg:");
}

void init_gdt(void)
{
  //
  gdt[NULL_DESCRIPTOR].limitLow = DEF_GDT_NULL_LIMIT;
  gdt[NULL_DESCRIPTOR].baseLow = DEF_GDT_NULL_BASELO;
  gdt[NULL_DESCRIPTOR].baseMid = DEF_GDT_NULL_BASEMID;
  gdt[NULL_DESCRIPTOR].accessRight = DEF_GDT_NULL_FLAGS;
  gdt[NULL_DESCRIPTOR].baseHigh = DEF_GDT_NULL_BASEHI;

  //code
  gdt[CODE_DESCRIPTOR].limitLow = DEF_GDT_CODE_LIMIT;
  gdt[CODE_DESCRIPTOR].baseLow = DEF_GDT_CODE_BASELO;
  gdt[CODE_DESCRIPTOR].baseMid = DEF_GDT_CODE_BASEMID;
  gdt[CODE_DESCRIPTOR].accessRight = DEF_GDT_CODE_FLAGS;
  gdt[CODE_DESCRIPTOR].baseHigh = DEF_GDT_CODE_BASEHI;

  //data
  gdt[DATA_DESCRIPTOR].limitLow = DEF_GDT_DATA_LIMIT;
  gdt[DATA_DESCRIPTOR].baseLow = DEF_GDT_DATA_BASELO;
  gdt[DATA_DESCRIPTOR].baseMid = DEF_GDT_DATA_BASEMID;
  gdt[DATA_DESCRIPTOR].accessRight = DEF_GDT_DATA_FLAGS;
  gdt[DATA_DESCRIPTOR].baseHigh = DEF_GDT_DATA_BASEHI;

  //code_user
  gdt[CODE_U_DESCRIPTOR].limitLow = DEF_GDT_U_CODE_LIMIT;
  gdt[CODE_U_DESCRIPTOR].baseLow = DEF_GDT_U_CODE_BASELO;
  gdt[CODE_U_DESCRIPTOR].baseMid = DEF_GDT_U_CODE_BASEMID;
  gdt[CODE_U_DESCRIPTOR].accessRight = DEF_GDT_U_CODE_FLAGS;
  gdt[CODE_U_DESCRIPTOR].baseHigh = DEF_GDT_U_CODE_BASEHI;

  //data_user
  gdt[DATA_U_DESCRIPTOR].limitLow = DEF_GDT_U_DATA_LIMIT;
  gdt[DATA_U_DESCRIPTOR].baseLow = DEF_GDT_U_DATA_BASELO;
  gdt[DATA_U_DESCRIPTOR].baseMid = DEF_GDT_U_DATA_BASEMID;
  gdt[DATA_U_DESCRIPTOR].accessRight = DEF_GDT_U_DATA_FLAGS;
  gdt[DATA_U_DESCRIPTOR].baseHigh = DEF_GDT_U_DATA_BASEHI;

  gdtr.size = GDTMAX * sizeof(SEGMENT_DESCRIPTOR);
  gdtr.base = (SEGMENT_DESCRIPTOR*)gdt;

  load_gdt();
}
