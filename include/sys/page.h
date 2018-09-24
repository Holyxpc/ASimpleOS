#ifndef __PAGE_H__
#define __PAGE_H__
#include "../type.h"
#include "../list.h"
typedef struct page{

}page_t;


#define MAX_ACTIVE_REGIONS
struct node_active_region{
	u32	start_pfn;
	u32	end_pfn;
	int nid;
	};

typedef struct bootmem_data {  
  unsigned long node_boot_start; // 描述的物理内存的起始地址  
  unsigned long node_low_pfn;    // 结束物理地址，即ZONE_NORMAL的结束  
  void *node_bootmem_map;        // 描述“使用或空闲的位图”的地址  
  unsigned long last_offset;     // 最后被分配的页内偏移量，即在llast_pos描述的物理页中，  
                                 // 从last_offset开始，没有被分配   
  unsigned long last_pos;        // 最后被分配的页的PFN  
  struct list_head list;
} bootmem_data_t; 

#endif

