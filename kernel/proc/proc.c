#include <types.h>
#include <memvals.h>
#include <proc/proc.h>
#include <arch/x86/mm/segdesc.h>
#include <arch/x86/processor.h>
#include <string.h>
#include <init.h>
#include <rdisk.h>

extern pde_t *global_pgdir;
struct Proc_List	empty_procs;

proc_t *proc_table;
void
init_proc_table(void){
	/*
	 * In Memory Map, Processes Table takes places after
	 * the pages table. at PROC_LIST
	 */

	int 	c,count;
	proc_t	*i_proc;
	LIST_INIT(&empty_procs);
	
	/*
	 * Initate all processes slots to empty.
	 */
	for(i_proc = &proc_table[0], count =0 ; count < MAX_PROCS; i_proc++, count++){

		i_proc->gpr_regs = (gpr_regs_t){0,0,0,0,0,0,0,0};
		i_proc->seg_regs = (seg_regs_t){0,0,0,0};
		i_proc->proc_status = PROC_EMPTY;
		i_proc->proc_id	= 0;
		i_proc->preempted = 0;
		i_proc->dequeqed = 0;
		i_proc->eip	= 0;
		i_proc->cs	= 0;
		i_proc->ss	= 0;
		i_proc->esp	= 0;
		i_proc->eflags	= 0;
		
		LIST_INSERT_HEAD(&empty_procs, &proc_table[count], link);
	}
	// Setup user space 16 Megs are enough..
	map_segment_page(global_pgdir, 0xA0000000, 0x1000000 ,
			0x20000000 , PAGE_PRESENT | PAGE_USER | PAGE_WRITABLE);


}


void
switch_address_space(proc_t *proc_to_run){
	/*
	 * Fooling the x86 CPU!
	 *      1- set stack top to point at proc structure start
	 *      2- set the cpu state to what the proc indicates
	 *      3- issue an iret
	 */
	printk("frame at %p\n", proc_to_run);
	proc_to_run->seg_regs.fs = 0x23;
	proc_to_run->seg_regs.es = 0x23;
	proc_to_run->seg_regs.gs = 0x23;
	proc_to_run->seg_regs.ds = 0x23;
	asm("xchg %bx,%bx");
//	asm volatile("cli");
	asm volatile("movl %0,%%esp":: "g" (proc_to_run) : "memory");

	asm volatile("popal");
	asm volatile("popl %gs\npopl %fs\npopl %es");
	asm volatile("popl %ds");
//	asm volatile("movl $5f,0(%esp)\n");
	asm volatile("iret\n5:\n");

	while(1);
}

