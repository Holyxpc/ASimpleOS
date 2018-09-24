#ifndef __TYPE_H__
#define __TYPE_H__

typedef unsigned int	__u32;
typedef unsigned int 	u32;
typedef unsigned long long __u64;
typedef unsigned long long u64;
typedef unsigned int size_t;


typedef          int   int32_t;
typedef unsigned int   uint32_t;
typedef          short int16_t;
typedef unsigned short uint16_t;
typedef          char  int8_t;
typedef unsigned char  uint8_t;

#ifndef NULL
	#define NULL 0
#endif

#ifndef TRUE
	#define TRUE  1
	#define FALSE 0
#endif

typedef int32_t pid_t;

#endif

