/*
 * =====================================================================================
 *
 *       Filename:  boot.S
 *
 *    Description:  系统启动引导
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
#define ASM_FILE	1
#include <multiboot2.h>
#define STACK_SIZE			0x4000

# define AOUT_KLUDGE MULTIBOOT_AOUT_KLUDGE
#ifdef HAVE_ASM_USCORE
	#define EXT_C(sym)				_ ## sym
#else
	#define EXT_C(sym)				sym
#endif

#ifdef __ELF__
	#define AOUT_KLUDGE 0
#else
	#define AOUT_KLUDGE MULTIBOOT_AOUT_KLUDGE
#endif

#define __PAGE_OFFSET 		0xC0000000
#define GDT_ENTRY_BOOT_CS	2
#define GDT_ENTRY_BOOT_DS	3
#define GDT_ENTRY_BOOT_TSS	4
#define __BOOT_CS			(GDT_ENTRY_BOOT_CS * 8)	
#define __BOOT_DS			(GDT_ENTRY_BOOT_DS * 8)	
#define __BOOT_TSS			(GDT_ENTRY_BOOT_TSS * 8)
#define	PDE_IDENT_ATTR 		0x63
#define page_pde_offset 	(__PAGE_OFFSET >> 20)


.section ".text.head", "ax"
	.globl start, _start, map_size, memory_size ,stack_init
start:
_start:
	jmp multiboot_entry

	/* Align 64 bits boundary. */
	.align 8
	
	/* Multiboot header. */
	
multiboot_header:
	/* magic */
	.long	MULTIBOOT2_HEADER_MAGIC
	/* ISA: i386 */
	.long	MULTIBOOT_ARCHITECTURE_I386
	/* Header length. */
	.long	multiboot_header_end - multiboot_header
	/* checksum */
	.long   -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (multiboot_header_end - multiboot_header))

#if	1
framebuffer_tag_start:
	.short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
	.short MULTIBOOT_HEADER_TAG_OPTIONAL
	.long framebuffer_tag_end - framebuffer_tag_start
	.long 1024
	.long 768
	.long 8
	.long 0
#endif
framebuffer_tag_end:
	.short MULTIBOOT_HEADER_TAG_END
	.short 0
	.long 8

multiboot_header_end:


multiboot_entry:
	/* Initialize the stack pointer. */
	
	movl $(stack_init), %esp
	/* Reset EFLAGS. */

	pushl	$0
	popf

	pushl	%eax
	
	lgdt 	gdt_descr - __PAGE_OFFSET
	mov 	$(__BOOT_DS), %eax
	mov	%eax, %ds	
	mov	%eax, %es	
	mov	%eax, %fs	
	mov	%eax, %gs	
	mov	%eax, %ss	
	ljmp	$(__BOOT_CS),$1f
//页表初始化
1:	
	movl	$(__page_table_base - __PAGE_OFFSET), %edi
	movl 	$(pde_index - __PAGE_OFFSET), %edx
	movl	$0x7, %eax
2:
	leal	PDE_IDENT_ATTR(%edi), %ecx 		//为了算出页全局目录项里应该被放入什么值
	movl	%ecx, (%edx) 					//存入对应的页全局目录项里面的0,1
	movl	%ecx, page_pde_offset(%edx) 	//存入对应的页全局目录项
	addl	$4, %edx 						//下一个页表项的地址
	movl	$1024, %ecx 					//每个页表有1024项
3:	
	stosl 									//存到页表里edi指向的方向
	addl	$0x1000, %eax 
	loop	3b 								//循环1024次

	movl	map_size, %ebp
	cmpl	%ebp, %eax
	jb		2b
	addl	$__PAGE_OFFSET, %edi
	movl	%edi, (__page_table_end - __PAGE_OFFSET)
	shrl	$12, %eax
	movl	%eax, (max_pfn_mapped - __PAGE_OFFSET)

// 建立完页表，启用分页机制
	movl	$(pde_index - __PAGE_OFFSET), %eax
	movl	%eax, %cr3
	movl	%cr0, %eax
	orl	$0x80000000, %eax
	movl	%eax, %cr0
	
	ljmp	$__BOOT_CS,$setup_page_ok
setup_page_ok:
	pop	%eax
	pushl	%ebx
	pushl	%eax
	
	call	EXT_C(copy_boot_params)
	addl	$8,   %esp
	
	call	start32

map_size:
	#.long (896 << 20)
	.long (1024 << 20)
	.align (1 << 10)
	
	.comm	stack, STACK_SIZE
	.align (4 << 10)
stack_init:
