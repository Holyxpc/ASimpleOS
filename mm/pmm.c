/*
 * =====================================================================================
 *
 *       Filename:  pmm.c
 *
 *    Description:  页内存管理
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
#include "../boot/include/multiboot2.h"
#include "../include/sys/page.h"
#include "../include/proto.h"
#include "../boot/include/boot.h"
#include "../include/unistd.h"
#include "../include/type.h"
#include "../include/stdio.h"
#include "../include/printk.h"

boot_params_t boot_params;
e820map_t	e820;
extern int __page_table_base; 	//内核结束地址
// 物理内存页面管理的栈
static uint32_t pmm_stack[PAGE_MAX_SIZE+1];

// 物理内存管理的栈指针
static uint32_t pmm_stack_top;
// 物理内存页的数量
uint32_t phy_page_count;

bootmem_data_t bdata_list[4];

page_t *mem_map = (page_t *)((char *)&__page_table_base + 0x100000);
const char *str[] = {"usable", "reserved", "ACPI NVS"};
unsigned char *boot_alloc;

void show_boot_params(){
	printk ("Command line = %s\n", boot_params.cmd_line);
	printk ("Boot loader name = %s\n", boot_params.loader_name);
	printk ("mem_lower = %uKB, mem_upper = %uKB\n",boot_params.mem_lower,boot_params.mem_upper);
	printk ("Boot device 0x%x,%u,%u\n",boot_params.biosdev,boot_params.slice,boot_params.part);
	int i = 0;
	
	
	printk("BIOS-provided physical RAM map:\n");

	for(; i < e820.nr_map; ++i){
		//sleep(3);

		printk ("BIOS-e820 0x%x%08x - "
		"0x%x%08x (%s)\n",
		(unsigned) (e820.map[i].addr >> 32),
		(unsigned) (e820.map[i].addr & 0xffffffff),
		(unsigned) (e820.map[i].size >> 32),
		(unsigned) (e820.map[i].size & 0xffffffff),
		str[(unsigned) e820.map[i].type - 1]);		
	}	
}

u32 calc_mem_map_size(){
		return (boot_params.mem_upper + (0x1000 - 1)) >> 12;
}

void init_boot_alloc(){
		boot_alloc = (unsigned char *)((char *)mem_map + (calc_mem_map_size() << 12));
		//printk("boot_alloc addr: %x\n", boot_alloc);
		memset1(boot_alloc, 0xff, (1 << 12));
}

void rm_rept_map(){
		int i = 0;
		while(i < boot_params.e820entries){
			u64 start = boot_params.e820_map[i].addr;
			u64 size = boot_params.e820_map[i].size;
			int type = boot_params.e820_map[i].type;
			u64 end = start + size;
			
			if(start > end)
				break;
			
			if((i + 1 != boot_params.e820entries) && (boot_params.e820_map[i + 1].addr < end))
				break;
				
			e820.map[i].addr = start;
			e820.map[i].size = size;
			e820.map[i].type = type;
			++i;
			}
		e820.nr_map = i;
		printf("e820.nr_map:%x\n",e820.nr_map);
}

void pmm_free_page(uint32_t p)
{
	assert(pmm_stack_top != PAGE_MAX_SIZE, "out of pmm_stack stack");

	pmm_stack[++pmm_stack_top] = p;
}

uint32_t pmm_alloc_page()
{
	//printf("0x%x",pmm_stack_top);
	assert(pmm_stack_top != 0, "out of memory");

	uint32_t page = pmm_stack[pmm_stack_top--];

	return page;
}

void init_mem()
{
	int i=0;
	
	for(; i < e820.nr_map; ++i)
	{
		if(e820.map[i].type == 1 && e820.map[i].addr == 0x100000) 
		{
			uint32_t page_addr = __page_table_base;
			uint32_t length =e820.map[i].addr+e820.map[i].size;
			printf("length0x%x",length);
			while(page_addr < length && page_addr <= PMM_MAX_SIZE)
			{
				pmm_free_page(page_addr);
				page_addr += PMM_PAGE_SIZE;
				phy_page_count++;
			}
		}
	}
}

void setup_memory()
{
	rm_rept_map();
	//show_boot_params();
	//printf("mem_map addr: %x\n", mem_map);   
	//printk("mem_map need %x page\n", calc_mem_map_size());
	init_boot_alloc();
	init_mem();
}
