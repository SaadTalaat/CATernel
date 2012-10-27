/** 
 * @file include/arch/x86/cpu_state.h
 * @date 28 Sept, 2012
 * @brief structures and constants used in storing and determining cpu state.
 * @author Saad Talaat
 */

#ifndef _CATERNEL_CPU_STATE_H_
#define _CATERNEL_CPU_STATE_H_

/*
 * Define how a caller cpu environment is
 * saved when interrupt or exception issued
 */
#include <types.h>

/**
 * @brief Interaupt/Trap cpu state.
 * @details
 * the cpu_state structure extends the default
 * intel hardware interrupt/trap frame stored
 * into stack. More detailed than the hardware
 * frame.
 */
typedef struct{

		/*
	 * Pointers
	 */
	reg_t   ebp_frame;
	reg_t   eip_frame;
	/*
	 * Segment registers
	 */
	reg_t   gs;
	reg_t   fs;
	reg_t   es;
	reg_t   ds;

	/*
	 * GP registers
	 
*/
	reg_t	edi;
	reg_t	esi;
	reg_t	o_ebp;
	reg_t	o_esp;
	reg_t	ebx;
	reg_t	edx;
	reg_t	ecx;
	reg_t	eax;
		

	/* 
	 * the useful stuff....
	 * This part is saved by the hardware, check
	 * Intel Manuals volume 3: 6.12 Interrupt or Exception handling
	 */
	uint32_t error_code;
	reg_t	eip;
	reg_t	cs;
	reg_t	eflags;
	reg_t	esp;
	reg_t	ss;
} cpu_state_t;


/**
 * @brief General purpose registers 
 * @details gpr registers structure contains
 * general purpose register. ordered to be filled
 * by a single PUSHAD/PUSHAL
 */
typedef struct{

	reg_t   edi;
	reg_t   esi;
	reg_t   ebp;
	reg_t   esp;
	reg_t   ebx;
	reg_t   edx;
	reg_t   ecx;
	reg_t   eax;

} gpr_regs_t;

/**
 * @brief Segment registers.
 * @details the structure contains segment registers
 * except for SP and CS since they are stored by the
 * hardware into the structure.
 */
typedef struct{
	reg_t   gs;
	reg_t   fs;
	reg_t   es;
	reg_t   ds;

} seg_regs_t;

/**
 * @brief Task State segment
 * @details
 * The Task state segment entry fields, merely used
 * into the kernel.\n
 * TSS is only used to support interrupts from user
 * space. other than that soft task switching is used.
 */
typedef struct{
	uint16_t prelink, _rsrvd1;
	reg_t esp0;
	uint16_t ss0, _rsrvd2;
	reg_t esp1;
	uint16_t ss1, _rsrvd3;
	reg_t esp2;
	uint16_t ss2, _rsrvd4;
	uint32_t cr3;
	reg_t eip;
	reg_t eflags;
	reg_t eax;
	reg_t ecx;
	reg_t edx;
	reg_t ebx;
	reg_t esp;
	reg_t ebp;
	reg_t esi;
	reg_t edi;
	uint16_t es, _rsrvd5;
	uint16_t cs, _rsrvd6;
	uint16_t ss, _rsrvd7;
	uint16_t ds, _rsrvd8;
	uint16_t fs, _rsrvd9;
	uint16_t gs, _rsrvda;
	uint16_t ldt, _rsrvdb;
	uint16_t trace;
	uint16_t iomap_base;

} __attribute__((packed)) tss_t;

/**
 * @brief Task state segment descirptor.
 * @details TSS desciptor is somehow a child
 * of a gate desciptor and similar to segment selectors.
 */
typedef struct {

	uint16_t limit_0_15;
	uint16_t base_0_15;
	uint8_t	base_16_23;
	uint8_t access;
	unsigned limit_16_19:4;
	unsigned available:1;
	unsigned unused:2;
	unsigned granularity:1;
	uint8_t	base_24_31;
} __attribute__((packed)) tss_desc;

/********************* Constants *******************************/

#define TSS_SIZE	104
#define TSS_IOMAP_SIZE	((0x8 * 0x400) +1)
#define TASK_BUSY	0xB
#define TASK_INACTIVE	0x9
#define TSS_AVL		1
#define TSS_DPL_KERNEL 	0
#define TSS_DPL_USER	60
#define TSS_PRESENT	0x80
#define TSS_GRANULARITY 0x8

#define SEGMENT_TSS(limit, base, type, flags)\
	(struct Segdesc)\
	{	\
	(((limit)>>12) & 0xffff),	\
	((base) & 0xffff),	\
	(((base) >> 16) & 0xff),	\
	(type),	\
	((limit) >> 28),	\
	((flags)),	\
	(((base) >> 24) & 0xff)	\
	}
#endif
