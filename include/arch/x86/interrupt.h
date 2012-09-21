/** include/arch/x86/interrupt.h
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 *
 * @date 27 Sept, 2012
 *
 */


#ifndef _CATRENEL_X86_INTERRUPT_H_
#define _CATERNEL_X86_INTERRUPT_H_
#include <types.h>
#include <memvals.h>


/* Gate descriptors as defined by
 * Intel Manuals.
 */
#ifndef __ASSEMBLER__
typedef struct {
	uint16_t offset_0_15;		// offset low 16 bit
	uint16_t  segment_s;		// segment selector value
	/*
	 * Args passed to interrupt or trap
	 * rarely used and undefined in HelenOS
	 * and defined in Linux as ist.
	 */
	unsigned args:5;
	unsigned reserved:3;	// Reserved, so 0 is a nice value for them
	/*
	 * Gate type can be broken into 6 types
	 * 1- Task Gate: 	16/32 bit = 0x5
	 * 2- Interrupt Gate:	16 bit = 0x6
	 * 3- Interrupt Gate:	32 bit = 0xE
	 * 4- Trap Gate:	16 bit = 0x7
	 * 5- Trap Gate:	32 bit = 0xF
	 */
	unsigned type:4;
	unsigned s:1;		//must be 0
	unsigned dpl:2;		// descriptor privilege level.
	unsigned p:1;		// present
	uint16_t offset_16_31;	// offset high 16 bit


} __attribute__((packed)) gatedesc;


typedef struct Gdtdesc Idtdesc;

extern gatedesc idt[];
#endif

/*
 * Constant values as gate types
 */

#define	GATE_TASK	0x5
#define	GATE_INT16	0x6
#define	GATE_TRAP16	0x7
#define GATE_INT32	0xE
#define	GATE_TRAP32	0xF

#define IDT_ENTRIES	0x100

/*
 * Macros to fill out Gate descriptors
 */

#define GATE_FILL(gate, type, dpl, sel, offset)	\
						\
	(gate)->offset_0_15 = (uint16_t)	((offset) & 0xffff);	\
	(gate)->segment_s = (uint16_t) (sel);			\
	(gate)->args	= 0;					\
	(gate)->reserved	= 0;					\
	(gate)->type	= 0xe;					\
	(gate)->dpl	= 0;					\
	(gate)->p	= 1;					\
	(gate)->offset_16_31 = (uint16_t) ((offset) >> 16);	
/*
#define EXTERN_VECTORS(cnt)	\
	uint32_t	UniqueCounterName;	\
	for(UniqueCounterName = 0; UniqueCounterName < cnt; UniqueCounterName++)	\
		extern ;

#define VECTOR_INDEX(cnt)	vector_0#cnt
*/
#define GATE_OFFSET(gate, vector)\
	(gate).offset_0_15 = (uint32_t) ((vector) & 0xffff);\
	(gate).offset_16_31= (uint32_t) ((vector) >> 16);

#endif
