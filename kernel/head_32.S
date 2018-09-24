#define __PAGE_OFFSET	0xC0000000
#define GDT_ENTRY_BOOT_CS	2
#define GDT_ENTRY_BOOT_DS	3
#define GDT_ENTRY_BOOT_TSS	4
#define __BOOT_CS		(GDT_ENTRY_BOOT_CS * 8)	
#define __BOOT_DS		(GDT_ENTRY_BOOT_DS * 8)	
#define __BOOT_TSS		(GDT_ENTRY_BOOT_TSS * 8)
#define PAGE_SIZE		(1024 * 4)
.data
	.global gdt_descr, boot_gdt
gdt_descr:
	.word	__BOOT_DS + 7
	.long	boot_gdt - __PAGE_OFFSET

boot_gdt:
	.fill	GDT_ENTRY_BOOT_CS,8,0
	.quad	0x00cf9a000000ffff
	.quad	0x00cf92000000ffff

	.global __page_table_base, max_pfn_mapped, __page_table_end, kernel_size, mem_size, _end
__page_table_base:
	.long	0
__page_table_end:
	.long	0
max_pfn_mapped:
	.long	0
kernel_size:
	.long	0
mem_size:
	.quad	0
_end:
	.long	0

	.global pde_index, flush_tlb
	.align PAGE_SIZE 
pde_index:
	.fill	1024, 4, 0
	
flush_tlb:
	movl	$(pde_index - __PAGE_OFFSET), %eax
	movl	%eax, %cr3
	ret
