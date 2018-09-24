#ifndef GDT_H
#define GDT_H

#define	NULL_DESCRIPTOR 0
#define	CODE_DESCRIPTOR 1
#define	DATA_DESCRIPTOR 2
#define	CODE_U_DESCRIPTOR 3
#define	DATA_U_DESCRIPTOR 4
#define	TASK_CODE_DESCRIPTOR		3
#define	TASK_DATA_DESCRIPTOR		4
#define	KTSS_DESCRIPTOR			5

/* Null Descriptor		*/
#define	DEF_GDT_NULL_LIMIT		0x0000
#define	DEF_GDT_NULL_BASELO		0x0000
#define	DEF_GDT_NULL_BASEMID		0x00
#define	DEF_GDT_NULL_FLAGS		0x0000
#define	DEF_GDT_NULL_BASEHI		0x00

/* Code Descriptor		*/
#define	DEF_GDT_CODE_LIMIT		0xFFFF
#define	DEF_GDT_CODE_BASELO		0x0000
#define	DEF_GDT_CODE_BASEMID		0x00
#define	DEF_GDT_CODE_FLAGS		0xCF9A
#define	DEF_GDT_CODE_BASEHI		0x00

/* Data Descriptor		*/
#define	DEF_GDT_DATA_LIMIT		0xFFFF
#define	DEF_GDT_DATA_BASELO		0x0000
#define	DEF_GDT_DATA_BASEMID		0x00
#define	DEF_GDT_DATA_FLAGS		0xCF92
#define	DEF_GDT_DATA_BASEHI		0x00

/* Usermode Code Descriptor*/
#define	DEF_GDT_U_CODE_LIMIT		0xFFFF
#define	DEF_GDT_U_CODE_BASELO		0x0000
#define	DEF_GDT_U_CODE_BASEMID		0x00
#define	DEF_GDT_U_CODE_FLAGS		0xCFFA
#define	DEF_GDT_U_CODE_BASEHI		0x00
/* Usermode Data Descriptor*/
#define	DEF_GDT_U_DATA_LIMIT		0xFFFF
#define	DEF_GDT_U_DATA_BASELO		0x0000
#define	DEF_GDT_U_DATA_BASEMID		0x00
#define	DEF_GDT_U_DATA_FLAGS		0xCFF2
#define	DEF_GDT_U_DATA_BASEHI		0x00

#define GDTMAX 5

typedef struct{
  unsigned short limitLow,baseLow;
  unsigned char baseMid;
  unsigned short accessRight;
  unsigned char baseHigh;
} __attribute__((packed))SEGMENT_DESCRIPTOR;

typedef struct{
  unsigned short size;
  SEGMENT_DESCRIPTOR* base;
} __attribute__((packed))GDTR;

void init_gdt(void);

#endif
