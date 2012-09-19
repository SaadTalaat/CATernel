/*
 * Saad Talaat
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 */

/* HOW GDT register is used
 * This is the GDT register
 * ---------------------------------------------
 *|31		      16| 15		       0|
 *|			|			|
 *|	Base 0:15	|	Limit 0:15	|
 *|			|			|
 *|---------------------------------------------|
 *|63   56|55 52| 51  48|47	 40|39        32|
 *| Base  |	| Limit	| Access   |		|
 *| 24:31 |Flags| 16:19	| Byte	   | Base 16:23	|
 *|	  |	|	|	   |		|
 * ---------------------------------------------
 * Access byte is
 * byte 0 = Accessed bit set to 1 by CPU when segment is accessed. we will set it to 0
 * byte 1 = read/write permissions 
 * byte 2 = Direction bit we will set that to 0 for growing up segments and 1 for growing down segments and conforming bit
 * byte 3 = Executable bit 1 if code segment 0 if data segment
 * byte 4 = always 1
 * byte 5,6 = Privilege since we are a kernel we will set that to 0
 * byte 7 = Present bit one for anything
 *
 * Flag 4 bits are always 1100
 */
#ifndef _CATERNEL_BOOT_MEMVALS_H_
#define _CATERNEL_BOOT_MEMVALS_H_
#ifndef __ASSEMBLER__
struct Segdesc {
	unsigned limit_0 : 16;
	unsigned base_0	: 16;
	unsigned base_1: 8;
	unsigned permission: 8;
	unsigned limit_1: 4;
	unsigned flags: 4;
	unsigned base: 8;


};
// Loadable descriptor
struct Gdtdesc{
	uint16_t size;
	uint32_t base;

} __attribute__ ((packed));
#endif
#define SEGACS_RW	0x2	/* Read and write flag Read for code segments and write for data segments */
#define SEGACS_X	0x8	/* The Executable flag */
#define SEGACS_D	0x4	/* growing down segment for data segments and means that this segments can be
					Executed from the user applications "Low Privilege" */
#define SEGACS_USR	0x60

/* This is a shitty MACRO to make segment descriptor you pass the limit and base address and SEGACS_* for type
 * And you get the descriptor. Don't try to trace this code. it might piss your calculator off! */
#ifdef __ASSEMBLER__
#define SEGMENT(limit,base,access) \
	.word  ((((limit)>>12) & 0xFFFF)),((base) & 0xffff);\
	.byte ( ((base)>>16) & 0xff),( (access) | 0x90) \
		,(0xC0 | (( ( (limit) >> 28 )&0xf) )), (( (base) >> 24) & 0xff)
#endif

#ifndef __ASSEMBLER__
#define SEG_NULL	(struct Segdesc){0,0,0,0,0,0,0}
#define SEGMENT(limit,base,access) (struct Segdesc)\
	{\
	(limit>>12) & 0xFFFF,\
	(base) & 0xFFFF,\
	((base)>>16) & 0xFF,\
	(access) | 0x90,\
	((limit) >> 28),\
	0xC,\
	(base)>>24\
	}
	
#endif

/********************************* Memory addresses Constants *****************************************************/

#define EXTMEM	0x100000
#define PAGESZ	0x1000		// Page size
#define PAGELG	0xC		// Page Shift which is log2 PAGESZ "to be used in memory allocation alignment"
#define PAGECNT	0x400		// Each Page table has 1024 Page enteries
#define PAGETSZ (PAGESZ*PAGECNT)// Page Table size
#define KERNEL_ADDR 0xF0000000	// Kernel Physical memory
#define KERNEL_STACK PAGESZ*8	// Kernel Stack size
#define VIRTPGT	(KERNEL_ADDR - PAGETSZ) 	// Virtual page table address is just after the kernel and before the stack
#define KERNEL_STACK_TOP VIRTPGT	// Stack top address
/*** user definitions ***/
#define USEREND		(VIRTPGT - PAGETSZ)
#define USERVIRTPGT     (USEREND - PAGETSZ)
#define USERPAGES	(USERVIRTPGT - PAGETSZ)
#define USERSTART	(USERPAGES - PAGETSZ)
#define USERSTACK_END	(USEREND - (2*(PAGESZ)))

/*** Physical address ***/
#define PA(va)({\
	uint32_t pa = (uint32_t) va;\
	pa-KERNEL_ADDR;\
})

/** Paging **/
// We are using 32 bit paging
//#define DIROFF(x)	(( ((uint32_t) (x)) >> 22) & 0x3FF)
//#define TBLOFF(x)	(( ((uint32_t) (x)) >> 12) & 0x3FF)

/* Place of IO Hole */
#define IOPHYMEM 0x0A0000

/* Sizes */

#define KB	0x400
#define	MB	KB*KB
#define GB	MB*KB

#endif
