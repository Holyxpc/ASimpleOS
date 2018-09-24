#ifndef IDT_H
#define IDT_H

#include "../include/common.h"
#define	DEF_IDT_FLAGS_INTGATE_16BIT		0x06
#define	DEF_IDT_FLAGS_TSKGATE			0x05
#define	DEF_IDT_FLAGS_CALL_GATE			0x0C
#define	DEF_IDT_FLAGS_INTGATE_32BIT		0x0E
#define	DEF_IDT_FLAGS_TRPGATE			0x0F
#define	DEF_IDT_FLAGS_DPL_LV0			0x00
#define	DEF_IDT_FLAGS_DPL_LV1			0x20
#define	DEF_IDT_FLAGS_DPL_LV2			0x40
#define	DEF_IDT_FLAGS_DPL_LV3			0x60
#define	DEF_IDT_FLAGS_PRESENT			0x80

#define	DEF_IDT_INT_SELECTOR			0x08

#define GATE_MAX 256

#define load_idt() ({__asm__ __volatile__("lidt idtr");})

void init_idt(void);

typedef struct{
  unsigned short offsetLow, selector;
  unsigned char reserved, accsessRight;
  unsigned short offsetHigh;
} __attribute__((packed))GATE_DESCRIPTOR;

typedef struct{
  unsigned short size;
  GATE_DESCRIPTOR* base;
} __attribute__((packed))IDTR;
// 寄存器类型
typedef
struct pt_regs_t {
	uint32_t ds;		// 用于保存用户的数据段描述符
	uint32_t edi; 		// 从 edi 到 eax 由 pusha 指令压入
	uint32_t esi; 
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t int_no; 	// 中断号
	uint32_t err_code;  	// 错误代码(有中断错误代码的中断会由CPU压入)
	uint32_t eip;		// 以下由处理器自动压入
	uint32_t cs; 		
	uint32_t eflags;
	uint32_t useresp;
	uint32_t ss;
} pt_regs;
void isr0(); 		// 0 #DE 除 0 异常 
void isr1(); 		// 1 #DB 调试异常 
void isr2(); 		// 2 NMI 
void isr3(); 		// 3 BP 断点异常 
void isr4(); 		// 4 #OF 溢出 
void isr5(); 		// 5 #BR 对数组的引用超出边界 
void isr6(); 		// 6 #UD 无效或未定义的操作码 
void isr7(); 		// 7 #NM 设备不可用(无数学协处理器) 
void isr8(); 		// 8 #DF 双重故障(有错误代码) 
void isr9(); 		// 9 协处理器跨段操作 
void isr10(); 		// 10 #TS 无效TSS(有错误代码) 
void isr11(); 		// 11 #NP 段不存在(有错误代码) 
void isr12(); 		// 12 #SS 栈错误(有错误代码) 
void isr13(); 		// 13 #GP 常规保护(有错误代码) 
void isr14(); 		// 14 #PF 页故障(有错误代码) 
void isr15(); 		// 15 CPU 保留 
void isr16(); 		// 16 #MF 浮点处理单元错误 
void isr17(); 		// 17 #AC 对齐检查 
void isr18(); 		// 18 #MC 机器检查 
void isr19(); 		// 19 #XM SIMD(单指令多数据)浮点异常

// 20-31 Intel 保留
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();
// 声明 IRQ 函数
// IRQ:中断请求(Interrupt Request)
void irq0();		// 电脑系统计时器
void irq1(); 		// 键盘
void irq2(); 		// 与 IRQ9 相接，MPU-401 MD 使用
void irq3(); 		// 串口设备
void irq4(); 		// 串口设备
void irq5(); 		// 建议声卡使用
void irq6(); 		// 软驱传输控制使用
void irq7(); 		// 打印机传输控制使用
void irq8(); 		// 即时时钟
void irq9(); 		// 与 IRQ2 相接，可设定给其他硬件
void irq10(); 		// 建议网卡使用
void irq11(); 		// 建议 AGP 显卡使用
void irq12(); 		// 接 PS/2 鼠标，也可设定给其他硬件
void irq13(); 		// 协处理器使用
void irq14(); 		// IDE0 传输控制使用
void irq15(); 		// IDE1 传输控制使用
#endif
