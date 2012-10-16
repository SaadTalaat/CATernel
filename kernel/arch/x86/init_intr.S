#/** include/arch/x86/init_intr.S
# * CATReloaded (C) Copyrights 2011
# * http://catreloaded.net
# *
# * @date 27 Sept, 2012
# *
# */

#include <arch/x86/mm/segdesc.h>


/*
 * This exception index masking for error code existance
 * is used by the SPARTAN kernel to determine exceptions
 * with error code.
 * Exceptions with no error code:
 *	0,1,2,3,4,5,6,7,9,18,19,20,21,22,23,24,25,26,27
 *	28,29,30,31
 * Exceptions with error code:
 *	8,10,11,12,13,14,17
 * Intel Manuals Software developer guide Volume 3
 *	6.15 Exception and interrupt references
 */
#define EXCEPTION_ERROR_CODE_MASK	0x000027d00

/*
 * define indexes of registers for the cpu state
 * structure. this is scheme is used by unix and
 * re-implemented in SPARTAN kernel.
 */
#define CPU_STATE_EDX_INDEX	0
#define	CPU_STATE_ECX_INDEX	4
#define	CPU_STATE_EBX_INDEX	8
#define	CPU_STATE_ESI_INDEX	12
#define	CPU_STATE_EDI_INDEX	16
#define	CPU_STATE_EBP_INDEX	20
#define CPU_STATE_EAX_INDEX	24
#define CPU_STATE_EBP_FRAME_INDEX	28
#define	CPU_STATE_EIP_FRAME_INDEX	32
#define CPU_STATE_GS_INDEX	36
#define CPU_STATE_FS_INDEX	40
#define CPU_STATE_ES_INDEX	44
#define CPU_STATE_DS_INDEX	48
#define	CPU_STATE_ERROR_CODE_INDEX	52
#define	CPU_STATE_EIP_INDEX	56
#define CPU_STATE_CS_INDEX	60
#define CPU_STATE_EFLAGS_INDEX	64
#define CPU_STATE_ESP_INDEX	68
#define CPU_STATE_SS_INDEX	72


#define CPU_STATE_UPPER_SIZE	52

.text
.global int_generic

int_generic:
	iret
.global save_regs
save_regs:

	
.macro handler i
.global	vector_\i
vector_\i:
	

	/* This works like an interrupt mapper */
	.iflt	\i - 32
		.if (1<< \i) & EXCEPTION_ERROR_CODE_MASK
			subl	$0, %esp
		.else
			subl	$4, %esp
		.endif
	.else
		subl	$4, %esp

	.endif	 
	
	/* Store GPR */
	 pushal
	
	/* store segment regs */
	movl %ds, %ecx
	movl %es, %edx
	
	pushl %ecx
	pushl %edx

	movl %fs, %ecx
	movl %gs, %edx
	
	pushl %ecx
	pushl %edx
	/* Save space for the pointers */
	subl $8, %esp

	# Switch to kernel mode

	movl $SEG_KERNDATA, %eax

	movl %eax, %es
	movl %eax, %ds

	xorl %eax,%eax

	#check what Code segment did we come from
	cmpl $SEG_KERNCODE, 64(%esp)	
	cmovnzl %eax, %ebp

	movl %ebp, (%esp)
	movl 60(%esp), %eax
	movl %eax, 4(%esp)
	leal (%esp), %ebp

	push %esp
	push $(\i)
	call map_exception

	addl $16,%esp

	popl %edx
	popl %ecx

	movl %edx, %gs
	movl %ecx, %fs

	popl %edx
	popl %ecx

	movl %edx, %es
	movl %ecx, %ds

	# Once we popad %esp should be restored and we shall lose
	# the whole cpu_state
	popal

	iret
.endm


/* Thank you python! */
#define INTR_COUNT_RANGE	\
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63
#,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255

.irp	cnt, INTR_COUNT_RANGE
	handler \cnt
.endr
