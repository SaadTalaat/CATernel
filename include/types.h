/*
 * Saad Talaat
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 */
#ifndef _ASM_TYPES_H
#define _ASM_TYPES_H
#define NULL ((void*) 0)
typedef unsigned int 	uint32_t;
typedef unsigned short	uint16_t;
typedef unsigned char 	uint8_t;
typedef unsigned long  	uint64_t;
typedef char 		int8_t;
typedef int 		int32_t;
typedef short 		int16_t;
typedef long long	int64_t;
typedef unsigned int 	size_t;
typedef uint64_t 	uintmax_t;
typedef int64_t		intmax_t;
typedef uint32_t	reg_t;

//Addresses
typedef uint32_t	paddr_t;
typedef uint32_t	vaddr_t;
typedef int 		(*fnptr_t)();
//Rounding operations
#define ROUND_DOWN(x,y)			\
({					\
	uint32_t z = (uint32_t)(x);	\
	(typeof(x)) (z - z % (y));	\
})
#define ROUND_UP(x,y) \
({\
	uint32_t z = (uint32_t)(y);\
	(typeof(x)) (ROUND_DOWN(x,y) + y);\
})
#endif
