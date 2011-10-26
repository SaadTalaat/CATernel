/*
 * Author: Saad Talaat
 * CATReloaded Team
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

#define SEGACS_RW	0x2	/* Read and write flag Read for code segments and write for data segments */
#define SEGACS_X	0x8	/* The Executable flag */
#define SEGACS_D	0x4	/* growing down segment for data segments and means that this segments can be
					Executed from the user applications "Low Privilege" */

/* This is a shitty MACRO to make segment descriptor you pass the limit and base address and SEGACS_* for type
 * And you get the descriptor. Don't try to trace this code. it might piss your calculator off! */

#define SEGMENT(limit,base,access) \
	.word  ((limit) & 0xFFFF),((base) & 0xffff);\
	.byte ( (base) & 0xff),( access & 0x90) \
		,(0xC0 | (( ( (limit) >> 12 )&0xf) )), (( (base) >> 24) & 0xff)


/********************************* Memory addresses Constants *****************************************************/

#define PAGESZ	0x1000		// Page size
#define PAGELG	0xC		// Page Shift which is log2 PAGESZ "to be used in memory allocation alignment"
#define PAGECNT	0x400		// Each Page table has 1024 Page enteries
#define PAGETSZ (PAGESZ*PAGECNT)// Page Table size
#define KERNEL_ADDR 0xF0000000	// Kernel Physical memory
#define KERNEL_STACK PAGESZ*8	// Kernel Stack size
#define VIRTPGT	(KERNEL_ADDR-PAGETSZ) 	// Virtual page table address is just after the kernel and before the stack
#define KERNEL_STACK_TOP VIRTPGT	// Stack top address

