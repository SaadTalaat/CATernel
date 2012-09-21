/** include/arch/x86/cpu_state.h
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 *
 * @date 28 Sept, 2012
 *
 */

#ifndef _CATERNEL_CPU_STATE_H_
#define _CATERNEL_CPU_STATE_H_

/*
 * Define how a caller cpu environment is
 * saved when interrupt or exception issued
 */
#include <types.h>

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

#endif
