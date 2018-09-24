#ifndef __PROTO_H__
#define __PROTO_H__
#include "type.h"
#include "printk.h"
#include "stdio.h"

/* mm/memory.c */
#define assert(x, info)                                       	\
	do {                                                	\
		if (!(x)) {                                     \
			printf(info);     			\
		}                                               \
	} while (0)
void setup_memory();
uint32_t pmm_alloc_page();
// 线程栈的大小
#define STACK_SIZE 0x4000
// 支持的最大物理内存(512MB)
#define PMM_MAX_SIZE 0x40000000
// 物理内存页框大小 
#define PMM_PAGE_SIZE 0x1000
// 最多支持的物理页面个数
#define PAGE_MAX_SIZE (PMM_MAX_SIZE/PMM_PAGE_SIZE)
// 页掩码 按照 4096 对齐地址
#define PHY_PAGE_MASK 0xFFFFF000
extern uint32_t phy_page_count;


/* lib/misc.c */
void *memset1(void *s, int c, size_t count);
/* head32.S */
void flush_tlb();
/* mm/ioremap.c */
// 页目录数据类型
typedef uint32_t pgd_t;

// 页表数据类型
typedef uint32_t pte_t;
unsigned int ioremap(unsigned long phys_addr, unsigned long size, unsigned long flags);
uint32_t get_mapping(pgd_t *pgd_now, uint32_t va, uint32_t *pa);
void unmap(pgd_t *pgd_now, uint32_t va);
void map(pgd_t *pgd_now, uint32_t va, uint32_t pa, uint32_t flags);

/**
 * P--位0是存在（Present）标志，用于指明表项对地址转换是否有效。P=1表示有效；P=0表示无效。
 * 在页转换过程中，如果说涉及的页目录或页表的表项无效，则会导致一个异常。
 * 如果P=0，那么除表示表项无效外，其余位可供程序自由使用。
 * 例如，操作系统可以使用这些位来保存已存储在磁盘上的页面的序号。
 */
#define PAGE_PRESENT 	0x1
/** 
 * R/W--位1是读/写（Read/Write）标志。如果等于1，表示页面可以被读、写或执行。
 * 如果为0，表示页面只读或可执行。
 * 当处理器运行在超级用户特权级（级别0、1或2）时，则R/W位不起作用。
 * 页目录项中的R/W位对其所映射的所有页面起作用。
 */
#define PAGE_WRITE 	0x2
/**
 * U/S--位2是用户/超级用户（User/Supervisor）标志。
 * 如果为1，那么运行在任何特权级上的程序都可以访问该页面。
 * 如果为0，那么页面只能被运行在超级用户特权级（0、1或2）上的程序访问。
 * 页目录项中的U/S位对其所映射的所有页面起作用。
 */
#define PAGE_USER 	0x4
// 虚拟分页大小
#define PAGE_SIZE 	4096
// 页掩码，用于 4KB 对齐
#define PAGE_MASK      0xFFFFF000
// 获取一个地址的页目录项
#define PGD_INDEX(x) (((x) >> 22) & 0x3FF)
// 获取一个地址的页表项
#define PTE_INDEX(x) (((x) >> 12) & 0x3FF)
// 获取一个地址的页內偏移
#define OFFSET_INDEX(x) ((x) & 0xFFF)
// 页表成员数
#define PGD_SIZE (PAGE_SIZE/sizeof(pte_t))
// 页表成员数
#define PTE_SIZE (PAGE_SIZE/sizeof(uint32_t))
// 映射 512MB 内存所需要的页表数
#define PTE_COUNT 256
// 内核页目录区域
extern pgd_t pgd_kern[PGD_SIZE];
#define PAGE_OFFSET 0xc0000000
void init_vmm();
/* video/video.c */
#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15
void set_palette(int start, int end, unsigned char *rgb);
void init_palette();
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen8(unsigned char *vram, int x, int y);
void putfont8(char *vram, int xsize, int x, int y, char c, unsigned char *font);
void sprint(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_video();
void init_osc();

#endif

