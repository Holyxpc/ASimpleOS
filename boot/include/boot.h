#ifndef __BOOT_H__
#define __BOOT_H__
#include "../../include/type.h"
#define MAX_MEM		32
typedef struct e820entry{
	__u64	addr;
	__u64	size;
	__u32	type;
	}e820entry_t;
	
typedef struct e820map{
	e820entry_t map[MAX_MEM];
	int nr_map;
	}e820map_t;
typedef struct boot_params{
	char	cmd_line[40];
	char	loader_name[40];
	
	unsigned int mem_lower;
	unsigned int mem_upper;

	int biosdev;
	int slice;
	int part;	
	
	int e820entries;
	e820entry_t e820_map[MAX_MEM];
	
}boot_params_t;

#endif

