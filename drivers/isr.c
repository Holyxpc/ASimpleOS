#include "../include/common.h"
#include "../include/isr.h"
#include "../include/pic.h"
isr_t int_handlers[256];

void register_int_handler(uint8_t n, void* handler)
{
  int_handlers[n] = handler;
}

void isr_handler(regs_t regs)
{
  if(int_handlers[regs.int_no] != 0)
  {
    isr_t handler = int_handlers[regs.int_no];
    handler(regs);
  }else{
    printf("[*]unhandled int:%d\n", regs.int_no);
    
 
    __asm__ __volatile__("nop");
  }
}

void irq_handler(regs_t regs)
{
  if(regs.int_no >= 40)
  {
    io_out8(PIC_SLAVE_ICW1, 0x20);
  }
  io_out8(PIC_MASTER_ICW1, 0x20);

  if(int_handlers[regs.int_no] != 0)
  {
    isr_t handler = int_handlers[regs.int_no];
    handler(regs);
  }
}
