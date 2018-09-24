#ifndef PIC_H
#define PIC_H

#define PIC_MASTER_ICW1    0x0020
#define PIC_MASTER_OCW2    0x0020
#define PIC_MASTER_IMR     0x0021
#define PIC_MASTER_ICW2    0x0021
#define PIC_MASTER_ICW3    0x0021
#define PIC_MASTER_ICW4    0x0021
#define PIC_SLAVE_ICW1     0x00a0
#define PIC_SLAVE_OCW2     0x00a0
#define PIC_SLAVE_IMR      0x00a1
#define PIC_SLAVE_ICW2     0x00a1
#define PIC_SLAVE_ICW3     0x00a1
#define PIC_SLAVE_ICW4     0x00a1

void init_pic(void);
void init_pit(void);

extern int timercount;
extern unsigned char keydata;

#endif
