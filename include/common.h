#ifndef _COMMON_H_
#define _COMMON_H_
#include "type.h"
unsigned char io_in8(unsigned short address);
void io_out8(uint16_t port, uint8_t value);
extern void io_hlt(void);
extern void io_sti(void);
extern void io_cli(void);
#endif