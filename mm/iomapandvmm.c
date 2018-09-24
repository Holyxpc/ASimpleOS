/*
 * =====================================================================================
 *
 *       Filename:  iomapandvmm.c
 *
 *    Description:  I/O映射和虚拟内存管理
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
#include "../include/type.h"
#include "../include/sys/global.h"
#include "../include/proto.h"
#include "../include/stdio.h"
#include "../include/idt.h"
#include "../include/isr.h"
#include "../include/sys/video.h"
#define ALIGN_BIT(addr, bit) ((addr) & ~((1 << (bit)) - 1))
extern video_t video;
unsigned int *pde_table = &pde_index;
unsigned int ioremap(unsigned long phys_addr, unsigned long size, unsigned long flags){

	int i;
	int pde_index = phys_addr >> 22;

	unsigned int *pte_table = (unsigned int *)ALIGN_BIT(pde_table[pde_index], 12);

	int pte_index = (int)((phys_addr >> 12) & 0x3ff);
	unsigned int *data = (unsigned int *)&pte_table[pte_index];

	for(i = 0; i < (size / (1 << 12)); ++i){
		if(pte_index + i >= 1024)
			break;
		*(data + i) = ((ALIGN_BIT(phys_addr, 12) + (1 << 12) * i) | flags);
	}
	return pde_index;	
}


// 内核页目录区域
pgd_t pgd_kern[PGD_SIZE] __attribute__ ((aligned(PAGE_SIZE)));
// 内核页表区域
static pte_t pte_kern[PTE_COUNT][PTE_SIZE] __attribute__ ((aligned(PAGE_SIZE)));
void switch_pgd(uint32_t pd)
{
	asm volatile ("movl %0, %%cr3" :: "r" (pd));
}

void map(pgd_t *pgd_now, uint32_t va, uint32_t pa, uint32_t flags)
{ 	
	uint32_t pgd_idx = PGD_INDEX(va);
	uint32_t pte_idx = PTE_INDEX(va);

	pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);
	if (!pte) {

		pte = (pte_t *)pmm_alloc_page();
		pgd_now[pgd_idx] = (uint32_t)pte | PAGE_PRESENT | PAGE_WRITE;

		// 转换到内核线性地址并清 0
		pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);
		bzero(pte, PAGE_SIZE);

	} else {
		// 转换到内核线性地址
		pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);
	}

	pte[pte_idx] = (pa & PAGE_MASK) | flags;


	// 通知 CPU 更新页表缓存
	asm volatile ("invlpg (%0)" : : "a" (va));

}


void unmap(pgd_t *pgd_now, uint32_t va)
{
	uint32_t pgd_idx = PGD_INDEX(va);
	uint32_t pte_idx = PTE_INDEX(va);

	pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);

	if (!pte) {
		return;
	}

	// 转换到内核线性地址
	pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);

	pte[pte_idx] = 0;

	// 通知 CPU 更新页表缓存
	asm volatile ("invlpg (%0)" : : "a" (va));
}

uint32_t get_mapping(pgd_t *pgd_now, uint32_t va, uint32_t *pa)
{
	uint32_t pgd_idx = PGD_INDEX(va);
	uint32_t pte_idx = PTE_INDEX(va);

	pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);
	if (!pte) {
	      return 0;
	}
	
	// 转换到内核线性地址
	pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);

	// 如果地址有效而且指针不为NULL，则返回地址
	if (pte[pte_idx] != 0 && pa) {
		 *pa = pte[pte_idx] & PAGE_MASK;
		return 1;
	}

	return 0;
}
void page_fault(pt_regs *regs)
{
	uint32_t cr2;
	asm volatile ("mov %%cr2, %0" : "=r" (cr2));

	printk("Page fault at EIP: 0x%x, virtual faulting address 0x%x\n", regs->eip, cr2);
	printk("Error code: %x\n", regs->err_code);

	// bit 0 为 0 指页面不存在内存里
	if ( !(regs->err_code & 0x1)) {
		printk("Because the page wasn't present.\n");
	}
	// bit 1 为 0 表示读错误，为 1 为写错误
	if (regs->err_code & 0x2) {
		printk("Write error.\n");
	} else {
		printk("Read error.\n");
	}
	// bit 2 为 1 表示在用户模式打断的，为 0 是在内核模式打断的
	if (regs->err_code & 0x4) {
		printk("In user mode.\n");
	} else {
		printk("In kernel mode.\n");
	}
	// bit 3 为 1 表示错误是由保留位覆盖造成的
	if (regs->err_code & 0x8) {
		printk("Reserved bits being overwritten.\n");
	}
	// bit 4 为 1 表示错误发生在取指令的时候
	if (regs->err_code & 0x10) {
		printk("The fault occurred during an instruction fetch.\n");
	}

	while (1);
}
void init_vmm()
{
	// 0xC0000000 这个地址在页目录的索引
	uint32_t kern_pte_first_idx = PGD_INDEX(PAGE_OFFSET);
	
	uint32_t i, j;
	for (i = kern_pte_first_idx, j = 0; i < PTE_COUNT + kern_pte_first_idx; i++, j++) {
		// 此处是内核虚拟地址，MMU 需要物理地址，所以减去偏移，下同
		pgd_kern[i] = ((uint32_t)pte_kern[j] - PAGE_OFFSET) | PAGE_PRESENT | PAGE_WRITE;
	}

	uint32_t *pte = (uint32_t *)pte_kern;
	// 不映射第 0 页，便于跟踪 NULL 指针
	for (i = 1; i < PTE_COUNT * PTE_SIZE; i++) {
		pte[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
	}

	uint32_t pgd_kern_phy_addr = (uint32_t)pgd_kern - PAGE_OFFSET;

	// 注册页错误中断的处理函数 ( 14 是页故障的中断号 )

	register_int_handler(14, &page_fault);
	//register_interrupt_handler(14, &page_fault);

//将旧地址用新地址代替
	unsigned int *pde_table = (unsigned int *)pgd_kern_phy_addr;
	unsigned int phys_addr = (unsigned int)video.fb;
	//printk("phys_addr:%x\n",phys_addr);
	int pde_index = phys_addr >> 22;

	unsigned int *pte_table = (unsigned int *)ALIGN_BIT(pde_table[pde_index], 12);
	int pte_index = (int)((phys_addr >> 12) & 0x3ff);
	unsigned int *data = (unsigned int *)&pte_table[pte_index];
	//printk("data!!!!!!!!!!!!!:%x\n",(unsigned int)data);
	unsigned long ii;
	for(ii = 0; ii < ((1024*768)<<12); ++ii){
		if(pte_index + ii >= 1024)
			break;
		*(data + ii) = ((ALIGN_BIT(phys_addr, 12) + (1 << 12) * ii) | 7);
	}

	switch_pgd(pgd_kern_phy_addr);

	
}
